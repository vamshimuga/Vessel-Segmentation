#include "sparsefieldlevelsetwithprinciplecurvature.h"

#define max(x,y)    ((x>y) ? x : y )
#define min(x,y)    ((x<y) ? x : y )
#define FARAWAY      10.0
#define NEIGHBORSIZE 1
#define LSMASK_FARAWAY       0x00
#define LSMASK_ACTIVE       0x10
#define LSMASK_MOVEUP       0x20
#define LSMASK_MOVEDOWN     0x30
#define LSMASK_SLEEP		 0x40
#define LSMASK_WAKEDUP		 0x50
#define LSMASK_OUT1NEIGHBOR	 0x60
#define LSMASK_IN1NEIGHBOR	 0x70
#define LSMASK_CHECKINGIN1NEIGHBOR	 0x80
#define LSMASK_CHECKINGOUT1NEIGHBOR	 0x90
#define LSMASK_OUT2NEIGHBOR	 0xa0
#define LSMASK_IN2NEIGHBOR	 0xb0

#define LSMASK_READOUTMASK     0xf0
#define MAXSTEP 0.8

SparseFieldLevelSetWithPrincipleCurvature::SparseFieldLevelSetWithPrincipleCurvature(QObject *parent)
    :QThread(parent)
{
    phi = NULL;
    externalForce = NULL;
    statusmap = NULL;
    principleDirectionMap = NULL;
    for( int i = 0 ; i < 3 ; i++ )
    {
        dimension[i] = 0;
        spacing[i] = 0;
        dimoffset[0] = 0;
    }
    totalSize = 0;
    sliceSize = 0;
    imageW = 0;
    imageH = 0;
    imageD = 0;
    updateInterval = 5;

    its = 0;
    periods = 0;

    maxiumStepLength = MAXSTEP;
    externalWeightFactor = 0.5;

    maxIteration = 15000;

    activeband = NULL;
    tempactiveband = NULL;
    removeactivepoints = NULL;
//    currentAlgorithm = MIA_ThresholdBased;
//    currentAlgorithm = MIA_GradientBased;

    isValid = false;
    isAbort = false;
    imDimension = 3;

    connect(this,SIGNAL(levelsetFunctionUpdated()), parent, SLOT(forwardLevelSetFunctionUpdated()));
    connect(this,SIGNAL(levelsetFunctionCompleted()), parent, SLOT(forwardLevelsetFunctionCompleted()));
    connect(this,SIGNAL(levelsetEnterNewPeriod(int)), parent, SLOT(forwardLevelsetEnterNewPeriod(int)));
    setPriority(QThread::HighestPriority);
}

void SparseFieldLevelSetWithPrincipleCurvature:: run()
{
    if(!isValid)
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return;
    }


    for( ; its < maxIteration ; its ++ )
    {
        if(isAbort)
            break;

        reinitialize();
        update3DphiUsingThreshold();

        if(its%updateInterval==0)
        {
            emit levelsetFunctionUpdated();
        }

    }
//    reinitialize();
    emit levelsetFunctionUpdated();
    emit levelsetFunctionCompleted();

    its = 0;
    isAbort = false;

}


void SparseFieldLevelSetWithPrincipleCurvature::abort()
{
    if(!isValid)
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return;
    }

    isAbort = true;

}

void SparseFieldLevelSetWithPrincipleCurvature::resume()
{
    if(!isValid)
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return;
    }
    if(its < maxIteration)
        start();
}


bool SparseFieldLevelSetWithPrincipleCurvature::validation()
{
    if(!phi || !externalForce || ! statusmap || imageW < 3 || imageH < 3 ||imageD < 1 )
        return false;
    return true;
}

