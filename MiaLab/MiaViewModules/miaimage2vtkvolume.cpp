#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeType,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#include "miaimage2vtkvolume.h"
#include "vtkVolume.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkMatrix4x4.h"
#include "vtkImageImport.h"


MiaImage2VTKVolume::MiaImage2VTKVolume(MiaNetwork *parent) :
    MiaImage2VTKProp(parent)
{
    curWinWidth = 400;
    curWinLevel = 300;
    vauleShiftForCasting = 0;
    currentLut = 0;
    prop = vtkVolume::New();
    mapper = vtkFixedPointVolumeRayCastMapper::New();
    volumeProperty = vtkVolumeProperty::New();
    opacityTransferFunction = vtkPiecewiseFunction::New();
    colorTransferFunction = vtkColorTransferFunction::New();
    colorTransferFunctionForRGB_Red = vtkColorTransferFunction::New();
    colorTransferFunctionForRGB_Red->AddRGBPoint(   0, 0, 0, 0 );
    colorTransferFunctionForRGB_Red->AddRGBPoint( 255, 1, 0, 0 );

    colorTransferFunctionForRGB_Green = vtkColorTransferFunction::New();
    colorTransferFunctionForRGB_Green->AddRGBPoint(   0, 0, 0, 0 );
    colorTransferFunctionForRGB_Green->AddRGBPoint( 255, 0, 1, 0 );

    colorTransferFunctionForRGB_Blue = vtkColorTransferFunction::New();
    colorTransferFunctionForRGB_Blue->AddRGBPoint(   0, 0, 0, 0 );
    colorTransferFunctionForRGB_Blue->AddRGBPoint( 255, 0, 0, 1 );

    ((vtkFixedPointVolumeRayCastMapper*)mapper)->SetInputConnection(importer->GetOutputPort());
    mapper->SetBlendModeToComposite();
    ((vtkFixedPointVolumeRayCastMapper*)mapper)->SetMinimumImageSampleDistance( 1.0);
    ((vtkFixedPointVolumeRayCastMapper*)mapper)->SetMaximumImageSampleDistance( 1.5);
    ((vtkFixedPointVolumeRayCastMapper*)mapper)->SetSampleDistance(1.0);
    ((vtkFixedPointVolumeRayCastMapper*)mapper)->SetInteractiveSampleDistance(2.5);
//    ((vtkFixedPointVolumeRayCastMapper*)mapper)->SetNumberOfThreads (16);
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.1);
    volumeProperty->SetDiffuse(0.9);
    volumeProperty->SetSpecular(0.2);
    volumeProperty->SetSpecularPower(10.0);
    volumeProperty->SetScalarOpacityUnitDistance(0.8919);

    ((vtkVolume*)prop)->SetMapper(mapper);
    ((vtkVolume*)prop)->SetProperty( volumeProperty);
    prop->PickableOff();


    wiredParameters.insert(QString("ww"),curWinWidth);
    wiredParameters.insert(QString("wl"),curWinLevel);

}

MiaImage2VTKVolume::~MiaImage2VTKVolume()
{
    if(prop)
        prop->Delete();
    if(mapper)
        mapper->Delete();
    if(volumeProperty)
        volumeProperty->Delete();
    if(colorTransferFunction)
        colorTransferFunction->Delete();
    if(opacityTransferFunction)
        opacityTransferFunction->Delete();
    if(colorTransferFunctionForRGB_Red)
        colorTransferFunctionForRGB_Red->Delete();
    if(colorTransferFunctionForRGB_Green)
        colorTransferFunctionForRGB_Green->Delete();
    if(colorTransferFunctionForRGB_Blue)
        colorTransferFunctionForRGB_Blue->Delete();

}

bool MiaImage2VTKVolume::reAllocOutputImage()
{
//  needed for ray cast to convert it to unsigned data.
//    if(outputImage)
//        outputImage->release();
//    outputImage = castImageToAnotherType(inputImage, IM_unsignedshort, vauleShiftForCasting);
//    if(outputImage)
//        outputImage->retain();
//    else
//        return false;

    MiaImage2VTKProp::reAllocOutputImage();

    if( inputImage->getType() == IM_RGBA)
    {

            volumeProperty->IndependentComponentsOn();

            volumeProperty->SetColor( 1,colorTransferFunctionForRGB_Red);
            volumeProperty->SetColor( 2,colorTransferFunctionForRGB_Green);
            volumeProperty->SetColor( 3,colorTransferFunctionForRGB_Blue);

            volumeProperty->SetScalarOpacity( 1, opacityTransferFunction);
            volumeProperty->SetScalarOpacity( 2, opacityTransferFunction);
            volumeProperty->SetScalarOpacity( 3, opacityTransferFunction);

            volumeProperty->SetComponentWeight( 0, 0); //first byte is opacity
    }
    else
    {
// needed for raycasting but not for fixpoint ray casting
//        volumeProperty->IndependentComponentsOff();
        volumeProperty->SetColor(colorTransferFunction);
        volumeProperty->SetScalarOpacity(opacityTransferFunction);


    }


    //SetInterpolationTypeToNearest();	//SetInterpolationTypeToLinear
    return true;

}

bool MiaImage2VTKVolume::runOperation()
{
    //update transfer functions
    if(currentLut)
    {

    }
    else
    {


        opacityTransferFunction->RemoveAllPoints();
        opacityTransferFunction->AddPoint(curWinLevel+vauleShiftForCasting-curWinWidth/2.0, 0);
        opacityTransferFunction->AddPoint(curWinLevel+vauleShiftForCasting-curWinWidth/4.0, 0.05);
        opacityTransferFunction->AddPoint(curWinLevel+vauleShiftForCasting+curWinWidth/4.0, 0.15);
        opacityTransferFunction->AddPoint(curWinLevel+vauleShiftForCasting+curWinWidth/2.0, 0.8);

        colorTransferFunction->RemoveAllPoints();
        colorTransferFunction->AddRGBPoint( curWinLevel+vauleShiftForCasting-curWinWidth/2.0, 1, 1, 1 );
        colorTransferFunction->AddRGBPoint( curWinLevel+vauleShiftForCasting-curWinWidth/4.0, 1, 0, 0 );
        colorTransferFunction->AddRGBPoint( curWinLevel+vauleShiftForCasting+curWinWidth/4.0, 1, 1, 0 );
        colorTransferFunction->AddRGBPoint( curWinLevel+vauleShiftForCasting+curWinWidth/2.0, 1, 1, 1 );

    }
    ((vtkVolume*)prop)->Update();
    return true;

}

void MiaImage2VTKVolume::dispatchParameters(const QMap <QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if(i.key() == "ww")
         {
             curWinWidth = i.value().toFloat();
         }
         else  if(i.key() == "wl")
         {
             curWinLevel = i.value().toFloat();
         }
//         qDebug() << i.key();
     }

}
