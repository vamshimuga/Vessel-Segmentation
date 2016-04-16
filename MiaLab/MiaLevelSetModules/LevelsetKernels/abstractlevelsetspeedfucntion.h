#ifndef ABSTRACTLEVELSETSPEEDFUCNTION_H
#define ABSTRACTLEVELSETSPEEDFUCNTION_H

//#define USESHORTTYPELEVELSETFUNCTION

#ifdef USESHORTTYPELEVELSETFUNCTION
#define FARAWAY 32767
#else
#define FARAWAY      10.0
#endif
#include <QtGui>
struct ImageInfo
{
    int imDimension;
    int dimension[3];
    int dimoffset[3];
    int sliceSize;
    int totalSize;
    float spacing[3];
};


class AbstractLevelSetSpeedFucntion : public QObject
{
    Q_OBJECT
public:
    explicit AbstractLevelSetSpeedFucntion(QObject *parent = 0);
    virtual float getDetlaPhi(const int cor[3]);
    virtual void setWeightingFactorAlpha(float a) { alpha = a; }
    virtual void setWeightingFactorBeta(float b) { beta = b; }
    virtual void setWeightingFactorGamma(float g) { gamma = g; }
    virtual bool isValid() {return (phi != NULL && externalForce != NULL  && imInfo.imDimension > 1 && imInfo.sliceSize > 0 && imInfo.totalSize > 0 );}
#ifdef USESHORTTYPELEVELSETFUNCTION
    short* getPhi(){return phi;}
#else
    float* getPhi(){return phi;}
#endif
    ImageInfo getImageInfo(){return imInfo;}
#ifdef USESHORTTYPELEVELSETFUNCTION
    virtual bool initialize( short* p, void* ext, int* d, float* s);
#else
    virtual bool initialize( float* p, void* ext, int* d, float* s);
#endif

signals:

public slots:
protected:
    virtual float speedFuction(const int ind,const float curvature,const float* dx) = 0;
    float extForce(const int index);
    void *externalForce;
#ifdef USESHORTTYPELEVELSETFUNCTION
    short *phi;
#else
    float *phi;
#endif
    ImageInfo imInfo;
    float alpha, beta, gamma;

};

#endif // ABSTRACTLEVELSETSPEEDFUCNTION_H
