#include "costfunctionbase.h"
#include "miaimagend.h"
#include "transformfunctionbase.h"
#include "miaconstants.h"
#include "intensitymappingfuctionbase.h"
#include <QtGui>
#include <omp.h>
#include <iostream>

CostFunctionBase::CostFunctionBase()
{
    fixedImage = NULL;
    weightingImage = NULL;
    movingImage= NULL;
    transformFunction = NULL;
    currentSampleStrategy = MIA_Sample_All;
    currentSampleCount[0] = 0;
    sampleSizePerThread[0] = 0;
    dataSize = 0;
    sampleSize = 0;
    imageWidth = 0,imageHeight = 0,imageDepth = 0;
    numberOfThreadRunning = 1;
    backgroundValue = -MAX_FLOAT;
    sampleInterval = 0;
    intensityMappingFucntion = NULL;
    for(int i = 0 ; i< 3; i++)
        downSampledDimSize[i] = 1;
}

CostFunctionBase::~CostFunctionBase()
{
    if(fixedImage)
        fixedImage->release();
    if(movingImage)
        movingImage->release();
    if(weightingImage)
        weightingImage->release();
}

void CostFunctionBase::setFixedImage(MiaImageND *input)
{
    if(fixedImage)
        fixedImage->release();
    fixedImage = input;
    if(input)
    {
        fixedImage->retain();
        transformFunction->setFixedImageTransform(fixedImage->getTransformMatrix().getBulkTransform().toEigenTransform());
    }
}

void CostFunctionBase::setMovingImage(MiaImageND *input)
{
    if(movingImage)
        movingImage->release();
    movingImage = input;
    if(input == NULL)
        return;
    movingImage->retain();
    transformFunction->setMovingImageTransform(movingImage->getTransformMatrix().getBulkTransform().toEigenTransform());
}

void CostFunctionBase::setWeightingImage(MiaImageND *input)
{
    if(weightingImage)
        weightingImage->release();
    weightingImage = input;
    if(input == NULL)
        return;
    weightingImage->retain();
}

MiaImageND *CostFunctionBase::getMovingImage()
{
    return movingImage;
}

void CostFunctionBase::setTransformFunction(TransformFunctionBase *func)
{
    transformFunction = func;
}

void CostFunctionBase::setIntensityMappingFunction(IntensityMappingFuctionBase *func)
{
    intensityMappingFucntion = func;
}

int CostFunctionBase::getNumberOfTransformationParameters() const
{
    return transformFunction->getNumberofParameters();
}

int CostFunctionBase::getNumberOfMappingParameters() const
{
    if(intensityMappingFucntion)
        return intensityMappingFucntion->getNumberofParameters();
    else
        return 0;
}

bool CostFunctionBase::setCurrentTransformationParameters(const Eigen::VectorXf &parameters)
{
    return transformFunction->setCurrentParameters(parameters);
}

bool CostFunctionBase::setCurrentIntensityMappingParameters(const Eigen::VectorXf &parameters)
{
    return intensityMappingFucntion->setCurrentParameters(parameters);
}

bool CostFunctionBase::getCurrentIntensityMappingParameters(Eigen::VectorXf &parameters)
{
    if(intensityMappingFucntion)
        return intensityMappingFucntion->getCurrentParameters(parameters);
    else
        return false;
}

bool CostFunctionBase::getCurrentTransformationParameters(Eigen::VectorXf &parameters)
{
    if(transformFunction)
        return transformFunction->getCurrentParameters(parameters);
    else
        return false;
}

void CostFunctionBase::initializeParamerter()
{
    if(isValid())
    {
        int dimension = fixedImage->getDimension();
        int numpts = pow(2,dimension);
        Eigen::Matrix3Xf fixedpts(3,numpts);
        Eigen::Matrix3Xf movingpts(3,numpts);
        int* fixedsize = fixedImage->getDimSize();
        int* movingsize = movingImage->getDimSize();
        if(dimension == 2)
        {
            fixedpts<<      0, fixedsize[0],            0, fixedsize[0],
                            0,            0, fixedsize[1], fixedsize[1],
                            0,            0,            0,            0;

            movingpts<<     0, movingsize[0],             0, movingsize[0],
                            0,             0, movingsize[1], movingsize[1],
                            0,            0,            0,            0;
        }
        if(dimension == 3)
        {
            fixedpts<<      0, fixedsize[0],            0, fixedsize[0],            0, fixedsize[0],            0, fixedsize[0],
                            0,            0, fixedsize[1], fixedsize[1],            0,            0, fixedsize[1], fixedsize[1],
                            0,            0,            0,            0, fixedsize[2], fixedsize[2], fixedsize[2], fixedsize[2];

            movingpts<<      0, movingsize[0],            0, movingsize[0],            0, movingsize[0],            0, movingsize[0],
                            0,            0, movingsize[1], movingsize[1],            0,            0, movingsize[1], movingsize[1],
                            0,            0,            0,            0, movingsize[2], movingsize[2], movingsize[2], movingsize[2];
        }
        transformFunction->initialParametersWithPointPairs(fixedpts,movingpts);
        if(intensityMappingFucntion)
            intensityMappingFucntion->resetParameters();
        qDebug() << "Cost Fuction is reset.";
    }
}

