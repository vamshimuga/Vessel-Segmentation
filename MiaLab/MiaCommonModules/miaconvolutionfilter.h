#ifndef MIACONVOLUTIONFILTER_H
#define MIACONVOLUTIONFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaConvolutionFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaConvolutionFilter(MiaNetwork *parent = 0);
    void setConvolutionKernel(const int size[3], const float* data);
    void setToUseGaussianKernel(int size, float sigma);
    void setToUseMeanKernel(int size);
signals:

public slots:
protected:
    virtual bool runOperation();
    template <class S>
    bool runOperation(S* inData, float* outdata);
    virtual bool reAllocOutputImage();
    float* convolutionKernel;
    int kernelSize[3];
    float backgroundValue;

};

#endif // MIACONVOLUTIONFILTER_H
