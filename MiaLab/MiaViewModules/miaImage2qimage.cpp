#include "miaImage2qimage.h"
#include "miagraphics2dimageitem.h"
#include <QtGui>

MiaImage2QImage::MiaImage2QImage(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "MiaImage2QImage";
    qimage = new QImage(5,5, QImage::Format_ARGB32);
    for( int i = 0 ; i < qimage->height() ; i++)
    {
        uchar* out = qimage->scanLine(i);
        for(int j = 0 ; j < qimage->width() ; j++)
        {
            *out++ = 0; //R
            *out++ = 0; //G
            *out++ = 0; //B
            *out++ = 0; //A
        }
    }

    qimageItem = new MiaGraphics2DImageItem(this);
    qimageItem->setPixmap(QPixmap::fromImage ( *qimage));

    curWinWidth = 255;
    curWinLevel = 127.5;
    currentLut = NULL;

    wiredParameters.insert(QString("ww"),curWinWidth);
    wiredParameters.insert(QString("wl"),curWinLevel);

    requiredInputImageType = IM_float;
    inputGeometryChanged = false;

}
MiaImage2QImage::~MiaImage2QImage()
{

    if(qimage)
        delete qimage;
    emit qimageWillRemoved(qimageItem);
    if(qimageItem && !qimageItem->scene())
        delete qimageItem;
// Note: graphics item is delete from the graphics scence if item is not removed from view,
// this requires the MiaImage2QImage module is delted before the view, so imageModuleHasBeenDeleted signal
//    will remove the item from the view.
// this is solved by calling destroyNetwork before the mainwindow is destroyed
// therefore the scene will always be destroyed after image items have been removed.

}
void MiaImage2QImage::inputUpdated()
{
    inputGeometryChanged = true;
    MiaModule::inputUpdated();
}

void MiaImage2QImage::setWLWW(float wl, float ww)
{
    curWinWidth = ww;
    curWinLevel = wl;
    wiredParameters.insert(QString("ww"),curWinWidth);
    wiredParameters.insert(QString("wl"),curWinLevel);
    emit parameterChanged(wiredParameters);
    update();
}

QImage* MiaImage2QImage::getQImage()
{
    if(needUpdate)
        update();
    return qimage;
}
MiaGraphics2DImageItem* MiaImage2QImage::getQImageItem()
{

    return qimageItem;
}
bool MiaImage2QImage::reloadInputImage()
{   
    bool result = false;
    MiaImageND* tempimg = NULL;
    emit requestUpdateInput(&tempimg);
    if(!tempimg)
        return false;
    if(tempimg->getType() == IM_RGBA )
    {
        if(inputImage!=tempimg)
        {
            if(inputImage)
                inputImage->release();
            inputImage = tempimg;
            inputImage->retain();

        }
        result = true;
    }
    else
    {
        result = MiaModule::reloadInputImage();
    }

    return result;
}

bool MiaImage2QImage::runOperation()
{
    if(inputImage->getType() == IM_RGBA)
    {
         convertRGBAImageToQImage();
    }
    else if(currentLut)
    {
        float* indata = (float*) inputImage->getDataPtr();
        currentLut->processingImage(qimage, indata, curWinWidth, curWinLevel);
    }
    else
    {
        float* indata = (float*) inputImage->getDataPtr();
        int i;
    //    #pragma omp parallel for
        for( i = 0 ; i < qimage->height() ; i++)
        {
            uchar* out = qimage->scanLine(i);
            float* in = indata + i*inputImage->getWidth();

            int j;
            for(j = 0 ; j < qimage->width() ; j++)
            {
                float color = (*(in +j) - curWinLevel)/curWinWidth *255.0 + 127.5 ;
                if(color < 0.0)
                    color = 0.0;
                else if (color > 255.0)
                    color = 255.0;
                uchar ucolor = (uchar) roundf(color); //from float to unchar there is 1 different;

                *out++ = ucolor; //R
                *out++ = ucolor; //G
                *out++ = ucolor; //B
                *out++ = 0xff; //A

            }
        }
    }
    qimageItem->setPixmap(QPixmap::fromImage ( *qimage));
    qimageItem->setInputImage(inputImage);
    if(inputGeometryChanged)
        emit qimageGeometryChanged(qimageItem);
    inputGeometryChanged = false;

    return true;
}

bool MiaImage2QImage::reAllocOutputImage()
{
    if(qimage)
    {
        if(qimage->width() != inputImage->getWidth() || qimage->height() != inputImage->getHeight())
        {
//                emit qimageWillRemoved(qimageitem); not need as item is not deleted
            delete qimage;
            qimage = new QImage(inputImage->getWidth(),inputImage->getHeight(),QImage::Format_ARGB32);

//                if(qimage)
//                    emit qimageAdded(qimage);
        }
    }
    else
    {
        qimage = new QImage(inputImage->getWidth(),inputImage->getHeight(),QImage::Format_ARGB32);
    }
//        qimageItem->setPixmap(QPixmap::fromImage ( *qimage));
    if(!qimage)
        return false;
    QTransform transform;
    float flipupdown = 1.0, flipleftright = 1.0;
    if(inputImage->needFlipLeftRight)
        flipleftright = -1.0;
    if(inputImage->needFlipUpDown)
        flipupdown = -1.0;
    float* spacing = inputImage->getSpacing();
    transform.translate(-0.5f*flipleftright*spacing[0], -0.5f*flipupdown*spacing[1]);
    transform.scale(flipleftright*spacing[0], flipupdown*spacing[1]);
    qimageItem->setTransform(transform);


    return true;
}
void MiaImage2QImage::dispatchParameters(const QMap <QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if(i.key() == "ww")
         {
             curWinWidth = i.value().toFloat();
         }
         else  if(i.key() == "wl")
         {
             curWinLevel = i.value().toFloat();
         }
     }
     if(logging_level >= 3 )
     {
         logMessage(moduleName+":change Window Settings WL:"+QVariant(curWinLevel).toString()+" WW:"+QVariant(curWinWidth).toString());
     }

}
void MiaImage2QImage::convertRGBAImageToQImage()
{

    QRgb* indata = (QRgb*) inputImage->getDataPtr();
    int i;
//    #pragma omp parallel for
    for( i = 0 ; i < qimage->height() ; i++)
    {
        QRgb* out = (QRgb*) qimage->scanLine(i);
        QRgb* in = indata + i*inputImage->getWidth();

        memcpy(out,in,qimage->width()*sizeof(QRgb));
    }
}
