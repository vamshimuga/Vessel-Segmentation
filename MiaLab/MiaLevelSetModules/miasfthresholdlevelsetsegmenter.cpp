#include "miasfthresholdlevelsetsegmenter.h"
#include "MiaLevelSetModules/LevelsetKernels/levelsetfunction_thresholdbased.h"

MiaSFThresholdLevelSetSegmenter::MiaSFThresholdLevelSetSegmenter(MiaNetwork *parent) :
    MiaSparseFieldLevelSetSegmenter(parent)
{
    moduleName = "Threshold based Sparse Field Level Set";
    setupSpeedFunction();
}

bool MiaSFThresholdLevelSetSegmenter::reAllocOutputImage()
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

        if(usingAdvectionForce)
        {
            LevelSetFunction_ThresholdBased_WithAdvectionForce_WithType<float> * sf = (LevelSetFunction_ThresholdBased_WithAdvectionForce_WithType<float> *)speedFunction;
            if(advectionFunction)
            {
                if(advectionFunction->getType() != IM_short)
                {
                    MiaImageND* tempimg = castImageToAnotherType(advectionFunction, IM_short);
                    if(!tempimg)
                        return false;
                    advectionFunction->release();
                    advectionFunction = tempimg;
                    advectionFunction->retain();
                }
                sf->setGrayScaleImageAndStatusImage((short*)advectionFunction->getDataPtr(),(uchar*)levelSetStatusMap->getDataPtr());
            }
        }
        needReIntialize = false;
    }
    return true;

}

void MiaSFThresholdLevelSetSegmenter::setupSpeedFunction()
{
    if(speedFunction)
    {
        delete speedFunction;
        speedFunction = NULL;
    }
    if(usingAdvectionForce)
    {
        speedFunction = new LevelSetFunction_ThresholdBased_WithAdvectionForce_WithType<float>(this);
    }
    else
    {
        speedFunction = new LevelSetFunction_ThresholdBased_WithType<float>(this);
    }
    kernelThread->setSpeedFunction(speedFunction);
}
