#include "levelsetfunction_gradientbased.h"
#include "LevelSetRoutines.h"

LevelSetFunction_GradientBased::LevelSetFunction_GradientBased(QObject *parent) :
    AbstractLevelSetSpeedFucntion(parent)
{
    alpha = -0.1; //balloonForceFactor
    beta = 0.5; //attractForceFactor
}

float LevelSetFunction_GradientBased::speedFuction(const int ind,const float curvature,const float* dx)
{

    return extForce(ind) * (curvature * alpha + (1.0f - alpha) ) +  beta * attForce(ind,dx);
}


float LevelSetFunction_GradientBased::attForce(const int index, const float* dx)
{
    //to do check border
    float f = 0.0f;
    for(int dim = 0 ; dim < imInfo.imDimension; dim++)
    {
        f += (extForce(index+imInfo.dimoffset[dim]) - extForce(index-imInfo.dimoffset[dim]))*dx[dim] * 0.5f;
    }

    return f;

}
