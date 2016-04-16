#include "miamorphologyfilter.h"

MiaMorphologyFilter::MiaMorphologyFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Morphology 3D Filter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    for(int i = 0 ;i < 3; i++)
    {
        kernelSize[i] = 0;
    }
    backgroundValue = 0;
    foregroundValue = 1;
    kernelSizeInmm = 0;
}

bool MiaMorphologyFilter::runOperation()
{
    bool result = false;
    float* spacing = inputImage->getSpacing();
    for(int dim = 0; dim < 3; dim++)
    {
        kernelSize[dim] = kernelSizeInmm / spacing[dim];
    }

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
//            qDebug() << "RGB image is not supported yet";
            return false;
        }
            break;
    }
    outputImage->copyGeometryInfoFrom(inputImage);


    return result;
}

bool MiaMorphologyFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    outputImage = creatEmptyImageFrom( inputImage, outputImage->getType());
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;
    return true;
}

template <class S>
bool MiaMorphologyFilter::runOperation(S *inData, S *outdata)
{
    throw std::runtime_error("Not implemented!");
    return true;
}
