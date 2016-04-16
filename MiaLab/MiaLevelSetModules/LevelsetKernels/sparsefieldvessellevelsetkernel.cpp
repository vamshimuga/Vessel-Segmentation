//#include "sparsefieldvessellevelsetkernel.h"
//#include <omp.h>
//#define max(x,y)    ((x>y) ? x : y )
//#define min(x,y)    ((x<y) ? x : y )
//#define NEIGHBORSIZE 1
//#define LSMASK_FARAWAY       0x00
//#define LSMASK_ACTIVE       0x10
//#define LSMASK_MOVEUP       0x20
//#define LSMASK_MOVEDOWN     0x30
//#define LSMASK_SLEEP		 0x40
//#define LSMASK_WAKEDUP		 0x50
//#define LSMASK_OUT1NEIGHBOR	 0x60
//#define LSMASK_IN1NEIGHBOR	 0x70
//#define LSMASK_CHECKINGIN1NEIGHBOR	 0x80
//#define LSMASK_CHECKINGOUT1NEIGHBOR	 0x90
//#define LSMASK_OUT2NEIGHBOR	 0xa0
//#define LSMASK_IN2NEIGHBOR	 0xb0

//#define LSMASK_READOUTMASK     0xf0

//#define MAXSTEP 0.8

//#define FARAWAY      10.0

//#define USINGOPENMP

//SparseFieldVesselLevelSetKernel::SparseFieldVesselLevelSetKernel(QObject *parent) :
//    AbstractLevelSetKernel(parent)
//{
//    phi = NULL;
//    externalForce = NULL;
//    statusmap = NULL;
//    for( int i = 0 ; i < 3 ; i++ )
//    {
//        dimension[i] = 0;
//        spacing[i] = 0;
//        dimoffset[0] = 0;
//    }
//    totalSize = 0;
//    sliceSize = 0;
//    imageW = 0;
//    imageH = 0;
//    imageD = 0;
//    updateInterval = 50;

//    initializeParameters();

//    activeband = NULL;
//    tempactiveband = NULL;
//    removeactivepoints = NULL;

//    isValid = false;
//    isAbort = false;
//    needRestart = false;
//    imDimension = 3;
//    maxPeriod = 1;
//    maxIteration = 15000;

//    stopPointIndex = -1;

//    connect(this,SIGNAL(levelsetFunctionUpdated()), parent, SLOT(forwardLevelSetFunctionUpdated()));
//    connect(this,SIGNAL(levelsetFunctionCompleted()), parent, SLOT(forwardLevelsetFunctionCompleted()));
//    connect(this,SIGNAL(levelsetEnterNewPeriod(int)), parent, SLOT(forwardLevelsetEnterNewPeriod(int)));
//    setPriority(QThread::HighestPriority);
//}
//void SparseFieldVesselLevelSetKernel::initializeParameters()
//{
//    its = 0;
//    periods = 0;
//    dampingFactor = 0.5;
//    currenttrend = -1;
//    maxiumStepLength = MAXSTEP;
//    initialStepLength = MAXSTEP;
//    externalWeightFactor = 0.5;

//}

//void SparseFieldVesselLevelSetKernel:: run()
//{
//    if(!isValid)
//    {
//        qDebug() << "LevelSet Kernel: input data is not valid";
//        return;
//    }
//    isAbort = false;
//    QTime t;
//    t.start();
//    recreateActiveBand();
//    virutaltime = 0;
//    totalComputation = 0;
//    averageForce = 0;
//    initialExternalForceFactor = externalWeightFactor;
//    for( ; its < maxIteration ; its ++ )
//    {
//        if(isAbort)
//            break;
//        if(needRestart)
//        {
//            initializeParameters();
//            recreateActiveBand();
//            needRestart = false;
//            qDebug()<<"level set restarted";
//        }
//        int bandsize = activeband->size();
//#ifdef USINGOPENMP
//        if(bandsize < 3000)
//        {
////        t.restart();
//            reinitialize();
////        qDebug() << "level set reinit: time elapsed: " << t.elapsed();
////        t.restart();
//            update3DphiUsingThreshold();
////        qDebug() << "level set update: time elapsed: " << t.elapsed();
//        }
//        else
//        {
//            reinitializet_multi_threaded();
//            update3DphiUsingThreshold_multi_threaded();
//        }
//#else
//        reinitialize();
////        qDebug() << "level set reinit: time elapsed: " << t.elapsed();
////        t.restart();
//        update3DphiUsingThreshold();
//#endif
////        switch(currentAlgorithm)
////        {
////        case MIA_ThresholdBased:
////            if(imDimension == 3)
////                update3DphiUsingThreshold();

////            else
////                update2DphiUsingThreshold();
////            break;

////        case MIA_GradientBased:
////            updatephiUsingGradient();
////            break;
////        default:
////            qDebug() << "Unknow algorithm \n";
////            break;

////        }



//        if(changed==0 || (stopPointIndex!= -1 && phi[stopPointIndex]*currenttrend>0.5) || its == maxIteration-1)
//        {
//            currenttrend=-currenttrend;
//            periods++;
//            maxiumStepLength *= dampingFactor;
//            qDebug() << "LevelSet Changing period" << its << "viruatl time" << virutaltime << totalComputation << "force" << averageForce/its;;
//            qDebug() << "level set hanging period, time elapsed: " << t.elapsed();
//            if(periods >= maxPeriod )
//                    its=maxIteration;
//            else {
//                    virutaltime = 0;
//                    its=1; // to avoid sent update signal
//                    emit levelsetEnterNewPeriod(periods);

//                    recreateActiveBand();
//            }
//            externalWeightFactor = initialExternalForceFactor;

//        }

//        if(its%updateInterval==0)
//        {
//            emit levelsetFunctionUpdated();
////            qDebug() << "LevelSet Updated";
//        }
//        if(periods%2 == 0 && its%5 == 0)
//        {
//            if(externalWeightFactor>0.2)
//                externalWeightFactor -= 0.01;
////            if(maxiumStepLength<0.8)
////                maxiumStepLength += 0.005;
//        }


//    }
//    reinitialize();
//    emit levelsetFunctionUpdated();
//    emit levelsetFunctionCompleted();
//    qDebug() << "LevelSet Finished" << virutaltime << totalComputation << "force" << averageForce/its;;;
//    qDebug() << "level set total running time: " << t.elapsed();
//    initializeParameters();


//}


//void SparseFieldVesselLevelSetKernel::recreateActiveBand()
//{

//#ifdef USINGOPENMP
//    int index;
//    int nthreads = omp_get_max_threads();
//    for(index=0; index < nthreads; index++)
//    {
//        tempband[index].clear();
//    }
//    activeband->clear();
//    int i;
//#pragma omp parallel for
//    for (i=0; i<totalSize; i++) {
//            if (statusmap[i] == LSMASK_SLEEP || statusmap[i] == LSMASK_ACTIVE) {
//                    tempband[omp_get_thread_num()].push_back(i);
//                    statusmap[i] = LSMASK_ACTIVE;
//            }

//    }
//    for(index=0; index < nthreads; index++)
//    {
//        activeband->insert(activeband->end(),tempband[index].begin(), tempband[index].end());
//    }
//#else
//    int i;
//    activeband->clear();
//    for (i=0; i<totalSize; i++) {
//            if (statusmap[i] == LSMASK_SLEEP || statusmap[i] == LSMASK_ACTIVE) {
//                    activeband->push_back(i);
//                    statusmap[i] = LSMASK_ACTIVE;
//            }

//    }
//#endif
//    qDebug()<<activeband->size();
//}

//void SparseFieldVesselLevelSetKernel::abort()
//{
//    if(!isValid)
//    {
//        qDebug() << "LevelSet Kernel: input data is not valid";
//        return;
//    }

//    isAbort = true;

//}

//void SparseFieldVesselLevelSetKernel::resume()
//{
//    if(!isValid)
//    {
//        qDebug() << "LevelSet Kernel: input data is not valid";
//        return;
//    }
//    if(its < maxIteration)
//        start();
//}

//void SparseFieldVesselLevelSetKernel::restart()
//{
//    needRestart = true;
//}

//void SparseFieldVesselLevelSetKernel::setStopPoint(int ind)
//{
//    stopPointIndex = ind;
//}

//bool SparseFieldVesselLevelSetKernel::validation()
//{
//    if(!phi || !externalForce || ! statusmap || imageW < 3 || imageH < 3 ||imageD < 1 )
//        return false;
//    return true;
//}

//bool SparseFieldVesselLevelSetKernel::initialize( float* p, float* ext, uchar* smap, int* d, float* s)
//{
//    //set up the play ground
//    phi = p;
//    externalForce = ext;
//    statusmap = smap;
//    totalSize = 1;
//    for( int i = 0 ; i < 3 ; i++ )
//    {
//        dimension[i] = d[i];
//        totalSize *= dimension[i];
//        spacing[i] = s[i];
//    }
//    if(d[2] ==1 )
//        imDimension = 2;
//    else
//        imDimension = 3;





//    its = 0;
//    periods = 0;
//    maxiumStepLength = initialStepLength;
//    currenttrend = -1;

//    imageW = dimension[0];
//    imageH = dimension[1];
//    imageD = dimension[2];
//    sliceSize = imageW*imageH;

////    //for test
////    imDimension = 2;
////    phi = p + sliceSize*(int)(imageD/2);
////    externalForce = ext + sliceSize*(int)(imageD/2);
////    totalSize /= dimension[2];
////    imageD = dimension[2] = 1;

//    dimoffset[0] = 1;
//    dimoffset[1] = imageW;
//    dimoffset[2] = sliceSize = imageW*imageH;;



//    //initialize
//    activeband1.clear();
//    activeband2.clear();
//    activeband1.reserve(totalSize/10);
//    activeband2.reserve(totalSize/10);
//    activeband = &activeband1;
//    removeactivepoints = &activeband2;

//    activebands_deltaphi.clear();
//    activebands_deltaphi.resize(totalSize/10);

