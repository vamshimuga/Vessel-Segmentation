#ifndef MIAZEROCROSSINGFILTER_H
#define MIAZEROCROSSINGFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaZeroCrossingFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaZeroCrossingFilter(MiaNetwork *parent = 0);

signals:

public slots:

protected:
    bool runOperation();
    bool reAllocOutputImage();
};

#endif // MIAZEROCROSSINGFILTER_H
