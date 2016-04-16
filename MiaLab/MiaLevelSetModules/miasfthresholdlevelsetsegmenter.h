#ifndef MIASFTHRESHOLDLEVELSETSEGMENTER_H
#define MIASFTHRESHOLDLEVELSETSEGMENTER_H

#include "miasparsefieldlevelsetsegmenter.h"

class MIALEVELSETMODULESSHARED_EXPORT MiaSFThresholdLevelSetSegmenter : public MiaSparseFieldLevelSetSegmenter
{
    Q_OBJECT
public:
    explicit MiaSFThresholdLevelSetSegmenter(MiaNetwork *parent = 0);
    void setThresholds(float lower, float mean, float upper);
    
signals:
    
public slots:
protected:
    virtual bool reAllocOutputImage();
    void setupSpeedFunction();
    
};

#endif // MIASFTHRESHOLDLEVELSETSEGMENTER_H
