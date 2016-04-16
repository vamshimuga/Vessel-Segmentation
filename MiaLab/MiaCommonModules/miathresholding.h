#ifndef MIATHRESHOLDING_H
#define MIATHRESHOLDING_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaThresholding : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaThresholding(MiaNetwork *parent = 0);
    float upperThreshold, lowerThreshold;

signals:

public slots:
    void setUpperThreshold( int value );
    void setLowerThreshold( int value );
    void setObjectValue(float value);
protected:
    bool runOperation();
    template <class S>
    bool runOperation(S* inData, uchar* outdata);
    bool reAllocOutputImage();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);
    float objectValue;


};

#endif // MIATHRESHOLDING_H
