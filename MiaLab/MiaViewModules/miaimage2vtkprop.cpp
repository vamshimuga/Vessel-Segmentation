#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeType,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#include "miaimage2vtkprop.h"
#include "vtkProp3D.h"
#include "vtkImageImport.h"
#include "vtkMatrix4x4.h"

//#include "vtkAbstractMapper3D.h"


MiaImage2VTKProp::MiaImage2VTKProp(MiaNetwork *parent) :
    MiaModule(parent)
{
 #ifndef QT_DEBUG
    vtkObject::GlobalWarningDisplayOff();
#endif
    importer = vtkImageImport::New();
    for(int i = 0; i < 27; i++)
        dataSample[i]=0;
    importer->SetImportVoidPointer(dataSample);
    importer->SetWholeExtent(0, 2, 0, 2, 0, 2);
    importer->SetDataExtentToWholeExtent();
    //  needed for ray cast to convert it to unsigned data.
    //  importer->SetDataScalarTypeToUnsignedShort();
    importer->SetDataScalarTypeToShort();
    importer->SetNumberOfScalarComponents(1);
}

MiaImage2VTKProp::~MiaImage2VTKProp()
{
    emit imageModuleWillBeDeleted();
    if(importer)
        importer->Delete();

}

vtkProp3D* MiaImage2VTKProp::getProp()
{
    if(needUpdate)
        update();
    return prop;
}
//bool MiaImage2VTKProp::runOperation()
//{
    //use subclass function
//}
MiaPoint4D MiaImage2VTKProp::getImageCenter()
{
    MiaPoint4D centerpt;
    if(!inputImage)
        return centerpt;


    centerpt.pos[0] = inputImage->getWidth() / 2.0;
    centerpt.pos[1] = inputImage->getHeight() / 2.0;
    centerpt.pos[2] = inputImage->getDepth() / 2.0;
    return inputImage->convertVoxelToPatientCoordinate(centerpt);

}

bool MiaImage2VTKProp::reAllocOutputImage()
{
    if(!inputImage)
        return false;
    sourceImage = inputImage;


    if(outputImage)
        sourceImage = outputImage;
    void* data = (void*) sourceImage->getDataPtr();
    importer->SetImportVoidPointer(data);
    importer->SetWholeExtent(0, sourceImage->getWidth()-1, 0, sourceImage->getHeight()-1, 0, sourceImage->getDepth()-1);
    importer->SetDataExtentToWholeExtent();
     switch(sourceImage->getType())
     {
        case IM_uchar:
         importer->SetDataScalarTypeToUnsignedChar();
         importer->SetNumberOfScalarComponents(1);
         break;
         case IM_short:
          importer->SetDataScalarTypeToShort();
          importer->SetNumberOfScalarComponents(1);
          break;
         case IM_unsignedshort:
          importer->SetDataScalarTypeToUnsignedShort();
          importer->SetNumberOfScalarComponents(1);
          break;
         case IM_int:
         importer->SetDataScalarTypeToInt();
          importer->SetNumberOfScalarComponents(1);
          break;
         case IM_float:
          importer->SetDataScalarTypeToFloat();
          importer->SetNumberOfScalarComponents(1);
          break;
         case IM_RGBA:
          importer->SetDataScalarTypeToUnsignedChar();
          importer->SetNumberOfScalarComponents(4);
          break;

     }

    float* spacing = sourceImage->getSpacing();
    importer->SetDataSpacing( spacing[0], spacing[1], spacing[2]);

    float* pos = sourceImage->getOffset();

//    prop->SetPosition(pos[0],pos[2],pos[3]);
    // the default internal traform matrix is
    // 1 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 1
    vtkMatrix4x4* matrixvtk =  getTranformMatrix();
    prop->SetUserMatrix(matrixvtk);
    matrixvtk->Delete();

    return true;
}

vtkMatrix4x4 *MiaImage2VTKProp::getTranformMatrix()
{
    vtkMatrix4x4* matrixvtk =  vtkMatrix4x4::New();
    int i,j;
//    float* matrix = sourceImage->getTransformMatrix();
//    for(i = 0; i < 4; i++)
//        for(j = 0; j < 4; j++)
//        {
//            matrixvtk->Element[i][j] = matrix[i*4+j];
//        }

    // The above does not work as in vtk, spacing is not part of the transform matrix.
    float* orientationmatrix = sourceImage->getOrientationMatrix();
    float* offset = sourceImage->getOffset();

    for( i = 0; i < 3 ; i++)
        for( j = 0; j < 3; j++)
        {
            matrixvtk->Element[i][j] = orientationmatrix[ j*3 + i];
        }

    for( i = 0; i < 3 ; i++)
    {
        matrixvtk->Element[i][3]  = offset[i];
    }
     for( j = 0; j < 3; j++)
     {
         matrixvtk->Element[3][j]  = 0;
     }

    matrixvtk->Element[3][3] = 1.0;
    prop->SetUserMatrix(matrixvtk);
    return matrixvtk;
}
bool MiaImage2VTKProp::reloadInputImage()
{
    if(MiaModule::reloadInputImage())
    {
        return true;
    }
    else
    {
        return false;
    }

}
