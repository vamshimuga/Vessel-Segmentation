#ifndef MIAFASTMARCHINGFILTER_H
#define MIAFASTMARCHINGFILTER_H

#include "miaimageforestingtransform.h"
struct FastMarchingNeighborNode
{
        int index;
        float cost;
        int axis;
        friend bool operator< (const FastMarchingNeighborNode& node1, const FastMarchingNeighborNode& node2)
        {
                return node1.cost < node2.cost;
        }
};
class MIACOMMONMODULESSHARED_EXPORT MiaFastMarchingFilter : public MiaImageForestingTransform
{
    Q_OBJECT
public:
    MiaFastMarchingFilter(MiaNetwork *parent = 0);
    ~MiaFastMarchingFilter();
    void setSeedsThresholds(float lower, float upper){ lowerSeedThreshold = lower; upperSeedThreshold = upper;}//voxels in this range are counted as seeds
    void setInputImageCarrysInitialValue(bool value);
    void setUseInputImageAsSpeed(bool value);

protected:
    virtual void initializeOutputs();
    virtual float getSpeedAt(int x, int y, int z, float inValue, float* outdata, unsigned char* tagOutput);
    bool inputImageCarrysInitialValue;
    FastMarchingNeighborNode neighborNodes[3];
    float upperSeedThreshold, lowerSeedThreshold; // range to define seed points. It only works when inputImageCarrysInitialValue is true!!!!
    bool useInputImageAsSpeed;
};

#endif // MIAFASTMARCHINGFILTER_H
