#ifndef MIAMANUALLIVERSEGMENTGENERATOR_H
#define MIAMANUALLIVERSEGMENTGENERATOR_H

#include "MiaCoreModules/miamodule.h"
#include "MiaCoreModules/miamarker.h"

class MIACOMMONMODULESSHARED_EXPORT MiaManualLiverSegmentGenerator : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaManualLiverSegmentGenerator(MiaNetwork *parent = 0);
    void setThresholds(float lower, float upper) { upperThreshold = upper; lowerThreshold = lower; needUpdate = true;}

signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;if(autoUpdateWhenInputMarkerUpdated) update();}
    void markerUpdated(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;if(autoUpdateWhenInputMarkerUpdated)update();}
    void markerRemoved(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;if(autoUpdateWhenInputMarkerUpdated)update();}

protected:
    bool runOperation();
    template <class S>
    bool runOperation(S* inData, short* outdata);
    bool reloadInputImage();
    bool reAllocOutputImage();
    float convertToAngle(float x, float y);
    float upperThreshold, lowerThreshold;
    float startAngle;
    QList <MiaMarker*> markerList;
};

#endif // MIAMANUALLIVERSEGMENTGENERATOR_H
