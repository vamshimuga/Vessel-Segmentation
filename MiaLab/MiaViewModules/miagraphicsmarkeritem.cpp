#include "miagraphicsmarkeritem.h"
#include <QtGui>
#include <QGraphicsSceneMouseEvent>
#include "miaimageview2d.h"


MiaGraphicsMarkerItem::MiaGraphicsMarkerItem(MiaImageView2D* view, MiaMarker* marker):
    QGraphicsItem(0)
{
    parentMarker = marker;
    parentView = view;
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setCursor(parentMarker->cursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    isUpdatingOrigin = false;
    isEditable = true;
    markersAlpha = 1.0f;
    visibleDistance = 0.0f;
}

MiaGraphicsMarkerItem::~MiaGraphicsMarkerItem()
{
    if(parentView)
        parentView->blockLeftButtonEvents = false;
}

void MiaGraphicsMarkerItem::updateShapePath()
{

    if(parentMarker->justSelectedAndNeedResetFocus)
    {
        parentMarker->justSelectedAndNeedResetFocus = false;
        MiaPoint4D pt = parentMarker->getFocus();
        parentView->moveCrossHairTo(pt);
    }
    path = QPainterPath();
    path_shape = QPainterPath();
    highlights = QPainterPath();
    controlNodesList.clear();
    prepareGeometryChange();
    if(parentMarker->isVisible)
    {
        origin = parentMarker->creatShapePath(path,path_shape,highlights, controlNodesList, transformFromSceneToPatient, visibleDistance);
    }

    isUpdatingOrigin = true;
    setPos(origin.pos[0],origin.pos[1]);
    isUpdatingOrigin = false;

    while( controlNodesItemList.size() <  controlNodesList.size() )
    {
        MiaGraphicsMarkerControlNodeItem* item = new MiaGraphicsMarkerControlNodeItem(parentView, parentMarker, this);
        controlNodesItemList.append( item );
    }
    while( controlNodesItemList.size() > controlNodesList.size() )
    {
        delete controlNodesItemList.takeLast();
    }
    for( int i = 0 ; i < controlNodesItemList.size() ; i++)
    {
        MiaPoint4D pos = controlNodesList.at(i).projectedPosition;
        controlNodesItemList.at(i)->ignorePositionChange = true;
        controlNodesItemList.at(i)->setPos( pos.pos[0], pos.pos[1] );
        controlNodesItemList.at(i)->setZValue( pos.pos[2] );
        controlNodesItemList.at(i)->correspondingKeyPointIndex = controlNodesList.at(i).index;
        controlNodesItemList.at(i)->ignorePositionChange = false;
    }


    if(!parentMarker->isFrozen && isEditable && (parentMarker->currentMode == MIA_Selected || parentMarker->currentMode == MIA_Initializing) )
    {
        showAllControlPoints(true);

    }
    else
    {
        showAllControlPoints(false);
    }


}

QRectF MiaGraphicsMarkerItem::boundingRect() const
{
    return path.boundingRect();
}
QPainterPath MiaGraphicsMarkerItem::shape() const
{
    if(parentMarker->isFrozen || !isEditable || parentMarker->currentMode == MIA_Initializing)
    {
        QPainterPath emptypath;
        return emptypath;
    }
    return path_shape;
}
void MiaGraphicsMarkerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    QColor c;
    if(parentMarker->currentMode == MIA_Initializing)
        c = parentMarker->initStateColor;
    else
        c = parentMarker->color;
    c.setAlphaF(c.alphaF()*markersAlpha);
    QPen pen(c);
    pen.setWidth(parentMarker->lineWidth);
    pen.setCosmetic(true);
    painter->setPen(pen);
    if(parentMarker->currentMode == MIA_Selected)
    {
        QColor semi_transparent = parentMarker->brushColor;
        painter->setBrush( QBrush (semi_transparent) );
    }
    else if(parentMarker->currentMode == MIA_PreActive)
    {
        QColor semi_transparent = parentMarker->preActiveColor;
        painter->setBrush( QBrush (semi_transparent) );
    }
    painter->drawPath(path);

    c.setAlphaF(1.0f);
    pen.setColor(c);
    pen.setWidth(parentMarker->lineWidth + 1);
    painter->setPen(pen);
    painter->drawPath(highlights);

//    qDebug() << path;
}

void MiaGraphicsMarkerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    setSelected(true);
    if(parentMarker->isFrozen || !isEditable)
        return;
    if(parentMarker->currentMode != MIA_Selected && parentMarker->currentMode != MIA_Initializing)
    {
        parentMarker->setSelected(true);
    }

    else if(parentMarker->currentMode == MIA_Selected)
    {
        MiaPoint4D pt = parentView->convertScenePointToPatientCoordinate( event->scenePos() );
//        if(parentMarker->crossHairNeedMoveAfterClickAt(pt))
//            parentView->moveCrossHairTo(pt);
        if(!parentMarker->handleMouseButtonPressedIn2DPrjectionView(pt,transformFromSceneToPatient))
            QGraphicsItem::mousePressEvent(event);
    }
}
void MiaGraphicsMarkerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clickpt = event->scenePos();
    MiaPoint4D pt(clickpt.rx(),clickpt.ry(),0,0);
    if(parentMarker->crossHairNeedMoveAfterClickAt(pt,transformFromSceneToPatient))
        parentView->moveCrossHairTo(pt);
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant MiaGraphicsMarkerItem::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change == QGraphicsItem::ItemPositionChange && !isUpdatingOrigin) {
        if(parentMarker->isFrozen)
            return QVariant(pos());
        isUpdatingOrigin = true;

        QPointF newpos = value.toPointF();
        origin.pos[0] = newpos.x();
        origin.pos[1] = newpos.y();
        MiaPoint4D pt = transformFromSceneToPatient.toPatientCoordinate( origin );

        parentMarker->translateOriginTo(pt, true);
        isUpdatingOrigin = false;

    }

    return value;
}
void MiaGraphicsMarkerItem::controlPointMoved( MiaGraphicsMarkerControlNodeItem* item , QPointF newpos)
{

    MiaPoint4D pt;
    pt.pos[0] = newpos.x();
    pt.pos[1] = newpos.y();
    pt.pos[2] = item->zValue();
    MiaPoint4D pat = transformFromSceneToPatient.toPatientCoordinate( pt );
    parentMarker->keyPointMoved( item->correspondingKeyPointIndex, pat );


}

void MiaGraphicsMarkerItem::controlPointSelected(int index)
{
    MiaPoint4D pt;
    if(parentMarker->crossHairNeedMoveAfterControlPointSelected(index, pt))
        parentView->moveCrossHairTo(pt);
}

void MiaGraphicsMarkerItem::setMarkerAlpha(float alpha)
{
    markersAlpha = alpha;
}

void MiaGraphicsMarkerItem::setVisibleDistance(float dist)
{
    visibleDistance = dist;
}

void MiaGraphicsMarkerItem::showAllControlPoints( bool isshow )
{

    for( int i = 0 ; i < controlNodesItemList.size() ; i++)
    {
        controlNodesItemList.at(i)->setVisible(isshow);
    }

}

void MiaGraphicsMarkerItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    if(parentView)
        parentView->blockLeftButtonEvents = true;
    parentMarker->setPreActive(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void MiaGraphicsMarkerItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    if(parentView)
        parentView->blockLeftButtonEvents = false;
    parentMarker->setPreActive(false);
    QGraphicsItem::hoverLeaveEvent(event);
}
