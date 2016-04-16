#include "miagraphicsmarkercontrolnodeitem.h"
#include "miaimageview2d.h"
#include "miagraphicsmarkeritem.h"

MiaGraphicsMarkerControlNodeItem::MiaGraphicsMarkerControlNodeItem(MiaImageView2D* view, MiaMarker* marker, QGraphicsItem* parent)
    :QGraphicsItem(parent)
{
    parentMarker = marker;
    parentView = view;
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
//    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
//    isUpdatingOrigin = false;
    nodeSize = 2.0 ;
    activeMode = false;
    setZValue(parent->zValue()+0.5);
    ignorePositionChange = false;
}

QRectF MiaGraphicsMarkerControlNodeItem::boundingRect() const
{
    return QRectF(-nodeSize*3.0,-nodeSize*3.0, 6.0*nodeSize, 6.0*nodeSize);
}
QPainterPath MiaGraphicsMarkerControlNodeItem::shape() const
{
    QPainterPath path;
    path.addEllipse(QRectF(-nodeSize*2.0,-nodeSize*2.0, 4.0*nodeSize ,4.0*nodeSize));
    return path;
}
void MiaGraphicsMarkerControlNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    if(activeMode)
    {

        painter->setPen(parentMarker->nodeActiveColor);
        painter->setBrush( QBrush( parentMarker->nodeActiveColor ) );
    }
    else
    {

        painter->setPen(parentMarker->nodeRestingColor);
        painter->setBrush( QBrush( parentMarker->nodeRestingColor ) );
    }
    if(parentMarker->controlNodeSize != nodeSize)
        setNodeSize(parentMarker->controlNodeSize);
    QPainterPath path;
    path.addEllipse(QRectF(-nodeSize,-nodeSize, 2.0*nodeSize, 2.0*nodeSize));
    painter->drawPath(path);
    return;
}

void MiaGraphicsMarkerControlNodeItem::setNodeSize(float s)
{
    prepareGeometryChange();
    nodeSize = s;
}

QVariant MiaGraphicsMarkerControlNodeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
     if (change == QGraphicsItem::ItemPositionChange && !ignorePositionChange)
     {
        MiaGraphicsMarkerItem* parent = (MiaGraphicsMarkerItem*) parentItem();
        ignorePositionChange = true;
        QPointF newpos = sceneTranformBeforeDrag.map( value.toPointF() ) ;
        parent->controlPointMoved(this,newpos);
        ignorePositionChange = false;
        return QGraphicsItem::itemChange(change, parent->mapFromScene(newpos));

     }
     return QGraphicsItem::itemChange(change, value);
}

void MiaGraphicsMarkerControlNodeItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    activeMode = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void MiaGraphicsMarkerControlNodeItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    activeMode = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

void MiaGraphicsMarkerControlNodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MiaGraphicsMarkerItem* parent = (MiaGraphicsMarkerItem*) parentItem();
    parent->controlPointSelected(correspondingKeyPointIndex);

    sceneTranformBeforeDrag = parentItem()->sceneTransform();
    QGraphicsItem::mousePressEvent(event);
}

void MiaGraphicsMarkerControlNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
