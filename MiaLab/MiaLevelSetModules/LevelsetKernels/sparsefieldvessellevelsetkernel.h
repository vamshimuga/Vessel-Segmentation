//#ifndef SPARSEFIELDVESSELLEVELSETKERNEL_H
//#define SPARSEFIELDVESSELLEVELSETKERNEL_H

//#include <QtGui>
//#include "abstractlevelsetkernel.h"
//#define MAXNUMTHREAD 256

//class SparseFieldVesselLevelSetKernel : public AbstractLevelSetKernel
//{
//    Q_OBJECT
//public:
//    explicit SparseFieldVesselLevelSetKernel(QObject *parent = 0);
//    virtual bool initialize( float* p, float* ext, uchar*smap, int* d, float* s);
//    virtual void run();
//    virtual void recreateActiveBand();
//    virtual void abort();
//    virtual void resume();
//    virtual void restart();
//    virtual void setStopPoint(int ind);

//signals:

//public slots:

//protected:
//    int changed;
//    bool validation();
//    void initializeParameters();

//    std::vector<int>* activeband;
//    std::vector<int>* tempactiveband;
//    std::vector<int>* removeactivepoints;
//    std::vector<int> activeband1;
//    std::vector<int> activeband2;

//    std::vector<float> activebands_deltaphi;

//    std::vector<int> neighborband;
//    std::vector<int> tempband[MAXNUMTHREAD];
//    std::vector<int> firstlayerneighbor;

//    void update3DphiUsingGradient();
//    void update2DphiUsingGradient();
//    void update2DphiUsingThreshold();
//    void update3DphiUsingThreshold();
//    void update3DphiUsingThreshold_multi_threaded();
//    void reinitialize();
//    void reinitializet_multi_threaded();
//    int dimension[3];
//    int imageW,imageH,imageD;
//    int dimoffset[3];
//    int sliceSize;
//    int totalSize;
//    float spacing[3];
//    float *phi, *externalForce;
//    uchar* statusmap;
//    int imDimension;
//    float virutaltime;
//    long totalComputation;
//    double averageForce;
//    int stopPointIndex;
//    float initialExternalForceFactor;
//};

//#endif // SPARSEFIELDVESSELLEVELSETKERNEL_H
