#ifndef MIACONNECTEDCOMPONENTSFILTER_H
#define MIACONNECTEDCOMPONENTSFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaConnectedComponentsFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaConnectedComponentsFilter(MiaNetwork *parent = 0);
    
signals:
    
public slots:

protected:
    bool runOperation();
    template <class S>
    bool runOperation(S* inData, float* outdata);
    bool reAllocOutputImage();
    
};

#endif // MIACONNECTEDCOMPONENTSFILTER_H
