#ifndef TRANSFORMFUNCTIONRIGID3D_H
#define TRANSFORMFUNCTIONRIGID3D_H
#include "transformfunctionbase.h"

class TransformFunctionRigid3d:TransformFunctionBase
{
public:
    TransformFunctionRigid3d(int d);
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

};

#endif // TRANSFORMFUNCTIONRIGID3D_H