//    int index_iter;
//    int nthreads = omp_get_max_threads();
//    for(index_iter=0; index_iter < nthreads; index_iter++)
//    {
//        tempband[index_iter].reserve(totalSize/10);
//    }
//    qDebug() << "max thread" << nthreads;
//    neighborband.clear();
//    firstlayerneighbor.clear();
//    neighborband.reserve(totalSize/5);
//    firstlayerneighbor.reserve(totalSize/10);

//    isValid = validation();

//    if(!isValid)
//    {
//        qDebug() << "LevelSet Kernel: input data is not valid";
//        return false;
//    }

//    //initial active band

//    memset(statusmap,LSMASK_FARAWAY,totalSize*sizeof(uchar));

//    int i;
//    for(i = 0 ; i < totalSize ; i++)
//    {

//            if(phi[i]>=-0.5&&phi[i]<0.5)
//            {

//                    activeband->push_back(i);
//                    statusmap[i]=LSMASK_ACTIVE;
////                    passedmap[i]++;

//            }
//            else
//            {
//                    if(phi[i]<-5.0)
//                            phi[i]=-FARAWAY;
//                    else if(phi[i]>5.0)
//                            phi[i]=FARAWAY;
//            }
//    }


//    // initialize statusmap
//    int dim,n_x,n_i;
//    int cor[3];

//    int narrowbandsize=activeband->size();
//    int index;
//    for(index = 0 ; index < narrowbandsize ;index++)
//    {

//            int ind = (*activeband)[index];
//            cor[2] = ind/(imageW*imageH);
//            cor[1] = (ind-imageW*imageH*cor[2])/imageW;
//            cor[0] = ind - imageW*imageH*cor[2] - imageW*cor[1];
//            for (dim=0; dim<imDimension; dim++) {
//                    for(n_x=-1;n_x<=1;n_x+=2)
//                    {
//                            n_i = ind + dimoffset[dim] * n_x;
//                            if(cor[dim]+n_x<0 || cor[dim]+n_x >= dimension[dim])
//                                continue;
//                            if(statusmap[n_i] == LSMASK_FARAWAY)
//                            {
//                                    if(phi[n_i] < -0.5)
//                                            statusmap[n_i] = LSMASK_IN1NEIGHBOR;
//                                    else {
//                                            statusmap[n_i] = LSMASK_OUT1NEIGHBOR;
//                                    }
//                            }
//                    }
//            }
//    }


//    return true;
//}
//void SparseFieldVesselLevelSetKernel::update3DphiUsingGradient()
//{
//    float maxPropogating=0;
//    float dx,dy,dz;
//    float dxplus, dyplus, dzplus, dxminus, dyminus, dzminus;
//    float dxplusy, dxminusy, dxplusz, dxminusz, dyplusx, dyminusx, dyplusz, dyminusz, dzplusx, dzminusx, dzplusy, dzminusy;
//    float gradphimax, gradphimin, nplusx, nplusy, nplusz, nminusx, nminusy, nminusz, curvature;
//    float delta_phi;
//    int ind;
//    int i,j,k;
//    removeactivepoints->clear();
//    activebands_deltaphi.clear();
////    int narrowbandsize=activeband->size();
//    std::vector<int>::iterator iter;
//    for(iter=activeband->begin();iter!=activeband->end();iter++)
//    {
//        ind=*iter;

////        if(statusmap[ind]==LSMASK_SLEEP)
////        {
////            activebands_deltaphi.push_back(0);
////            continue;
////        }
//        //it is no need to check sleep points as sleep points are exclude from reinitialization

//        k=ind/sliceSize;
//        j=(ind-k*sliceSize)/imageW;
//        i=ind-k*sliceSize-j*imageW;

//        if(i==0||i==imageW-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
//        if(j==0||j==imageH-1){dy=0;} else {dy=(phi[ind-imageW]-phi[ind+imageW])/2;}
//        if(k==0||k==imageD-1){dz=0;} else {dz=(phi[ind+sliceSize]-phi[ind-sliceSize])/2;}

//        if(i==imageW-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
//        if(j==0){dyplus=0;} else {dyplus=(phi[ind-imageW]-phi[ind]);}
//        if(k==imageD-1){dzplus=0;} else {dzplus=(phi[ind+sliceSize]-phi[ind]);}
//        if(i==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
//        if(j==imageH-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imageW]);}
//        if(k==0){dzminus=0;} else {dzminus=(phi[ind]-phi[ind-sliceSize]);}

//        if(i==0||i==imageW-1||j==0){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
//        if(i==0||i==imageW-1||j==imageH-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
//        if(i==0||i==imageW-1||k==imageD-1) {dxplusz=0;} else {dxplusz=(phi[ind+sliceSize+1]-phi[ind+sliceSize-1])/2;}
//        if(i==0||i==imageW-1||k==0) {dxminusz=0;} else {dxminusz=(phi[ind-sliceSize+1]-phi[ind-sliceSize-1])/2;}
//        if(j==0||j==imageH-1||i==imageW-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
//        if(j==0||j==imageH-1||i==0){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
//        if(j==0||j==imageH-1||k==imageD-1) {dyplusz=0;} else {dyplusz=(phi[ind+sliceSize-imageW]-phi[ind+sliceSize+imageW])/2;}
//        if(j==0||j==imageH-1||k==0) {dyminusz=0;} else {dyminusz=(phi[ind-sliceSize-imageW]-phi[ind-sliceSize+imageW])/2;}
//        if(k==0||k==imageD-1||i==imageW-1) {dzplusx=0;} else {dzplusx=(phi[ind+1+sliceSize]-phi[ind+1-sliceSize])/2;}
//        if(k==0||k==imageD-1||i==0) {dzminusx=0;} else {dzminusx=(phi[ind-1+sliceSize]-phi[ind-1-sliceSize])/2;}
//        if(k==0||k==imageD-1||j==0) {dzplusy=0;} else {dzplusy=(phi[ind-imageW+sliceSize]-phi[ind-imageW-sliceSize])/2;}
//        if(k==0||k==imageD-1||j==imageH-1) {dzminusy=0;} else {dzminusy=(phi[ind+imageW+sliceSize]-phi[ind+imageW-sliceSize])/2;}


//        gradphimax=sqrt((sqrt(max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)))*(sqrt(max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)))
//                                        +(sqrt(max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)))*(sqrt(max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)))
//                                        +(sqrt(max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0)))*(sqrt(max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0))));

//        gradphimin=sqrt((sqrt(min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)))*(sqrt(min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)))
//                                        +(sqrt(min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)))*(sqrt(min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)))
//                                        +(sqrt(min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0)))*(sqrt(min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0))));

//        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) + ((dzplusx + dz)*(dzplusx + dz)*0.25));
//        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) + ((dzplusy + dz)*(dzplusy + dz)*0.25));
//        nplusz= dzplus / sqrt(1.192092896e-07F + (dzplus*dzplus) + ((dxplusz + dz)*(dxplusz + dz)*0.25) + ((dyplusz + dy)*(dyplusz + dy)*0.25));

//        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) + ((dzminusx + dz)*(dzminusx + dz)*0.25));
//        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) + ((dzminusy + dz)*(dzminusy + dz)*0.25));
//        nminusz= dzminus / sqrt(1.192092896e-07F + (dzminus*dzminus) + ((dxminusz + dz)*(dxminusz + dz)*0.25) + ((dyminusz + dy)*(dyminusz + dy)*0.25));

//        curvature= ((nplusx-nminusx)+(nplusy-nminusy)+(nplusz-nminusz))/2;

//        float gradient[3];
//        float tempg = externalForce[ind];
//        float temp1 = externalForce[ind-1];
//        float temp2 = externalForce[ind+1];
//        float temp3 = externalForce[ind-imageW];
//        float temp4 = externalForce[ind+imageW];
//        float temp5 = externalForce[ind-sliceSize];
//        float temp6 = externalForce[ind+sliceSize];
//        if(i == 0)
//            gradient[0]= externalForce[ind]- externalForce[ind+1];
//        else if(i == imageW-1)
//            gradient[0]= externalForce[ind-1]- externalForce[ind];
//        else
//            gradient[0]= externalForce[ind-1]- externalForce[ind+1];

//        if(j == 0)
//            gradient[1]= externalForce[ind]- externalForce[ind+imageW];
//        else if(j == imageH-1)
//            gradient[1]= externalForce[ind-imageW]- externalForce[ind];
//        else
//            gradient[1]= externalForce[ind-imageW]- externalForce[ind+imageW];

//        if(k == 0)
//            gradient[2]= externalForce[ind]- externalForce[ind+sliceSize];
//        else if(k == imageD-1)
//            gradient[2]= externalForce[ind-sliceSize]- externalForce[ind];
//        else
//            gradient[2]= externalForce[ind-sliceSize]- externalForce[ind+sliceSize];

//        float advection = gradient[0]*dx + gradient[1]*dy + gradient[2]*dz;

//        if(advection<0)
//            advection = 0;
////        delta_phi = (-externalWeightFactor * externalForce[ind]) + ((1-externalWeightFactor) * curvature) + advection*15;
//        delta_phi = (-externalWeightFactor  + ((1-externalWeightFactor) * curvature))* externalForce[ind]+ advection*15;
//        if(delta_phi > 0)
//            delta_phi = delta_phi * gradphimax;
//        else
//            delta_phi = delta_phi * gradphimin;

//        if(currenttrend*delta_phi < 0)
//        {
//            delta_phi = 0;
//        }
//        if (fabs(delta_phi) < 0.00001) {
//            statusmap[ind] = LSMASK_SLEEP;
//        }
//        else if(fabs(delta_phi) > maxPropogating)
//        {
//            maxPropogating=fabs(delta_phi);
//        }
//        activebands_deltaphi.push_back(delta_phi);

//    }
//    float dt = maxiumStepLength / (maxPropogating+1.0e-6);

//    if(maxPropogating<0.0001)
//    {
//        changed=0;
//        return;
//    }

//    float newvalue=0;
//    std::vector<float>::iterator delta_iter;

//    iter=activeband->begin();
//    delta_iter = activebands_deltaphi.begin();
//    changed=0;

//    for( ; iter!=activeband->end() ; iter++, delta_iter++)
//    {
//        int ind=*iter;
//        if(statusmap[ind]==LSMASK_SLEEP)
//        {
//            continue;
//        }

