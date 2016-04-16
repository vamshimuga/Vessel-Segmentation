#include "miaintensitymappingfilter.h"
#include <QtGui>


MiaIntensityMappingFilter::MiaIntensityMappingFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Intensity mapping";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    parameterB = 100.0;
    parameterA = 0.0;
    parameterC = -MAX_FLOAT;
    parameterD = MAX_FLOAT;
    wiredParameters.insert(QString("lower_threshold"), parameterA);
    wiredParameters.insert(QString("upper_threshold"), parameterB);
    selectedMappingMethod = MIA_Tent;
}
bool MiaIntensityMappingFilter::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();
    float* out = (float*)outputImage->getDataPtr();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            result = runOperation<uchar>(in,out);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            result = runOperation<short>(in,out);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            result = runOperation<unsigned short>(in,out);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            result = runOperation<int>(ini,out);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
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
    default:
        throw std::runtime_error("Unkown Image Type");
    }
    outputImage->copyGeometryInfoFrom(inputImage);

    return result;
}

template <class S>
bool MiaIntensityMappingFilter::runOperation(S* inData, float* outdata)
{
    switch(selectedMappingMethod)
    {
    case MIA_Tent:
        tentMapping<S>(inData, outdata);
        break;
    case MIA_Multiply:
        multiplyMapping<S>(inData, outdata);
        break;
    case MIA_Exponential:
        exponentialMapping<S>(inData,outdata);
        break;
    case MIA_Cubic:
        cubicMapping<S>(inData, outdata);
        break;
    case MIA_LUTMapping:
        lutMapping<S>(inData, outdata);
        break;
    default:
        throw std::runtime_error("Unkown Mapping method");;
    }

//    qDebug()<<"mapping image";
    return true;
}

template <class S>
bool MiaIntensityMappingFilter::tentMapping(S* inData, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
    float median = ( parameterA + parameterB ) / 2.0;
    float epsilon = fabs(parameterB - median);
    float leftborder, rightborder;
    if( parameterA <= parameterB )
    {
        leftborder = parameterA - epsilon;
        rightborder = parameterB + epsilon;
 #pragma omp parallel for
        for( i = 0; i < size; i++ )
        {

            float var = *(inData+i);
            if(var < leftborder)
                var = leftborder;
            else if (var > rightborder)
                var = rightborder;
            *(outdata+i) = (epsilon - fabs(var - median))/epsilon;

        }
    }
    else
    {
        leftborder = parameterB + epsilon;
        rightborder = parameterA + epsilon;
 #pragma omp parallel for
        for( i = 0; i < size; i++ )
        {
            float var = *(inData + i);
            if(var < leftborder)
                var = leftborder;
            else if (var > rightborder)
                var = rightborder;
            *(outdata + i) = (fabs(var - median) - epsilon)/epsilon;

        }
    }
    return true;
}


template <class S>
bool MiaIntensityMappingFilter::multiplyMapping(S* inData, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
//#pragma omp parallel for
       for( i = 0; i < size; i++ )
       {
           float var = *(inData+i);
           var = (var+parameterB)*parameterA;
           if(var>parameterD)
               var = parameterD;
           if(var<parameterC)
               var = parameterC;
           *(outdata+i) = var;
       }
    return true;
}

template <class S>
bool MiaIntensityMappingFilter::exponentialMapping(S* inData, float* outdata)
{
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
       for( i = 0; i < size; i++ )
       {
           float var = *(inData+i);
           *(outdata+i) = exp(var*parameterA);
           if(*(outdata+i)<=0)
               *(outdata+i) = 1.0e-34;
       }
       return true;
}

template <class S>
bool MiaIntensityMappingFilter::cubicMapping(S* inData, float* outdata)
{
    if( parameterB < 0)
        parameterB = - parameterB;
    float limit = pow(parameterB,0.33f);
    int i;
    int size = inputImage->getImageSize();
#pragma omp parallel for
       for( i = 0; i < size; i++ )
       {
           float var = *(inData+i);
           if(var>-limit && var<limit)
               *(outdata+i) = parameterA*var*var*var;
           else
           {
               if(var > 0)
                    *(outdata+i) = parameterB;
               else
                   *(outdata+i) = -parameterB;
           }
       }
       return true;
}

bool MiaIntensityMappingFilter::reAllocOutputImage()
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

void MiaIntensityMappingFilter::dispatchParameters(const QMap<QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if(i.key() == "lower_threshold")
         {
             parameterA = i.value().toFloat();
         }
         else if(i.key() == "upper_threshold")
         {
             parameterB = i.value().toFloat();
         }

     }
}
template <class S>
bool MiaIntensityMappingFilter::lutMapping(S* inData, float* outdata)
{
    float x[256],y[256];
    if(lookupTabel.size()>256)
    {
        qDebug()<< "look up table too big";
        throw std::runtime_error("look up table too big");;
    }
    if(lookupTabel.size()<0)
    {
        qDebug()<< "look up table has no enough points";
        return false;
    }
    int tablesize = lookupTabel.size();
    for(int i = 0 ;i < tablesize; i ++)
    {
        x[i] = lookupTabel[i].toPointF().rx();
        y[i] = lookupTabel[i].toPointF().ry();
    }

    int size = inputImage->getImageSize();
#pragma omp parallel for
   for(int i = 0; i < size; i++ )
    {
        float var = *(inData+i);
        int index = -1;
        for(int j = 0; j < tablesize; j ++)
        {
            if(var>x[j])
            {
                index = j;
            }
            else
            {
                break;
            }
        }
        if(index == -1)
            *(outdata+i) = y[0];
        else if(index == tablesize-1)
            *(outdata+i) = y[tablesize-1];
        else
        {
            *(outdata+i) = y[index] + (var-x[index])*(y[index+1] - y[index])/(x[index+1]-x[index]);
        }

    }
   return true;

}
