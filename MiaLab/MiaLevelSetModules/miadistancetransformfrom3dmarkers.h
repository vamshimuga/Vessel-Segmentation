#ifndef MIADISTANCETRANSFORMFROM3DMARKERS_H
#define MIADISTANCETRANSFORMFROM3DMARKERS_H

#include "MiaCoreModules/miamodule.h"
#include "MiaCoreModules/miamarker.h"
#include <QProgressDialog>

class MIALEVELSETMODULESSHARED_EXPORT MiaDistanceTransformFrom3DMarkers : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaDistanceTransformFrom3DMarkers(MiaNetwork *parent = 0);
    void reset();
signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) { Q_UNUSED(marker); needUpdate = true; update();}
    void markerUpdated(MiaMarker* marker) { Q_UNUSED(marker); needUpdate = false;}
    void markerRemoved(MiaMarker* marker) { Q_UNUSED(marker); needUpdate = true;}
protected:
    virtual bool reloadInputImage();
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    void distanceTransformFor3DBalls(QList <MiaMarker*>& mlist, QProgressDialog* dlg);
    void distanceTransformFor3DBoxs(QList <MiaMarker*>& mlist, QProgressDialog* dlg);
    void getActiveBoundingRegionForMarker(MiaMarker* marker, int* box);

    QList <MiaMarker*> markerList;

};

#endif // MIADISTANCETRANSFORMFROM3DMARKERS_H