//        k=ind/sliceSize;
//        j=(ind-k*sliceSize)/imageW;
//        i=ind-k*sliceSize-j*imageW;

//        newvalue=phi[ind]+dt*(*delta_iter);
//        changed++;

//        if(newvalue>=0.5)
//        {
//            int neighbormovingdown=0;
//            if(i>0 && statusmap[ind-1]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(i<imageW-1 && statusmap[ind+1]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(j>0 && statusmap[ind-imageW]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(j<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(k>0 && statusmap[ind-sliceSize]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(k<imageD-1 && statusmap[ind+sliceSize]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(!neighbormovingdown)
//            {
//                statusmap[ind]=LSMASK_MOVEUP;
//                removeactivepoints->push_back(ind);
//            }
//            else
//            {
//                continue;
//            }
//        }
//        if(newvalue<-0.5)
//        {
//            int neighbormovingdown=0;
//            if(i>0 && statusmap[ind-1]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(i<imageW-1 && statusmap[ind+1]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(j>0 && statusmap[ind-imageW]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(j<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(k>0 && statusmap[ind-sliceSize]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(k<imageD-1 && statusmap[ind+sliceSize]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;

//            if(!neighbormovingdown)
//            {
//                statusmap[ind]=LSMASK_MOVEDOWN;
//                removeactivepoints->push_back(ind);
//            }
//            else
//            {
//                continue;
//            }
//        }

//        phi[ind]=newvalue;

//    }

//}
//void SparseFieldVesselLevelSetKernel::update2DphiUsingGradient()
//{
//    float maxPropogating=0;
//    float dx,dy;
//    float dxplus, dyplus, dxminus, dyminus;
//    float dxplusy, dxminusy, dyplusx, dyminusx;
//    float maxdxplus, mindxplus, maxdyplus, mindyplus, maxminusdxminus, minminusdxminus, maxminusdyminus, minminusdyminus;
//    float gradphimax, gradphimin, nplusx, nplusy, nminusx, nminusy, curvature;
//    float delta_phi;
//    int ind;
//    int r, c;
//    removeactivepoints->clear();
//    activebands_deltaphi.clear();
////    int narrowbandsize=activeband->size();
//    std::vector<int>::iterator iter;
//    totalComputation += activeband->size();

//    for(iter=activeband->begin();iter!=activeband->end();iter++)
//    {
//        ind=*iter;
//        r=ind/imageW;
//        c=ind-r*imageW;

//        if(c==0||c==imageW-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
//        if(c==imageW-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
//        if(c==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
//        if(r==0||c==0||c==imageW-1){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
//        if(r==imageH-1||c==0||c==imageW-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
//        if(dxplus<0){maxdxplus=0;} else { maxdxplus= dxplus*dxplus; }
//        if(-dxminus<0){maxminusdxminus=0;} else { maxminusdxminus= dxminus*dxminus; }
//        if(dxplus>0){mindxplus=0;} else { mindxplus= dxplus*dxplus; }
//        if(-dxminus>0){minminusdxminus=0;} else { minminusdxminus= dxminus*dxminus; }

//        if(r==0||r==imageH-1){dy=0;} else {dy=(phi[ind-imageW]-phi[ind+imageW])/2;}
//        if(r==0){dyplus=0;} else {dyplus=(phi[ind-imageW]-phi[ind]);}
//        if(r==imageH-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imageW]);}
//        if(r==0||c==imageW-1||r==imageH-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
//        if(r==0||c==0||r==imageH-1){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
//        if(dyplus<0){maxdyplus=0;} else { maxdyplus= dyplus*dyplus; }
//        if(-dyminus<0){maxminusdyminus=0;} else { maxminusdyminus= dyminus*dyminus; }
//        if(dyplus>0){mindyplus=0;} else { mindyplus= dyplus*dyplus; }
//        if(-dyminus>0){minminusdyminus=0;} else { minminusdyminus= dyminus*dyminus; }

//        gradphimax=sqrt((sqrt(maxdxplus+maxminusdxminus))*(sqrt(maxdxplus+maxminusdxminus))+(sqrt(maxdyplus+maxminusdyminus))*(sqrt(maxdyplus+maxminusdyminus)));
//        gradphimin=sqrt((sqrt(mindxplus+minminusdxminus))*(sqrt(mindxplus+minminusdxminus))+(sqrt(mindyplus+minminusdyminus))*(sqrt(mindyplus+minminusdyminus)));
//        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) );
//        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) );
//        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) );
//        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) );
//        curvature= ((nplusx-nminusx)+(nplusy-nminusy))/2;


//        delta_phi = (-externalWeightFactor * externalForce[ind]) + ((1-externalWeightFactor) * curvature);
////        if(delta_phi > 0)
////            delta_phi = delta_phi * gradphimax;
////        else
////            delta_phi = delta_phi * gradphimin;

//        if(currenttrend*delta_phi < 0)
//        {
//            delta_phi = 0;
//        }
//        if (fabs(delta_phi) < 0.00001) {
//            statusmap[ind] = LSMASK_SLEEP;
//        }
//        else if(fabs(delta_phi) > maxPropogating)
//        {
//            maxPropogating=fabs(delta_phi);
//        }
//        activebands_deltaphi.push_back(delta_phi);

//    }
//    float dt = maxiumStepLength / (maxPropogating+1.0e-6);



//    if(maxPropogating<0.0001)
//    {
//        changed=0;
//        return;
//    }
//    virutaltime += dt;
//    averageForce +=maxPropogating;

//    float newvalue=0;
//    std::vector<float>::iterator delta_iter;

//    iter=activeband->begin();
//    delta_iter = activebands_deltaphi.begin();
//    changed=0;

//    for( ; iter!=activeband->end() ; iter++, delta_iter++)
//    {
//        int ind=*iter;
//        if(statusmap[ind]==LSMASK_SLEEP)
//        {
//            continue;
//        }
//        r=ind/imageW;
//        c=ind-r*imageW;

//        newvalue=phi[ind]+dt*(*delta_iter);
//        changed++;

//        if(newvalue>=0.5)
//        {
//            int neighbormovingdown=0;
//            if(c>0 && statusmap[ind-1]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(c<imageW-1 && statusmap[ind+1]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(r>0 && statusmap[ind-imageW]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(r<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;

//            if(!neighbormovingdown)
//            {
//                statusmap[ind]=LSMASK_MOVEUP;
//                removeactivepoints->push_back(ind);
//            }
//            else
//            {
//                continue;
//            }
//        }
//        if(newvalue<-0.5)
//        {
//            int neighbormovingdown=0;
//            if(c>0 && statusmap[ind-1]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(c<imageW-1 && statusmap[ind+1]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(r>0 && statusmap[ind-imageW]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(r<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;

//            if(!neighbormovingdown)
//            {
//                statusmap[ind]=LSMASK_MOVEDOWN;
//                removeactivepoints->push_back(ind);
//            }
//            else
//            {
//                continue;
//            }
//        }

//        phi[ind]=newvalue;

//    }

//}
////void SparseFieldVesselLevelSetKernel::updatephiUsingThreshold()
////{
////    float maxPropogating=0;
////    float dx[3];
////    float dx_plus[3], dx_minus[3];
////    float dxplusy, dxminusy, dxplusz, dxminusz, dyplusx, dyminusx, dyplusz, dyminusz, dzplusx, dzminusx, dzplusy, dzminusy;
////    float gradphimax, gradphimin, nplusx, nplusy, nplusz, nminusx, nminusy, nminusz, curvature;
////    float delta_phi;
////    int ind;
////    int dim,j,k;
////    int index[3];
////    int dim;
////    removeactivepoints->clear();
////    activebands_deltaphi.clear();
//////    int narrowbandsize=activeband->size();
////    std::vector<int>::iterator iter;
////    for(iter=activeband->begin();iter!=activeband->end();iter++)
////    {
////        ind=*iter;

//////        if(statusmap[ind]==LSMASK_SLEEP)
//////        {
//////            activebands_deltaphi.push_back(0);
//////            continue;
//////        }
////        //it is no need to check sleep points as sleep points are exclude from reinitialization

////        index[2] = ind/sliceSize;
////        index[1] = (ind-k*sliceSize)/imageW;
////        index[0] = ind-k*sliceSize-j*imageW;

////        gd->m_GradMagSqr = 1.0e-6;
////        for( dim = 0 ; dim < imDimension; dim++)
////        {
////            ind_a = ind + dimoffset[dim];
////            ind_b = ind - dimoffset[dim];
////            if( index[dim] == 0 || index[dim] == dimension[dim]-1 ){d[dim]=0;} else {dx[dim]=(phi[ind_a]-phi[ind_b])*0.5;}
////            if( index[dim] == 0 ) dx_minus[dim] = 0; else dx_minus[dim] = phi[ind]-phi[ind_b];
////            if( index[dim] == dimension[dim]-1 ) dx_plus[dim] =0; else dx_plus[dim] = phi[ind_a]-phi[ind];

////            for(dim_i = 0; dim_i < imDimension; dim_i++ )
////            {
////                if(dim == dim_i)
////                    continue;


////            }

////                gd->m_dxy[dim][dim] = it.GetPixel( positionA )
////                + it.GetPixel( positionB ) - 2.0 * center_value;

////                gd->m_dx_forward[dim]  = it.GetPixel( positionA ) - center_value;
////                gd->m_dx_backward[dim] = center_value - it.GetPixel( positionB );
////                gd->m_GradMagSqr += gd->m_dx[dim] * gd->m_dx[dim];

////                for( j = dim+1; j < ImageDimension; j++ )
////                {
////                        const unsigned int positionAa = static_cast<unsigned int>(
////                                                                                                                                          m_Center - m_xStride[dim] - m_xStride[j] );
////                        const unsigned int positionBa = static_cast<unsigned int>(
////                                                                                                                                          m_Center - m_xStride[dim] + m_xStride[j] );
////                        const unsigned int positionCa = static_cast<unsigned int>(
////                                                                                                                                          m_Center + m_xStride[dim] - m_xStride[j] );
////                        const unsigned int positionDa = static_cast<unsigned int>(
////                                                                                                                                          m_Center + m_xStride[dim] + m_xStride[j] );