bool SparseFieldLevelSetWithPrincipleCurvature::initialize( float* p, float* ext, uchar* smap, int* d, float* s)
{
    //set up the play ground
    phi = p;
    externalForce = ext;
    statusmap = smap;
    totalSize = 1;
    for( int i = 0 ; i < 3 ; i++ )
    {
        dimension[i] = d[i];
        totalSize *= dimension[i];
        spacing[i] = s[i];
    }
    if(d[2] ==1 )
        imDimension = 2;
    else
        imDimension = 3;





    its = 0;
    periods = 0;
    maxiumStepLength = MAXSTEP;

    imageW = dimension[0];
    imageH = dimension[1];
    imageD = dimension[2];
    sliceSize = imageW*imageH;


    dimoffset[0] = 1;
    dimoffset[1] = imageW;
    dimoffset[2] = sliceSize = imageW*imageH;;



    //initialize
    activeband1.clear();
    activeband2.clear();
    activeband1.reserve(totalSize/10);
    activeband2.reserve(totalSize/10);
    activeband = &activeband1;
    removeactivepoints = &activeband2;

    activebands_deltaphi.clear();
    activebands_deltaphi.reserve(totalSize/10);

    neighborband.clear();
    inneighborband.clear();
    outneighborband.clear();
    neighborband.reserve(totalSize/5);
    inneighborband.reserve(totalSize/10);
    outneighborband.reserve(totalSize/10);

    isValid = validation();

    if(!isValid)
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return false;
    }

    //initial active band

    memset(statusmap,LSMASK_FARAWAY,totalSize*sizeof(uchar));

    int i;
    for(i = 0 ; i < totalSize ; i++)
    {

            if(phi[i]>=-0.5&&phi[i]<0.5)
            {

                    activeband->push_back(i);
                    statusmap[i]=LSMASK_ACTIVE;
//                    passedmap[i]++;

            }
            else
            {
                    if(phi[i]<-5.0)
                            phi[i]=-FARAWAY;
                    else if(phi[i]>5.0)
                            phi[i]=FARAWAY;
            }
    }


    // initialize statusmap
    int dim,n_x,n_i;
    int cor[3];

    int narrowbandsize=activeband->size();
    int index;
    for(index = 0 ; index < narrowbandsize ;index++)
    {

            int ind = (*activeband)[index];
            cor[2] = ind/(imageW*imageH);
            cor[1] = (ind-imageW*imageH*cor[2])/imageW;
            cor[0] = ind - imageW*imageH*cor[2] - imageW*cor[1];
            for (dim=0; dim<imDimension; dim++) {
                    for(n_x=-1;n_x<=1;n_x+=2)
                    {
                            n_i = ind + dimoffset[dim] * n_x;
                            if(cor[dim]+n_x<0 || cor[dim]+n_x >= dimension[dim])
                                continue;
                            if(statusmap[n_i] == LSMASK_FARAWAY)
                            {
                                    if(phi[n_i] < -0.5)
                                            statusmap[n_i] = LSMASK_IN1NEIGHBOR;
                                    else {
                                            statusmap[n_i] = LSMASK_OUT1NEIGHBOR;
                                    }
                            }
                    }
            }
    }



    return true;
}



