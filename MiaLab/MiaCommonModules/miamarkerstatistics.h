#ifndef MIAMARKERSTATISTICS_H
#define MIAMARKERSTATISTICS_H

#include "MiaCoreModules/miamodule.h"
#include "MiaCoreModules/miamarker.h"

enum MIACOMMONMODULESSHARED_EXPORT StatisticsType { MIA_NormalStatistics,MIA_ReadKeyValues,MIA_LineProfile};

class MIACOMMONMODULESSHARED_EXPORT MiaMarkerStatistics : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaMarkerStatistics(MiaNetwork *parent = 0);
    void setCenterlineSmoothingRange(float w){ soomthingWindow = w; }
    float mean;
    float standard_deviation;
    float max,min;
    int number_voxel;
    float volume;
    float area;
    QString summaryStr;
    StatisticsType currentStatisticsMethod;


signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;if(autoUpdateWhenInputMarkerUpdated) update();}
    void markerUpdated(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;if(autoUpdateWhenInputMarkerUpdated)update();}
    void markerRemoved(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;if(autoUpdateWhenInputMarkerUpdated)update();}

protected:
    bool runOperation();
    template <class A>
    bool runOperation(A* inData);
    template <class A>
    void readKeyValues(MiaMarker* marker, A* inData);
    template <class A>
    void normalStatistics(MiaMarker* marker, A* inData);
    template <class A>
    void lineProfile(MiaMarker* marker, A* inData);
    template <class A>
    void crossHairProfile(MiaMarker* marker, A* inData);
    template <class A>
    int statisticsOnLine(MiaPoint4D& pt1, MiaPoint4D& pt2, float& m, float& std, A* inData);
    bool reloadInputImage();
    void scanBifurcations();
    QList <int> findBifurcationPoints(QList<MiaPoint4D>& lineA, QList<MiaPoint4D>& lineB);
    QList <MiaMarker*> markerList;
    float soomthingWindow;

};

#endif // MIAMARKERSTATISTICS_H