////                        gd->m_dxy[dim][j] = gd->m_dxy[j][dim] = 0.25 *( it.GetPixel( positionAa )
////                                                                                                           - it.GetPixel( positionBa )
////                                                                                                           - it.GetPixel( positionCa )
////                                                                                                           + it.GetPixel( positionDa )
////                                                                                                           );
////                }
////        }


////        if(i==0||i==imageW-1||j==0){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
////        if(i==0||i==imageW-1||j==imageH-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
////        if(i==0||i==imageW-1||k==imageD-1) {dxplusz=0;} else {dxplusz=(phi[ind+sliceSize+1]-phi[ind+sliceSize-1])/2;}
////        if(i==0||i==imageW-1||k==0) {dxminusz=0;} else {dxminusz=(phi[ind-sliceSize+1]-phi[ind-sliceSize-1])/2;}
////        if(j==0||j==imageH-1||i==imageW-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
////        if(j==0||j==imageH-1||i==0){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
////        if(j==0||j==imageH-1||k==imageD-1) {dyplusz=0;} else {dyplusz=(phi[ind+sliceSize-imageW]-phi[ind+sliceSize+imageW])/2;}
////        if(j==0||j==imageH-1||k==0) {dyminusz=0;} else {dyminusz=(phi[ind-sliceSize-imageW]-phi[ind-sliceSize+imageW])/2;}
////        if(k==0||k==imageD-1||i==imageW-1) {dzplusx=0;} else {dzplusx=(phi[ind+1+sliceSize]-phi[ind+1-sliceSize])/2;}
////        if(k==0||k==imageD-1||i==0) {dzminusx=0;} else {dzminusx=(phi[ind-1+sliceSize]-phi[ind-1-sliceSize])/2;}
////        if(k==0||k==imageD-1||j==0) {dzplusy=0;} else {dzplusy=(phi[ind-imageW+sliceSize]-phi[ind-imageW-sliceSize])/2;}
////        if(k==0||k==imageD-1||j==imageH-1) {dzminusy=0;} else {dzminusy=(phi[ind+imageW+sliceSize]-phi[ind+imageW-sliceSize])/2;}


////        gradphimax=sqrt((sqrt(max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)))*(sqrt(max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)))
////                                        +(sqrt(max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)))*(sqrt(max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)))
////                                        +(sqrt(max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0)))*(sqrt(max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0))));

////        gradphimin=sqrt((sqrt(min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)))*(sqrt(min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)))
////                                        +(sqrt(min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)))*(sqrt(min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)))
////                                        +(sqrt(min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0)))*(sqrt(min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0))));

////        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) + ((dzplusx + dz)*(dzplusx + dz)*0.25));
////        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) + ((dzplusy + dz)*(dzplusy + dz)*0.25));
////        nplusz= dzplus / sqrt(1.192092896e-07F + (dzplus*dzplus) + ((dxplusz + dz)*(dxplusz + dz)*0.25) + ((dyplusz + dy)*(dyplusz + dy)*0.25));

////        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) + ((dzminusx + dz)*(dzminusx + dz)*0.25));
////        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) + ((dzminusy + dz)*(dzminusy + dz)*0.25));
////        nminusz= dzminus / sqrt(1.192092896e-07F + (dzminus*dzminus) + ((dxminusz + dz)*(dxminusz + dz)*0.25) + ((dyminusz + dy)*(dyminusz + dy)*0.25));

////        curvature= ((nplusx-nminusx)+(nplusy-nminusy)+(nplusz-nminusz))/2;

////        delta_phi = (-externalWeightFactor * externalForce[ind]) + ((1-externalWeightFactor) * curvature);
////        if(delta_phi > 0)
////            delta_phi = delta_phi * gradphimax;
////        else
////            delta_phi = delta_phi * gradphimin;

////        if(currenttrend*delta_phi < 0)
////        {
////            delta_phi = 0;
////        }
////        if (fabs(delta_phi) < 0.00001) {
////            statusmap[ind] = LSMASK_SLEEP;
////        }
////        else if(fabs(delta_phi) > maxPropogating)
////        {
////            maxPropogating=fabs(delta_phi);
////        }
////        activebands_deltaphi.push_back(delta_phi);

////    }
////    float dt = maxiumStepLength / (maxPropogating+1.0e-6);

////    if(maxPropogating<0.0001)
////    {
////        changed=0;
////        return;
////    }

////    float newvalue=0;
////    std::vector<float>::iterator delta_iter;

////    iter=activeband->begin();
////    delta_iter = activebands_deltaphi.begin();
////    changed=0;

////    for( ; iter!=activeband->end() ; iter++, delta_iter++)
////    {
////        int ind=*iter;
////        if(statusmap[ind]==LSMASK_SLEEP)
////        {
////            continue;
////        }

////        k=ind/sliceSize;
////        j=(ind-k*sliceSize)/imageW;
////        dim=ind-k*sliceSize-j*imageW;

////        newvalue=phi[ind]+dt*(*delta_iter);
////        changed++;

////        if(newvalue>=0.5)
////        {
////            int neighbormovingdown=0;
////            if(dim>0 && statusmap[ind-1]==LSMASK_MOVEDOWN)
////                 neighbormovingdown=1;
////            if(dim<imageW-1 && statusmap[ind+1]==LSMASK_MOVEDOWN)
////                 neighbormovingdown=1;
////            if(j>0 && statusmap[ind-imageW]==LSMASK_MOVEDOWN)
////                 neighbormovingdown=1;
////            if(j<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEDOWN)
////                 neighbormovingdown=1;
////            if(k>0 && statusmap[ind-sliceSize]==LSMASK_MOVEDOWN)
////                 neighbormovingdown=1;
////            if(k<imageD-1 && statusmap[ind+sliceSize]==LSMASK_MOVEDOWN)
////                 neighbormovingdown=1;
////            if(!neighbormovingdown)
////            {
////                statusmap[ind]=LSMASK_MOVEUP;
////                removeactivepoints->push_back(ind);
////            }
////            else
////            {
////                continue;
////            }
////        }
////        if(newvalue<-0.5)
////        {
////            int neighbormovingdown=0;
////            if(dim>0 && statusmap[ind-1]==LSMASK_MOVEUP)
////                 neighbormovingdown=1;
////            if(dim<imageW-1 && statusmap[ind+1]==LSMASK_MOVEUP)
////                 neighbormovingdown=1;
////            if(j>0 && statusmap[ind-imageW]==LSMASK_MOVEUP)
////                 neighbormovingdown=1;
////            if(j<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEUP)
////                 neighbormovingdown=1;
////            if(k>0 && statusmap[ind-sliceSize]==LSMASK_MOVEUP)
////                 neighbormovingdown=1;
////            if(k<imageD-1 && statusmap[ind+sliceSize]==LSMASK_MOVEUP)
////                 neighbormovingdown=1;

////            if(!neighbormovingdown)
////            {
////                statusmap[ind]=LSMASK_MOVEDOWN;
////                removeactivepoints->push_back(ind);
////            }
////            else
////            {
////                continue;
////            }
////        }

////        phi[ind]=newvalue;

////    }

////}

//void SparseFieldVesselLevelSetKernel::update2DphiUsingThreshold()
//{
//    float maxPropogating=0;
//    float dx,dy;
//    float dxplus, dyplus, dxminus, dyminus;
//    float dxplusy, dxminusy, dyplusx, dyminusx;
//    float maxdxplus, mindxplus, maxdyplus, mindyplus, maxminusdxminus, minminusdxminus, maxminusdyminus, minminusdyminus;
//    float gradphimax, gradphimin, nplusx, nplusy, nminusx, nminusy, curvature;
//    float delta_phi;
//    int ind;
//    int r, c;
//    removeactivepoints->clear();
//    activebands_deltaphi.clear();
////    int narrowbandsize=activeband->size();
//    std::vector<int>::iterator iter;
//    totalComputation += activeband->size();

//    for(iter=activeband->begin();iter!=activeband->end();iter++)
//    {
//        ind=*iter;
//        r=ind/imageW;
//        c=ind-r*imageW;

//        if(c==0||c==imageW-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
//        if(c==imageW-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
//        if(c==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
//        if(r==0||c==0||c==imageW-1){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
//        if(r==imageH-1||c==0||c==imageW-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
//        if(dxplus<0){maxdxplus=0;} else { maxdxplus= dxplus*dxplus; }
//        if(-dxminus<0){maxminusdxminus=0;} else { maxminusdxminus= dxminus*dxminus; }
//        if(dxplus>0){mindxplus=0;} else { mindxplus= dxplus*dxplus; }
//        if(-dxminus>0){minminusdxminus=0;} else { minminusdxminus= dxminus*dxminus; }

//        if(r==0||r==imageH-1){dy=0;} else {dy=(phi[ind-imageW]-phi[ind+imageW])/2;}
//        if(r==0){dyplus=0;} else {dyplus=(phi[ind-imageW]-phi[ind]);}
//        if(r==imageH-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imageW]);}
//        if(r==0||c==imageW-1||r==imageH-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
//        if(r==0||c==0||r==imageH-1){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
//        if(dyplus<0){maxdyplus=0;} else { maxdyplus= dyplus*dyplus; }
//        if(-dyminus<0){maxminusdyminus=0;} else { maxminusdyminus= dyminus*dyminus; }
//        if(dyplus>0){mindyplus=0;} else { mindyplus= dyplus*dyplus; }
//        if(-dyminus>0){minminusdyminus=0;} else { minminusdyminus= dyminus*dyminus; }

//        gradphimax=sqrt((sqrt(maxdxplus+maxminusdxminus))*(sqrt(maxdxplus+maxminusdxminus))+(sqrt(maxdyplus+maxminusdyminus))*(sqrt(maxdyplus+maxminusdyminus)));
//        gradphimin=sqrt((sqrt(mindxplus+minminusdxminus))*(sqrt(mindxplus+minminusdxminus))+(sqrt(mindyplus+minminusdyminus))*(sqrt(mindyplus+minminusdyminus)));
//        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) );
//        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) );
//        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) );
//        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) );
//        curvature= ((nplusx-nminusx)+(nplusy-nminusy))/2;


