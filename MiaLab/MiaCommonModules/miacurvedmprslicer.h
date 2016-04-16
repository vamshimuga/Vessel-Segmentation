#ifndef MIACURVEDMPRSLICER_H
#define MIACURVEDMPRSLICER_H

#include "MiaCoreModules/miamodule.h"

class MiaMarker3DBSpline;

enum MIACOMMONMODULESSHARED_EXPORT RotationMode {MIA_RotateAroundOriginalViewYAxis, MIA_RotateAroundStardAndEndPoints, MIA_RotateAroundGivenAxis};

class MIACOMMONMODULESSHARED_EXPORT MiaCurvedMPRSlicer : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaCurvedMPRSlicer(MiaNetwork *parent = 0);
    
signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void setRotateAxis(MiaPoint4D& axis);
    void setRotationAngle(float angle);
    void setBulkTransform(MiaAffineTransform& transfomr);
    void markerAdded(MiaMarker* marker) { needUpdate = true; update();}
    void markerUpdated(MiaMarker* marker) { needUpdate = true; update();}
    void markerRemoved(MiaMarker* marker);
    void setRotationMode(RotationMode mode) { rotationMode = mode; update();}

protected:
    virtual bool reloadInputImage();
    virtual bool runOperation();
    virtual bool checkIfNeedReAllocOutputImage(){return needReAllocOutputImage;}
    void scroll(int delta);
    virtual bool reAllocOutputImage();
    virtual void dispatchParameters(const QMap <QString, QVariant> &parameters);
    //get a transform so that the y axis of the "output" image is along the axis
    MiaAffineTransform getTransformMatrixFromAxis(const MiaPoint4D &axis);
    QList <MiaMarker*> markerList;
    MiaMarker3DBSpline* selectedSpline;
    QList<MiaPoint4D> pathIn3D;
    double currentRotationAngle;
    MiaPoint4D rotationAxis;
    MiaTransform curvedMPRtransform;
    RotationMode rotationMode;
    bool needReAllocOutputImage;

};

#endif // MIACURVEDMPRSLICER_H
