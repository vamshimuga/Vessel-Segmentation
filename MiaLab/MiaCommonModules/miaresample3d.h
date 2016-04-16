#ifndef MIARESAMPLE3D_H
#define MIARESAMPLE3D_H
#include "MiaCoreModules/miamodule2in1out.h"

enum MIACOMMONMODULESSHARED_EXPORT ResampleStrategy { Mia_Resampe_SetSize, Mia_Resample_SetSpacing, Mia_Resampe_SetScale,Mia_Resample_IsotropicMaxSpacing, Mia_Resampe_IsotropicMinSpacing, Mia_Resample_UseAnotherImage};

class MIACOMMONMODULESSHARED_EXPORT MiaResample3D : public MiaModule2In1Out
{
    Q_OBJECT
public:
    explicit MiaResample3D(MiaNetwork *parent = 0);
    int newdimension[3];
    float scale[3];
    void setOutputSize(int x, int y, int z);
    void setResampleScale(float x, float y, float z);
    void setResampleSpacing(float x, float y, float z);
    void setResampleToIsotropic(bool max);
    void setResampleStrategy(ResampleStrategy strategy);
signals:
public slots:
protected:
    bool runOperation();
    virtual bool reloadInputImage();
    template <class S>
    bool runOperation(S* inData, S* outdata);
    bool reAllocOutputImage();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);
    float delta[3];
    float newspacing[4];
    ResampleStrategy currentResampleStrategy;




};

#endif // MIARESAMPLE3D_H