//        delta_phi = (-externalWeightFactor * externalForce[ind]) + ((1-externalWeightFactor) * curvature);
////        if(delta_phi > 0)
////            delta_phi = delta_phi * gradphimax;
////        else
////            delta_phi = delta_phi * gradphimin;

//        if(currenttrend*delta_phi < 0)
//        {
//            delta_phi = 0;
//        }
//        if (fabs(delta_phi) < 0.00001) {
//            statusmap[ind] = LSMASK_SLEEP;
//        }
//        else if(fabs(delta_phi) > maxPropogating)
//        {
//            maxPropogating=fabs(delta_phi);
//        }
//        activebands_deltaphi.push_back(delta_phi);

//    }
//    float dt = maxiumStepLength / (maxPropogating+1.0e-6);



//    if(maxPropogating<0.0001)
//    {
//        changed=0;
//        return;
//    }
//    virutaltime += dt;
//    averageForce +=maxPropogating;

//    float newvalue=0;
//    std::vector<float>::iterator delta_iter;

//    iter=activeband->begin();
//    delta_iter = activebands_deltaphi.begin();
//    changed=0;

//    for( ; iter!=activeband->end() ; iter++, delta_iter++)
//    {
//        int ind=*iter;
//        if(statusmap[ind]==LSMASK_SLEEP)
//        {
//            continue;
//        }
//        r=ind/imageW;
//        c=ind-r*imageW;

//        newvalue=phi[ind]+dt*(*delta_iter);
//        changed++;

//        if(newvalue>=0.5)
//        {
//            int neighbormovingdown=0;
//            if(c>0 && statusmap[ind-1]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(c<imageW-1 && statusmap[ind+1]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(r>0 && statusmap[ind-imageW]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;
//            if(r<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEDOWN)
//                 neighbormovingdown=1;

//            if(!neighbormovingdown)
//            {
//                statusmap[ind]=LSMASK_MOVEUP;
//                removeactivepoints->push_back(ind);
//            }
//            else
//            {
//                continue;
//            }
//        }
//        if(newvalue<-0.5)
//        {
//            int neighbormovingdown=0;
//            if(c>0 && statusmap[ind-1]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(c<imageW-1 && statusmap[ind+1]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(r>0 && statusmap[ind-imageW]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;
//            if(r<imageH-1 && statusmap[ind+imageW]==LSMASK_MOVEUP)
//                 neighbormovingdown=1;

//            if(!neighbormovingdown)
//            {
//                statusmap[ind]=LSMASK_MOVEDOWN;
//                removeactivepoints->push_back(ind);
//            }
//            else
//            {
//                continue;
//            }
//        }

//        phi[ind]=newvalue;

//    }

//}
//void SparseFieldVesselLevelSetKernel::update3DphiUsingThreshold_multi_threaded()
//{

////    activebands_deltaphi.clear();
//    int narrowbandsize=activeband->size();
//    int index;
////    QTime t;
////     t.start();
//#pragma omp parallel for if(narrowbandsize>100)
//    for(index = 0 ; index < narrowbandsize ;index++)
//    {
//        float dx,dy,dz;
//        float dxplus, dyplus, dzplus, dxminus, dyminus, dzminus;
//        float dxplusy, dxminusy, dxplusz, dxminusz, dyplusx, dyminusx, dyplusz, dyminusz, dzplusx, dzminusx, dzplusy, dzminusy;
//        float gradphi, nplusx, nplusy, nplusz, nminusx, nminusy, nminusz, curvature;
//        float delta_phi;
//        int ind;
//        int i,j,k;
//        ind = (*activeband)[index];

////        if(statusmap[ind]==LSMASK_SLEEP)
////        {
////            activebands_deltaphi.push_back(0);
////            continue;
////        }
//        //it is no need to check sleep points as sleep points are exclude from reinitialization

//        k=ind/sliceSize;
//        j=(ind-k*sliceSize)/imageW;
//        i=ind-k*sliceSize-j*imageW;

//        if(i==0||i==imageW-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
//        if(j==0||j==imageH-1){dy=0;} else {dy=(phi[ind-imageW]-phi[ind+imageW])/2;}
//        if(k==0||k==imageD-1){dz=0;} else {dz=(phi[ind+sliceSize]-phi[ind-sliceSize])/2;}

//        if(i==imageW-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
//        if(j==0){dyplus=0;} else {dyplus=(phi[ind-imageW]-phi[ind]);}
//        if(k==imageD-1){dzplus=0;} else {dzplus=(phi[ind+sliceSize]-phi[ind]);}
//        if(i==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
//        if(j==imageH-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imageW]);}
//        if(k==0){dzminus=0;} else {dzminus=(phi[ind]-phi[ind-sliceSize]);}

//        if(i==0||i==imageW-1||j==0){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
//        if(i==0||i==imageW-1||j==imageH-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
//        if(i==0||i==imageW-1||k==imageD-1) {dxplusz=0;} else {dxplusz=(phi[ind+sliceSize+1]-phi[ind+sliceSize-1])/2;}
//        if(i==0||i==imageW-1||k==0) {dxminusz=0;} else {dxminusz=(phi[ind-sliceSize+1]-phi[ind-sliceSize-1])/2;}
//        if(j==0||j==imageH-1||i==imageW-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
//        if(j==0||j==imageH-1||i==0){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
//        if(j==0||j==imageH-1||k==imageD-1) {dyplusz=0;} else {dyplusz=(phi[ind+sliceSize-imageW]-phi[ind+sliceSize+imageW])/2;}
//        if(j==0||j==imageH-1||k==0) {dyminusz=0;} else {dyminusz=(phi[ind-sliceSize-imageW]-phi[ind-sliceSize+imageW])/2;}
//        if(k==0||k==imageD-1||i==imageW-1) {dzplusx=0;} else {dzplusx=(phi[ind+1+sliceSize]-phi[ind+1-sliceSize])/2;}
//        if(k==0||k==imageD-1||i==0) {dzminusx=0;} else {dzminusx=(phi[ind-1+sliceSize]-phi[ind-1-sliceSize])/2;}
//        if(k==0||k==imageD-1||j==0) {dzplusy=0;} else {dzplusy=(phi[ind-imageW+sliceSize]-phi[ind-imageW-sliceSize])/2;}
//        if(k==0||k==imageD-1||j==imageH-1) {dzminusy=0;} else {dzminusy=(phi[ind+imageW+sliceSize]-phi[ind+imageW-sliceSize])/2;}


//        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) + ((dzplusx + dz)*(dzplusx + dz)*0.25));
//        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) + ((dzplusy + dz)*(dzplusy + dz)*0.25));
//        nplusz= dzplus / sqrt(1.192092896e-07F + (dzplus*dzplus) + ((dxplusz + dz)*(dxplusz + dz)*0.25) + ((dyplusz + dy)*(dyplusz + dy)*0.25));

//        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) + ((dzminusx + dz)*(dzminusx + dz)*0.25));
//        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) + ((dzminusy + dz)*(dzminusy + dz)*0.25));
//        nminusz= dzminus / sqrt(1.192092896e-07F + (dzminus*dzminus) + ((dxminusz + dz)*(dxminusz + dz)*0.25) + ((dyminusz + dy)*(dyminusz + dy)*0.25));

//        curvature= ((nplusx-nminusx)+(nplusy-nminusy)+(nplusz-nminusz))/2;

//        delta_phi = (-externalWeightFactor * externalForce[ind]) + ((1-externalWeightFactor) * curvature);
//        if(delta_phi > 0)
//            gradphi=sqrt( max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)
//                           + max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)
//                           + max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0));
//        else
//            gradphi=sqrt( min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)
//                           + min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)
//                           + min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0));

//        delta_phi = delta_phi * gradphi;

//        if(currenttrend*delta_phi < 0)
//        {
//            delta_phi = 0;
//        }
//        if (fabs(delta_phi) < 0.00001) {
//            statusmap[ind] = LSMASK_SLEEP;
//        }
//        else if (delta_phi<0 && (i==0||i==imageW-1||j==0||j==imageH-1)) //which should be         else if (delta_phi<0 && (i==0||i==imageW-1||j==0||j==imageH-1||k==0||k==imageD-1))
//        {
//            statusmap[ind] = LSMASK_SLEEP;
//        }

//        activebands_deltaphi[index] = delta_phi;

//    }

//    float maxPropogating=0;

//    removeactivepoints->clear();


////    for(index = 0 ; index < narrowbandsize ;index++)
////    {
////        float delta_phi = activebands_deltaphi[index];
////        if(fabs(delta_phi) > maxPropogating)
////        {
////            maxPropogating=fabs(delta_phi);
////        }
////    }
//    int nthreads = omp_get_max_threads();
//    float gloabmax[MAXNUMTHREAD];
//    #pragma omp parallel
//    {
//        float privatemax = 0.0;
//        #pragma omp for nowait
//        for(index = 0 ; index < narrowbandsize ;index++)
//        {
//            float delta_phi = activebands_deltaphi[index];
//            if(fabs(delta_phi) > privatemax)
//                privatemax=fabs(delta_phi);
//        }
//        gloabmax[omp_get_thread_num()] = privatemax;

//    }
//    for(index = 0; index < nthreads; index++)
//    {
//        maxPropogating = max(maxPropogating, gloabmax[index]);
//    }
//    float dt = maxiumStepLength / (maxPropogating+1.0e-6);



//    if(maxPropogating<0.0001)
//    {
//        changed=0;
//        return;
//    }
//    virutaltime += dt;

////    qDebug() << "compute phi: time elapsed: " << t.elapsed();
////    t.restart();

//    changed=1;

//     for(index=0; index < nthreads; index++)
//     {
//         tempband[index].clear();
//     }
//#pragma omp parallel for
//    for(index = 0 ; index < narrowbandsize ;index++)
//    {
//        float newvalue;
//        int ind=(*activeband)[index];
//        if(statusmap[ind]==LSMASK_SLEEP)
//            continue;

//        newvalue=phi[ind]+dt*(activebands_deltaphi[index]);
////        changed++;

//        if(newvalue>=0.5)
//        {
//            statusmap[ind]=LSMASK_MOVEUP;
////            #pragma omp critical
//            tempband[omp_get_thread_num()].push_back(ind);
//        }
//        if(newvalue<-0.5)
//        {
//            statusmap[ind]=LSMASK_MOVEDOWN;
////            #pragma omp critical
//            tempband[omp_get_thread_num()].push_back(ind);
//        }

