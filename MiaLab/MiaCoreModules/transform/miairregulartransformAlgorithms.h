#ifndef MIAIRREGULARTRANSFORM_H
#define MIAIRREGULARTRANSFORM_H
#include <QtGui>
#include "../miaretainableobject.h"
#include "MiaAffineTransform.h"
#include "miapoint4d.h"

class MIACOREMODULESSHARED_EXPORT MiaIrregularTransformAlgorithm : public MiaRetainableObject
{
public:
    explicit MiaIrregularTransformAlgorithm();
    virtual MiaIrregularTransformAlgorithm* clone() = 0;
    virtual void copyToAnother(MiaIrregularTransformAlgorithm* other) const;
    virtual MiaPoint4D toPatientCoordinate(const MiaPoint4D& inpt) = 0;
    virtual MiaPoint4D fromPatientCoordinate(const MiaPoint4D& inpt) = 0;
    virtual void setOutputSpacing(const float* s);
    virtual void updateCoefficients() = 0;
    void setBulkTransform(const MiaAffineTransform& t) { bulkTransform = t; needUpdate = true;}
    void setKeyPoints(const QList<MiaPoint4D>& pts){ keyPoints = pts; needUpdate = true;}
    virtual void setParameters(const QMap<QString,QVariant>& paras){ keyParameters = paras; needUpdate = true;}
protected:
    //bulk transform doesn't contain spacing information
    MiaAffineTransform bulkTransform;
    QList<MiaPoint4D> keyPoints;
    QMap<QString,QVariant> keyParameters;

    bool needUpdate;
    float spacing[4];

};

#endif // MIAIRREGULARTRANSFORM_H
