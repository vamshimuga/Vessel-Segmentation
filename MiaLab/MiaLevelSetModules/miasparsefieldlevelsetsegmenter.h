#ifndef MIASPARSEFIELDLEVELSETSEGMENTER_H
#define MIASPARSEFIELDLEVELSETSEGMENTER_H

#include "MiaCoreModules/miamodule.h"
#include <QtGui>
#include "MiaLevelSetModules/LevelsetKernels/abstractlevelsetkernel.h"
#include "MiaLevelSetModules/LevelsetKernels/abstractlevelsetspeedfucntion.h"



class MIALEVELSETMODULESSHARED_EXPORT MiaSparseFieldLevelSetSegmenter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaSparseFieldLevelSetSegmenter(MiaNetwork *parent = 0);
    virtual ~MiaSparseFieldLevelSetSegmenter();
    void setLevelSetFunctionInputModule( MiaModule* input);
    void setAdvectionFunctionInputModule( MiaModule* input);
    void setCurvatureWeight(float w);
    void start();
    void stop();
    void restart();
    void setStopPoint(int ind){ kernelThread->setStopPoint(ind);}
    int getIteration(){ return kernelThread->its;}
    int getPeriod(){return kernelThread->periods;}
    void setMaxIteration(int maxit){ kernelThread->maxIteration = maxit;}
    void setMaxPeriods(int p){ kernelThread->maxPeriod = p;}
    void setInitialMaxStepLength(float step){ kernelThread->maxiumStepLength = step; kernelThread->initialStepLength = step;}
    void setToUseCoherentPropagation(bool onoff);
signals:
    void requestUpdateInitialLevelSetFunction(MiaImageND** img);
    void getInputCenterlines(QList <MiaMarker*>& list);
    void requestUpdateAdvectionFunction(MiaImageND** img);
    void levelsetFunctionUpdated();
    void levelsetEnterNewPeriod(int n); //n is the nth period
    void levelsetFunctionCompleted();
public slots:
    void forwardLevelSetFunctionUpdated(){ emit outputUpdated(); emit levelsetFunctionUpdated(); }
    void forwardLevelsetFunctionCompleted() { emit levelsetFunctionCompleted();threadRunning = false;}
     void forwardLevelsetEnterNewPeriod(int n) { emit levelsetEnterNewPeriod(n);}
    void levelsetFunctionReset();
    void advectionFunctionReset();
    void getStatusMap(MiaImageND** img);
    void getNarrowBandPoints(std::vector<int>& list);
protected:
    virtual bool reloadInputImage();
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    virtual bool checkIfNeedReAllocOutputImage(){return true;}
    MiaImageND* externalSpeedFunction;
    MiaImageND* levelSetFunction;
    MiaImageND* levelSetStatusMap;
    MiaImageND* advectionFunction;
    AbstractLevelSetKernel* kernelThread;
    AbstractLevelSetSpeedFucntion* speedFunction;
    bool needReIntialize;
    MiaModule* levelsetInputModule;
    MiaModule* advectionInputModule;
    bool threadRunning;
    bool usingInteractiveSpeedFuction;
    bool usingAdvectionForce;
    float curvatureWeightingFactor;

};

#endif // MIASPARSEFIELDLEVELSETSEGMENTER_H
