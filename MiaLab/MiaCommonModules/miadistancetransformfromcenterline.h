#ifndef MIADISTANCETRANSFORMFROMCENTERLINE_H
#define MIADISTANCETRANSFORMFROMCENTERLINE_H

#include "MiaCoreModules/miamodule.h"
#include "MiaCoreModules/miamarker.h"

enum MIACOMMONMODULESSHARED_EXPORT WorkingModeType { MIA_DistanceTransformOnly, MIA_ReadAllKeyValues, MIA_ReadKeyValue_Pos3};
class MIACOMMONMODULESSHARED_EXPORT MiaDistanceTransformFromCenterline : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaDistanceTransformFromCenterline(MiaNetwork *parent = 0);
    void reset();
    WorkingModeType workingMode;
signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) { needUpdate = true;}
    void markerUpdated(MiaMarker* marker) { needUpdate = false;}
    void markerRemoved(MiaMarker* marker) { needUpdate = true;}
protected:
    virtual bool reloadInputImage();
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    virtual bool distanceTransformNear(QList <MiaPoint4D> &centerpts,QList <float> &initdis);
    QList <MiaMarker*> markerList;
    QMap <int, int> closestNeighborLUT;
    float* outData;
    QRgb* outDataRGB;
    int imDimension;
    int numberOfNeighbors;
    float spacing[3];
    int size[3];


};

#endif // MIADISTANCETRANSFORMFROMCENTERLINE_H
