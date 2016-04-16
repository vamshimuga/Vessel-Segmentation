#include "sparsefieldchanveselevelset.h"

SparseFieldChanVeseLevelSet::SparseFieldChanVeseLevelSet(QObject *parent) :
    SparseFieldLevelSetKernel(parent)
{

}

bool SparseFieldChanVeseLevelSet::initialize( float* p, float* ext, uchar*smap, int* d, float* s)
{
    SparseFieldLevelSetKernel::initialize(smap);
    sum_out = 0.0;
    sum_in = 0.0;
    count_out = 0;
    count_in = 0;

    return true;
}

void SparseFieldChanVeseLevelSet::run()
{

}

void SparseFieldChanVeseLevelSet::recreateActiveBand()
{

}

void SparseFieldChanVeseLevelSet::abort()
{

}

void SparseFieldChanVeseLevelSet::resume()
{

}

void SparseFieldChanVeseLevelSet::restart()
{

}

void SparseFieldChanVeseLevelSet::setStopPoint(int ind)
{

}

void SparseFieldChanVeseLevelSet::setThresholds(float lower, float mean, float upper)
{

}
float SparseFieldChanVeseLevelSet::extForce(int index)
{
    return 0;
}
