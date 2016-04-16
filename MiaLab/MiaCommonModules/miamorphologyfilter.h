#ifndef MIAMORPHOLOGYFILTER_H
#define MIAMORPHOLOGYFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaMorphologyFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaMorphologyFilter(MiaNetwork *parent = 0);
    void setKernelSize(float x){kernelSizeInmm = x; needUpdate = true;}
    void setForegroundBackgroundValue(float fore, float background){
        backgroundValue = background;
        foregroundValue = fore;
        needUpdate = true;
    }

signals:

public slots:
protected:
    bool runOperation();
    template <class S>
    bool runOperation(S* inData, S* outdata);
    bool reAllocOutputImage();
    float kernelSizeInmm;
    int kernelSize[3];
    float backgroundValue;
    float foregroundValue;
};

#endif // MIAMORPHOLOGYFILTER_H
