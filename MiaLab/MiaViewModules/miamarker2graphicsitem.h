#ifndef MIAMARKER2GRAPHICSITEM_H
#define MIAMARKER2GRAPHICSITEM_H

#include <QObject>
#include "MiaCoreModules/miamarker.h"
#include "MiaViewModules/miagraphicsmarkeritem.h"
#include "miamodules_global.h"

class MiaImageView2D;
class MiaDrawingManager;
class MIAVIEWMODULESSHARED_EXPORT MiaMarker2GraphicsItem : public QObject
{
    Q_OBJECT
public:
    explicit MiaMarker2GraphicsItem(QObject *parent = 0);
    virtual ~MiaMarker2GraphicsItem();
    void setTransformMatrix(MiaTransform& t);
    void update();
    void setImageView(MiaImageView2D* v);
    void setNewMarkerManger(MiaDrawingManager* manager);
    void setMarkersToEditable(bool editable);
    void setMarkersAlpha(float alpha);
    void setVisibleDistance(float dist);

signals:
    void moduleWillBeRemoved(MiaMarker2GraphicsItem* module);
public slots:
    void markerAdded(MiaMarker* marker);
    void markerUpdated(MiaMarker* marker);
    void markerRemoved(MiaMarker* marker);
    void markerModeChanged(MiaMarker* marker);
protected:
    MiaTransform transform;
    QList <MiaGraphicsMarkerItem*> markerItemList;
    MiaImageView2D* imageViewer;
    float topMostMarkerZValue;
    MiaDrawingManager* markerManager;
    bool markersAreEditable;
    float markersAlpha;
    float visibleDistance;

};

#endif // MIAMARKER2GRAPHICSITEM_H
