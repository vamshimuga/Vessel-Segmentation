#ifndef MIAORTHOPERMUTEFILTER_H
#define MIAORTHOPERMUTEFILTER_H

#include "MiaCoreModules/miamodule2in1out.h"

enum MIACOMMONMODULESSHARED_EXPORT PermuteStrategy { Mia_Permute_AlwaysUpFront,Mia_Permute_UseTemplate, Mia_Permute_ManuallySet};

class MIACOMMONMODULESSHARED_EXPORT MiaOrthoPermuteFilter : public MiaModule2In1Out
{
    Q_OBJECT
public:
    explicit MiaOrthoPermuteFilter(MiaNetwork *parent = 0);
    ~MiaOrthoPermuteFilter();
    void setPermutationStrategy(PermuteStrategy st){currentPermuteStrategy = st;}
    void setNewDimensionOrderManually(int d0, int d1, int d2, int d3=3,
                                      bool f0 = false, bool f1 = false,
                                      bool f2 = false, bool f3 = false);
signals:
public slots:
protected:
    virtual bool reloadInputImage();
    bool computeNewDimensionOrder();
    virtual bool runOperation();
    template <class S>
    bool runOperation(S* inData, S* outdata);
    virtual bool reAllocOutputImage();
    virtual bool checkIfNeedReAllocOutputImage();
    PermuteStrategy currentPermuteStrategy;
    int newDimensionOrder[4];
    bool axisFlipCoefficient[4];

};

#endif // MIAORTHOPERMUTEFILTER_H
