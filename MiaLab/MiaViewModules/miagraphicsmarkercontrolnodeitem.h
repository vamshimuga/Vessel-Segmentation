#ifndef GraphicsMarkerControlNodeItem_H
#define GraphicsMarkerControlNodeItem_H


#include <QGraphicsItem>
#include "MiaCoreModules/miamarker.h"

class MiaImageView2D;
class MiaGraphicsMarkerControlNodeItem : public QGraphicsItem
{
public:
    MiaGraphicsMarkerControlNodeItem(MiaImageView2D* view, MiaMarker* marker, QGraphicsItem* parent);

    float  nodeSize;
    int correspondingKeyPointIndex;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setNodeSize(float s);
    bool ignorePositionChange;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    MiaImageView2D* parentView;
    MiaMarker* parentMarker;
    bool activeMode;
    QTransform sceneTranformBeforeDrag;

};

#endif // GraphicsMarkerControlNodeItem_H
