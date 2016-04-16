#ifndef GRAPHICSMARKERITEM_H
#define GRAPHICSMARKERITEM_H

#include <QGraphicsItem>
#include "MiaCoreModules/miamarker.h"
#include "miagraphicsmarkercontrolnodeitem.h"

class MiaImageView2D;
class MiaGraphicsMarkerItem : public QGraphicsItem
{
public:
    MiaGraphicsMarkerItem(MiaImageView2D* view, MiaMarker* marker);
    virtual ~MiaGraphicsMarkerItem();
    MiaMarker* parentMarker;
    MiaTransform transformFromSceneToPatient;
    void updateShapePath();
    void resetParentView(MiaImageView2D* view) { parentView = view; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void controlPointMoved( MiaGraphicsMarkerControlNodeItem* item, QPointF newpos);
    void controlPointSelected(int index);
    void setEditable(bool enable){ isEditable = enable; updateShapePath();}
    void setMarkerAlpha(float alpha);
    void setVisibleDistance(float dist);
protected:
    void showAllControlPoints( bool isshow );
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    QPainterPath path;
    QPainterPath path_shape;
    QPainterPath highlights;
    QList<MiaGraphicsMarkerControlNodeItem*> controlNodesItemList;
    QList<ControlNode> controlNodesList;
    MiaImageView2D* parentView;

    bool isUpdatingOrigin;
    MiaPoint4D origin;
    bool isEditable;
    float markersAlpha;
    float visibleDistance;

};

#endif // GRAPHICSMARKERITEM_H
