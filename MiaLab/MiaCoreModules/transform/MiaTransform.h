#ifndef MIATRANSFORM_H
#define MIATRANSFORM_H
#include "miapoint4d.h"
#include "MiaAffineTransform.h"

class MiaIrregularTransformAlgorithm;
class MiaAffineTransform;

class MIACOREMODULESSHARED_EXPORT MiaTransform
{
public:
    explicit MiaTransform();
    MiaTransform(const MiaTransform& other);
    virtual ~MiaTransform();
    void releaseAlgorithmWhenNecessary();


    MiaTransform& operator=(const MiaTransform& other);

    MiaPoint4D toPatientCoordinate(const MiaPoint4D& inpt) const;
    MiaPoint4D fromPatientCoordinate (const MiaPoint4D& inpt) const;
    void setTransformAlgorithm(MiaIrregularTransformAlgorithm* algorithm);

    void flipLeftRight()
    {
        bulkTransform.flipLeftRight();
    }
    void flipUpDown()
    {
        bulkTransform.flipUpDown();
    }

    MiaIrregularTransformAlgorithm* getTransformAlgorithm() const { return transformAlgorithm; }
    void setBulkTranform(MiaAffineTransform& t);
    MiaAffineTransform getBulkTransform() const { return bulkTransform; }
protected:

    MiaAffineTransform bulkTransform;
    MiaIrregularTransformAlgorithm* transformAlgorithm;

};

#endif // MIATRANSFORM_H
