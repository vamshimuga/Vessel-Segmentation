#include "transformfunctionrigid3d.h"
#include <iostream>
#include <Eigen/src/Geometry/Scaling.h>

TransformFunctionRigid3d::TransformFunctionRigid3d(int d) : TransformFunctionBase(d)
{
    if(d != 3)
        throw std::runtime_error("TransformFunctionRigid3d only works with 3D images");
    rotationCenter.setZero();
    //this will only call the virtual function at this level
    //not the derived sub-classes
    //a work around?
    resetParameters();
}

void TransformFunctionRigid3d::resetParameters()
{
    currentParamerters.resize(7);
    //zero rotation and zero translation
    currentParamerters.setZero();
    //scale is set to 1
    currentParamerters(6) = 1.0;
    //rotation center is at origin
//    rotationCenter.setZero();
    updateBulkTransform();
}

void TransformFunctionRigid3d::getDerivative(Eigen::MatrixXd &jocobian, Eigen::Vector3f coor) const
{

    // compute derivatives with respect to rotation
    const double vx = rotationQuaternion.x();
    const double vy = rotationQuaternion.y();
    const double vz = rotationQuaternion.z();
    const double vw = rotationQuaternion.w();

    jocobian.setZero();
    //itk implementation suggested pp = coor - rotationCenter;
    //But it is not working properly.
    const Eigen::Vector3f pp = coor - rotationCenter;

    const double px = pp[0];
    const double py = pp[1];
    const double pz = pp[2];

    const double vxx = vx * vx;
    const double vyy = vy * vy;
    const double vzz = vz * vz;
    const double vww = vw * vw;

    const double vxy = vx * vy;
    const double vxz = vx * vz;
    const double vxw = vx * vw;

    const double vyz = vy * vz;
    const double vyw = vy * vw;

    const double vzw = vz * vw;


    // compute Jacobian with respect to quaternion parameters
    jocobian(0, 0) = 2.0 * (               (vyw+vxz)*py + (vzw-vxy)*pz)
                           / vw;
    jocobian(0, 1) = 2.0 * ((vyw-vxz)*px   -2*vxw   *py + (vxx-vww)*pz)
                           / vw;
    jocobian(0, 2) = 2.0 * ((vzw+vxy)*px + (vww-vxx)*py   -2*vxw   *pz)
                           / vw;

    jocobian(1, 0) = 2.0 * ( -2*vyw  *px + (vxw+vyz)*py + (vww-vyy)*pz)
                           / vw;
    jocobian(1, 1) = 2.0 * ((vxw-vyz)*px                + (vzw+vxy)*pz)
                           / vw;
    jocobian(1, 2) = 2.0 * ((vyy-vww)*px + (vzw-vxy)*py   -2*vyw   *pz)
                           / vw;

    jocobian(2, 0) = 2.0 * ( -2*vzw  *px + (vzz-vww)*py + (vxw-vyz)*pz)
                           / vw;
    jocobian(2, 1) = 2.0 * ((vww-vzz)*px   -2*vzw   *py + (vyw+vxz)*pz)
                           / vw;
    jocobian(2, 2) = 2.0 * ((vxw+vyz)*px + (vyw-vxz)*py               )
                           / vw;

    // compute Jacobian with respect to the translation parameters
    jocobian(3, 0) = 1.0;
    jocobian(4, 1) = 1.0;
    jocobian(5, 2) = 1.0;

    // compute Jacobian with respect to the scale parameter

    const Eigen::Vector3f mpp = fixedToMovingBulkTransform * coor;
    double m_Scale = currentParamerters[6];
    jocobian(6, 0) = mpp[0] / m_Scale;
    jocobian(6, 1) = mpp[1] / m_Scale;
    jocobian(6, 2) = mpp[2] / m_Scale;

}

bool TransformFunctionRigid3d::isValid() const
{
    if(currentParamerters(0)*currentParamerters(0) + currentParamerters(1)*currentParamerters(1) + currentParamerters(2)*currentParamerters(2) <= 1.0)
        return true;

    return false;
}

TransformFunctionBase *TransformFunctionRigid3d::clone()
{
    TransformFunctionRigid3d * newfunc = new TransformFunctionRigid3d(dimension);
    newfunc->setCurrentParameters(currentParamerters);
    newfunc->setRotationCenter(rotationCenter[0], rotationCenter[1], rotationCenter[2]);
    return newfunc;
}

bool TransformFunctionRigid3d::initialParametersWithPointPairs(const Eigen::Matrix3Xf &fixedpts, const Eigen::Matrix3Xf &movingpts)
{
    if(fixedpts.cols() == 8 && movingpts.cols() == 8)
    {
            currentParamerters(6) = movingpts(0,7)/fixedpts(0,7);
            std::cout << "initial scaling to " << currentParamerters(6) <<std::endl;
    }
    updateBulkTransform();
    return true;
}

void TransformFunctionRigid3d::setRotationCenter(float x, float y, float z)
{
    rotationCenter[0] = x;
    rotationCenter[1] = y;
    rotationCenter[2] = z;
    updateBulkTransform();
}

float TransformFunctionRigid3d::getScalingFactor()
{
    return currentParamerters[6];
}

void TransformFunctionRigid3d::updateBulkTransform()
{
    Eigen::Translation<float, 3> moveToCenterTransform(-rotationCenter[0], -rotationCenter[1], -rotationCenter[2]);
    Eigen::Translation<float, 3> moveToCenterTransformInverse(rotationCenter[0], rotationCenter[1], rotationCenter[2]);
    Eigen::Translation<float, 3> translationPart(currentParamerters[3], currentParamerters[4], currentParamerters[5]);
    double w = sqrt(1.0 - currentParamerters[0]*currentParamerters[0] - currentParamerters[1]*currentParamerters[1] - currentParamerters[2]*currentParamerters[2]);
    Eigen::Quaternionf rotationPart(w, currentParamerters[0], currentParamerters[1], currentParamerters[2]);
    rotationQuaternion = rotationPart;
    fixedToMovingBulkTransform = Eigen::Scaling(currentParamerters[6]) * translationPart * ( moveToCenterTransformInverse * rotationPart * moveToCenterTransform);
    std::cout << "moveToCenterTransform matrix" << moveToCenterTransform.x() <<", " << moveToCenterTransform.y() <<", "<<moveToCenterTransform.z() <<std::endl;
    std::cout << "rotationPart matrix" << rotationPart.x() <<", "<< rotationPart.y() <<", "<< rotationPart.z() <<", "<< rotationPart.w() <<std::endl;
    std::cout << "moveToCenterTransformInverse matrix" << moveToCenterTransformInverse.x() <<", "<< moveToCenterTransformInverse.y() <<", "<<moveToCenterTransformInverse.z() <<std::endl;
    std::cout << "translationPart matrix" << translationPart.x() <<", "<< translationPart.y() <<", "<< translationPart.z() <<std::endl;
    std::cout << "scaling " << currentParamerters[6] << std::endl;
    movingToFixedBulkTransform = fixedToMovingBulkTransform.inverse();
//    std::cout << "transform matrix" << fixedToMovingBulkTransform.affine() <<std::endl;
}
