#ifndef TRANSFORMFUNCTIONTRASLATION_H
#define TRANSFORMFUNCTIONTRASLATION_H
#include "transformfunctionbase.h"
class TransformFunctionTraslation : TransformFunctionBase
{
public:
    TransformFunctionTraslation(int d);
    virtual void resetParameters();
    virtual void getDerivative(Eigen::MatrixXd& jocobian, Eigen::Vector3f coor) const;
    virtual bool isValid() const;
    virtual TransformFunctionBase* clone();
    virtual bool initialParametersWithPointPairs(const Eigen::Matrix3Xf& fixedpts, const Eigen::Matrix3Xf& movingpts);
    virtual float getScalingFactor();
protected:
    virtual void updateBulkTransform();
};

#endif // TRANSFORMFUNCTIONTRASLATION_H
