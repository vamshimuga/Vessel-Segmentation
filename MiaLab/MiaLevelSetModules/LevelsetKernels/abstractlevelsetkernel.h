#ifndef ABSTRACTLEVELSETKERNEL_H
#define ABSTRACTLEVELSETKERNEL_H

#include <QtGui>
#include "abstractlevelsetspeedfucntion.h"


class AbstractLevelSetKernel : public QThread
{
    Q_OBJECT
public:
    explicit AbstractLevelSetKernel(QObject *parent = 0);
    void setSpeedFunction(AbstractLevelSetSpeedFucntion* func) { speedFunction = func;}
    int maxIteration;
    int updateInterval;
    int its; //current iteration times

    int currenttrend;
    int maxPeriod;
    int periods; //current period
    float dampingFactor;
    float initialStepLength;

    float maxiumStepLength;

    bool isAbort;
    bool needRestart;

    virtual bool isValid() = 0;
    virtual bool initialize(uchar* smap);
    virtual void run() = 0;
    virtual void abort() = 0;
    virtual void resume() = 0;
    virtual void restart() = 0;

    virtual void setStopPoint(int ind) { qDebug()<<"Do not support stop point"<<ind ;}
    virtual void getNarrowBandPoints(std::vector<int>& list);

signals:
    void levelsetFunctionUpdated();
    void levelsetEnterNewPeriod(int n); //n is the nth period
    void levelsetFunctionCompleted();
public slots:

protected:
    virtual void updatePhi() = 0;
    ImageInfo imInfo;
#ifdef USESHORTTYPELEVELSETFUNCTION
    short *phi;
#else
    float *phi;
#endif
    uchar* statusmap;
    AbstractLevelSetSpeedFucntion* speedFunction;

};

#endif // ABSTRACTLEVELSETKERNEL_H
