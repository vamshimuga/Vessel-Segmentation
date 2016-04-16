#ifndef THRESHOLDLEVELSETFUNCTION_H
#define THRESHOLDLEVELSETFUNCTION_H

#include "abstractlevelsetspeedfucntion.h"
#include "LevelSetRoutines.h"

class LevelSetFunction_ThresholdBased : public AbstractLevelSetSpeedFucntion
{
    Q_OBJECT
public:
    explicit LevelSetFunction_ThresholdBased(QObject *parent = 0);
signals:

public slots:
protected:
    virtual float speedFuction(const int ind,const float curvature,const float* dx);
    virtual float extForce(const int index) { return externalForceFloat[index];}
    float* externalForceFloat;

};





template <class PixelType>
class LevelSetFunction_ThresholdBased_WithType : public LevelSetFunction_ThresholdBased
{
public:
    explicit LevelSetFunction_ThresholdBased_WithType(QObject *parent = 0):LevelSetFunction_ThresholdBased(parent){}
#ifdef USESHORTTYPELEVELSETFUNCTION
    virtual bool initialize( short* p, void* ext, int* d, float* s)
#else
    virtual bool initialize( float* p, void* ext, int* d, float* s)
#endif
    {
        castedExternalForce = (PixelType*)ext;
        return AbstractLevelSetSpeedFucntion::initialize(p,ext,d,s);
    }
    virtual float extForce(const int index){ return castedExternalForce[index];}
protected:
    PixelType* castedExternalForce;
};

template <class PixelType>
class LevelSetFunction_ThresholdBased_WithAdvectionForce_WithType : public LevelSetFunction_ThresholdBased
{
public:
    explicit LevelSetFunction_ThresholdBased_WithAdvectionForce_WithType(QObject *parent = 0):LevelSetFunction_ThresholdBased(parent)
    {
        grayscaleImage = NULL;
        filterSize = 3;
    }
#ifdef USESHORTTYPELEVELSETFUNCTION
    virtual bool initialize( short* p, void* ext, int* d, float* s)
#else
    virtual bool initialize( float* p, void* ext, int* d, float* s)
#endif
    {
        castedExternalForce = (PixelType*)ext;
        return AbstractLevelSetSpeedFucntion::initialize(p,ext,d,s);
    }
    void setGrayScaleImageAndStatusImage(short* g, uchar* s)
    {
        grayscaleImage = g;
        statusImage = s;
    }

    virtual float extForce(const int index)
    {
        int i,j,k;
        k=index/imInfo.sliceSize;
        j=(index-k*imInfo.sliceSize)/imInfo.dimension[0];
        i=index-k*imInfo.sliceSize-j*imInfo.dimension[0];
        int inside_count = 0, zero_count = 0, outside_count = 0;
        float inside_mean = 0, zero_mean = 0, outside_mean = 0;
        for(int z = k-3; z <= k+3; z++)
            for(int y = j-3; y <= j+3; y++)
                for(int x = i-3; x <= i+3; x++)
                {
                    if(z<0 || z >= imInfo.dimension[2] || y < 0 || y >= imInfo.dimension[1] || x < 0 || x >= imInfo.dimension[0])
                        continue;
                    int ind = z*imInfo.sliceSize + y * imInfo.dimension[0] + x;
                    if(statusImage[ind] == LSMASK_OUT1NEIGHBOR)
                    {
                        outside_count++;
                        outside_mean += grayscaleImage[ind];
                    }
                    else if(statusImage[ind] == LSMASK_IN1NEIGHBOR)
                    {
                        inside_count++;
                        inside_mean += grayscaleImage[ind];
                    }
                    else if(statusImage[ind] == LSMASK_SLEEP || statusImage[ind] == LSMASK_ACTIVE)
                    {
                        zero_count++;
                        zero_mean += grayscaleImage[ind];
                    }

                }
        outside_mean /= outside_count;
        inside_mean /= inside_count;
        zero_mean /= zero_count;
        float advection_force =  outside_mean + inside_mean - 2.0f*zero_mean;
        advection_force /= 30.0;
        if(advection_force > 1.0)
            advection_force = 1.0;
        else if(advection_force < -1.0)
            advection_force = -1.0;
//        if((outside_mean - zero_mean)*(zero_mean - inside_mean) < 0)
//        {
//            advection_force *= -1.0;
//        }
        return (castedExternalForce[index] - advection_force*0.5)*0.5;
    }
protected:
    PixelType* castedExternalForce;
    short* grayscaleImage;
    uchar* statusImage;
    int filterSize;
    float advectionNormalizeFactor;
    float advectionWeightingFactor;
};

