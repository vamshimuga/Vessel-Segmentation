#include "miagraphicscrosshairitem.h"
#include <QtGui>
#include "miaimageview2d.h"

MiaGraphicsCrossHairItem::MiaGraphicsCrossHairItem(MiaImageView2D* view, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    setCursor(Qt::OpenHandCursor);
    parentView = view;
    activeMode = false;
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
}

QRectF MiaGraphicsCrossHairItem::boundingRect() const
{

   return QRectF(-1000, -1000, 2000, 2000);
}

QPainterPath MiaGraphicsCrossHairItem::shape() const
{
    QPainterPath path;
    path.addRect(-10, -10, 20, 20);
    return path;
}

void MiaGraphicsCrossHairItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    painter->setPen(Qt::white);
    painter->setPen(Qt::red);
    QPointF pt1(-1000,0);
    QPointF pt2(1000,0);
    QPointF pt3(0, -1000);
    QPointF pt4(0, 1000);
    painter->drawLine(pt1, pt2);
    painter->drawLine(pt3, pt4);
    if(activeMode)
    {
        painter->drawRect(-10, -10, 20, 20);
    }
}


 QVariant MiaGraphicsCrossHairItem::itemChange(GraphicsItemChange change,
                      const QVariant &value)
 {
     if (change == QGraphicsItem::ItemPositionChange) {
// do something
         QPointF pt = parentView->crosshairMoved(value.toPointF());
         return QVariant(pt);
     }

     return value;
 }

 void MiaGraphicsCrossHairItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
 {
     activeMode = true;
     parentView->blockLeftButtonEvents = true;
     QGraphicsItem::hoverEnterEvent(event);
 }

 void MiaGraphicsCrossHairItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
 {
     activeMode = false;
     parentView->blockLeftButtonEvents = false;
     QGraphicsItem::hoverLeaveEvent(event);
 }
