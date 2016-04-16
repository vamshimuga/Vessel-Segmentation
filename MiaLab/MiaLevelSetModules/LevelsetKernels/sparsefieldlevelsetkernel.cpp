#include "sparsefieldlevelsetkernel.h"
#include <assert.h>
#include "LevelSetRoutines.h"

SparseFieldLevelSetKernel::SparseFieldLevelSetKernel(QObject *parent)
    :AbstractLevelSetKernel(parent)
{

    activeband = NULL;
    tempactiveband = NULL;
    removeactivepoints = NULL;

    connect(this,SIGNAL(levelsetFunctionUpdated()), parent, SLOT(forwardLevelSetFunctionUpdated()));
    connect(this,SIGNAL(levelsetFunctionCompleted()), parent, SLOT(forwardLevelsetFunctionCompleted()));
    connect(this,SIGNAL(levelsetEnterNewPeriod(int)), parent, SLOT(forwardLevelsetEnterNewPeriod(int)));
    setPriority(QThread::HighestPriority);
}

void SparseFieldLevelSetKernel:: run()
{
    if(!isValid())
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return;
    }

    QTime t;
    t.start();
    for( ; its < maxIteration ; its ++ )
    {
        if(isAbort)
            break;

        reinitialize();
        updatePhi();

        if(its%updateInterval==0)
        {
            emit levelsetFunctionUpdated();
        }

    }
    //    reinitialize();
    qDebug() << "LevelSet Finished" ;
    qDebug() << "level set total running time: " << t.elapsed();
    emit levelsetFunctionUpdated();
    emit levelsetFunctionCompleted();

    its = 0;
    isAbort = false;

}


void SparseFieldLevelSetKernel::abort()
{
    if(!isValid())
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return;
    }

    isAbort = true;

}

void SparseFieldLevelSetKernel::resume()
{
    if(!isValid())
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return;
    }
    if(its < maxIteration)
        start();
}


bool SparseFieldLevelSetKernel::isValid()
{
    return( statusmap != NULL  && speedFunction->isValid());
}