//        phi[ind]=newvalue;

//    }
//    for(index=0; index < nthreads; index++)
//    {
//        removeactivepoints->insert(removeactivepoints->end(),tempband[index].begin(), tempband[index].end());
//    }
////    qDebug() << "update phi: time elapsed: " << t.elapsed();
////    t.restart();

//}
//void SparseFieldVesselLevelSetKernel::update3DphiUsingThreshold()
//{

////    activebands_deltaphi.clear();
//    int narrowbandsize=activeband->size();
//    int index;
////    QTime t;
////     t.start();
//    float maxPropogating=0;
//    for(index = 0 ; index < narrowbandsize ;index++)
//    {
//        float dx,dy,dz;
//        float dxplus, dyplus, dzplus, dxminus, dyminus, dzminus;
//        float dxplusy, dxminusy, dxplusz, dxminusz, dyplusx, dyminusx, dyplusz, dyminusz, dzplusx, dzminusx, dzplusy, dzminusy;
//        float gradphi, nplusx, nplusy, nplusz, nminusx, nminusy, nminusz, curvature;
//        float delta_phi;
//        int ind;
//        int i,j,k;
//        ind = (*activeband)[index];

////        if(statusmap[ind]==LSMASK_SLEEP)
////        {
////            activebands_deltaphi.push_back(0);
////            continue;
////        }
//        //it is no need to check sleep points as sleep points are exclude from reinitialization

//        k=ind/sliceSize;
//        j=(ind-k*sliceSize)/imageW;
//        i=ind-k*sliceSize-j*imageW;

//        if(i==0||i==imageW-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
//        if(j==0||j==imageH-1){dy=0;} else {dy=(phi[ind-imageW]-phi[ind+imageW])/2;}
//        if(k==0||k==imageD-1){dz=0;} else {dz=(phi[ind+sliceSize]-phi[ind-sliceSize])/2;}

//        if(i==imageW-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
//        if(j==0){dyplus=0;} else {dyplus=(phi[ind-imageW]-phi[ind]);}
//        if(k==imageD-1){dzplus=0;} else {dzplus=(phi[ind+sliceSize]-phi[ind]);}
//        if(i==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
//        if(j==imageH-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imageW]);}
//        if(k==0){dzminus=0;} else {dzminus=(phi[ind]-phi[ind-sliceSize]);}

//        if(i==0||i==imageW-1||j==0){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
//        if(i==0||i==imageW-1||j==imageH-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
//        if(i==0||i==imageW-1||k==imageD-1) {dxplusz=0;} else {dxplusz=(phi[ind+sliceSize+1]-phi[ind+sliceSize-1])/2;}
//        if(i==0||i==imageW-1||k==0) {dxminusz=0;} else {dxminusz=(phi[ind-sliceSize+1]-phi[ind-sliceSize-1])/2;}
//        if(j==0||j==imageH-1||i==imageW-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
//        if(j==0||j==imageH-1||i==0){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
//        if(j==0||j==imageH-1||k==imageD-1) {dyplusz=0;} else {dyplusz=(phi[ind+sliceSize-imageW]-phi[ind+sliceSize+imageW])/2;}
//        if(j==0||j==imageH-1||k==0) {dyminusz=0;} else {dyminusz=(phi[ind-sliceSize-imageW]-phi[ind-sliceSize+imageW])/2;}
//        if(k==0||k==imageD-1||i==imageW-1) {dzplusx=0;} else {dzplusx=(phi[ind+1+sliceSize]-phi[ind+1-sliceSize])/2;}
//        if(k==0||k==imageD-1||i==0) {dzminusx=0;} else {dzminusx=(phi[ind-1+sliceSize]-phi[ind-1-sliceSize])/2;}
//        if(k==0||k==imageD-1||j==0) {dzplusy=0;} else {dzplusy=(phi[ind-imageW+sliceSize]-phi[ind-imageW-sliceSize])/2;}
//        if(k==0||k==imageD-1||j==imageH-1) {dzminusy=0;} else {dzminusy=(phi[ind+imageW+sliceSize]-phi[ind+imageW-sliceSize])/2;}


//        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) + ((dzplusx + dz)*(dzplusx + dz)*0.25));
//        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) + ((dzplusy + dz)*(dzplusy + dz)*0.25));
//        nplusz= dzplus / sqrt(1.192092896e-07F + (dzplus*dzplus) + ((dxplusz + dz)*(dxplusz + dz)*0.25) + ((dyplusz + dy)*(dyplusz + dy)*0.25));

//        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) + ((dzminusx + dz)*(dzminusx + dz)*0.25));
//        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) + ((dzminusy + dz)*(dzminusy + dz)*0.25));
//        nminusz= dzminus / sqrt(1.192092896e-07F + (dzminus*dzminus) + ((dxminusz + dz)*(dxminusz + dz)*0.25) + ((dyminusz + dy)*(dyminusz + dy)*0.25));

//        curvature= ((nplusx-nminusx)+(nplusy-nminusy)+(nplusz-nminusz))/2;

//        delta_phi = (-externalWeightFactor * externalForce[ind]) + ((1-externalWeightFactor) * curvature);
//        if(delta_phi > 0)
//            gradphi=sqrt( max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)
//                           + max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)
//                           + max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0));
//        else
//            gradphi=sqrt( min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)
//                           + min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)
//                           + min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0));

//        delta_phi = delta_phi * gradphi;

//        if(currenttrend*delta_phi < 0)
//        {
//            delta_phi = 0;
//        }
//        if (fabs(delta_phi) < 0.00001) {
//            statusmap[ind] = LSMASK_SLEEP;
//        }
//        else if (delta_phi<0 && (i==0||i==imageW-1||j==0||j==imageH-1)) //which should be         else if (delta_phi<0 && (i==0||i==imageW-1||j==0||j==imageH-1||k==0||k==imageD-1))
//        {
//            statusmap[ind] = LSMASK_SLEEP;
//        }

//        activebands_deltaphi[index] = delta_phi;
//        if(fabs(delta_phi) > maxPropogating)
//            maxPropogating=fabs(delta_phi);

//    }



//    removeactivepoints->clear();

//    float dt = maxiumStepLength / (maxPropogating+1.0e-6);

//    if(maxPropogating<0.0001)
//    {
//        changed=0;
//        return;
//    }
//    virutaltime += dt;


////    qDebug() << "compute phi: time elapsed: " << t.elapsed();
////    t.restart();

//    changed=1;
////#pragma omp parallel for
//    for(index = 0 ; index < narrowbandsize ;index++)
//    {
//        float newvalue;
//        int ind=(*activeband)[index];
//        if(statusmap[ind]==LSMASK_SLEEP)
//            continue;

//        newvalue=phi[ind]+dt*(activebands_deltaphi[index]);
////        changed++;

//        if(newvalue>=0.5)
//        {
//            statusmap[ind]=LSMASK_MOVEUP;
////            #pragma omp critical
//            removeactivepoints->push_back(ind);
//        }
//        if(newvalue<-0.5)
//        {
//            statusmap[ind]=LSMASK_MOVEDOWN;
////            #pragma omp critical
//            removeactivepoints->push_back(ind);
//        }

//        phi[ind]=newvalue;

//    }
////    qDebug() << "update phi: time elapsed: " << t.elapsed();
////    t.restart();

//}

//void SparseFieldVesselLevelSetKernel::reinitializet_multi_threaded()
//{
//    std::vector<int>::const_iterator iter;
//    neighborband.clear();

//    int bandsize;
//    int dim,n_i,n_x;
////    int i,j,k;
////    QTime t;
////     t.start();

//    //check the neighbors of retired Active Points, promote them to activepoits if proper
//    for(iter=removeactivepoints->begin();iter!=removeactivepoints->end();iter++)
//    {
//            int ind=*iter;
//            uchar possiblelayer;
//            float ds;
//            // we use the active points' moving direction to estimate inner or outer neighbors will be promoted
//            if( statusmap[ind] == LSMASK_MOVEUP)
//            {
//                    possiblelayer = LSMASK_IN1NEIGHBOR;
//                    ds = -1.0;
//                    statusmap[ind] = LSMASK_OUT1NEIGHBOR;
//            }
//            else {
//                    possiblelayer = LSMASK_OUT1NEIGHBOR;
//                    ds = 1.0;
//                    statusmap[ind] = LSMASK_IN1NEIGHBOR;
//            }
//            // scan the neighbors for desired inner or outer layer
//            for (dim=0; dim<imDimension; dim++) {
//                    for(n_x=-1;n_x<=1;n_x+=2)
//                    {
//                            n_i = ind + dimoffset[dim] * n_x;

//                            if (n_i<0 || n_i>=totalSize) {
//                                    continue;
//                            }
//                            if(statusmap[n_i] == possiblelayer)
//                            {
//                                //set the neighbor to LSMASK_ACTIVE so others can not find it again.
//                                    statusmap[n_i] = LSMASK_ACTIVE;
//                                    phi[n_i] = phi[ind] + ds; //update the promoted points phi using distance transform
//                                    //this is not accurate as there maybe more than one neighbor retiring, RETHINKING is needed.
//                                    activeband->push_back(n_i);
//                                    neighborband.push_back(n_i);

//                            }
//                            else if(statusmap[n_i] != LSMASK_ACTIVE){
//                                // if the neighbor is not active (this condition is necessary)
//                                    neighborband.push_back(n_i);
//                            }

//                    }
//            }

//    }

//    //then we need to set the neighbor points status to inner or outer neighter
//    for(iter=neighborband.begin();iter!=neighborband.end();iter++){
//        int ind=*iter;
//        if(statusmap[ind] == LSMASK_IN1NEIGHBOR || statusmap[ind] == LSMASK_OUT1NEIGHBOR)
//        {
//            int findactive=0;
//            //scan the neighbors, if there is LSMASK_ACTIVE or LSMASK_SLEEP points, this point is still on the inner or outer neighbor band

