#ifndef ORTHOGONALSLICER_H
#define ORTHOGONALSLICER_H
#include "MiaCoreModules/miamodule.h"

enum MIACOMMONMODULESSHARED_EXPORT Orient_Mode { Coronal, Saggital, Axial };
class MIACOMMONMODULESSHARED_EXPORT MiaOrthogonalSlicer : public MiaModule
{
    Q_OBJECT
public:
    MiaOrthogonalSlicer(MiaNetwork *parent = 0);
    void resetPositionToCenter();
    void setPositionTo(MiaPoint4D & apos);
    void setOrientation(Orient_Mode mode );
protected:
    MiaPoint4D position;
    MiaPoint4D pat_coordinate;
    int timephase;
    virtual bool runOperation();
    virtual bool checkIfNeedReAllocOutputImage(){return true;}
    void scroll(int delta);
    template <class S>
    bool runOperation(S* inData, float* outdata);
    bool reAllocOutputImage();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);
    Orient_Mode sliceMode;
    int outWidth, outHeight;
    bool flipUpDown;
    bool flipLeftRight;
    void checkIfNeedFlip();

};

#endif // ORTHOGONALSLICER_H
