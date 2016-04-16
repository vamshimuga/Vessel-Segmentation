#ifndef TRANSFORMFUNCTIONBASE_H
#define TRANSFORMFUNCTIONBASE_H
#include <Eigen/Dense>
#include <stdexcept>
class TransformFunctionBase
{
public:
    TransformFunctionBase(int d);
    virtual void setFixedImageTransform(const Eigen::Affine3f& t){fixedImageTransform = t;fixedImageInvertTransform = t.inverse();}
    virtual void setMovingImageTransform(const Eigen::Affine3f& t){movingImageTransform = t;movingImageInvertTransform = t.inverse();}
    virtual void resetParameters() = 0;

    virtual void invertTransform();//use carefully as it doesn't update the current parameter

    virtual int getNumberofParameters() const;
    virtual bool getCurrentParameters(Eigen::VectorXf& parameters) const;
    virtual bool setCurrentParameters(const Eigen::VectorXf& parameters);
    virtual void getDerivative(Eigen::MatrixXd& jocobian,Eigen::Vector3f coor) const = 0;
    virtual bool isValid() const = 0;
    virtual Eigen::Vector3f fixedImageVoxelToMovingImageVoxel(const Eigen::Vector3f fixedpt) const { return fixedToMovingBulkTransform*fixedpt;}
    virtual Eigen::Vector3f movingImageVoxelToFixedImageVoxel(const Eigen::Vector3f movingpt) const { return movingToFixedBulkTransform*movingpt;}
    virtual TransformFunctionBase* clone() = 0;
    virtual bool initialParametersWithPointPairs(const Eigen::Matrix3Xf& fixedpts, const Eigen::Matrix3Xf& movingpts) = 0;
    virtual void setRotationCenter(float x, float y, float z){throw std::runtime_error("This transformFunction does not take rotation center");}
    virtual void setFixedTranslation(float x, float y, float z){throw std::runtime_error("This TranformFunction does not take fixed tranlation");}
    virtual float getScalingFactor() = 0;
protected:
    int dimension;
    virtual void updateBulkTransform() = 0;
    Eigen::VectorXf currentParamerters;
    Eigen::Affine3f fixedImageTransform;
    Eigen::Affine3f fixedImageInvertTransform;
    Eigen::Affine3f movingImageTransform;
    Eigen::Affine3f movingImageInvertTransform;
    Eigen::Affine3f fixedToMovingBulkTransform;
    Eigen::Affine3f movingToFixedBulkTransform;

};

#endif // TRANSFORMFUNCTIONBASE_H
