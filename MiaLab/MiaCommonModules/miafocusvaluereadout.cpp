#include "miafocusvaluereadout.h"
#include <QtGui>

MiaFocusValueReadOut::MiaFocusValueReadOut(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Focus Value read out";
    curWinWidth = 0;
    curWinLevel = 0;
    value = 0;
    rgbValue = 0;
    wiredParameters.insert(QString("pat_x"), pat_coordinate.pos[0]);
    wiredParameters.insert(QString("pat_y"), pat_coordinate.pos[1]);
    wiredParameters.insert(QString("pat_z"), pat_coordinate.pos[2]);
    wiredParameters.insert(QString("pat_t"), pat_coordinate.pos[3]);
    wiredParameters.insert(QString("ww"),curWinWidth);
    wiredParameters.insert(QString("wl"),curWinLevel);
}
bool MiaFocusValueReadOut::runOperation()
{
    voxel_coordinate = inputImage->convertPatientCoordinateToVoxel(pat_coordinate);
    int* size = inputImage->getDimSize();
    int index = (int)(voxel_coordinate.pos[2])*size[1]*size[0] + (int)(voxel_coordinate.pos[1])*size[0] + (int)(voxel_coordinate.pos[0]);
    if(index < 0 || index > inputImage->getImageSize()-1)
    {
        emit UpdateSummaryText(QString("Invalid!"));
        return false;
    }
    switch(inputImage->getType())
    {
    case IM_uchar:
    {
        char* inData = (char*) inputImage->getDataPtr();
        value = inData[index];
    }
        break;
    case IM_short:
    {
        short* inData = (short*) inputImage->getDataPtr();
        value = inData[index];
    }
        break;
    case IM_unsignedshort:
    {
        unsigned short* inData = (unsigned short*) inputImage->getDataPtr();
        value = inData[index];
    }
        break;
    case IM_int:
    {
        int* inData = (int*) inputImage->getDataPtr();
        value = inData[index];
    }
        break;
    case IM_float:
    {
        float* inData = (float*) inputImage->getDataPtr();
        value = inData[index];
    }
        break;
    case IM_RGBA:
    {
        QRgb* inData = (QRgb*) inputImage->getDataPtr();
        rgbValue = inData[index];
    }
    }
    summaryStr = QString("");
    QTextStream(&summaryStr) << "WW:" << curWinWidth;
    QTextStream(&summaryStr) << " WL:" << curWinLevel;
    QTextStream(&summaryStr) << "  Crosshair voxel coordinate ( " << voxel_coordinate.pos[0] << ",";
    QTextStream(&summaryStr) << voxel_coordinate.pos[1] << ",";
    QTextStream(&summaryStr) << voxel_coordinate.pos[2] << " ),patient coordinate ( ";
    QTextStream(&summaryStr) << pat_coordinate.pos[0] << ",";
    QTextStream(&summaryStr) << pat_coordinate.pos[1] << ",";
    QTextStream(&summaryStr) << pat_coordinate.pos[2] << " ) ";

    if(inputImage->getType() == IM_RGBA)
    {
        QTextStream(&summaryStr) << "CurrentValue:"
                                 << "r:" << qRed(rgbValue)
                                 << "g:" << qGreen(rgbValue)
                                 << "b:" << qBlue(rgbValue)
                                 << "a:" << qAlpha(rgbValue);
    }
    else
    {
        QTextStream(&summaryStr) << "CurrentValue:" << value;
    }
    emit UpdateSummaryText(summaryStr);
    return true;
}
void MiaFocusValueReadOut::dispatchParameters(const QMap<QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if(i.key() == "pat_x")
         {
             pat_coordinate.pos[0] = i.value().toFloat();
         }
         else if(i.key() == "pat_y")
         {
             pat_coordinate.pos[1] = i.value().toFloat();
         }
         else if(i.key() == "pat_z")
         {
             pat_coordinate.pos[2] = i.value().toFloat();
         }
         else if(i.key() == "pat_t")
         {
             pat_coordinate.pos[3] = i.value().toFloat();
         }
         else if(i.key() == "ww")
         {
             curWinWidth = i.value().toFloat();
         }
         else  if(i.key() == "wl")
         {
             curWinLevel = i.value().toFloat();
         }
     }

}
