#ifndef MIASFGRADIENTLEVELSETSEGMENTER_H
#define MIASFGRADIENTLEVELSETSEGMENTER_H

#include "miasparsefieldlevelsetsegmenter.h"

class MIALEVELSETMODULESSHARED_EXPORT MiaSFGradientLevelSetSegmenter : public MiaSparseFieldLevelSetSegmenter
{
    Q_OBJECT
public:
    explicit MiaSFGradientLevelSetSegmenter(MiaNetwork *parent = 0);
    
signals:
    
public slots:
protected:
    virtual bool reAllocOutputImage();
    void setupSpeedFunction();
    
};

#endif // MIASFGRADIENTLEVELSETSEGMENTER_H