//            for (dim=0; dim<imDimension; dim++) {
//                for(n_x=-1;n_x<=1;n_x+=2)
//                {
//                    n_i = ind + dimoffset[dim] * n_x;
//                    if (n_i<0 || n_i>=totalSize) {
//                            continue;
//                    }
//                    if(statusmap[n_i] == LSMASK_ACTIVE ||statusmap[n_i] == LSMASK_SLEEP)
//                    {
//                            findactive=1;
//                            n_x=2;dim=3;
//                    }
//                }
//            }
//            // otherwise set the point status to faraway
//            if(findactive == 0)
//                    statusmap[ind] = LSMASK_FARAWAY;
//        }
//        else if(statusmap[ind] == LSMASK_ACTIVE)
//        {
//            //if this point has just been promoted, we set the neighbors status to outer or inner neighbor
//            for (dim=0; dim<imDimension; dim++) {
//                for(n_x=-1;n_x<=1;n_x+=2)
//                {
//                    n_i = ind + dimoffset[dim] * n_x;
//                    if (n_i<0 || n_i>=totalSize) {
//                            continue;
//                    }
//                    if(statusmap[n_i] == LSMASK_FARAWAY)
//                    {
//                            if(phi[n_i] > 0)
//                                    statusmap[n_i] = LSMASK_OUT1NEIGHBOR;
//                            else {
//                                    statusmap[n_i] = LSMASK_IN1NEIGHBOR;
//                            }
//                    }
//                }
//            }

//        }

//    }

////    qDebug() << "cset the neighbor points status: time elapsed: " << t.elapsed();
////    t.restart();

//    std::vector<int>::const_iterator aiter;
//    if(activeband == &activeband1)
//            tempactiveband = &activeband2;
//    else {
//            tempactiveband = &activeband1;
//    }
//    tempactiveband->clear();
//    neighborband.clear();

//    bandsize = activeband->size();
//    // now we remove the deactived (moved or sleeping) points from active band
//    // we copy the active points to a temporay buffer band
//    for(aiter=activeband->begin();aiter!=activeband->end();aiter++){
//        int ind=*aiter;
//        if(statusmap[ind] != LSMASK_ACTIVE)
//                continue;
//        tempactiveband->push_back(ind);

//        //here we also wake up the neighboring sleeping point for each active point
//        int ii,jj,kk,neighboindex;
//        for(kk=-1;kk<2;kk++)
//            for(jj=-1;jj<2;jj++)
//                for(ii=-1;ii<2;ii++)
//                {
//                    neighboindex=ind+kk*sliceSize+jj*imageW+ii;
//                    if(neighboindex>0&&neighboindex<totalSize)
//                    {
//                        if(statusmap[neighboindex]==LSMASK_SLEEP)
//                        {
//                                statusmap[neighboindex]=LSMASK_WAKEDUP;
//                                // the waked up points are set to LSMASK_WAKEDUP so it will not be added towice if
//                                // the point is still in the active band.
//                                neighborband.push_back(neighboindex);
//                                tempactiveband->push_back(neighboindex);
//                        }

//                    }
//                }

//    }
//    //now we set the waked up points status to LSMASK_ACTIVE, this is to avoid redanduncy
//    for(aiter=neighborband.begin();aiter!=neighborband.end();aiter++){
//            int ind=*aiter;
//            statusmap[ind] = LSMASK_ACTIVE;
//    }
//    //we now give the reduct band to active band, then we can use the old activeband array to store reomved active points
//    removeactivepoints = activeband;
//    activeband = tempactiveband;

//    neighborband.clear();
//    firstlayerneighbor.clear();

////    qDebug() << "wake up neighbors: time elapsed: " << t.elapsed() << bandsize ;
////    t.restart();

//    // now we need do the reinitialzing business
//    // we first find the inner and outer neighbor of all active points
//    for(aiter=activeband->begin();aiter!=activeband->end();aiter++){
//        int ind=*aiter;
//        if(statusmap[ind] == LSMASK_SLEEP)
//                continue;
//        // we set the checked neighbors status to CHECKING to avoid duplication of the neighbor list
//        for (dim=0; dim<imDimension; dim++) {
//            for(n_x=-1;n_x<=1;n_x+=2)
//            {
//                n_i = ind + dimoffset[dim] * n_x;
//                if (n_i<0 || n_i>=totalSize) {
//                     continue;
//                }
//                if(statusmap[n_i] == LSMASK_IN1NEIGHBOR )
//                {

//                    statusmap[n_i] = LSMASK_CHECKINGIN1NEIGHBOR;
//                    firstlayerneighbor.push_back(n_i);

//                }
//                else if(statusmap[n_i] == LSMASK_OUT1NEIGHBOR){
//                    statusmap[n_i] = LSMASK_CHECKINGOUT1NEIGHBOR;
//                    firstlayerneighbor.push_back(n_i);
//                }

//            }
//        }
//    }

////    qDebug() << "creat neighbor list: time elapsed: " << t.elapsed();
////    t.restart();
//   int index_iter;
//    bandsize = firstlayerneighbor.size();
//    int nthreads = omp_get_max_threads();
//    for(index_iter=0; index_iter < nthreads; index_iter++)
//    {
//        tempband[index_iter].clear();
//    }
//    // we search the closest LSMASK_ACTIVE or LSMASK_SLEEP points and do distance transform from it
//    // at the same time we search for the 2 layer neighbors.

//#pragma omp parallel for
//for(index_iter = 0 ; index_iter < bandsize; index_iter++) {
//    int ind=firstlayerneighbor[index_iter];
////        int ind=*iter;
//    float direction;
//    if(statusmap[ind] == LSMASK_CHECKINGIN1NEIGHBOR)
//        direction = -1.0;
//    else
//        direction = 1.0;
//        float ds = 1.0*direction;
//        float tempvalue = FARAWAY*direction;
//        int dim_i;
//        for (dim_i=0; dim_i<imDimension; dim_i++) {
//            int neighbor_1d;
//            for(neighbor_1d=-1;neighbor_1d<=1;neighbor_1d+=2)
//            {
//                int neighbor_index = ind + dimoffset[dim_i] * neighbor_1d;
//                if (neighbor_index<0 || neighbor_index>=totalSize) {
//                        continue;
//                }
//                if(statusmap[neighbor_index] == LSMASK_ACTIVE ||statusmap[neighbor_index] == LSMASK_SLEEP  )
//                {
//                    if((phi[neighbor_index]+ds)*direction < tempvalue*direction)
//                    {
//                            tempvalue = phi[neighbor_index]+ds;
//                    }
//                }
//                else if(statusmap[neighbor_index] == LSMASK_FARAWAY){
//                    if(direction < 0)
//                        statusmap[neighbor_index] = LSMASK_IN2NEIGHBOR;
//                    else
//                        statusmap[neighbor_index] = LSMASK_OUT2NEIGHBOR;
//                        tempband[omp_get_thread_num()].push_back(neighbor_index);
////                        #pragma omp critical
////                        neighborband.push_back(neighbor_index);
//                }
//            }
//        }
//        phi[ind]=tempvalue;
//        if(direction < 0)
//            statusmap[ind] = LSMASK_IN1NEIGHBOR;
//        else
//            statusmap[ind] = LSMASK_OUT1NEIGHBOR;
//    }

////    qDebug() << "dt for firt layer: time elapsed: " << t.elapsed() << bandsize;
////    t.restart();

//// distance transform to the 2nd layer neighbors, this value will be used to caculate the normal of level set

//    for(index_iter=1; index_iter < nthreads; index_iter++)
//    {
//        tempband[0].insert(tempband[0].end(),tempband[index_iter].begin(), tempband[index_iter].end());
////        qDebug() << tempband[index_iter].size();
//    }

//    bandsize = tempband[0].size();
////    qDebug() << bandsize;

//#pragma omp parallel for if(bandsize>100)
//    for(index_iter = 0 ; index_iter < bandsize; index_iter++) {
//        int ind=tempband[0][index_iter];
//        if(statusmap[ind] == LSMASK_FARAWAY)
//            continue;
//            float ds = 1.0;
//            float tempvalue = FARAWAY;
//            if(statusmap[ind] == LSMASK_IN2NEIGHBOR)
//            {
//                    ds = -1.0;
//                    tempvalue = -FARAWAY;
//            }

//            int dim_i;
//            for (dim_i=0; dim_i<imDimension; dim_i++) {
//                    int neighbor_1d;
//                    for(neighbor_1d=-1;neighbor_1d<=1;neighbor_1d+=2)
//                    {
//                            int neighbor_index = ind + dimoffset[dim_i] * neighbor_1d;
//                            if (neighbor_index<0 || neighbor_index>=totalSize) {
//                                    continue;
//                            }
//                            if(statusmap[neighbor_index] == LSMASK_IN1NEIGHBOR ||statusmap[neighbor_index] == LSMASK_OUT1NEIGHBOR  )
//                            {

//                                    if(fabs(phi[neighbor_index]+ds) < fabs(tempvalue))
//                                    {
//                                            tempvalue = phi[neighbor_index]+ds;
//                                    }

//                            }

//                    }
//            }
//            phi[ind]=tempvalue;
//            statusmap[ind] = LSMASK_FARAWAY;
//    }

////    qDebug() << "dt for second layer: time elapsed: " << t.elapsed() << bandsize;
////    t.restart();


//}

//void SparseFieldVesselLevelSetKernel::reinitialize()
//{
//    std::vector<int>::const_iterator iter;
//    neighborband.clear();

//    int dim,n_i,n_x;
////    int i,j,k;
////    QTime t;
////     t.start();

//    //check the neighbors of retired Active Points, promote them to activepoits if proper
//    for(iter=removeactivepoints->begin();iter!=removeactivepoints->end();iter++)
//    {
//            int ind=*iter;
//            uchar possiblelayer;
//            float ds;
//            // we use the active points' moving direction to estimate inner or outer neighbors will be promoted
//            if( statusmap[ind] == LSMASK_MOVEUP)
//            {
//                    possiblelayer = LSMASK_IN1NEIGHBOR;
//                    ds = -1.0;
//                    statusmap[ind] = LSMASK_OUT1NEIGHBOR;
//            }
//            else {
//                    possiblelayer = LSMASK_OUT1NEIGHBOR;
//                    ds = 1.0;
//                    statusmap[ind] = LSMASK_IN1NEIGHBOR;
//            }
//            // scan the neighbors for desired inner or outer layer
//            for (dim=0; dim<imDimension; dim++) {
//                    for(n_x=-1;n_x<=1;n_x+=2)
//                    {
//                            n_i = ind + dimoffset[dim] * n_x;

