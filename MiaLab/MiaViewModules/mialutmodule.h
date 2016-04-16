#ifndef LUTMODULE_H
#define LUTMODULE_H

#include <QObject>
#include <QtGui>
#include "miamodules_global.h"

enum LUTType { LUT_SingleColor, LUT_BinaryColor , LUT_RainBow};

struct LUTNode
{
    float intensity;
    QColor color;
};

class MIAVIEWMODULESSHARED_EXPORT MiaLUTModule : public QObject
{
    Q_OBJECT
public:
    explicit MiaLUTModule(LUTType type, QObject *parent = 0);
    void insertKeyPoint( float intensity, QColor color );
    void resetKeyPoint( int index, float intensity, QColor color );
    void removeKeyPoint( int index );
    LUTNode getKeyPoint( int index );
    void removeAllKeyPoints();
    QRgb getColorForIndex( float index );
    QRgb getColorForValue( float intensity );
    void processingImage( QImage* img, float* indata, float ww, float wl );
    void processingBinaryImage( QImage* img, float* indata );
    void getDefaultWindowSetting(float &ww, float &wl);

signals:

public slots:
protected:
    void updateLUTVector();
    int searchIndexOfValue( float intensity );
    QVector<LUTNode> keyPointsVector;
    int lutResolution;
    float lutBinSize; // lutResolution/256.0
    float lutMinValue;
    QRgb* lookupTable;
    LUTType lookupTableType;




};

#endif // LUTMODULE_H
