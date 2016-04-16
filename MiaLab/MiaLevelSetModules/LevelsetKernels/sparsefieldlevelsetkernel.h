#ifndef SPARSEFIELDLEVELSETWITHOUTCOHERENT_H
#define SPARSEFIELDLEVELSETWITHOUTCOHERENT_H
#include "abstractlevelsetkernel.h"

class SparseFieldLevelSetKernel:public AbstractLevelSetKernel
{
    Q_OBJECT
public:
    explicit SparseFieldLevelSetKernel(QObject *parent = 0);
    virtual bool isValid();
    virtual bool initialize(uchar* smap);
    virtual void run() ;
    virtual void abort();
    virtual void resume();
    virtual void restart(){qDebug()<<"Do not support restart";}

signals:
    void levelsetFunctionUpdated();
    void levelsetEnterNewPeriod(int n); //n is the nth period
    void levelsetFunctionCompleted();
public slots:

protected:

    std::vector<int>* activeband;
    std::vector<int>* tempactiveband;
    std::vector<int>* removeactivepoints;
    std::vector<int> activeband1;
    std::vector<int> activeband2;
    std::vector<float> activebands_deltaphi;
    std::vector<int> neighborband;
    std::vector<int> inneighborband;
    std::vector<int> outneighborband;
    std::vector<int> changedpoints;
    virtual void updatePhi();
    void reinitialize();
    int changed;



};

#endif // SPARSEFIELDLEVELSETWITHOUTCOHERENT_H
