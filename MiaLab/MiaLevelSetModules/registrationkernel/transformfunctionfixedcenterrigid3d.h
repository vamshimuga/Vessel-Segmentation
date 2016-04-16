#ifndef TRANSFORMFUNCTIONFIXEDCENTERRIGID3D_H
#define TRANSFORMFUNCTIONFIXEDCENTERRIGID3D_H
#include "transformfunctionbase.h"

class TransformFunctionFixedCenterRigid3D:TransformFunctionBase
{
public:
    TransformFunctionFixedCenterRigid3D(int d);
    virtual void resetParameters();
    virtual void getDerivative(Eigen::MatrixXd& jocobian, Eigen::Vector3f coor) const;
    virtual bool isValid() const;
    virtual TransformFunctionBase* clone();
    virtual bool initialParametersWithPointPairs(const Eigen::Matrix3Xf& fixedpts, const Eigen::Matrix3Xf& movingpts);
    virtual void setRotationCenter(float x, float y, float z);
    virtual void setFixedTranslation(float x, float y, float z);
    virtual float getScalingFactor();
protected:
    virtual void updateBulkTransform();
    Eigen::Vector3f rotationCenter;
    Eigen::Vector3f fixedTranslation;
    Eigen::Quaternionf rotationQuaternion;
};

#endif // TRANSFORMFUNCTIONFIXEDCENTERRIGID3D_H
