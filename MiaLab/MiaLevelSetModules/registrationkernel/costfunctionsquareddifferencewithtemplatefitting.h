#ifndef COSTFUNCTIONSQUAREDDIFFERENCEWITHTEMPLATEFITTING_H
#define COSTFUNCTIONSQUAREDDIFFERENCEWITHTEMPLATEFITTING_H
#include "costfunctionsquareddifference.h"

class CostFunctionSquaredDifferenceWithTemplateFitting : public CostFunctionSquaredDifference
{
public:
    CostFunctionSquaredDifferenceWithTemplateFitting();
    virtual double getValueAndDerivativeForMappingFunction(Eigen::VectorXd& value, Eigen::MatrixXd& jacobian);
protected:
    virtual double getValueAndTransformDerivativeAtPoint(Eigen::Vector3f& fixedpt, Eigen::Vector3f& movingpt,
                                                Eigen::VectorXd& value, Eigen::MatrixXd& jacobian,
                                                Eigen::VectorXd& dD, Eigen::MatrixXd& dT,
                                                Eigen::VectorXd& jacob_v);
    virtual double getValueAndMappingDerivativeAtPoint(Eigen::Vector3f& fixedpt, Eigen::Vector3f& movingpt,
                                                       Eigen::VectorXd& value, Eigen::MatrixXd& jacobian,
                                                       Eigen::VectorXd& jacob_v);
};

#endif // COSTFUNCTIONSQUAREDDIFFERENCEWITHTEMPLATEFITTING_H
