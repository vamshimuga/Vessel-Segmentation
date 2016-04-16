#include "miaimageexporter.h"
#include "vtkNIFTIImageWriter.h"
#include "vtkImageImport.h"
#include "vtkMatrix4x4.h"
#include <qfile.h>
#include <QtGui>

MiaImageExporter::MiaImageExporter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Mia Exporter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    currentFormat = MIA_MHD;
}

void MiaImageExporter::setFileName(QString name, FileFormat format)
{
    fileName = name;
#ifdef Q_OS_WIN
     fileName.replace("\\","/");
#endif
    currentFormat = format;
}

bool MiaImageExporter::runOperation()
{
    switch (currentFormat) {
    case MIA_MHD:
        return saveMHDImage();
        break;
    case MIA_NII:
        return saveNIFTIImage();
        break;
    default:
        throw std::runtime_error("Unsupported image format!");
        break;
    }
    return false;
}

bool MiaImageExporter::saveNIFTIImage()
{
    if(fileName.right(4)!= ".nii")
        fileName.append(".nii");
    if(!inputImage)
        return false;
    vtkImageImport* importer = vtkImageImport::New();
    short* data = (short*)malloc(sizeof(short)*inputImage->getImageSize());
    inputImage->castToShort(data);
//    void* data = (void*) inputImage->getDataPtr();
    importer->SetImportVoidPointer(data);
    importer->SetWholeExtent(0, inputImage->getWidth()-1, 0, inputImage->getHeight()-1, 0, inputImage->getDepth()-1);
    importer->SetDataExtentToWholeExtent();
     switch(IM_short)
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

    float* spacing = inputImage->getSpacing();
    importer->SetDataSpacing( spacing[0], spacing[1], spacing[2]);

    float* pos = inputImage->getOffset();

//    prop->SetPosition(pos[0],pos[2],pos[3]);
    // the default internal traform matrix is
    // 1 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 1
    vtkMatrix4x4* matrixvtk =  getTranformMatrix();

    vtkNIFTIImageWriter* writer = vtkNIFTIImageWriter::New();
    writer->SetInputConnection(importer->GetOutputPort());
    QByteArray byteArray = fileName.toUtf8();
    const char* cString = byteArray.constData();
    writer->SetFileName(cString);
    writer->SetNIFTIVersion(1);
    writer->SetQFormMatrix(matrixvtk);
    writer->Update();

    writer->Delete();
    matrixvtk->Delete();
    free(data);
    return true;
}

bool MiaImageExporter::saveMHDImage()
{
    if(fileName.right(4)!= ".mhd")
        fileName.append(".mhd");
    QFile mhdfile(fileName);
    if (!mhdfile.open(QIODevice::WriteOnly | QIODevice::Text))
       return false;
    QByteArray aparameter;
    char para[512];
    aparameter = "ObjectType = Image\n";
    mhdfile.write(aparameter);

    sprintf(para,"NDims = %d\n",inputImage->getDimension());
    aparameter = para;
    mhdfile.write(aparameter);

    aparameter = "BinaryData = True\n";
    mhdfile.write(aparameter);

    aparameter = "BinaryDataByteOrderMSB = False\n";
    mhdfile.write(aparameter);

    float* orientationmatrix = inputImage->getOrientationMatrix();
    sprintf(para, "TransformMatrix = %f %f %f %f %f %f %f %f %f\n",
            orientationmatrix[0],orientationmatrix[1],orientationmatrix[2],
            orientationmatrix[3],orientationmatrix[4],orientationmatrix[5],
            orientationmatrix[6],orientationmatrix[7],orientationmatrix[8]);

    aparameter = para;
    mhdfile.write(aparameter);

    float* offset = inputImage->getOffset();
    sprintf(para,"Offset = %f %f %f\n",offset[0],offset[1],offset[2]);
    aparameter = para;
    mhdfile.write(aparameter);

    aparameter = "CenterOfRotation = 0 0 0\n";
    mhdfile.write(aparameter);

    float* spacing = inputImage->getSpacing();
    sprintf(para, "ElementSpacing = %f %f %f\n",spacing[0],spacing[1],spacing[2]);
    aparameter = para;
    mhdfile.write(aparameter);

    int* dimSize = inputImage->getDimSize();
    sprintf(para, "DimSize = %d %d %d\n",dimSize[0],dimSize[1],dimSize[2]);
    aparameter = para;
    mhdfile.write(aparameter);

    sprintf(para, "ElementSize = %f %f %f\n",spacing[0],spacing[1],spacing[2]);
    aparameter = para;
    mhdfile.write(aparameter);

    switch(inputImage->getType())
    {
    case IM_uchar:
        aparameter = "ElementType = MET_UCHAR\n";
        break;
    case IM_short:
        aparameter = "ElementType = MET_SHORT\n";
        break;
    case IM_float:
        aparameter = "ElementType = MET_FLOAT\n";
        break;
    case IM_int:
        aparameter = "ElementType = MET_INT\n";
        break;
    case IM_unsignedshort:
        aparameter = "ElementType = MET_USHORT\n";
        break;
    default:
        qDebug()<<"can not export image: unsupported type";
        return false;

    }


    mhdfile.write(aparameter);

    QString rawFileName;
    rawFileName = fileName.right(fileName.length() - 1 - fileName.lastIndexOf("/"));
    int presiffix = rawFileName.lastIndexOf(".");
    if(presiffix == -1)
        rawFileName = rawFileName + ".raw";
    else
        rawFileName = rawFileName.left(presiffix) + ".raw";
    QString p = "ElementDataFile = " + rawFileName ;
    aparameter = p.toUtf8();
    mhdfile.write(aparameter);

    mhdfile.flush();
    mhdfile.close();
    rawFileName = fileName.left(fileName.lastIndexOf("/")+1) + rawFileName;
    QFile rawfile(rawFileName);
    if (!rawfile.open(QIODevice::WriteOnly))
       return false;

    rawfile.write((char*)inputImage->getDataPtr(), inputImage->getImageSize()*inputImage->getBytePerElement());

    rawfile.flush();
    rawfile.close();

    return true;
}

vtkMatrix4x4 *MiaImageExporter::getTranformMatrix()
{
    vtkMatrix4x4* matrixvtk =  vtkMatrix4x4::New();
    int i,j;
//    float* matrix = inputImage->getTransformMatrix();
//    for(i = 0; i < 4; i++)
//        for(j = 0; j < 4; j++)
//        {
//            matrixvtk->Element[i][j] = matrix[i*4+j];
//        }

    // The above does not work as in vtk, spacing is not part of the transform matrix.
    float* orientationmatrix = inputImage->getOrientationMatrix();
    float* offset = inputImage->getOffset();

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

    return matrixvtk;
}
