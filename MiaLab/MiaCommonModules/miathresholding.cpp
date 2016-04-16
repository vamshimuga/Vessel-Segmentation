#include "miathresholding.h"
#include <QtGui>

MiaThresholding::MiaThresholding(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "MiaThresholding";
    upperThreshold = 100.0;
    lowerThreshold = 0.0;
    wiredParameters.insert(QString("lower_threshold"), lowerThreshold);
    wiredParameters.insert(QString("upper_threshold"), upperThreshold);
    objectValue = 255.0f;
}

bool MiaThresholding::runOperation()
{
    bool result;
    ImageType sourcetype = inputImage->getType();
    uchar* out = (uchar*)outputImage->getDataPtr();
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
            qDebug() << "RGB image is not supported yet";
            return false;
        }
            break;
    }
    outputImage->copyGeometryInfoFrom(inputImage);
    return result;
}

template <class S>
bool MiaThresholding::runOperation(S* inData, uchar* outdata)
{
    int i;
    int size = inputImage->getImageSize();
    if( lowerThreshold <= upperThreshold )
    {
        for( i = 0; i < size; i++ )
        {
            if( *inData >= lowerThreshold && *inData <= upperThreshold )
                *outdata = objectValue;
            else
                *outdata = 0;

            inData++;
            outdata++;

        }
    }
    else
    {
        for( i = 0; i < size; i++ )
        {
            if( *inData >= lowerThreshold || *inData <= upperThreshold )
                *outdata = objectValue;
            else
                *outdata = 0;

            inData++;
            outdata++;

        }
    }
    return true;
}

bool MiaThresholding::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    outputImage = creatEmptyImageFrom( inputImage, IM_uchar);
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;

    return true;

}

void MiaThresholding::setUpperThreshold( int value )
{
    upperThreshold = value;
    update();
}

void MiaThresholding::setLowerThreshold( int value )
{
    lowerThreshold = value;
    update();
}

void MiaThresholding::setObjectValue(float value)
{
    objectValue = value;
}

void MiaThresholding::dispatchParameters(const QMap<QString, QVariant> &parameters)
{

    QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if(i.key() == "lower_threshold")
         {
             lowerThreshold = i.value().toFloat();
         }
         else if(i.key() == "upper_threshold")
         {
             upperThreshold = i.value().toFloat();
         }

     }
}
