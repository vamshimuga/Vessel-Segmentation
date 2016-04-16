#include "miasparsefieldlevelsetsegmenter.h"
#include "MiaLevelSetModules/LevelsetKernels/denselevelset.h"
#include "MiaLevelSetModules/LevelsetKernels/sparsefieldlevelsetkernel.h"
#include "MiaLevelSetModules/LevelsetKernels/sparsefieldvessellevelsetkernel.h"
#ifndef Q_OS_WIN
#include <unistd.h>
#endif



MiaSparseFieldLevelSetSegmenter::MiaSparseFieldLevelSetSegmenter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Sparse Field Level Set";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    threadRunning = false;
    requiredInputImageType = IM_float;
    needReIntialize = true;
//    kernelThread = new SparseFieldWithCoherentLevelSetKernel(this);
    kernelThread = new SparseFieldLevelSetKernel(this);
//    kernelThread = new DenseLevelSet(this);
//    kernelThread = new SparseFieldVesselLevelSetKernel(this);
    levelsetInputModule = NULL;
    levelSetFunction = NULL;
    levelSetStatusMap = NULL;
    advectionFunction = NULL;
    advectionInputModule = NULL;
    usingAdvectionForce = false;
    speedFunction = NULL;
    curvatureWeightingFactor = 0.5f;
}
MiaSparseFieldLevelSetSegmenter::~MiaSparseFieldLevelSetSegmenter()
{
    if(kernelThread)
    {
        if(kernelThread->isRunning())
        {
            kernelThread->abort();
#ifndef Q_OS_WIN
            sleep(1);
#endif

            kernelThread->quit();
        }
        delete kernelThread;
    }
    if(levelSetFunction)
        levelSetFunction->release();
    if(levelSetStatusMap)
        levelSetStatusMap->release();
    if(advectionFunction)
        advectionFunction->release();

}

void MiaSparseFieldLevelSetSegmenter::levelsetFunctionReset()
{
    needReIntialize = true;
    if(autoUpdateWhenInputChanged)
        update();
}

void MiaSparseFieldLevelSetSegmenter::advectionFunctionReset()
{
    needReIntialize = true;
    if(autoUpdateWhenInputChanged)
        update();
}
void MiaSparseFieldLevelSetSegmenter::setToUseCoherentPropagation(bool onoff)
{
    if(kernelThread)
    {
        if(kernelThread->isRunning())
        {
            kernelThread->abort();
#ifndef Q_OS_WIN
            sleep(1);
#endif

            kernelThread->quit();
        }
        delete kernelThread;
    }
    if(onoff)
    {
//        kernelThread = new SparseFieldWithCoherentLevelSetKernel(this);
        throw(std::runtime_error("Do not support coherent propogation"));
    }
    else
    {
        kernelThread = new SparseFieldLevelSetKernel(this);
    }
    if(kernelThread)
    {
        kernelThread->setSpeedFunction(speedFunction);
    }
}

bool MiaSparseFieldLevelSetSegmenter::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        if(externalSpeedFunction != inputImage)
        {
            externalSpeedFunction = inputImage;
            needReIntialize = true;
        }
        MiaImageND* tempimg = NULL;
        emit requestUpdateInitialLevelSetFunction(&tempimg);
        if(!tempimg)
            return false;
#ifdef USESHORTTYPELEVELSETFUNCTION
        if( IM_short != tempimg->getType() )
        {
            tempimg = castImageToAnotherType(tempimg, IM_short);
            if(!tempimg)
                return false;
        }
#else
        if( IM_float != tempimg->getType() )
        {
            tempimg = castImageToAnotherType(tempimg, IM_float);
            if(!tempimg)
                return false;
        }
#endif
        if(levelSetFunction)
            levelSetFunction->release();
        levelSetFunction = tempimg;
        levelSetFunction->retain();

        tempimg = NULL;

        emit requestUpdateAdvectionFunction(&tempimg);
        if(tempimg)
        {
            if(advectionFunction)
                advectionFunction->release();
            advectionFunction = tempimg;
            advectionFunction->retain();
        }
    }
    else
        return false;
    return true;
}

bool MiaSparseFieldLevelSetSegmenter::runOperation()
{
    return true;
}

bool MiaSparseFieldLevelSetSegmenter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
        outputImage = NULL;
    }
    // reset output Image
    if(levelSetFunction)
    {
        outputImage = levelSetFunction;
        outputImage->retain();
        // if need re-initialize
        if(needReIntialize)
        {
            // re-create level set status map
            if(levelSetStatusMap)
                levelSetStatusMap->release();
            levelSetStatusMap = creatEmptyImageFrom(inputImage,IM_uchar);
            if(levelSetStatusMap)
            {
                levelSetStatusMap->retain();
            }
            else
            {
                return false;
            }

            //needReIntialize = false; we cannot reset this property, because the subclass need it to do things on their own
        }
    }
    else
        return false;
    return true;
}
void MiaSparseFieldLevelSetSegmenter::getStatusMap(MiaImageND **img)
{
    if(needUpdate)
        update();
    *img = levelSetStatusMap;

}

void MiaSparseFieldLevelSetSegmenter::getNarrowBandPoints(std::vector<int> &list)
{
    kernelThread->getNarrowBandPoints(list);
}

void MiaSparseFieldLevelSetSegmenter::setLevelSetFunctionInputModule( MiaModule* input)
{
    if(levelsetInputModule)
    {
        levelsetInputModule->disconnect(this);
        disconnect(levelsetInputModule);

    }
    levelsetInputModule = input;
    connect(this, SIGNAL(requestUpdateInitialLevelSetFunction(MiaImageND**)), input, SLOT(getOutputImage(MiaImageND**)));
    connect(input, SIGNAL(outputUpdated()), this, SLOT(levelsetFunctionReset()));
    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }

}

void MiaSparseFieldLevelSetSegmenter::setAdvectionFunctionInputModule(MiaModule *input)
{
    if(advectionInputModule)
    {
        advectionInputModule->disconnect(this);
        disconnect(advectionInputModule);
    }
    if(input)
    {
        usingAdvectionForce = true;
    }
    else
    {
        usingAdvectionForce = false;
    }
    advectionInputModule = input;
    connect(this, SIGNAL(requestUpdateAdvectionFunction(MiaImageND**)), input, SLOT(getOutputImage(MiaImageND**)));
    connect(input, SIGNAL(outputUpdated()), this, SLOT(advectionFunctionReset()));
    needReIntialize = true;
    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }
}
 void MiaSparseFieldLevelSetSegmenter::setCurvatureWeight(float w)
 {
     curvatureWeightingFactor = w;
 }

 void MiaSparseFieldLevelSetSegmenter::start()
 {
     assert(inputImage->getImageSize()==levelSetFunction->getImageSize() );
     speedFunction->setWeightingFactorAlpha(1.0f - curvatureWeightingFactor);
      if(threadRunning)
          kernelThread->restart();
      else
      {
        kernelThread->start();
//      kernelThread->run();
      }
      threadRunning = true;

 }

 void MiaSparseFieldLevelSetSegmenter::stop()
 {
      kernelThread->abort();
      threadRunning = false;
 }


