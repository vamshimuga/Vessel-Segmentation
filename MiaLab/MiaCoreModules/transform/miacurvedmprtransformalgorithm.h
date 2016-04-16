#ifndef MIACURVEDMPRTRANSFORMALGORITHM_H
#define MIACURVEDMPRTRANSFORMALGORITHM_H

#include "miairregulartransformAlgorithms.h"

class MIACOREMODULESSHARED_EXPORT MiaCurvedMPRTransformAlgorithm : public MiaIrregularTransformAlgorithm
{
    Q_OBJECT
public:
    explicit MiaCurvedMPRTransformAlgorithm();
    virtual MiaPoint4D toPatientCoordinate(const MiaPoint4D& inpt) ;
    virtual MiaPoint4D fromPatientCoordinate(const MiaPoint4D& inpt);
    virtual MiaIrregularTransformAlgorithm* clone();
//    virtual void retain()
//    {
//        MiaRetainableObject::retain();
//        qDebug()<< "algorithm is retained";
//    }
//    virtual void release()
//    {
//        MiaRetainableObject::release();
//        qDebug()<< "algorithm is released";
//    }
    virtual void updateCoefficients();
    int getProjectedLength();
signals:
    
public slots:
protected:
    virtual void createProjectedCenterlineWithEvenStepLength();
    MiaPoint4D lookupInPlanePointInBulkTransfomrSpace(const MiaPoint4D &inpt) const;
    MiaPoint4D lookupNearestInplanePoint(const MiaPoint4D &inpt) const;
    static double distanceIn2DProjectedPlane(const MiaPoint4D &inpt1, const MiaPoint4D &inpt2);
    double shortestDistanceToLineSegment(const MiaPoint4D &start, const MiaPoint4D &end, const MiaPoint4D &pt, double &offset) const;
    double searchShortestDistanceFromIndex(int ind, const MiaPoint4D& inpt, int direction, double& projected_index) const;
    QList<MiaPoint4D> projectedCenterlineWithEvenStepLength;
    QList<MiaPoint4D> projectedCenterline;
    QList<float> yCoordinatesOfProjectedCenterlinePoints;
};

#endif // MIACURVEDMPRTRANSFORMALGORITHM_H
