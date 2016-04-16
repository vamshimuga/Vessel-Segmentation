#ifndef MIACURVATUREFILTER_H
#define MIACURVATUREFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaCurvatureFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaCurvatureFilter(MiaNetwork *parent = 0);
    
signals:
    
public slots:
protected:
    bool runOperation();
    bool reAllocOutputImage();
    int sliceSize;
    int totalSize;
    int dimension[3];
    int dimoffset[3];
    int imDimension;
    float* phi;
    float* output;
    float getCurvature(const int cor[3]);
};

#endif // MIACURVATUREFILTER_H