bool SparseFieldLevelSetKernel::initialize(uchar* smap)
{
    AbstractLevelSetKernel::initialize(smap);
    its = 0;
    periods = 0;
    maxiumStepLength = MAXSTEP;


    //initialize
    activeband1.clear();
    activeband2.clear();
    activeband1.reserve(imInfo.totalSize/10);
    activeband2.reserve(imInfo.totalSize/10);
    activeband = &activeband1;
    removeactivepoints = &activeband2;

    activebands_deltaphi.clear();
    activebands_deltaphi.reserve(imInfo.totalSize/10);

    neighborband.clear();
    inneighborband.clear();
    outneighborband.clear();
    neighborband.reserve(imInfo.totalSize/5);
    inneighborband.reserve(imInfo.totalSize/10);
    outneighborband.reserve(imInfo.totalSize/10);


    if(!isValid())
    {
        qDebug() << "LevelSet Kernel: input data is not valid";
        return false;
    }

    //initial active band

    memset(statusmap,LSMASK_FARAWAY,imInfo.totalSize*sizeof(uchar));

    int i;
    for(i = 0 ; i < imInfo.totalSize ; i++)
    {
#ifdef USESHORTTYPELEVELSETFUNCTION
        if(phi[i]>=-5000&&phi[i]<5000)
#else
        if(phi[i]>=-0.5&&phi[i]<0.5)
#endif
        {
            activeband->push_back(i);
            statusmap[i]=LSMASK_ACTIVE;
        }
        else
        {
#ifdef USESHORTTYPELEVELSETFUNCTION
            if(phi[i]<-5000)
                phi[i]=-FARAWAY;
            else if(phi[i]>5000)
                phi[i]=FARAWAY;
#else
            if(phi[i]<-5.0)
                phi[i]=-FARAWAY;
            else if(phi[i]>5.0)
                phi[i]=FARAWAY;
#endif
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
        cor[2] = ind/imInfo.sliceSize;
        cor[1] = (ind-imInfo.sliceSize*cor[2])/imInfo.dimension[0];
        cor[0] = ind - imInfo.sliceSize*cor[2] - imInfo.dimension[0]*cor[1];
        for (dim=0; dim<imInfo.imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + imInfo.dimoffset[dim] * n_x;
                if(cor[dim]+n_x<0 || cor[dim]+n_x >= imInfo.dimension[dim])
                    continue;
                if(statusmap[n_i] == LSMASK_FARAWAY)
                {
#ifdef USESHORTTYPELEVELSETFUNCTION
                    if(phi[n_i] < -5000)
#else
                    if(phi[n_i] < -0.5)
#endif
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



void SparseFieldLevelSetKernel::updatePhi()
{
    float maxPropogating=0;
    int i,j,k;
    removeactivepoints->clear();
    activebands_deltaphi.clear();

    std::vector<int>::iterator iter;
    for(iter=activeband->begin();iter!=activeband->end();iter++)
    {
        int ind= *iter;
        int cor[3];
        cor[2]=ind/imInfo.sliceSize;
        cor[1]=(ind-cor[2]*imInfo.sliceSize)/imInfo.dimension[0];
        cor[0]=ind-cor[2]*imInfo.sliceSize-cor[1]*imInfo.dimension[0];
        float delta_phi = speedFunction->getDetlaPhi(cor);

        if(fabs(delta_phi) > maxPropogating)
        {
            maxPropogating=fabs(delta_phi);
        }
        activebands_deltaphi.push_back(delta_phi);

    }
    float dt = 0.5 / (maxPropogating+1.0e-6);

    if(maxPropogating<0.0001)
    {
        changed=0;
        return;
    }

    std::vector<float>::iterator delta_iter;
    iter=activeband->begin();
    delta_iter = activebands_deltaphi.begin();
    changed=0;

    for( ; iter!=activeband->end() ; iter++, delta_iter++)
    {
        int ind=*iter;

        k=ind/imInfo.sliceSize;
        j=(ind-k*imInfo.sliceSize)/imInfo.dimension[0];
        i=ind-k*imInfo.sliceSize-j*imInfo.dimension[0];
#ifdef USESHORTTYPELEVELSETFUNCTION
        float newvalue=(float)(phi[ind])*0.0001f+dt*(*delta_iter);
#else
        float newvalue=phi[ind]+dt*(*delta_iter);
#endif
        changed++;

        if(newvalue>=0.5)
        {
            int neighbormovingdown=0;
            if(i>0 && statusmap[ind-1]==LSMASK_MOVEDOWN)
                neighbormovingdown=1;
            if(i<imInfo.dimension[0]-1 && statusmap[ind+1]==LSMASK_MOVEDOWN)
                neighbormovingdown=1;
            if(j>0 && statusmap[ind-imInfo.dimension[0]]==LSMASK_MOVEDOWN)
                neighbormovingdown=1;
            if(j<imInfo.dimension[1]-1 && statusmap[ind+imInfo.dimension[0]]==LSMASK_MOVEDOWN)
                neighbormovingdown=1;
            if(k>0 && statusmap[ind-imInfo.sliceSize]==LSMASK_MOVEDOWN)
                neighbormovingdown=1;
            if(k<imInfo.dimension[2]-1 && statusmap[ind+imInfo.sliceSize]==LSMASK_MOVEDOWN)
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
            if(i<imInfo.dimension[0]-1 && statusmap[ind+1]==LSMASK_MOVEUP)
                neighbormovingdown=1;
            if(j>0 && statusmap[ind-imInfo.dimension[0]]==LSMASK_MOVEUP)
                neighbormovingdown=1;
            if(j<imInfo.dimension[1]-1 && statusmap[ind+imInfo.dimension[0]]==LSMASK_MOVEUP)
                neighbormovingdown=1;
            if(k>0 && statusmap[ind-imInfo.sliceSize]==LSMASK_MOVEUP)
                neighbormovingdown=1;
            if(k<imInfo.dimension[2]-1 && statusmap[ind+imInfo.sliceSize]==LSMASK_MOVEUP)
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
#ifdef USESHORTTYPELEVELSETFUNCTION
        phi[ind]=(short)(newvalue*10000);
#else
        phi[ind]=newvalue;
#endif

    }

}


void SparseFieldLevelSetKernel::reinitialize()
{
    std::vector<int>::const_iterator iter;
    neighborband.clear();
    int dim,n_i,n_x;

    //check the neighbors of retired Active Points, promote them to activepoits if proper
    for(iter=removeactivepoints->begin();iter!=removeactivepoints->end();iter++)
    {
        int ind=*iter;
        uchar possiblelayer;
#ifdef USESHORTTYPELEVELSETFUNCTION
        short ds;
#else
        float ds;
#endif
        // we use the active points' moving direction to estimate inner or outer neighbors will be promoted
        if( statusmap[ind] == LSMASK_MOVEUP)
        {
            possiblelayer = LSMASK_IN1NEIGHBOR;
#ifdef USESHORTTYPELEVELSETFUNCTION
            ds = -10000;
#else
            ds = -1.0;
#endif
            statusmap[ind] = LSMASK_OUT1NEIGHBOR;
        }
        else {
            possiblelayer = LSMASK_OUT1NEIGHBOR;
#ifdef USESHORTTYPELEVELSETFUNCTION
            ds = 10000;
#else
            ds = 1.0;
#endif
            statusmap[ind] = LSMASK_IN1NEIGHBOR;
        }
        // scan the neighbors for desired inner or outer layer
        for (dim=0; dim<imInfo.imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + imInfo.dimoffset[dim] * n_x;

                if (n_i<0 || n_i>=imInfo.totalSize) {
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

            for (dim=0; dim<imInfo.imDimension; dim++) {
                for(n_x=-1;n_x<=1;n_x+=2)
                {
                    n_i = ind + imInfo.dimoffset[dim] * n_x;
                    if (n_i<0 || n_i>=imInfo.totalSize) {
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
            for (dim=0; dim<imInfo.imDimension; dim++) {
                for(n_x=-1;n_x<=1;n_x+=2)
                {
                    n_i = ind + imInfo.dimoffset[dim] * n_x;
                    if (n_i<0 || n_i>=imInfo.totalSize) {
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
        for (dim=0; dim<imInfo.imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + imInfo.dimoffset[dim] * n_x;
                if (n_i<0 || n_i>=imInfo.totalSize) {
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
#ifdef USESHORTTYPELEVELSETFUNCTION
        short ds = -10000;
        short tempvalue = -FARAWAY;
#else
        float ds = -1.0;
        float tempvalue = -FARAWAY;
#endif
        for (dim=0; dim<imInfo.imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + imInfo.dimoffset[dim] * n_x;
                if (n_i<0 || n_i>=imInfo.totalSize) {
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
#ifdef USESHORTTYPELEVELSETFUNCTION
        short ds = 10000;
        short tempvalue = FARAWAY;
#else
        float ds = 1.0;
        float tempvalue = FARAWAY;
#endif

        for (dim=0; dim<imInfo.imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + imInfo.dimoffset[dim] * n_x;
                if (n_i<0 || n_i>=imInfo.totalSize) {
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
#ifdef USESHORTTYPELEVELSETFUNCTION
        short ds = 10000;
        short tempvalue = FARAWAY;
#else
        float ds = 1.0;
        float tempvalue = FARAWAY;
#endif
        if(statusmap[ind] == LSMASK_IN2NEIGHBOR)
        {
#ifdef USESHORTTYPELEVELSETFUNCTION
            ds = -10000;
            tempvalue = -FARAWAY;
#else
            ds = -1.0;
            tempvalue = -FARAWAY;
#endif
        }
        for (dim=0; dim<imInfo.imDimension; dim++) {
            for(n_x=-1;n_x<=1;n_x+=2)
            {
                n_i = ind + imInfo.dimoffset[dim] * n_x;
                if (n_i<0 || n_i>=imInfo.totalSize) {
                    continue;
                }
                if(statusmap[n_i] == LSMASK_IN1NEIGHBOR ||statusmap[n_i] == LSMASK_OUT1NEIGHBOR  )
                {
#ifdef USESHORTTYPELEVELSETFUNCTION
                    if(abs(phi[n_i]+ds) < abs(tempvalue))
#else
                    if(fabs(phi[n_i]+ds) < fabs(tempvalue))
#endif
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
