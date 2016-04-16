#include "MiaTransform.h"
#include "miairregulartransformAlgorithms.h"

MiaTransform::MiaTransform()
{
    transformAlgorithm = NULL;
}

MiaTransform::~MiaTransform()
{
    releaseAlgorithmWhenNecessary();
}

void MiaTransform::releaseAlgorithmWhenNecessary()
{
    if( transformAlgorithm != NULL )
    {
        transformAlgorithm->release();
        transformAlgorithm = NULL;
    }
}
MiaTransform::MiaTransform(const MiaTransform &other)
{
    bulkTransform = other.getBulkTransform();
    transformAlgorithm = other.getTransformAlgorithm();
    if(transformAlgorithm)
        transformAlgorithm->retain();
}

MiaTransform& MiaTransform::operator=(const MiaTransform& other) // Assignment operator
{
    bulkTransform = other.getBulkTransform();
    setTransformAlgorithm(other.getTransformAlgorithm());
    return *this;
}

MiaPoint4D MiaTransform::toPatientCoordinate(const MiaPoint4D& inpt) const
{
    if(transformAlgorithm)
        return transformAlgorithm->toPatientCoordinate(inpt);
    return bulkTransform.transform(inpt);
}
MiaPoint4D MiaTransform::fromPatientCoordinate (const MiaPoint4D& inpt) const
{
    if(transformAlgorithm)
        return transformAlgorithm->fromPatientCoordinate(inpt);
    return bulkTransform.inversedTransform(inpt);
}
void MiaTransform::setTransformAlgorithm(MiaIrregularTransformAlgorithm* algorithm)
{
    releaseAlgorithmWhenNecessary();
    if(algorithm != NULL)
    {
        transformAlgorithm = algorithm;
        algorithm->retain();
    }
}
void MiaTransform::setBulkTranform(MiaAffineTransform& t)
{
    bulkTransform = t;
    if(transformAlgorithm)
    {
        transformAlgorithm->setBulkTransform(t);
    }
}
