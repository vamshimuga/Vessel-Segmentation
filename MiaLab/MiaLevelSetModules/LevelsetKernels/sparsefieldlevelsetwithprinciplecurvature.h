#ifndef SPARSEFIELDLEVELSETWITHPRINCIPLECURVATURE_H
#define SPARSEFIELDLEVELSETWITHPRINCIPLECURVATURE_H

#include <QtGui>

class SparseFieldLevelSetWithPrincipleCurvature : public QThread
{
    Q_OBJECT
public:
    explicit SparseFieldLevelSetWithPrincipleCurvature(QObject *parent = 0);

    int maxIteration;
    int updateInterval;
    int its; //current iteration times
    float maxiumStepLength;
    bool initialize( float* p, float* ext, uchar*smap, int* d, float* s);
    void run();
    void recreateActiveBand();
    bool isValid;
    bool isAbort;
    float externalWeightFactor;
    void abort();
    void resume();
    void resetExternalForce(float*ext) {externalForce = ext;}
    void setDirectionMap(QRgb* map) {principleDirectionMap = map;}

    // members that are not used but required by the interface
    float initialStepLength;
    int periods;
    void setStopPoint(int ind){ qDebug()<<"Do not support stop point"<<ind ;}
    void restart(){qDebug()<<"Do not support restart";}

signals:
    void levelsetFunctionUpdated();
    void levelsetEnterNewPeriod(int n); //n is the nth period
    void levelsetFunctionCompleted();
public slots:

protected:
    int changed;
    bool validation();

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
    void update3DphiUsingThreshold();
    void reinitialize();
    int dimension[3];
    int imageW,imageH,imageD;
    int dimoffset[3];
    int sliceSize;
    int totalSize;
    float spacing[3];
    float *phi, *phi1, *externalForce;
    uchar* statusmap;
    QRgb* principleDirectionMap;
    int imDimension;


};

#endif // SPARSEFIELDLEVELSETWITHPRINCIPLECURVATURE_H
