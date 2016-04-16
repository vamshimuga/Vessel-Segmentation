#ifndef COSTFUNCTIONSQUAREDDIFFERENCE_H
#define COSTFUNCTIONSQUAREDDIFFERENCE_H
#include "costfunctionbase.h"
class CostFunctionSquaredDifference : public CostFunctionBase
{
public:
    CostFunctionSquaredDifference();
    virtual double getValueAndDerivativeForTransformFunction(Eigen::VectorXd& value, Eigen::MatrixXd& jacobian);
    virtual double getValueAndDerivativeForMappingFunction(Eigen::VectorXd& value, Eigen::MatrixXd& jacobian) { return 0;}
protected:
    virtual double getValueAndTransformDerivativeAtPoint(Eigen::Vector3f& fixedpt, Eigen::Vector3f& movingpt,
                                                Eigen::VectorXd& value, Eigen::MatrixXd& jacobian,
                                                Eigen::VectorXd& dD, Eigen::MatrixXd& dT,
                                                Eigen::VectorXd& jacob_v);
    virtual double getValueAndMappingDerivativeAtPoint(Eigen::Vector3f& fixedpt, Eigen::Vector3f& movingpt,
                                                       Eigen::VectorXd& value, Eigen::MatrixXd& jacobian,
                                                       Eigen::VectorXd& jacob_v){ return 0;}
};

#endif // COSTFUNCTIONSQUAREDDIFFERENCE_H
