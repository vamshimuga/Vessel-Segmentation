#ifndef TRANSFORMFUNCTIONAFFINE_H
#define TRANSFORMFUNCTIONAFFINE_H
#include "transformfunctionbase.h"
class TransformFunctionAffine:TransformFunctionBase
{
    // this affine transform is in the voxel space to voxel space
    // to get the patient to patient coordinate transform
public:
    TransformFunctionAffine(int d);
    virtual void resetParameters();
    virtual void getDerivative(Eigen::MatrixXd& jocobian, Eigen::Vector3f coor) const;
    virtual bool isValid() const;
    virtual TransformFunctionBase* clone();
    virtual bool initialParametersWithPointPairs(const Eigen::Matrix3Xf& fixedpts, const Eigen::Matrix3Xf& movingpts);
    virtual float getScalingFactor();
protected:
    virtual void updateBulkTransform();
};

#endif // TRANSFORMFUNCTIONAFFINE_H
