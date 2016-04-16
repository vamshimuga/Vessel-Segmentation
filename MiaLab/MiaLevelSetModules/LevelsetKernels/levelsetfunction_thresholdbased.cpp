#include "levelsetfunction_thresholdbased.h"

LevelSetFunction_ThresholdBased::LevelSetFunction_ThresholdBased(QObject *parent) :
    AbstractLevelSetSpeedFucntion(parent)
{
    alpha = 0.5;
}

float LevelSetFunction_ThresholdBased::speedFuction(const int ind,const float curvature,const float* dx)
{
    return (-alpha * extForce(ind)) + ((1-alpha) * curvature);
}

