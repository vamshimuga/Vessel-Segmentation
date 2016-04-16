#include "miairregulartransformAlgorithms.h"

MiaIrregularTransformAlgorithm::MiaIrregularTransformAlgorithm()
{
    needUpdate = true;
    spacing[0] = spacing[1] = spacing[2] = spacing[3] = 1.0;
}

void MiaIrregularTransformAlgorithm::copyToAnother(MiaIrregularTransformAlgorithm *other) const
{
    other->setBulkTransform(bulkTransform);
    other->setKeyPoints(keyPoints);
    other->setParameters(keyParameters);
    other->setOutputSpacing(spacing);
}

void MiaIrregularTransformAlgorithm::setOutputSpacing(const float *s)
{
    for(int i = 0 ; i < 4; i++)
    {
        spacing[i] = s[i];
    }
}
