#ifndef MIAMULTIINPUTINTENSITYMAPPINGFILTER_H
#define MIAMULTIINPUTINTENSITYMAPPINGFILTER_H

#include "MiaCoreModules/miamodule2in1out.h"

enum MIACOMMONMODULESSHARED_EXPORT MultiInputMappingType { MIA_Multi_Add, MIA_Multi_Multiply,MIA_Multi_Mask,MIA_Zero_Replace, MIA_Brain_Model_Combining,MIA_Brain_Distance_Map_Combining, MIA_Phase_Analysis};

class MIACOMMONMODULESSHARED_EXPORT MiaMultiInputIntensityMappingFilter : public MiaModule2In1Out
{
    Q_OBJECT
public:
    explicit MiaMultiInputIntensityMappingFilter(MiaNetwork *parent = 0);
    MultiInputMappingType selectedMappingMethod;

signals:

public slots:
    void setMultiplyFactor( float a, float b ){ parameterA = a; parameterB = b; needUpdate = true;}
    void setBrainSurfaceThickness(float c){parameterC = c;}
    void setBackgroundValueOutsideMask(float a){parameterA = a;}
    void setFavoritePhase(float offset, float threshold, float normalize) { parameterA = offset, parameterB = threshold; parameterC = normalize; needUpdate = true; }
protected:
    template <class S>
    bool runOperation(S* inData,S* inData2, float* outdata);
    template <class S>
    bool add(S* inData,S* inData2, float* outdata);
    template <class S>
    bool zeroRegionReplace(S* inData,S* inData2, float* outdata);
    template <class S>
    bool multiply(S* inData,S* inData2, float* outdata);
    template <class S>
    bool mask(S* inData,S* inData2, float* outdata);
    template <class S>
    bool combineBrainDistanceMap(S* inData,S* inData2, float* outdata);
    template <class S>
    bool combineBrainModel(S* inData,S* inData2, float* outdata);
    template <class S>
    bool phaseAnalysis(S* inData,S* inData2, float* outdata);
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    float parameterA, parameterB,parameterC;

};

#endif // MIAMULTIINPUTINTENSITYMAPPINGFILTER_H
