#ifndef DENSELEVELSET_H
#define DENSELEVELSET_H

#include <QtGui>

class DenseLevelSet : public QThread
{
    Q_OBJECT
public:
    explicit DenseLevelSet(QObject *parent = 0);
    int maxPeriod, maxIteration;
    int currenttrend;
    int updateInterval;
    int its; //current iteration times
    int periods; //current period
    float dampingFactor;
    float maxiumStepLength;
    bool isValid;
    bool isAbort;
    float externalWeightFactor;
    void run();
    void abort();
    void resume();
    void recreateActiveBand();
    bool initialize( float* p, float* ext, uchar* smap, int* d, float* s);

signals:
    void levelsetFunctionUpdated();
    void levelsetEnterNewPeriod(int n); //n is the nth period
    void levelsetFunctionCompleted();
public slots:
protected:
    void update_phi();
    void distanceTransform2D(int *_d,unsigned char *_bimg,int _h,int _w);

    int dimension[3];
    int imageW,imageH,imageD;
    int dimoffset[3];
    int sliceSize;
    int totalSize;
    float spacing[3];
    float *phi, *phi1, *externalForce;
    uchar* statusmap;
    int imDimension;
    float virtualtime;
    long backwardPropagation;
    long totalComputation;
    double backwardAverageForce;
    double averageForce;
    float* wigglemap;

};

#endif // DENSELEVELSET_H
