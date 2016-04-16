#ifndef MIACENTERTRACKINGONARRIVALMAP_H
#define MIACENTERTRACKINGONARRIVALMAP_H

#include "MiaCoreModules/miamodule.h"
#include <list>
#include <queue>
#include "MiaCoreModules/miamarker.h"

enum MIACOMMONMODULESSHARED_EXPORT TracingCondition { MIA_Unfinished, MIA_StoppedAtSeed, MIA_StoppedAtBorder, MIA_StoppedAtLocalMINorMAX };
class MIACOMMONMODULESSHARED_EXPORT MiaCenterTrackingOnArrivalMap : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaCenterTrackingOnArrivalMap(MiaNetwork *parent = 0);
    TracingCondition currentCondition;
    float stepLength;

signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) { needUpdate = true;}
    void markerUpdated(MiaMarker* marker) { needUpdate = true;}
    void markerRemoved(MiaMarker* marker) { needUpdate = true;}
    void getOutputMarkers(QList <MiaMarker*>& list);
    void setTracingToMaximum() { direction = 1.0;}
    void setTracingToMinimum() { direction = -1.0; }

protected:
    virtual bool reloadInputImage();
    bool runOperation();
    float getValueAt(MiaPoint4D voxelCoordinate);
    MiaPoint4D getGradientAt(MiaPoint4D voxelCoordinate);
    QList <MiaMarker*> centerlinesList;
    void traceCenterPoints(MiaPoint4D start,MiaPoint4D end, QList <MiaPoint4D> &centerpts);
    int imDimension;
    int numberOfNeighbors;
    float* inData;
    int inputSize[3];
    float inputSpacing[3];
    float direction;
    float m_GradientMagnitudeTolerance;
    float m_CurrentStepLength;
    float m_RelaxationFactor;
    float m_MinimumStepLength;
};

#endif // MIACENTERTRACKINGONARRIVALMAP_H
