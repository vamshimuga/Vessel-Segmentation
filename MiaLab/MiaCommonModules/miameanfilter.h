#ifndef MIAMEANFILTER_H
#define MIAMEANFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaMeanFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaMeanFilter(MiaNetwork *parent = 0);
    
signals:
    
public slots:
protected:
    bool runOperation();
    template <class S>
    bool runOperation(S* inData, float* outdata);
    bool reAllocOutputImage();
};

#endif // MIAMEANFILTER_H
