#ifndef GRAPHICS2DIMAGEITEM_H
#define GRAPHICS2DIMAGEITEM_H

#include <QGraphicsPixmapItem>
#include "miaImage2qimage.h"


class MiaGraphics2DImageItem : public QGraphicsPixmapItem
{
public:
    MiaGraphics2DImageItem(MiaImage2QImage* module);
    virtual  ~MiaGraphics2DImageItem()
    {

    }

    MiaImage2QImage* miaImageItemModule;
    void setInputImage(MiaImageND* img) {inputImage = img;}
    MiaPoint4D getOrigin();
    MiaTransform getImageTransformMatrix();
    bool isValid() { if(inputImage) return true; else return false;}
    float getSliceThickness();

protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    MiaImageND* inputImage;
//    QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GRAPHICS2DIMAGEITEM_H
