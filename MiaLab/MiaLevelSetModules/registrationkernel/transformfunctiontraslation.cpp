#include "transformfunctiontraslation.h"

TransformFunctionTraslation::TransformFunctionTraslation(int d): TransformFunctionBase(d)
{
    //this will only call the virtual function at this level
    //not the derived sub-classes
    //a work around?
    resetParameters();
}

void TransformFunctionTraslation::resetParameters()
{
    currentParamerters.resize(dimension);
    currentParamerters.setZero();
    updateBulkTransform();
}


void TransformFunctionTraslation::getDerivative(Eigen::MatrixXd &jocobian,Eigen::Vector3f coor) const
{
    if(dimension == 2)
    {
        jocobian = Eigen::Matrix2d::Identity();
    }
    else
    {
        jocobian = Eigen::Matrix3d::Identity();
    }
}

bool TransformFunctionTraslation::isValid() const
{
    return true;
}

TransformFunctionBase *TransformFunctionTraslation::clone()
{
    TransformFunctionBase * newfunc = new TransformFunctionTraslation(dimension);
    newfunc->setCurrentParameters(currentParamerters);
    return newfunc;
}

bool TransformFunctionTraslation::initialParametersWithPointPairs(const Eigen::Matrix3Xf& fixedpts, const Eigen::Matrix3Xf& movingpts)
{
    if(fixedpts.size() != movingpts.size()|| fixedpts.rows() != dimension)
        return false;
    Eigen::MatrixXf fixedpts_in_patient_coor = fixedImageTransform*fixedpts;
    Eigen::MatrixXf movingpts_in_patient_coor = movingImageTransform*movingpts;
    Eigen::MatrixXf offset = movingpts_in_patient_coor - fixedpts_in_patient_coor;
    Eigen::MatrixXf ones(1,offset.rows());
    Eigen::Vector3f translation_3d = offset*ones;
    currentParamerters[0] = translation_3d[0];
    currentParamerters[1] = translation_3d[1];
    updateBulkTransform();
    return true;
}

float TransformFunctionTraslation::getScalingFactor()
{
    return 1.0f;
}

void TransformFunctionTraslation::updateBulkTransform()
{
    Eigen::Vector3f translation_3d;
    Eigen::Affine3f fixedToMovingPatientToPatient = Eigen::Affine3f::Identity();
    for(int i = 0; i < 3; i++)
    {
        if(i < dimension)
        {
            translation_3d[i] = currentParamerters[i];
        }
        else
        {
            translation_3d[i] = 0;
        }
    }

    fixedToMovingPatientToPatient.translate(translation_3d);
    fixedToMovingBulkTransform = movingImageInvertTransform*fixedToMovingPatientToPatient*fixedImageTransform;
    movingToFixedBulkTransform = fixedToMovingBulkTransform.inverse();
}
