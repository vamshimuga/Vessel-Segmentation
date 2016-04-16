#ifndef MIAOBLIQUEMPRSLICER_H
#define MIAOBLIQUEMPRSLICER_H

#include "MiaCoreModules/miamodule.h"
class MiaMarker3DBSpline;
enum MIACOMMONMODULESSHARED_EXPORT MiaObliqueMPRMode {MIA_MPR_UserDefine, MIA_MPR_CrossSectionOfCurve};
class MIACOMMONMODULESSHARED_EXPORT MiaObliqueMPRSlicer : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaObliqueMPRSlicer(MiaNetwork *parent = 0);
    void setPositionAndOrientation(const MiaPoint4D& pos, const MiaPoint4D& ori);
    void scroll(float delta, bool notify_crosshair_change = true);
    void scrollTo(float delta,bool notify_crosshair_change = true);
signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) { needUpdate = true; scroll(0,false); update();}
    void markerUpdated(MiaMarker* marker) { needUpdate = true; if(!isSelfScrolling)scroll(0,false); update();}
    void markerRemoved(MiaMarker* marker);
    void setMPRMode(MiaObliqueMPRMode mode) { mprMode = mode; update();}

protected:
    virtual bool reloadInputImage();
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);

    MiaAffineTransform getTransformMatrixFromOrientation(const MiaPoint4D &ori);
    QList <MiaMarker*> markerList;
    MiaMarker3DBSpline* selectedSpline;
    QList<MiaPoint4D> pathIn3D;
    MiaObliqueMPRMode mprMode;
    MiaPoint4D currentCrossHairPostionInPatientSpace;
    MiaPoint4D currentPlaneNormalDirectionInPatientSpace;
    MiaTransform currentTransform;
    bool isSelfScrolling;
    float fovSize;
    float scrollStepLength;
    bool needReAllocOutputImage;
};

#endif // MIAOBLIQUEMPRSLICER_H
