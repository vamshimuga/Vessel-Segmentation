#include "abstractlevelsetkernel.h"
#include "LevelSetRoutines.h"

AbstractLevelSetKernel::AbstractLevelSetKernel(QObject *parent) :
    QThread(parent)
{
    maxPeriod = 4;
    maxIteration = 15000;
    currenttrend = -1;
    updateInterval = 50;
    its = 0; //current iteration times
    periods = 0; //current period
    dampingFactor = 0.5;
    maxiumStepLength = MAXSTEP;
    initialStepLength = MAXSTEP;
    isAbort = false;
    needRestart = false;

    phi = NULL;
    statusmap = NULL;
    imInfo.imDimension = 3;
    for( int i = 0 ; i < 3 ; i++ )
    {
        imInfo.dimension[i] = 0;
        imInfo.spacing[i] = 0;
        imInfo.dimoffset[0] = 0;
    }
    imInfo.totalSize = 0;
    imInfo.sliceSize = 0;

}
bool AbstractLevelSetKernel::initialize(uchar* smap)
{
    if(speedFunction && speedFunction->isValid())
    {
        phi = speedFunction->getPhi();
        imInfo = speedFunction->getImageInfo();
        statusmap = smap;
        return true;
    }
    return false;
}


void AbstractLevelSetKernel::getNarrowBandPoints(std::vector<int> &list)
{
    list.clear();
}
