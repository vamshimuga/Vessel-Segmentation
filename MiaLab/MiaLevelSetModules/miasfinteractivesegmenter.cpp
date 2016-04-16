#include "miasfinteractivesegmenter.h"
#include "MiaLevelSetModules/LevelsetKernels/levelsetfunction_thresholdbased.h"

MiaSFInteractiveSegmenter::MiaSFInteractiveSegmenter(MiaNetwork *parent) :
    MiaSparseFieldLevelSetSegmenter(parent)
{
    moduleName = "Interactive Sparse Field Level Set";
    requiredInputImageType = IM_short;
    setupSpeedFunction();
}

bool MiaSFInteractiveSegmenter::reAllocOutputImage()
{
    if(MiaSparseFieldLevelSetSegmenter::reAllocOutputImage() == false)
        return false;
    if(needReIntialize)
    {
#ifdef USESHORTTYPELEVELSETFUNCTION
        speedFunction->initialize((short*)levelSetFunction->getDataPtr(),
                                 (void*)inputImage->getDataPtr(),
                                 inputImage->getDimSize(),
                                 inputImage->getSpacing());
#else
        speedFunction->initialize((float*)levelSetFunction->getDataPtr(),
                                 (void*)inputImage->getDataPtr(),
                                 inputImage->getDimSize(),
                                 inputImage->getSpacing());
#endif
        kernelThread->initialize((uchar*)levelSetStatusMap->getDataPtr());

        needReIntialize = false;
    }
    return true;
}

void MiaSFInteractiveSegmenter::setupSpeedFunction()
{
    if(speedFunction)
    {
        delete speedFunction;
        speedFunction = NULL;
    }
    if(usingAdvectionForce)
    {
        throw std::runtime_error("Not implemented");
    }
    else
    {
        speedFunction = new LevelSetFunction_Interactive_ThresholdBased_WithType(this);
        kernelThread->setSpeedFunction(speedFunction);
    }
}
void MiaSFInteractiveSegmenter::setLearnedDistribution(float o_m , float o_s, float i_m, float i_s,float ds)
{
    ((LevelSetFunction_Interactive_ThresholdBased_WithType*)speedFunction)->setLearnedDistribution(o_m, o_s, i_m, i_s,ds);
}
