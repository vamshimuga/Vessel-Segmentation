#ifndef TRANSFORMFUNCTIONCONSTRAINEDRIGID3D_H
#define TRANSFORMFUNCTIONCONSTRAINEDRIGID3D_H
#include "transformfunctionbase.h"

class TransformFunctionConstrainedRigid3D:TransformFunctionBase
{
public:
    TransformFunctionConstrainedRigid3D(int d);
    virtual void resetParameters();
    virtual void getDerivative(Eigen::MatrixXd& jocobian, Eigen::Vector3f coor) const;
    virtual bool isValid() const;
    virtual TransformFunctionBase* clone();
    virtual bool initialParametersWithPointPairs(const Eigen::Matrix3Xf& fixedpts, const Eigen::Matrix3Xf& movingpts);
    virtual void setRotationCenter(float x, float y, float z);
    virtual float getScalingFactor();
protected:
    virtual void updateBulkTransform();
    Eigen::Vector3f rotationCenter;
    Eigen::Quaternionf rotationQuaternion;
    // the contraint surface defined by ax+by+c = z
    double suf_a,suf_b,suf_c;
};

#endif // TRANSFORMFUNCTIONCONSTRAINEDRIGID3D_H
