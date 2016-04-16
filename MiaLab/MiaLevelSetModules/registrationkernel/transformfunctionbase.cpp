#include "transformfunctionbase.h"
#include <iostream>
TransformFunctionBase::TransformFunctionBase(int d)
{
    dimension = d;
    fixedImageInvertTransform = Eigen::Affine3f::Identity();
    fixedImageTransform = Eigen::Affine3f::Identity();
    fixedToMovingBulkTransform = Eigen::Affine3f::Identity();
    movingImageInvertTransform = Eigen::Affine3f::Identity();
    movingImageTransform = Eigen::Affine3f::Identity();
    movingToFixedBulkTransform = Eigen::Affine3f::Identity();
}

void TransformFunctionBase::invertTransform()
{
    fixedToMovingBulkTransform = fixedToMovingBulkTransform.inverse();
    movingToFixedBulkTransform = movingToFixedBulkTransform.inverse();
}

int TransformFunctionBase::getNumberofParameters() const
{
    return currentParamerters.rows();
}

bool TransformFunctionBase::getCurrentParameters(Eigen::VectorXf &parameters) const
{
    if(isValid())
    {
        parameters = currentParamerters;
    }
    else
        return false;
    return true;
}

bool TransformFunctionBase::setCurrentParameters(const Eigen::VectorXf &parameters)
{
    if(isValid() && currentParamerters.rows() ==  parameters.rows())
    {
        currentParamerters = parameters;
        updateBulkTransform();
    }
    else
    {
        std::cout << "Isvalid:" << isValid() << " LengthMathc"
                  <<currentParamerters.rows() << "vs" << parameters.rows() << std::endl;
        return false;
    }
    return true;

}