#define P_UNKNOWN 0.325

class LevelSetFunction_Interactive_ThresholdBased_WithType : public LevelSetFunction_ThresholdBased_WithType<short>
{
public:
    explicit LevelSetFunction_Interactive_ThresholdBased_WithType(QObject *parent = 0)
        :LevelSetFunction_ThresholdBased_WithType<short>(parent)
    {
        out_mean = 0.0;
        out_std = 0.0;
        in_mean = 0.0;
        in_std = 0.0;
        belief_coefficient = 0.0;
        normalize_coefficient = 1.0;
        //    previous_distance = 0.0;
        //    bias_trend = 1.0;
    }
    void setLearnedDistribution(float o_m, float o_s, float i_m, float i_s,float ds)
    {
        out_mean = o_m;
        out_std = o_s;
        in_mean = i_m;
        in_std = i_s;
        float t_value = 2.0f * fabs(out_mean-in_mean)/sqrt(in_std*in_std+out_std*out_std);
        float approx = (1 + 0.278393*t_value + 0.230389*t_value*t_value + 0.000972*t_value*t_value*t_value + 0.078108*t_value*t_value*t_value*t_value);
        approx = approx*approx*approx*approx;
        belief_coefficient = 1.0f - 1.0f/approx;
        belief_coefficient *= 0.9;

        float maximum = findMaximumLogProbability();
        normalize_coefficient = 0.5/maximum;

        //        if(ds >= previous_distance)
        //            bias_trend = 1.0;
        //        else
        //            bias_trend = -1.0;
        //        previous_distance = ds;

    }
    float findMaximumLogProbability()
    {
        float up_limit, low_limit;
        if(out_mean > in_mean)
        {
            up_limit = (in_mean*out_std + out_mean*in_std)/(in_std+out_std);
            low_limit = in_mean - 3.0*in_std;
        }
        else
        {
            low_limit = (in_mean*out_std + out_mean*in_std)/(in_std+out_std);
            up_limit = in_mean + 3.0*in_std;
        }
    //    qDebug()<<out_mean<<in_mean<<up_limit<<low_limit;
        while(up_limit - low_limit > 2.0)
        {
            float mid = (up_limit + low_limit)*0.5;
            float left = ProbabilitySpeed(mid - 0.5);
            float right = ProbabilitySpeed(mid + 0.5);
            if(left == right)
            {
                return ProbabilitySpeed(mid);
            }
            else if(left > right)
            {
                up_limit = mid;
            }
            else
            {
                low_limit = mid;
            }
        }
        return ProbabilitySpeed((up_limit + low_limit)*0.5);
    }
    float ProbabilitySpeed(float value)
    {
        float gaussian = (value-in_mean)/in_std;
        float p_in = exp(-0.5*gaussian*gaussian);
        gaussian = (value-out_mean)/out_std;
        float p_out= exp(-0.5*gaussian*gaussian);
        return p_in - (belief_coefficient*p_out + P_UNKNOWN*(1.0f - belief_coefficient));
    //    if(bias_trend > 0)
    //    {
    //        return p_in - (belief_coefficient*p_out + P_UNKNOWN*(1.0f - belief_coefficient));
    //    }
    //    else
    //    {

    //        return (belief_coefficient*p_in + P_UNKNOWN*(1.0f - belief_coefficient)) - p_out;
    //    }
    }
    virtual float extForce(const int index){
                                             float eforce = ProbabilitySpeed(castedExternalForce[index]);
                                             eforce *= normalize_coefficient;
                                             return eforce;
                                           }
protected:
    float out_mean;
    float in_mean;
    float out_std;
    float in_std;
    float belief_coefficient;
    float normalize_coefficient;
//    float previous_distance;
//    float bias_trend;
};
#endif // THRESHOLDLEVELSETFUNCTION_H
