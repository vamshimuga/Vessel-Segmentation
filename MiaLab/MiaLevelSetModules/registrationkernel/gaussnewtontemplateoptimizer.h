#ifndef GAUSSNEWTONTEMPLATEOPTIMIZER_H
#define GAUSSNEWTONTEMPLATEOPTIMIZER_H
#include "gaussnewtonoptimizer.h"

class GaussNewtonTemplateOptimizer: GaussNewtonOptimizer
{
public:
    GaussNewtonTemplateOptimizer(QObject *parent = 0);
    virtual void run();
};

#endif // GAUSSNEWTONTEMPLATEOPTIMIZER_H
