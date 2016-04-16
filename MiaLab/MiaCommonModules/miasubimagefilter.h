#ifndef MIASUBIMAGEFILTER_H
#define MIASUBIMAGEFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaSubImageFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaSubImageFilter(MiaNetwork *parent = 0);
    void setOutputImageBoundingBox(int* box);
    void cropInZAxis(int bottom, int top);
    void setBackgroundValue(float value){backgroundValue = value;}
signals:
    
public slots:
protected:
    int boundingBox[8];
    virtual bool checkIfNeedReAllocOutputImage();
    virtual bool runOperation();
    template <class S>
    bool runOperation(S* inData, S* outdata);
    virtual bool reAllocOutputImage();
    float backgroundValue;
    
};

#endif // MIASUBIMAGEFILTER_H
