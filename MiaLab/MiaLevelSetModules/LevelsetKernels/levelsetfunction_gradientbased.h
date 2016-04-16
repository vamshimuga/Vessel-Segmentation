#ifndef SPARSEFIELDGEODESICLEVELSET_H
#define SPARSEFIELDGEODESICLEVELSET_H

#include "abstractlevelsetspeedfucntion.h"

class LevelSetFunction_GradientBased : public AbstractLevelSetSpeedFucntion
{
    Q_OBJECT
public:
    explicit LevelSetFunction_GradientBased(QObject *parent = 0);

signals:

public slots:
protected:
    virtual float speedFuction(const int ind,const float curvature,const float* dx);
    virtual float extForce(const int index) = 0;
    virtual float attForce(const int index, const float* dx);

};


template <class PixelType>
class LevelSetFunction_GradientBased_WithType : public LevelSetFunction_GradientBased
{
public:
    explicit LevelSetFunction_GradientBased_WithType(QObject *parent = 0):LevelSetFunction_GradientBased(parent){}
#ifdef USESHORTTYPELEVELSETFUNCTION
    virtual bool initialize( short* p, void* ext, int* d, float* s)
#else
    virtual bool initialize( float* p, void* ext, int* d, float* s)
#endif
    {
        casedExternalForce = (PixelType*)ext;
        return AbstractLevelSetSpeedFucntion::initialize(p,ext,d,s);
    }
    virtual float extForce(const int index){ return 1.0/(1.0+casedExternalForce[index]);}
protected:
    PixelType* casedExternalForce;
};

#endif // SPARSEFIELDGEODESICLEVELSET_H
