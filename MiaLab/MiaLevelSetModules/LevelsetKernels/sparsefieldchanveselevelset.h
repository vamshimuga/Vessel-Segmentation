#ifndef SPARSEFIELDCHANVESELEVELSET_H
#define SPARSEFIELDCHANVESELEVELSET_H

#include "sparsefieldlevelsetkernel.h"

class SparseFieldChanVeseLevelSet : public SparseFieldLevelSetKernel
{
    Q_OBJECT
public:
    explicit SparseFieldChanVeseLevelSet(QObject *parent = 0);
    virtual bool initialize( float* p, float* ext, uchar*smap, int* d, float* s);
signals:

public slots:
protected:
    virtual void run();
    virtual void recreateActiveBand();
    virtual void abort();
    virtual void resume();
    virtual void restart();
    virtual void setStopPoint(int ind);
    virtual void setThresholds(float lower, float mean, float upper);
    virtual float extForce(int index);
    double sum_out,sum_in;
    long   count_out, count_in;

};

#endif // SPARSEFIELDCHANVESELEVELSET_H
