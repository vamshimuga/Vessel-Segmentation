#ifndef MIASFINTERACTIVESEGMENTER_H
#define MIASFINTERACTIVESEGMENTER_H

#include "miasparsefieldlevelsetsegmenter.h"

class MIALEVELSETMODULESSHARED_EXPORT MiaSFInteractiveSegmenter : public MiaSparseFieldLevelSetSegmenter
{
    Q_OBJECT
public:
    explicit MiaSFInteractiveSegmenter(MiaNetwork *parent = 0);
    void setLearnedDistribution(float o_m , float o_s, float i_m, float i_s,float ds);
    
signals:
    
public slots:
protected:
    virtual bool reAllocOutputImage();
    void setupSpeedFunction();
};

#endif // MIASFINTERACTIVESEGMENTER_H
