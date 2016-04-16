#ifndef GRAPHICSCROSSHAIRITEM_H
#define GRAPHICSCROSSHAIRITEM_H

#include <QGraphicsItem>
#include "MiaCoreModules/miaimagend.h"

class MiaImageView2D;

class MiaGraphicsCrossHairItem : public QGraphicsItem
{
public:
    explicit MiaGraphicsCrossHairItem(MiaImageView2D* view, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    MiaPoint4D centerPoint;

signals:

public slots:

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    MiaImageView2D* parentView;
    bool activeMode;

};

#endif // GRAPHICSCROSSHAIRITEM_H
