#include "miasfgradientlevelsetsegmenter.h"
#include "MiaLevelSetModules/LevelsetKernels/levelsetfunction_gradientbased.h"
MiaSFGradientLevelSetSegmenter::MiaSFGradientLevelSetSegmenter(MiaNetwork *parent) :
    MiaSparseFieldLevelSetSegmenter(parent)
{
    moduleName = "Gradient based Sparse Field Level Set";
    setupSpeedFunction();
}

bool MiaSFGradientLevelSetSegmenter::reAllocOutputImage()
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

void MiaSFGradientLevelSetSegmenter::setupSpeedFunction()
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
        speedFunction = new LevelSetFunction_GradientBased_WithType<float>(this);
        kernelThread->setSpeedFunction(speedFunction);
    }
}
