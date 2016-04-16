#ifndef INTENSITYMAPPINGFUCTIONBASE_H
#define INTENSITYMAPPINGFUCTIONBASE_H
#include <Eigen/Dense>

class IntensityMappingFuctionBase
{
public:
    IntensityMappingFuctionBase();
    virtual float getMappedIntensityValue(float in) = 0;
    virtual void resetParameters() = 0;
    virtual int getNumberofParameters() const;
    virtual bool getCurrentParameters(Eigen::VectorXf& parameters) const;
    virtual bool setCurrentParameters(const Eigen::VectorXf& parameters);
    virtual void getDerivative(Eigen::VectorXd &jacob_v, float x) const = 0;
    virtual bool isValid() const = 0;
    virtual IntensityMappingFuctionBase* clone() = 0;
protected:
    int dimension;
    Eigen::VectorXf currentParamerters;
};

#endif // INTENSITYMAPPINGFUCTIONBASE_H
