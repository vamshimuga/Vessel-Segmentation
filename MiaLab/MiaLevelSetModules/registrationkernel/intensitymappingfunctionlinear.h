#ifndef INTENSITYMAPPINGFUNCTIONLINEAR_H
#define INTENSITYMAPPINGFUNCTIONLINEAR_H
#include "intensitymappingfuctionbase.h"

class IntensityMappingFunctionLinear : IntensityMappingFuctionBase
{
public:
    IntensityMappingFunctionLinear();
    virtual float getMappedIntensityValue(float x){ return para_A*x + para_b;}
    virtual void resetParameters();
    virtual bool setCurrentParameters(const Eigen::VectorXf& parameters);
    virtual void getDerivative(Eigen::VectorXd &jacob_v, float x) const;
    virtual bool isValid() const {return (para_A != 0.0f);}
    virtual IntensityMappingFuctionBase* clone();
protected:
    float para_A,para_b;
};

#endif // INTENSITYMAPPINGFUNCTIONLINEAR_H