void SparseFieldLevelSetWithPrincipleCurvature::update3DphiUsingThreshold()
{
    float maxPropogating=0;
    float dx,dy,dz;
    float dxplus, dyplus, dzplus, dxminus, dyminus, dzminus;
    float dxplusy, dxminusy, dxplusz, dxminusz, dyplusx, dyminusx, dyplusz, dyminusz, dzplusx, dzminusx, dzplusy, dzminusy;
    float gradphimax, gradphimin, nplusx, nplusy, nplusz, nminusx, nminusy, nminusz, curvature;
    float delta_phi;
    int ind;
    int i,j,k;
    removeactivepoints->clear();
    activebands_deltaphi.clear();

    std::vector<int>::iterator iter;
    for(iter=activeband->begin();iter!=activeband->end();iter++)
    {
        ind=*iter;

        k=ind/sliceSize;
        j=(ind-k*sliceSize)/imageW;
        i=ind-k*sliceSize-j*imageW;

        if(i==0||i==imageW-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
        if(j==0||j==imageH-1){dy=0;} else {dy=(phi[ind-imageW]-phi[ind+imageW])/2;}
        if(k==0||k==imageD-1){dz=0;} else {dz=(phi[ind+sliceSize]-phi[ind-sliceSize])/2;}

        if(i==imageW-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
        if(j==0){dyplus=0;} else {dyplus=(phi[ind-imageW]-phi[ind]);}
        if(k==imageD-1){dzplus=0;} else {dzplus=(phi[ind+sliceSize]-phi[ind]);}
        if(i==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
        if(j==imageH-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imageW]);}
        if(k==0){dzminus=0;} else {dzminus=(phi[ind]-phi[ind-sliceSize]);}

        if(i==0||i==imageW-1||j==0){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
        if(i==0||i==imageW-1||j==imageH-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
        if(i==0||i==imageW-1||k==imageD-1) {dxplusz=0;} else {dxplusz=(phi[ind+sliceSize+1]-phi[ind+sliceSize-1])/2;}
        if(i==0||i==imageW-1||k==0) {dxminusz=0;} else {dxminusz=(phi[ind-sliceSize+1]-phi[ind-sliceSize-1])/2;}
        if(j==0||j==imageH-1||i==imageW-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
        if(j==0||j==imageH-1||i==0){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
        if(j==0||j==imageH-1||k==imageD-1) {dyplusz=0;} else {dyplusz=(phi[ind+sliceSize-imageW]-phi[ind+sliceSize+imageW])/2;}
        if(j==0||j==imageH-1||k==0) {dyminusz=0;} else {dyminusz=(phi[ind-sliceSize-imageW]-phi[ind-sliceSize+imageW])/2;}
        if(k==0||k==imageD-1||i==imageW-1) {dzplusx=0;} else {dzplusx=(phi[ind+1+sliceSize]-phi[ind+1-sliceSize])/2;}
        if(k==0||k==imageD-1||i==0) {dzminusx=0;} else {dzminusx=(phi[ind-1+sliceSize]-phi[ind-1-sliceSize])/2;}
        if(k==0||k==imageD-1||j==0) {dzplusy=0;} else {dzplusy=(phi[ind-imageW+sliceSize]-phi[ind-imageW-sliceSize])/2;}
        if(k==0||k==imageD-1||j==imageH-1) {dzminusy=0;} else {dzminusy=(phi[ind+imageW+sliceSize]-phi[ind+imageW-sliceSize])/2;}


        gradphimax=sqrt((sqrt(max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)))*(sqrt(max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)))
                                        +(sqrt(max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)))*(sqrt(max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)))
                                        +(sqrt(max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0)))*(sqrt(max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0))));

        gradphimin=sqrt((sqrt(min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)))*(sqrt(min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)))
                                        +(sqrt(min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)))*(sqrt(min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)))
                                        +(sqrt(min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0)))*(sqrt(min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0))));

        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) + ((dzplusx + dz)*(dzplusx + dz)*0.25));
        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) + ((dzplusy + dz)*(dzplusy + dz)*0.25));
        nplusz= dzplus / sqrt(1.192092896e-07F + (dzplus*dzplus) + ((dxplusz + dz)*(dxplusz + dz)*0.25) + ((dyplusz + dy)*(dyplusz + dy)*0.25));

        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) + ((dzminusx + dz)*(dzminusx + dz)*0.25));
        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) + ((dzminusy + dz)*(dzminusy + dz)*0.25));
        nminusz= dzminus / sqrt(1.192092896e-07F + (dzminus*dzminus) + ((dxminusz + dz)*(dxminusz + dz)*0.25) + ((dyminusz + dy)*(dyminusz + dy)*0.25));
        if(principleDirectionMap == NULL)
        {
            curvature= ((nplusx-nminusx)+(nplusy-nminusy)+(nplusz-nminusz))/2;
        }
        else
        {
            float dVector[3];
            float cVector[3];
            QRgb dRGB = principleDirectionMap[ind];
            dVector[0] = ((float)qRed(dRGB) - 128.0)/127.0;
            dVector[1] = ((float)qGreen(dRGB) - 128.0)/127.0;
            dVector[2] = ((float)qBlue(dRGB) - 128.0)/127.0;
            cVector[0] = (nplusx-nminusx);
            cVector[1] = (nplusy-nminusy);
            cVector[2] = (nplusz-nminusz);
            curvature = (nplusx-nminusx)*fabs(dVector[0]) + (nplusy-nminusy)*fabs(dVector[1]) + (nplusz-nminusz)*fabs(dVector[2]);
//            if(k == 20)
//            {
//                qDebug()<<ind<<cVector[0]<<cVector[1]<<cVector[2];
//                qDebug()<<dVector[0]<<dVector[1]<<dVector[2]<<curvature;
//            }
        }

        delta_phi = (-externalWeightFactor * externalForce[ind]) + ((1-externalWeightFactor) * curvature);
        if(delta_phi > 0)
            delta_phi = delta_phi * gradphimax;
        else
            delta_phi = delta_phi * gradphimin;


        if(fabs(delta_phi) > maxPropogating)
        {
            maxPropogating=fabs(delta_phi);
        }
        activebands_deltaphi.push_back(delta_phi);

    }
    float dt = maxiumStepLength / (maxPropogating+1.0e-6);

    if(maxPropogating<0.0001)
    {
        changed=0;
        return;
    }
    float newvalue=0;
    std::vector<float>::iterator delta_iter;

    iter=activeband->begin();
    delta_iter = activebands_deltaphi.begin();
    changed=0;

    for( ; iter!=activeband->end() ; iter++, delta_iter++)
    {
        int ind=*iter;

        k=ind/sliceSize;
        j=(ind-k*sliceSize)/imageW;
        i=ind-k*sliceSize-j*imageW;

        newvalue=phi[ind]+dt*(*delta_iter);
        changed++;

        if(newvalue>=0.5)
        {
            int neighbormovingdown=0;
            if(i>0 && statusmap[ind-1]==LSMASK_MOVEDOWN)
                 neighbormovingdown=1;
            if(i<imageW-1 && statusmap[ind+1]==LSMASK_MOVEDOWN)
                 neighbormovingdown=1;
            if(j>0 && statusmap[ind-imageW]==LSMASK_MOVEDOWN)
                 neighbormovingdown=1;
            if(j<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEDOWN)
                 neighbormovingdown=1;
            if(k>0 && statusmap[ind-sliceSize]==LSMASK_MOVEDOWN)
                 neighbormovingdown=1;
            if(k<imageD-1 && statusmap[ind+sliceSize]==LSMASK_MOVEDOWN)
                 neighbormovingdown=1;
            if(!neighbormovingdown)
            {
                statusmap[ind]=LSMASK_MOVEUP;
                removeactivepoints->push_back(ind);
            }
            else
            {
                continue;
            }
        }
        if(newvalue<-0.5)
        {
            int neighbormovingdown=0;
            if(i>0 && statusmap[ind-1]==LSMASK_MOVEUP)
                 neighbormovingdown=1;
            if(i<imageW-1 && statusmap[ind+1]==LSMASK_MOVEUP)
                 neighbormovingdown=1;
            if(j>0 && statusmap[ind-imageW]==LSMASK_MOVEUP)
                 neighbormovingdown=1;
            if(j<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEUP)
                 neighbormovingdown=1;
            if(k>0 && statusmap[ind-sliceSize]==LSMASK_MOVEUP)
                 neighbormovingdown=1;
            if(k<imageD-1 && statusmap[ind+sliceSize]==LSMASK_MOVEUP)
                 neighbormovingdown=1;

            if(!neighbormovingdown)
            {
                statusmap[ind]=LSMASK_MOVEDOWN;
                removeactivepoints->push_back(ind);
            }
            else
            {
                continue;
            }
        }

        phi[ind]=newvalue;

    }

}

