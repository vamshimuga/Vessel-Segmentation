#include "miaregionstatistics.h"
#include <QtGui>

MiaRegionStatistics::MiaRegionStatistics(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "image statistic";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    inputModule = NULL;
    maskInputModule = NULL;
    maskImage = NULL;

    numberOfRegions = 0;
    summaryStr = QString("");


}

MiaRegionStatistics::~MiaRegionStatistics()
{
    if(maskImage)
        maskImage->release();
}


bool MiaRegionStatistics::runOperation()
{

    int imageSize = inputImage->getImageSize();
    if(imageSize != maskImage->getImageSize())
    {
        qDebug()<<"input Image and mask Image doesn't have the same dimension";
        return false;
    }
    ImageType inputImageType, maskImageType;
    inputImageType = inputImage->getType();
    maskImageType = maskImage->getType();
    if(inputImageType == IM_float && maskImageType == IM_float)
        runOperation<float,float>((float*)inputImage->getDataPtr(),(float*)maskImage->getDataPtr());
    else if(inputImageType == IM_float && maskImageType == IM_int)
        runOperation<float,int>((float*)inputImage->getDataPtr(),(int*)maskImage->getDataPtr());
    else if(inputImageType == IM_float && maskImageType == IM_short)
        runOperation<float,short>((float*)inputImage->getDataPtr(),(short*)maskImage->getDataPtr());
    else if(inputImageType == IM_float && maskImageType == IM_uchar)
        runOperation<float,unsigned char>((float*)inputImage->getDataPtr(),(unsigned char*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_float)
        runOperation<short,float>((short*)inputImage->getDataPtr(),(float*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_int)
        runOperation<short,int>((short*)inputImage->getDataPtr(),(int*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_short)
        runOperation<short,short>((short*)inputImage->getDataPtr(),(short*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_uchar)
        runOperation<short,unsigned char>((short*)inputImage->getDataPtr(),(unsigned char*)maskImage->getDataPtr());
    else if(inputImageType == IM_unsignedshort && maskImageType == IM_float)
        runOperation<unsigned short,float>((unsigned short*)inputImage->getDataPtr(),(float*)maskImage->getDataPtr());
    else if(inputImageType == IM_unsignedshort && maskImageType == IM_int)
        runOperation<unsigned short,int>((unsigned short*)inputImage->getDataPtr(),(int*)maskImage->getDataPtr());
    else if(inputImageType == IM_unsignedshort && maskImageType == IM_short)
        runOperation<unsigned short,short>((unsigned short*)inputImage->getDataPtr(),(short*)maskImage->getDataPtr());
    else if(inputImageType == IM_unsignedshort && maskImageType == IM_uchar)
        runOperation<unsigned short,unsigned char>((unsigned short*)inputImage->getDataPtr(),(unsigned char*)maskImage->getDataPtr());

    else
    {
        qDebug() << "image type not supported by" << moduleName;
        return false;
    }
    return true;

}

template <class A, class B>
bool MiaRegionStatistics::runOperation(A* inData, B* regiondata)
{
    int r_i;
    int imageSize = inputImage->getImageSize();
    int i;
    for(r_i = 0; r_i < numberOfRegions; r_i++)
    {
        mean[r_i] = 0;
        standard_deviation[r_i] = 0;
        max[r_i] = -3.8E+35;
        min[r_i] = 3.8E+35;
        volume[r_i] = 0;
        number_voxel[r_i] = 0;
    }
    for(i = 0 ; i < imageSize ; i++)
    {
        float value = regiondata[i];
        for(r_i = 0; r_i < numberOfRegions; r_i++)
        {
            if( value> region_lower_threshold[r_i] && value < region_upper_threshold[r_i]  )
            {
                mean[r_i] += inData[i];
                standard_deviation[r_i] += inData[i]*inData[i];
                number_voxel[r_i] ++;
                if(max[r_i] < inData[i])
                    max[r_i] = inData[i];
                if(min[r_i] > inData[i])
                    min[r_i] = inData[i];
            }
        }

    }
    summaryStr = QString("");
    for(r_i = 0; r_i < numberOfRegions; r_i++)
    {
        if(number_voxel[r_i]>0)
        {
            mean[r_i] = mean[r_i]/(float)number_voxel[r_i];
            float* spacing = inputImage->getSpacing();
            volume[r_i] = number_voxel[r_i]*spacing[0]*spacing[1]*spacing[2]/1000.0;

            standard_deviation[r_i] = sqrt(standard_deviation[r_i]/number_voxel[r_i]-mean[r_i]*mean[r_i]);
            QTextStream(&summaryStr) << regionNameList[r_i] << "\n";
            QTextStream(&summaryStr) << "Volume:" << volume[r_i] << "ml\n";
            QTextStream(&summaryStr) << "Mean:" << mean[r_i] << "\n";
            QTextStream(&summaryStr) << "STD:" << standard_deviation[r_i] << "\n";
            QTextStream(&summaryStr) << "Max:" << max[r_i] << "\n";
            QTextStream(&summaryStr) << "Min:" << min[r_i] << "\n";
        }
        else
        {
            QTextStream(&summaryStr) << regionNameList[r_i]<<"\n";
            QTextStream(&summaryStr) << "Volume:" << 0 << "ml\n";
            QTextStream(&summaryStr) << "Mean:N/A\n";
            QTextStream(&summaryStr) << "STD:N/A\n";
            QTextStream(&summaryStr) << "Max:N/A\n";
            QTextStream(&summaryStr) << "Min:N/A\n";
        }
    }

    qDebug() << summaryStr;
    return true;
}
bool MiaRegionStatistics::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        MiaImageND* tempimg = NULL;
        emit requestUpdateMaskImage(&tempimg);
        if(!tempimg)
            return false;
        if(maskImage)
            maskImage->release();
        maskImage = tempimg;
        maskImage->retain();
    }
    else
        return false;
    return true;
}

void MiaRegionStatistics::setMaskImageInputModule( MiaModule* input)
{
    if(maskInputModule)
    {
        maskInputModule->disconnect(this);
        disconnect(maskInputModule);

    }

    connect(this, SIGNAL(requestUpdateMaskImage(MiaImageND**)), input, SLOT(getOutputImage(MiaImageND**)));
    connect(input, SIGNAL(outputUpdated()), this, SLOT(maskImageReset()));
    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }

}
void MiaRegionStatistics::maskImageReset()
{
    needUpdate = true;
    if(autoUpdateWhenInputChanged)
        update();
}
void MiaRegionStatistics::dispatchParameters(const QMap<QString, QVariant> &parameters)
{
}
void MiaRegionStatistics::exportSamplesToPlainText(QString filename)
{

    ImageType inputImageType, maskImageType;
    inputImageType = inputImage->getType();
    maskImageType = maskImage->getType();
    if(inputImageType == IM_float && maskImageType == IM_float)
        exportSamplesToPlainText<float,float>(filename, (float*)inputImage->getDataPtr(),(float*)maskImage->getDataPtr());
    else if(inputImageType == IM_float && maskImageType == IM_int)
        exportSamplesToPlainText<float,int>(filename, (float*)inputImage->getDataPtr(),(int*)maskImage->getDataPtr());
    else if(inputImageType == IM_float && maskImageType == IM_short)
        exportSamplesToPlainText<float,short>(filename, (float*)inputImage->getDataPtr(),(short*)maskImage->getDataPtr());
    else if(inputImageType == IM_float && maskImageType == IM_uchar)
        exportSamplesToPlainText<float,unsigned char>(filename, (float*)inputImage->getDataPtr(),(unsigned char*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_float)
        exportSamplesToPlainText<short,float>(filename, (short*)inputImage->getDataPtr(),(float*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_int)
        exportSamplesToPlainText<short,int>(filename, (short*)inputImage->getDataPtr(),(int*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_short)
        exportSamplesToPlainText<short,short>(filename, (short*)inputImage->getDataPtr(),(short*)maskImage->getDataPtr());
    else if(inputImageType == IM_short && maskImageType == IM_uchar)
        exportSamplesToPlainText<short,unsigned char>(filename, (short*)inputImage->getDataPtr(),(unsigned char*)maskImage->getDataPtr());
    else
    {
        qDebug() << "image type not supported by" << moduleName;
        return;
    }

}

void MiaRegionStatistics::addRegion(QString name, float lower, float upper)
{
    regionNameList << name;
    region_lower_threshold[numberOfRegions] = lower;
    region_upper_threshold[numberOfRegions] = upper;
    numberOfRegions++;
    needUpdate = true;

}
template <class A, class B>
void MiaRegionStatistics::exportSamplesToPlainText(QString filename,A* inData, B* regiondata)
{
    if(filename.right(4)!= ".txt")
        filename.append(".txt");
    QFile mhdfile(filename);
    if (!mhdfile.open(QIODevice::WriteOnly | QIODevice::Text))
       return;

    float value;
    QByteArray aparameter;
    char para[512];
    int imageSize = inputImage->getImageSize();

    for(int i = 0 ; i < imageSize ; i++)
    {
        if(regiondata[i] > region_lower_threshold[0] && regiondata[i] < region_upper_threshold[0]  )
        {
            value = inData[i];
            sprintf(para, "%f\n", value);
            aparameter = para;
            mhdfile.write(aparameter);
        }

    }

    mhdfile.flush();
    mhdfile.close();
}