//                            if (n_i<0 || n_i>=totalSize) {
//                                    continue;
//                            }
//                            if(statusmap[n_i] == possiblelayer)
//                            {
//                                //set the neighbor to LSMASK_ACTIVE so others can not find it again.
//                                    statusmap[n_i] = LSMASK_ACTIVE;
//                                    phi[n_i] = phi[ind] + ds; //update the promoted points phi using distance transform
//                                    //this is not accurate as there maybe more than one neighbor retiring, RETHINKING is needed.
//                                    activeband->push_back(n_i);
//                                    neighborband.push_back(n_i);

//                            }
//                            else if(statusmap[n_i] != LSMASK_ACTIVE){
//                                // if the neighbor is not active (this condition is necessary)
//                                    neighborband.push_back(n_i);
//                            }

//                    }
//            }

//    }

//    //then we need to set the neighbor points status to inner or outer neighter
//    for(iter=neighborband.begin();iter!=neighborband.end();iter++){
//        int ind=*iter;
//        if(statusmap[ind] == LSMASK_IN1NEIGHBOR || statusmap[ind] == LSMASK_OUT1NEIGHBOR)
//        {
//            int findactive=0;
//            //scan the neighbors, if there is LSMASK_ACTIVE or LSMASK_SLEEP points, this point is still on the inner or outer neighbor band

//            for (dim=0; dim<imDimension; dim++) {
//                for(n_x=-1;n_x<=1;n_x+=2)
//                {
//                    n_i = ind + dimoffset[dim] * n_x;
//                    if (n_i<0 || n_i>=totalSize) {
//                            continue;
//                    }
//                    if(statusmap[n_i] == LSMASK_ACTIVE ||statusmap[n_i] == LSMASK_SLEEP)
//                    {
//                            findactive=1;
//                            n_x=2;dim=3;
//                    }
//                }
//            }
//            // otherwise set the point status to faraway
//            if(findactive == 0)
//                    statusmap[ind] = LSMASK_FARAWAY;
//        }
//        else if(statusmap[ind] == LSMASK_ACTIVE)
//        {
//            //if this point has just been promoted, we set the neighbors status to outer or inner neighbor
//            for (dim=0; dim<imDimension; dim++) {
//                for(n_x=-1;n_x<=1;n_x+=2)
//                {
//                    n_i = ind + dimoffset[dim] * n_x;
//                    if (n_i<0 || n_i>=totalSize) {
//                            continue;
//                    }
//                    if(statusmap[n_i] == LSMASK_FARAWAY)
//                    {
//                            if(phi[n_i] > 0)
//                                    statusmap[n_i] = LSMASK_OUT1NEIGHBOR;
//                            else {
//                                    statusmap[n_i] = LSMASK_IN1NEIGHBOR;
//                            }
//                    }
//                }
//            }

//        }

//    }

////    qDebug() << "set the neighbor points status: time elapsed: " << t.elapsed();
////    t.restart();

//    std::vector<int>::const_iterator aiter;
//    if(activeband == &activeband1)
//            tempactiveband = &activeband2;
//    else {
//            tempactiveband = &activeband1;
//    }
//    tempactiveband->clear();
//    neighborband.clear();

//    // now we remove the deactived (moved or sleeping) points from active band
//    // we copy the active points to a temporay buffer band
//    for(aiter=activeband->begin();aiter!=activeband->end();aiter++){
//        int ind=*aiter;
//        if(statusmap[ind] != LSMASK_ACTIVE)
//                continue;
//        tempactiveband->push_back(ind);

//        //here we also wake up the neighboring sleeping point for each active point
//        int ii,jj,kk,neighboindex;
//        for(kk=-1;kk<2;kk++)
//            for(jj=-1;jj<2;jj++)
//                for(ii=-1;ii<2;ii++)
//                {
//                    neighboindex=ind+kk*sliceSize+jj*imageW+ii;
//                    if(neighboindex>0&&neighboindex<totalSize)
//                    {
//                        if(statusmap[neighboindex]==LSMASK_SLEEP)
//                        {
//                                statusmap[neighboindex]=LSMASK_WAKEDUP;
//                                // the waked up points are set to LSMASK_WAKEDUP so it will not be added towice if
//                                // the point is still in the active band.
//                                neighborband.push_back(neighboindex);
//                                tempactiveband->push_back(neighboindex);
//                        }

//                    }
//                }

//    }
//    //now we set the waked up points status to LSMASK_ACTIVE, this is to avoid redanduncy
//    for(aiter=neighborband.begin();aiter!=neighborband.end();aiter++){
//            int ind=*aiter;
//            statusmap[ind] = LSMASK_ACTIVE;
//    }
//    //we now give the reduct band to active band, then we can use the old activeband array to store reomved active points
//    removeactivepoints = activeband;
//    activeband = tempactiveband;

//    neighborband.clear();
//    firstlayerneighbor.clear();

////    qDebug() << "wake up neighbors: time elapsed: " << t.elapsed() << activeband->size() ;
////    t.restart();

//    // now we need do the reinitialzing business
//    // we first find the inner and outer neighbor of all active points
//    for(aiter=activeband->begin();aiter!=activeband->end();aiter++){
//        int ind=*aiter;
//        if(statusmap[ind] == LSMASK_SLEEP)
//                continue;
//        // we set the checked neighbors status to CHECKING to avoid duplication of the neighbor list
//        for (dim=0; dim<imDimension; dim++) {
//            for(n_x=-1;n_x<=1;n_x+=2)
//            {
//                n_i = ind + dimoffset[dim] * n_x;
//                if (n_i<0 || n_i>=totalSize) {
//                     continue;
//                }
//                if(statusmap[n_i] == LSMASK_IN1NEIGHBOR )
//                {

//                    statusmap[n_i] = LSMASK_CHECKINGIN1NEIGHBOR;
//                    firstlayerneighbor.push_back(n_i);

//                }
//                else if(statusmap[n_i] == LSMASK_OUT1NEIGHBOR){
//                    statusmap[n_i] = LSMASK_CHECKINGOUT1NEIGHBOR;
//                    firstlayerneighbor.push_back(n_i);
//                }

//            }
//        }
//    }

////    qDebug() << "creat neighbor list: time elapsed: " << t.elapsed();
////    t.restart();

//    // we search the closest LSMASK_ACTIVE or LSMASK_SLEEP points and do distance transform from it
//    // at the same time we search for the 2 layer neighbors.
//    for(iter=firstlayerneighbor.begin();iter!=firstlayerneighbor.end();iter++){
//        int ind=*iter;
//        float direction;
//        if(statusmap[ind] == LSMASK_CHECKINGIN1NEIGHBOR)
//            direction = -1.0;
//        else
//            direction = 1.0;
//            float ds = 1.0*direction;
//            float tempvalue = FARAWAY*direction;
//            int dim_i;
//            for (dim_i=0; dim_i<imDimension; dim_i++) {
//                int neighbor_1d;
//                for(neighbor_1d=-1;neighbor_1d<=1;neighbor_1d+=2)
//                {
//                    int neighbor_index = ind + dimoffset[dim_i] * neighbor_1d;
//                    if (neighbor_index<0 || neighbor_index>=totalSize) {
//                            continue;
//                    }
//                    if(statusmap[neighbor_index] == LSMASK_ACTIVE ||statusmap[neighbor_index] == LSMASK_SLEEP  )
//                    {
//                        if((phi[neighbor_index]+ds)*direction < tempvalue*direction)
//                        {
//                                tempvalue = phi[neighbor_index]+ds;
//                        }
//                    }
//                    else if(statusmap[neighbor_index] == LSMASK_FARAWAY){
//                        if(direction < 0)
//                            statusmap[neighbor_index] = LSMASK_IN2NEIGHBOR;
//                        else
//                            statusmap[neighbor_index] = LSMASK_OUT2NEIGHBOR;
//                            neighborband.push_back(neighbor_index);
//    //                        #pragma omp critical
//    //                        neighborband.push_back(neighbor_index);
//                    }
//                }
//            }
//            phi[ind]=tempvalue;
//            if(direction < 0)
//                statusmap[ind] = LSMASK_IN1NEIGHBOR;
//            else
//                statusmap[ind] = LSMASK_OUT1NEIGHBOR;
//        }
////    qDebug() << "dt for firt layer: time elapsed: " << t.elapsed() << firstlayerneighbor.size();
////    t.restart();
//    // distance transform to the 2nd layer neighbors, this value will be used to caculate the normal of level set


//    for(iter=neighborband.begin();iter!=neighborband.end();iter++){

//        int ind=*iter;
//        float ds = 1.0;
//        float tempvalue = FARAWAY;
//        if(statusmap[ind] == LSMASK_IN2NEIGHBOR)
//        {
//                ds = -1.0;
//                tempvalue = -FARAWAY;
//        }
//        int dim_i;
//        for (dim_i=0; dim_i<imDimension; dim_i++) {
//                int neighbor_1d;
//                for(neighbor_1d=-1;neighbor_1d<=1;neighbor_1d+=2)
//                {
//                        int neighbor_index = ind + dimoffset[dim_i] * neighbor_1d;
//                        if (neighbor_index<0 || neighbor_index>=totalSize) {
//                                continue;
//                        }
//                        if(statusmap[neighbor_index] == LSMASK_IN1NEIGHBOR ||statusmap[neighbor_index] == LSMASK_OUT1NEIGHBOR  )
//                        {

//                                if(fabs(phi[neighbor_index]+ds) < fabs(tempvalue))
//                                {
//                                        tempvalue = phi[neighbor_index]+ds;
//                                }

//                        }

//                }
//        }
//        phi[ind]=tempvalue;
//        statusmap[ind] = LSMASK_FARAWAY;
//    }

////    qDebug() << "dt for second layer: time elapsed: " << t.elapsed() << neighborband.size();
////    t.restart();


//}