bool CostFunctionBase::isValid() const
{
    if(transformFunction != NULL && fixedImage != NULL && movingImage != NULL
            && fixedImage->getDimension() == movingImage->getDimension())
    {
        if(currentSampleStrategy == MIA_Sample_Given && sampleSize <= 0)
            return false;
        if(currentSampleStrategy == MIA_Sample_DownSampled && sampleInterval <= 1)
            return false;
        return transformFunction->isValid();
    }
    if(fixedImage == NULL)
    {
        std::cout <<"No fixed image" <<std::endl;
    }
    if(movingImage == NULL)
    {
        std::cout <<"No movingImage image" <<std::endl;
    }
    if(transformFunction == NULL)
    {
        std::cout <<"No Transform function" <<std::endl;
    }
    if(transformFunction != NULL && fixedImage != NULL && fixedImage->getDimension() != movingImage->getDimension())
    {
        std::cout << "fixed image:" <<fixedImage->getDimension()
                  << " moving image" << movingImage->getDimension() << std::endl;
    }
    return false;
}

void CostFunctionBase::setSampleStrategy(SampleStrategy strategy, int numpt, std::vector<int> *pts)
{
    currentSampleStrategy = strategy;
    sampleSize = numpt;
    sampleInterval = numpt;
    if(strategy == MIA_Sample_Given)
    {
        if(pts!=NULL)
        {
            samplePointsList = *pts;
        }
        else
        {
            throw std::runtime_error("Sample points cannot be empty");
        }
    }
    reinitialSamples();
}

void CostFunctionBase::reinitialSamples()
{
    if(fixedImage != NULL)
    {
        dataSize = fixedImage->getImageSize();
        imageWidth = fixedImage->getWidth();
        imageHeight = fixedImage->getHeight();
        imageDepth = fixedImage->getDepth();
    }
#ifdef USE_OPENMP
    numberOfThreadRunning = omp_get_max_threads();
#else
    numberOfThreadRunning = 1;
#endif
    switch(currentSampleStrategy)
    {
    case MIA_Sample_All:
        sampleSize = dataSize;
        samplePointsList.clear();
        break;
    case MIA_Sample_From_MovingImage:
    {
        if(movingImage != NULL)
        {
            sampleSize = movingImage->getImageSize();
            samplePointsList.clear();
        }
    }
        break;
    case MIA_Sample_Random:
        samplePointsList.clear();
        break;
    case MIA_Sample_Given:
        sampleSize = samplePointsList.size();
        break;
    case MIA_Sample_DownSampled:
    {
        if(fixedImage != NULL)
        {
            int* dimSize = fixedImage->getDimSize();
            sampleSize = 1;
            for(int dim = 0 ; dim < 3; dim++)
            {
                if(dimSize[dim]/sampleInterval>0)
                {
                    downSampledDimSize[dim] = dimSize[dim]/sampleInterval;
                    sampleSize *= downSampledDimSize[dim];
                }
                else
                    downSampledDimSize[dim] = 1;
            }
        }
    }
        break;
    }
    for(int i = 0; i < numberOfThreadRunning; i++)
    {
        currentSampleCount[i] = 0;
        sampleSizePerThread[i] = sampleSize/numberOfThreadRunning;
    }
    sampleSizePerThread[numberOfThreadRunning-1] += sampleSize%numberOfThreadRunning;
}

