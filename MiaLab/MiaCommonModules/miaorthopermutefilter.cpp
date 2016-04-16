#include "miaorthopermutefilter.h"

MiaOrthoPermuteFilter::MiaOrthoPermuteFilter(MiaNetwork *parent) : MiaModule2In1Out(parent)
{
    moduleName = "3D resmaple";
    currentPermuteStrategy = Mia_Permute_AlwaysUpFront;
    for(int dim = 0; dim < 4; dim++)
    {
        newDimensionOrder[dim] = dim;
        axisFlipCoefficient[dim] = false;
    }
}

MiaOrthoPermuteFilter::~MiaOrthoPermuteFilter()
{

}

void MiaOrthoPermuteFilter::setNewDimensionOrderManually(int d0, int d1, int d2, int d3,
                                                         bool f0, bool f1, bool f2, bool f3)
{
    currentPermuteStrategy = Mia_Permute_ManuallySet;
    newDimensionOrder[0] = d0;
    newDimensionOrder[1] = d1;
    newDimensionOrder[2] = d2;
    newDimensionOrder[3] = d3;
    axisFlipCoefficient[0] = f0;
    axisFlipCoefficient[1] = f1;
    axisFlipCoefficient[2] = f2;
    axisFlipCoefficient[3] = f3;
}

bool MiaOrthoPermuteFilter::reloadInputImage()
{
    bool result;
    if(currentPermuteStrategy == Mia_Permute_UseTemplate)
    {
        result = MiaModule2In1Out::reloadInputImage();
    }
    else
        result =  MiaModule::reloadInputImage();
    if(result)
        computeNewDimensionOrder();
    return result;
}

bool MiaOrthoPermuteFilter::computeNewDimensionOrder()
{
    switch (currentPermuteStrategy) {
    case Mia_Permute_AlwaysUpFront:
    {
        float* inputOrientationMatrix = inputImage->getOrientationMatrix();
        for(int dim = 0; dim < 3; dim++)
        {
            float largestAbsolutValue = -1;
            int inputdim = -1;
            for(int i = 0; i < 3; i++)
            {
                if(fabs(inputOrientationMatrix[i*3+dim]) > largestAbsolutValue)
                {
                    largestAbsolutValue = fabs(inputOrientationMatrix[i*3+dim]);
                    inputdim = i;
                }
            }
            newDimensionOrder[dim] = inputdim;
            if( inputOrientationMatrix[ inputdim*3 + dim ] < 0)
                axisFlipCoefficient[dim] = true;
        }
    }
        break;
    case Mia_Permute_UseTemplate:
    {
        throw std::runtime_error("NOt implemented!");
    }
        break;
    default:
        break;
    }
    return true;
}

bool MiaOrthoPermuteFilter::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            uchar* out = (uchar*)outputImage->getDataPtr();
            result = runOperation<uchar>(in,out);
        }
        break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            short* out = (short*)outputImage->getDataPtr();
            result = runOperation<short>(in,out);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            unsigned short* out = (unsigned short*)outputImage->getDataPtr();
            result = runOperation<unsigned short>(in,out);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            int* out = (int*)outputImage->getDataPtr();
            result = runOperation<int>(ini,out);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            float* out = (float*)outputImage->getDataPtr();
            result = runOperation<float>(inf,out);
        }
            break;
        case IM_RGBA:
        {
        //            QRgb* inr = (QRgb*) inputImage->getDataPtr();
        //            runOperation<QRgb>(inr,out);
            qDebug() << "RGB image is not supported yet";
            return false;
        }
        break;
        default:
            throw std::runtime_error("Unkown image type!");
    }


        return result;
}


bool MiaOrthoPermuteFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
        outputImage = NULL;
    }
    float* inputSpacing = inputImage->getSpacing();
    int* inputSize = inputImage->getDimSize();
    float* inputOrientationMatrix = inputImage->getOrientationMatrix();

    float newSpacing[4];
    int newSize[4];
    float newOrientationMatrix[9];
    MiaPoint4D newOriginInInputVoxelSpace;
    for(int dim = 0; dim < 4; dim++)
    {
        int inputdim = newDimensionOrder[dim];
        newSpacing[dim] = inputSpacing[inputdim];
        newSize[dim] = inputSize[inputdim];
        if(dim < 3)
        {
            int tempinputdim = inputdim;
            if(inputdim == 3)
                tempinputdim = newDimensionOrder[3];
            assert(tempinputdim < 3);
            float flipfactor = 1.0f;
            if(axisFlipCoefficient[dim])
                flipfactor = -1.0f;
            for(int i = 0; i < 3; i++)
            {
                newOrientationMatrix[dim*3+i] = inputOrientationMatrix[tempinputdim*3+i]*flipfactor;
            }
        }
        if(!axisFlipCoefficient[dim])
            newOriginInInputVoxelSpace.pos[inputdim] = 0;
        else
            newOriginInInputVoxelSpace.pos[inputdim] = inputSize[inputdim] - 1;
    }
    outputImage =  new MiaImageND(newSize[0], newSize[1], newSize[2], newSize[3], inputImage->getType());
    MiaPoint4D newOffset = inputImage->convertVoxelToPatientCoordinate(newOriginInInputVoxelSpace);
    if(outputImage)
    {
        outputImage->setOffset(newOffset.pos);
        outputImage->setSpacing(newSpacing);
        outputImage->set3x3OrientationMatrix(newOrientationMatrix);
        outputImage->computeTransformMatrix();
        outputImage->retain();

    }
    else
        return false;

    return true;
}

bool MiaOrthoPermuteFilter::checkIfNeedReAllocOutputImage()
{
    if(outputImage)
    {
        int* inputSize = inputImage->getDimSize();
        int* outputSize = outputImage->getDimSize();
        bool sizeMatch = true;
        for(int dim = 0; dim < 4; dim++)
        {
            int inputdim = newDimensionOrder[dim];
            if(inputSize[inputdim] != outputSize[dim])
                sizeMatch = false;
        }
        if(sizeMatch)
            return false;
    }
    return true;
}


template <class S>
bool MiaOrthoPermuteFilter::runOperation(S* inData, S* outdata)
{
    int* outputSize = outputImage->getDimSize();
    int* inputSize = inputImage->getDimSize();
    int outputCoor[4];
    for(outputCoor[3] = 0; outputCoor[3] < outputSize[3]; outputCoor[3]++)
        for(outputCoor[2] = 0; outputCoor[2] < outputSize[2]; outputCoor[2]++)
            for(outputCoor[1] = 0; outputCoor[1] < outputSize[1]; outputCoor[1]++)
                for(outputCoor[0] = 0; outputCoor[0] < outputSize[0]; outputCoor[0]++)
                {
                    int outputIndex = outputCoor[3]*outputSize[0]*outputSize[1]*outputSize[2]
                            + outputCoor[2]*outputSize[0]*outputSize[1] + outputCoor[1]*outputSize[0] + outputCoor[0];
                    int inputCoor[4];
                    for(int dim = 0; dim < 4; dim++)
                    {
                        int inputdim = newDimensionOrder[dim];
                        inputCoor[inputdim] = outputCoor[dim];
                        if(axisFlipCoefficient[dim])
                            inputCoor[inputdim] = inputSize[inputdim] - 1 - inputCoor[inputdim];
                    }
                    int inputIndex = inputCoor[3]*inputSize[0]*inputSize[1]*inputSize[2]
                            + inputCoor[2]*inputSize[0]*inputSize[1] + inputCoor[1]*inputSize[0] + inputCoor[0];
                    outdata[outputIndex] = inData[inputIndex];
                }
    return true;
}
