#ifndef MIAGRADIENT3DFILTER_H
#define MIAGRADIENT3DFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaGradient3DFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaGradient3DFilter(MiaNetwork *parent = 0);

signals:

public slots:

protected:
    bool runOperation();
    template <class S>
    bool runOperation(S* inData, float* outdata);
    bool reAllocOutputImage();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);

};

#endif // MIAGRADIENT3DFILTER_H