bool CostFunctionBase::getNextSamplePairs(Eigen::Vector3f& fixedpt, Eigen::Vector3f& movingpt, int threadID)
{
    if(currentSampleCount[threadID] >= sampleSizePerThread[threadID])
        return false;
    int one_dimension_index = -1;
    switch(currentSampleStrategy)
    {
    case MIA_Sample_All:
        one_dimension_index = currentSampleCount[threadID]+sampleSizePerThread[0]*threadID;
        break;
    case MIA_Sample_From_MovingImage:
        one_dimension_index = currentSampleCount[threadID]+sampleSizePerThread[0]*threadID;;
        break;
    case MIA_Sample_Random:
        if(RAND_MAX >= dataSize)
        {
            one_dimension_index = rand()%dataSize;
        }
        else
        {
            one_dimension_index = (rand()%imageDepth)*imageWidth*imageHeight + (rand()%imageHeight)*imageWidth + rand()%imageWidth;
        }
        break;
    case MIA_Sample_Given:
        one_dimension_index = samplePointsList.at(currentSampleCount[threadID]+sampleSizePerThread[0]*threadID);
        break;
    case MIA_Sample_DownSampled:
    {
        int resampled_index = currentSampleCount[threadID]+sampleSizePerThread[0]*threadID;
        int z = resampled_index/(downSampledDimSize[0]*downSampledDimSize[1]);
        int y = (resampled_index - z*(downSampledDimSize[0]*downSampledDimSize[1]))/downSampledDimSize[0];
        int x = resampled_index - z*(downSampledDimSize[0]*downSampledDimSize[1]) - y*downSampledDimSize[0];
        int* dimSize = fixedImage->getDimSize();
        one_dimension_index = (z*sampleInterval)*(dimSize[0]*dimSize[1]) + (y*sampleInterval)*dimSize[0] + x*sampleInterval;

    }
        break;
    }
    currentSampleCount[threadID] ++;
    if(currentSampleStrategy == MIA_Sample_From_MovingImage)
    {
        movingImage->convert1DIndexTo3DVoxel(one_dimension_index,movingpt.data());
        fixedpt = transformFunction->movingImageVoxelToFixedImageVoxel(movingpt);
        return true;
    }
    if( fixedImage->convert1DIndexTo3DVoxel(one_dimension_index,fixedpt.data()) )
    {
        movingpt = transformFunction->fixedImageVoxelToMovingImageVoxel(fixedpt);
        return true;
    }
    return false;
}
bool CostFunctionBase::getSampleWithIndex(Eigen::Vector3f& voxel, int index)
{
    if(index >= sampleSize)
        return false;
    int one_dimension_index = -1;
    switch(currentSampleStrategy)
    {
    case MIA_Sample_All:
        one_dimension_index = index;
        break;
    case MIA_Sample_From_MovingImage:
        one_dimension_index = index;
        break;
    case MIA_Sample_Random:
    {
        bool sampleValidate = false;
        do
        {
            if(RAND_MAX >= dataSize)
            {
                one_dimension_index = rand()%dataSize;
            }
            else
            {
                one_dimension_index = (rand()%imageDepth)*imageWidth*imageHeight + (rand()%imageHeight)*imageWidth + rand()%imageWidth;
            }
            if(fixedImage->convert1DIndexTo3DVoxel(one_dimension_index,voxel.data()))
            {
                float fixedvalue = fixedImage->getValueAt(voxel.data());
                if(fixedvalue == backgroundValue)
                    continue;
                Eigen::Vector3f movingpt = transformFunction->fixedImageVoxelToMovingImageVoxel(voxel);
                float movingvalue = movingImage->getValueAt(movingpt.data());
                if(movingvalue != backgroundValue)
                {
                    sampleValidate = true;
                }
            }
        }while(!sampleValidate);
        return true;
    }
        break;
    case MIA_Sample_Given:
        one_dimension_index = samplePointsList.at(index);
        break;
    case MIA_Sample_DownSampled:
    {
        int z = index/(downSampledDimSize[0]*downSampledDimSize[1]);
        int y = (index - z*(downSampledDimSize[0]*downSampledDimSize[1]))/downSampledDimSize[0];
        int x = index - z*(downSampledDimSize[0]*downSampledDimSize[1]) - y*downSampledDimSize[0];
        int* dimSize = fixedImage->getDimSize();
        one_dimension_index = (z*sampleInterval)*(dimSize[0]*dimSize[1]) + (y*sampleInterval)*dimSize[0] + x*sampleInterval;

    }
        break;
    }
    if(currentSampleStrategy == MIA_Sample_From_MovingImage)
    {
        Eigen::Vector3f movingvoxel;
        movingImage->convert1DIndexTo3DVoxel(one_dimension_index,movingvoxel.data());
        voxel = transformFunction->movingImageVoxelToFixedImageVoxel(movingvoxel);
        return true;
    }
    return fixedImage->convert1DIndexTo3DVoxel(one_dimension_index,voxel.data());

}
