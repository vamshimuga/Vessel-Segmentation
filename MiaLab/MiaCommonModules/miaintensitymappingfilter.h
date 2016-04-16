#ifndef MIAINTENSITYMAPPINGFILTER_H
#define MIAINTENSITYMAPPINGFILTER_H

#include "MiaCoreModules/miamodule.h"
#include "miaconstants.h"

enum MIACOMMONMODULESSHARED_EXPORT MappingType { MIA_Thresholding, MIA_Tent, MIA_DistancePreparation, MIA_Multiply, MIA_Invert,MIA_Exponential,MIA_Cubic,MIA_LUTMapping};

class MIACOMMONMODULESSHARED_EXPORT MiaIntensityMappingFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaIntensityMappingFilter(MiaNetwork *parent = 0);
    MappingType selectedMappingMethod;
    void setMultiplyFactor( float factor, float offset = 0, float clip_lower=-MAX_FLOAT, float clip_upper=MAX_FLOAT )
    {
      parameterA = factor;
      parameterB = offset;
      parameterC = clip_lower;
      parameterD = clip_upper;
      needUpdate = true;
    }
    void setToLUTMapping(QList<QVariant>& lut)
    {
        selectedMappingMethod = MIA_LUTMapping;
        lookupTabel.clear();
        lookupTabel.append(lut);
        needUpdate = true;
    }

signals:

public slots:
    void setUpperThreshold( float value ){ parameterB = value; needUpdate = true;}
    void setLowerThreshold( float value ){ parameterA = value; needUpdate = true;}

protected:
    bool runOperation();
    template <class S>
    bool runOperation(S* inData, float* outdata);
    bool reAllocOutputImage();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);
    float parameterA, parameterB, parameterC, parameterD;
    template <class S>
    bool tentMapping(S* inData, float* outdata);
    template <class S>
    bool multiplyMapping(S* inData, float* outdata);
    template <class S>
    bool invertMapping(S* inData, float* outdata);
    template <class S>
    bool exponentialMapping(S* inData, float* outdata);
    template <class S>
    bool cubicMapping(S* inData, float* outdata);
    template <class S>
    bool lutMapping(S* inData, float* outdata);
    QList<QVariant> lookupTabel;

};

#endif // MIAINTENSITYMAPPINGFILTER_H
