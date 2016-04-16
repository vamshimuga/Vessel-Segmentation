#ifndef ImageND_H
#define ImageND_H

#include <QString>
#include <QMap>
#include "miaretainableobject.h"
#include "transform/miapoint4d.h"
#include "transform/MiaTransform.h"
#include <stdexcept>

#ifdef Q_OS_WIN
#define MAXFLOAT 3.4e+38
#endif

enum ImageType { IM_undefined, IM_uchar, IM_short, IM_unsignedshort, IM_int, IM_float, IM_RGBA };

class MIACOREMODULESSHARED_EXPORT MiaImageND : public MiaRetainableObject
{
    Q_OBJECT
public:

    explicit MiaImageND(int x, int y, int z, int t, ImageType type, QByteArray* data = NULL);
    explicit MiaImageND(int x, int y, int z, int t, ImageType type, void* data);
    virtual ~MiaImageND();
    char* getDataPtr() { return dataPtr; }
    int* getDimSize() {return dimSize; }
    int getWidth() { return dimSize[0]; } //x axis
    int getHeight() { return dimSize[1]; } //y axis
    int getDepth() { return dimSize[2]; } // z axis
    int getNumberOfPhases() { return dimSize[3]; } // time axis
    int getImageSize() { return dimSize[0]*dimSize[1]*dimSize[2]*dimSize[3]; }
    int getBytePerElement() { return bytePerElement; }
    void printSummary();
    float* getOffset() { return offset; }
    void setOffset(float* var);
    float* getSpacing() { return spacing; }
    void setSpacing(float* var);
    void setSliceThicknes(float thickness) { sliceThickness = thickness;}
    float getSliceThickness();
    float* getOrientationMatrix() { return orientationMatrix; }
    void set3x3OrientationMatrix(float* var);
    void set3x2OrientationMatrix(float* var);
    MiaTransform getTransformMatrix() { return transform; }
    void setTransformMatrix(MiaTransform& t){ transform = t;}
    void computeTransformMatrix();
    MiaTransform computeTransformMatrixWithoutSpacing();
    int getDimension() { return dimension; }
    MiaPoint4D convertVoxelToPatientCoordinate(MiaPoint4D inpt);
    MiaPoint4D convertPatientCoordinateToVoxel(MiaPoint4D inpt);
    void getMaxMin(float& min, float& max);
    void castToUChar(uchar* outport, float delta = 0.0, float scale = 1.0);
    void castToShort(short* outport, float delta = 0.0, float scale = 1.0);
    void castToUsignedShort(unsigned short* outport, float delta = 0.0, float scale = 1.0);
    void castToFloat(float* outport, float delta = 0.0, float scale = 1.0);
    void castToInt(int* outport, float delta = 0.0, float scale = 1.0);
    void resetValueToZero();
    float getValueAt(float* voxelCoordinate);
    template <class A>
    float getValueAt(float* voxelCoordinate, A* inData);
    void setValueAt(float* voxelCoordinate,float value);
    void setOutBoundaryValue(float value) { outBoundaryValue = value;}
    int numberOfNeighbors;
    float defaultWindowCenter;
    float defaultWindowWidth;


    ImageType getType() { return dataType; }
    bool setType(ImageType type);

//    bool dataStoreInRightHandSystem;
    bool needFlipUpDown;
    bool needFlipLeftRight;
    void copyGeometryInfoFrom( MiaImageND* other );
    bool isCastingToUnsignedValue;
    bool convert1DIndexTo3DVoxel(const int index, float* voxel) const;
    bool getGradientAt(float* voxelCoordinate, float* gradient);


signals:

public slots:
protected:
    void normalizeVector(float* var);
    void inverse3by3Matrix(float *inm, float *outm);
    template <class A, class B>
    void castTypeAtoB(A* indata, B* outdata, float delta, float scale);
    template <class A>
    void scanMaxMinValueForTypeA(A* data,A typeMin, A typeMax);

    ImageType dataType;
    char* dataPtr;
    QMap<QString, QString> dicomTags;
    QByteArray* dataArray;
    int dimSize[4];
    float offset[4], spacing[4];
    float orientationMatrix[9];
    MiaTransform transform;
    int dimension;
    bool isUsingSharedMemory;
    int bytePerElement;
    float sliceThickness;
    float outBoundaryValue;
    float maxValue,minValue;
    void scanMaxMinValue();

};

#endif // ImageND_H
