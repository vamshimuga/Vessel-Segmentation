#include "intensitymappingfuctionbase.h"
#include <iostream>
IntensityMappingFuctionBase::IntensityMappingFuctionBase()
{
}
int IntensityMappingFuctionBase::getNumberofParameters() const
{
    return currentParamerters.rows();
}

bool IntensityMappingFuctionBase::getCurrentParameters(Eigen::VectorXf &parameters) const
{
    if(isValid())
    {
        parameters = currentParamerters;
    }
    else
        return false;
    return true;
}

bool IntensityMappingFuctionBase::setCurrentParameters(const Eigen::VectorXf &parameters)
{
    if(isValid() && currentParamerters.rows() ==  parameters.rows())
    {
        currentParamerters = parameters;
    }
    else
    {
        std::cout << "Isvalid:" << isValid() << " LengthMathc"
                  <<currentParamerters.rows() << "vs" << parameters.rows() << std::endl;
        return false;
    }
    return true;

}
