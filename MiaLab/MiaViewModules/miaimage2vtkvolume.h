#ifndef MIAIMAGE2VTKVOLUME_H
#define MIAIMAGE2VTKVOLUME_H

#include "miaimage2vtkprop.h"
#include "mialutmodule.h"
#include "miamodules_global.h"

class vtkVolumeRayCastCompositeFunction;
class vtkFixedPointVolumeRayCastMapper;
class vtkVolumeProperty;
class vtkColorTransferFunction;
class vtkPiecewiseFunction;

class MIAVIEWMODULESSHARED_EXPORT MiaImage2VTKVolume : public MiaImage2VTKProp
{
    Q_OBJECT
public:
    explicit MiaImage2VTKVolume(MiaNetwork *parent = 0);
    virtual ~MiaImage2VTKVolume();
    void setCurrentLUT(MiaLUTModule* lut) { currentLut = lut; }

signals:

public slots:

protected:
    virtual bool reAllocOutputImage();
    virtual bool runOperation();
    virtual void dispatchParameters(const QMap <QString, QVariant> &parameters);
    MiaLUTModule* currentLut;
    vtkFixedPointVolumeRayCastMapper* mapper;
    vtkVolumeProperty* volumeProperty;
    vtkColorTransferFunction* colorTransferFunction;
    vtkPiecewiseFunction* opacityTransferFunction;

    vtkColorTransferFunction	*colorTransferFunctionForRGB_Red, *colorTransferFunctionForRGB_Green, *colorTransferFunctionForRGB_Blue;
    float curWinWidth,curWinLevel;
    float vauleShiftForCasting;
};

#endif // MIAIMAGE2VTKVOLUME_H
