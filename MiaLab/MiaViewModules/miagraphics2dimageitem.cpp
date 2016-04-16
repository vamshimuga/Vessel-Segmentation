#include "miagraphics2dimageitem.h"
#include <QtGui>
MiaGraphics2DImageItem::MiaGraphics2DImageItem(MiaImage2QImage* module):QGraphicsPixmapItem()
{
    miaImageItemModule = module;
    inputImage = NULL;
    setTransformationMode( Qt::SmoothTransformation );
}

MiaPoint4D MiaGraphics2DImageItem::getOrigin()
{
    MiaPoint4D origin;
    if(inputImage)
    {
        float* o = inputImage->getOffset();
        for(int i = 0 ; i < 4; i++)
            origin.pos[i] = o[i];
    }
    return origin;
}

//Graphics2DImageItem::~Graphics2DImageItem()
//{
//    qDebug() << "item deleted";
//}

//void Graphics2DImageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{

//    qDebug() << "mouse pressed in image";
//}



MiaTransform MiaGraphics2DImageItem::getImageTransformMatrix()
{
    MiaTransform t;
    if(!inputImage)
    {
        return t;
    }
    t = inputImage->computeTransformMatrixWithoutSpacing();

    if ( inputImage-> needFlipLeftRight )
    {
        t.flipLeftRight();
    }
    if ( inputImage->needFlipUpDown )
    {
        t.flipUpDown();
    }
    return t;
}

float MiaGraphics2DImageItem::getSliceThickness()
{
    return inputImage->getSliceThickness();
}

//QVariant Graphics2DImageItem::itemChange ( GraphicsItemChange change, const QVariant & value )
//{
//    qDebug() << "image Changed";
//    QGraphicsPixmapItem::itemChange(change, value);
//    //notive view to reposition images
//}

//void Graphics2DImageItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{

//}
//void Graphics2DImageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{

//}