void SparseFieldLevelSetWithPrincipleCurvature::reinitialize()
{
    std::vector<int>::const_iterator iter;
    neighborband.clear();
    int dim,n_i,n_x;

    //check the neighbors of retired Active Points, promote them to activepoits if proper
    for(iter=removeactivepoints->begin();iter!=removeactivepoints->end();iter++)
    {
            int ind=*iter;
            uchar possiblelayer;
            float ds;
            // we use the active points' moving direction to estimate inner or outer neighbors will be promoted
            if( statusmap[ind] == LSMASK_MOVEUP)
            {
                    possiblelayer = LSMASK_IN1NEIGHBOR;
                    ds = -1.0;
                    statusmap[ind] = LSMASK_OUT1NEIGHBOR;
            }
            else {
                    possiblelayer = LSMASK_OUT1NEIGHBOR;
                    ds = 1.0;
                    statusmap[ind] = LSMASK_IN1NEIGHBOR;
            }
            // scan the neighbors for desired inner or outer layer
            for (dim=0; dim<imDimension; dim++) {
                    for(n_x=-1;n_x<=1;n_x+=2)
                    {
                            n_i = ind + dimoffset[dim] * n_x;

                            if (n_i<0 || n_i>=totalSize) {
                                    continue;
                            }
                            if(statusmap[n_i] == possiblelayer)
                            {
                                //set the neighbor to LSMASK_ACTIVE so others can not find it again.
                                    statusmap[n_i] = LSMASK_ACTIVE;
                                    phi[n_i] = phi[ind] + ds; //update the promoted points phi using distance transform
                                    //this is not accurate as there maybe more than one neighbor retiring, RETHINKING is needed.
                                    activeband->push_back(n_i);
                                    neighborband.push_back(n_i);

                            }
                            else if(statusmap[n_i] != LSMASK_ACTIVE){
                                // if the neighbor is not active (this condition is necessary)
                                    neighborband.push_back(n_i);
                            }

                    }
            }

    }
    //then we need to set the neighbor points status to inner or outer neighter
    for(iter=neighborband.begin();iter!=neighborband.end();iter++){
        int ind=*iter;
        if(statusmap[ind] == LSMASK_IN1NEIGHBOR || statusmap[ind] == LSMASK_OUT1NEIGHBOR)
        {
            int findactive=0;
            //scan the neighbors, if there is LSMASK_ACTIVE or LSMASK_SLEEP points, this point is still on the inner or outer neighbor band

            for (dim=0; dim<imDimension; dim++) {
                for(n_x=-1;n_x<=1;n_x+=2)
                {
                    n_i = ind + dimoffset[dim] * n_x;
                    if (n_i<0 || n_i>=totalSize) {
                            continue;
                    }
                    if(statusmap[n_i] == LSMASK_ACTIVE ||statusmap[n_i] == LSMASK_SLEEP)
                    {
                            findactive=1;
                            n_x=2;dim=3;
                    }
                }
            }
            // otherwise set the point status to faraway
            if(findactive == 0)
                    statusmap[ind] = LSMASK_FARAWAY;
        }
        else if(statusmap[ind] == LSMASK_ACTIVE)
        {
            //if this point has just been promoted, we set the neighbors status to outer or inner neighbor
            for (dim=0; dim<imDimension; dim++) {
                for(n_x=-1;n_x<=1;n_x+=2)
                {
                    n_i = ind + dimoffset[dim] * n_x;
                    if (n_i<0 || n_i>=totalSize) {
                            continue;
                    }
                    if(statusmap[n_i] == LSMASK_FARAWAY)
                    {
                            if(phi[n_i] > 0)
                                    statusmap[n_i] = LSMASK_OUT1NEIGHBOR;
                            else {
                                    statusmap[n_i] = LSMASK_IN1NEIGHBOR;
                            }
                    }
                }
            }

        }

    }

    std::vector<int>::const_iterator aiter;
    if(activeband == &activeband1)
            tempactiveband = &activeband2;
    else {
            tempactiveband = &activeband1;
    }
    tempactiveband->clear();

    // now we remove the deactived (moved or sleeping) points from active band
    // we copy the active points to a temporay buffer band
    for(aiter=activeband->begin();aiter!=activeband->end();aiter++){
        int ind=*aiter;
        if(statusmap[ind] != LSMASK_ACTIVE)
                continue;
        tempactiveband->push_back(ind);


    }

    //we now give the reduct band to active band, then we can use the old activeband array to store reomved active points
    removeactivepoints = activeband;
    activeband = tempactiveband;

    neighborband.clear();
    inneighborband.clear();
    outneighborband.clear();

    // now we need do the reinitialzing business
    // we first find the inner and outer neighbor of all active points
    for(aiter=activeband->begin();aiter!=activeband->end();aiter++){
        int ind=*aiter;
        // we set the checked neighbors status to CHECKING to avoid duplication of the neighbor list
        for (dim=0; dim<imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + dimoffset[dim] * n_x;
                if (n_i<0 || n_i>=totalSize) {
                     continue;
                }
                if(statusmap[n_i] == LSMASK_IN1NEIGHBOR )
                {

                    statusmap[n_i] = LSMASK_CHECKINGIN1NEIGHBOR;
                    inneighborband.push_back(n_i);

                }
                else if(statusmap[n_i] == LSMASK_OUT1NEIGHBOR){
                    statusmap[n_i] = LSMASK_CHECKINGOUT1NEIGHBOR;
                    outneighborband.push_back(n_i);
                }

            }
        }
    }

    // we search the closest LSMASK_ACTIVE or LSMASK_SLEEP points and do distance transform from it
    // at the same time we search for the 2 layer neighbors.
    for(iter=inneighborband.begin();iter!=inneighborband.end();iter++){
        int ind=*iter;
        float ds = -1.0;
        float tempvalue = -FARAWAY;
        for (dim=0; dim<imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + dimoffset[dim] * n_x;
                if (n_i<0 || n_i>=totalSize) {
                        continue;
                }
                if(statusmap[n_i] == LSMASK_ACTIVE ||statusmap[n_i] == LSMASK_SLEEP  )
                {
                    if(phi[n_i]+ds >tempvalue)
                    {
                            tempvalue = phi[n_i]+ds;
                    }
                }
                else if(statusmap[n_i] == LSMASK_FARAWAY){
                        statusmap[n_i] = LSMASK_IN2NEIGHBOR;
                        neighborband.push_back(n_i);
                }
            }
        }
        phi[ind]=tempvalue;
        statusmap[ind] = LSMASK_IN1NEIGHBOR;
    }

    // we search the closest LSMASK_ACTIVE or LSMASK_SLEEP points and do distance transform from it
    // at the same time we search for the 2 layer neighbors.

    for(iter=outneighborband.begin();iter!=outneighborband.end();iter++){
            int ind=*iter;
            float ds = 1.0;
            float tempvalue = FARAWAY;


            for (dim=0; dim<imDimension; dim++) {
                    for(n_x=-1;n_x<=1;n_x+=2)
                    {
                            n_i = ind + dimoffset[dim] * n_x;
                            if (n_i<0 || n_i>=totalSize) {
                                    continue;
                            }
                            if(statusmap[n_i] == LSMASK_ACTIVE ||statusmap[n_i] == LSMASK_SLEEP  )
                            {

                                    if(phi[n_i]+ds < tempvalue)
                                    {
                                            tempvalue = phi[n_i]+ds;
                                    }

                            }
                            else if(statusmap[n_i] == LSMASK_FARAWAY){
                                    statusmap[n_i] = LSMASK_OUT2NEIGHBOR;
                                    neighborband.push_back(n_i);
                            }

                    }
            }
            phi[ind]=tempvalue;
            statusmap[ind] = LSMASK_OUT1NEIGHBOR;
    }
    // distance transform to the 2nd layer neighbors, this value will be used to caculate the normal of level set
    for(iter=neighborband.begin();iter!=neighborband.end();iter++){
            int ind=*iter;
            float ds = 1.0;
            float tempvalue = FARAWAY;
            if(statusmap[ind] == LSMASK_IN2NEIGHBOR)
            {
                    ds = -1.0;
                    tempvalue = -FARAWAY;
            }
            for (dim=0; dim<imDimension; dim++) {
                    for(n_x=-1;n_x<=1;n_x+=2)
                    {
                            n_i = ind + dimoffset[dim] * n_x;
                            if (n_i<0 || n_i>=totalSize) {
                                    continue;
                            }
                            if(statusmap[n_i] == LSMASK_IN1NEIGHBOR ||statusmap[n_i] == LSMASK_OUT1NEIGHBOR  )
                            {

                                    if(fabs(phi[n_i]+ds) < fabs(tempvalue))
                                    {
                                            tempvalue = phi[n_i]+ds;
                                    }

                            }

                    }
            }
            phi[ind]=tempvalue;
            statusmap[ind] = LSMASK_FARAWAY;
    }


}
