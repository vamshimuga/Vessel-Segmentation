#include "miamultiinputintensitymappingfilter.h"
#include "miaconstants.h"

MiaMultiInputIntensityMappingFilter::MiaMultiInputIntensityMappingFilter(MiaNetwork *parent) :
    MiaModule2In1Out(parent)
{
    moduleName = "Multi-input Intensity mapping";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    parameterB = 1.0;
    parameterA = 1.0;
    parameterC = 0;
    wiredParameters.insert(QString("lower_threshold"), parameterA);
    wiredParameters.insert(QString("upper_threshold"), parameterB);
    selectedMappingMethod = MIA_Multi_Add;
    needImageTypeMatch = true;
}

bool MiaMultiInputIntensityMappingFilter::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();
    float* out = (float*)outputImage->getDataPtr();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            uchar* in2 = (uchar*) secondInputImage->getDataPtr();
            result = runOperation<uchar>(in,in2,out);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            short* in2 = (short*) secondInputImage->getDataPtr();
            result = runOperation<short>(in,in2,out);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            unsigned short* in2 = (unsigned short*) secondInputImage->getDataPtr();
            result = runOperation<unsigned short>(in,in2,out);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            int* in2 = (int*) secondInputImage->getDataPtr();
            result = runOperation<int>(ini,in2,out);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            float* in2 = (float*) secondInputImage->getDataPtr();
            result = runOperation<float>(inf,in2,out);
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

bool MiaMultiInputIntensityMappingFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    if(operateInSitu && inputImage->getType()==IM_float)
        outputImage = inputImage;
    else
        outputImage = creatEmptyImageFrom( inputImage, IM_float);
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;

    return true;
}

template <class S>
bool MiaMultiInputIntensityMappingFilter::runOperation(S* inData,S* inData2, float* outdata)
{
    switch(selectedMappingMethod)
    {
    case MIA_Multi_Add:
        return add(inData,inData2,outdata);
        break;
    case MIA_Multi_Multiply:
        return multiply(inData, inData2, outdata);
        break;
    case MIA_Multi_Mask:
        return mask(inData, inData2, outdata);
    case MIA_Brain_Distance_Map_Combining:
        return combineBrainDistanceMap(inData, inData2, outdata);
        break;
    case MIA_Brain_Model_Combining:
        return combineBrainModel(inData, inData2, outdata);
        break;
    case MIA_Phase_Analysis:
        return phaseAnalysis(inData, inData2, outdata);
        break;
    case MIA_Zero_Replace:
        return zeroRegionReplace(inData, inData2, outdata);
        break;
    default:
        throw std::runtime_error("Unknow operation");
        break;
    }
    return false;
}
template <class S>
bool MiaMultiInputIntensityMappingFilter::add(S* inData,S* inData2, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
    for( i = 0; i < size; i++ )
    {
         *(outdata+i) = (*(inData+i)) * parameterA + (*(inData2+i)) * parameterB;
    }
    return true;
}
template <class S>
bool MiaMultiInputIntensityMappingFilter::multiply(S* inData,S* inData2, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
    for( i = 0; i < size; i++ )
    {
         *(outdata+i) = (*(inData+i)) * (*(inData2+i)) * parameterA;
    }
    return true;
}
template <class S>
bool MiaMultiInputIntensityMappingFilter::mask(S* inData,S* inData2, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
    for( i = 0; i < size; i++ )
    {
        if(*(inData2+i) != 0)
            *(outdata+i) = (*(inData+i));
        else
            *(outdata+i) = parameterA;
    }
    return true;
}
template <class S>
bool MiaMultiInputIntensityMappingFilter::combineBrainDistanceMap(S* inData,S* inData2, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
    for( i = 0; i < size; i++ )
    {
        float wmvalue = (*(inData+i)) * parameterA + parameterC;
        float gmvalue = (*(inData2+i)) * parameterB - parameterC;
        if(wmvalue<0)
            wmvalue = 0;
        if(gmvalue>0)
            gmvalue = 0;

        *(outdata+i) = wmvalue + gmvalue;
    }
    return true;
}

template <class S>
bool MiaMultiInputIntensityMappingFilter::combineBrainModel(S* inData,S* inData2, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
    for( i = 0; i < size; i++ )
    {
        float intensity =  (*(inData+i));
        float distance = (*(inData2+i)) - parameterC;

          *(outdata+i) = intensity * parameterA + distance * parameterB;
    }
    return true;
}

template <class S>
bool MiaMultiInputIntensityMappingFilter::phaseAnalysis(S *inData, S *inData2, float *outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
    for( i = 0; i < size; i++ )
    {
        float magnitude = sqrt((*(inData+i))*(*(inData+i)) + (*(inData2+i))*(*(inData2+i)));
        if(magnitude == 0)
        {
            *(outdata+i) = 0.0;
            continue;
        }
        float phase  = atan2(*(inData2+i), *(inData+i));
        if(phase < 0)
            phase += parameterA*E_PI;
        float speed =  magnitude * (phase - parameterB*E_PI)*parameterC;
        if(speed > 1.0)
            speed = 1.0;
        else if (speed < -1.0)
            speed = -1.0;
        *(outdata+i) = speed;
    }
    return true;
}
template <class S>
bool MiaMultiInputIntensityMappingFilter::zeroRegionReplace(S *inData, S *inData2, float *outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
    for( i = 0; i < size; i++ )
    {
        if(*(inData+i) == 0)
            *(outdata+i) = (*(inData2+i)) * parameterB;
        else
            *(outdata+i) = (*(inData+i)) * parameterA;
    }
    return true;
}
