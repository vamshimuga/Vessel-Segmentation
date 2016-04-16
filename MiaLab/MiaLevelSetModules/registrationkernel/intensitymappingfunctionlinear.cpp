#include "intensitymappingfunctionlinear.h"

IntensityMappingFunctionLinear::IntensityMappingFunctionLinear()
{
    currentParamerters.resize(2);
}

void IntensityMappingFunctionLinear::resetParameters()
{
    para_A = 1;
    para_b = 0;
    currentParamerters[0] = para_A;
    currentParamerters[1] = para_b;
}

bool IntensityMappingFunctionLinear::setCurrentParameters(const Eigen::VectorXf &parameters)
{
    para_A = parameters[0];
    para_b = parameters[1];
    currentParamerters[0] = para_A;
    currentParamerters[1] = para_b;
    return true;
}

void IntensityMappingFunctionLinear::getDerivative(Eigen::VectorXd &jacob_v, float x) const
{
    jacob_v(0) = x;
    jacob_v(1) = 1.0;
}


IntensityMappingFuctionBase *IntensityMappingFunctionLinear::clone()
{
    IntensityMappingFunctionLinear* acopy = new IntensityMappingFunctionLinear();
    Eigen::VectorXf parameters;
    this->getCurrentParameters(parameters);
    acopy->setCurrentParameters(parameters);
    return acopy;
}
