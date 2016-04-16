#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeType,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)

#include "miaimagereader.h"
#include <qfile.h>
#include <QtGui>
#include <QMessageBox>
#include "vtkDICOMImageReader.h"
#include "vtkNIFTIImageReader.h"
#include "vtkDataSetReader.h"
#include "vtkImageData.h"
#include "vtkImageCast.h"
#include "vtkMatrix4x4.h"
#ifdef Q_WS_MAC
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#endif
#include <iostream>
//#include <unistd.h>
//#include "miamarker3dpolygon.h"
//#include "dcmtk/config/osconfig.h"
//#include "dcmtk/dcmdata/dctk.h"
//#include "dcmtk/dcmimgle/dcmimage.h"
//#include "dcmtk/dcmimage/diregist.h"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define HEADINFO_SIZE 32

MiaImageReader::MiaImageReader(MiaNetwork *parent) :
    MiaModule(parent)
{
        moduleName = "MiaImageReader";
        sharedMemorySize = 0;
        aNewFileSet = true;
}
MiaImageReader::~MiaImageReader()
{
    if(sharedMemorySize>0)
    {
        freeSharedMemory();
    }
}

void MiaImageReader::setFileName(QString name, FileFormat format)
{
     fileName = name;
#ifdef Q_OS_WIN
     fileName.replace("\\","/");
#endif
     currentFormat = format;
     aNewFileSet = true;
     inputImageIsSet = true;
}

bool MiaImageReader::reAllocOutputImage()
{
    bool result;
    if(outputImage)
    {
        outputImage->release();
        outputImage = NULL;
    }
    if(fileName.isEmpty())
    {
        qDebug() << moduleName << "Input image is empty";
    }
    switch(currentFormat)
    {
    case MIA_DICOM:
        result = loadDICOMImage();
        break;
    case MIA_CHALLENGE:
        result = loadChallengeFolder();
        break;
    case MIA_MHD:
        result = loadMHDImage();
        break;
    case MIA_2D:
        result = load2DImage();
        break;
    case MIA_NII:
        result = loadNIFTIImage();
        break;
    case MIA_VTK:
        result = loadVTKImage();
        break;
    default:
        result = false;
        break;
    }
    if(result)
        aNewFileSet = false;
    return result;

}

bool MiaImageReader::loadMHDImage()
{
    QFile mhdfile(fileName);
    if (!mhdfile.open(QIODevice::ReadOnly | QIODevice::Text))
             return false;
    QString firstline = mhdfile.readLine();
    if(!firstline.contains("ObjectType", Qt::CaseInsensitive) || !firstline.contains("image",Qt::CaseInsensitive))
    {
        qDebug() << "This file is no a mhd image format";
        return false;
    }
    int Dimension = 3;
    bool isBinary = true, isByteOrderMSB = false, isCompressedData = false;
    float orientationmatrix[9];
    float offset[4];
    float spacing[4];
    float dimSize[4];
    int i,j;
    for(i = 0; i < 4; i++)
    {
        offset[i] = 0;
        spacing[i] = 0;
        dimSize[i] = 1;
    }
    for(i = 0; i < 9; i++)
        orientationmatrix[i] = 0;

    ImageType type;
    QString rawFileName;
    int sizeOfType;
    rawFileName = fileName.left(fileName.lastIndexOf("/")+1);
    while (!mhdfile.atEnd()) {
            QString line = mhdfile.readLine();
            if(line.contains("NDims", Qt::CaseInsensitive) )
            {
                QStringList list = line.split("=");
                QString para = list.last();
                Dimension = para.toInt();

            }
            else if(line.contains("BinaryData", Qt::CaseInsensitive) && !line.contains("BinaryDataByteOrderMSB", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString para = list.last();
                isBinary  = para.contains("true",  Qt::CaseInsensitive);
            }
            else if(line.contains("BinaryDataByteOrderMSB", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString para = list.last();
                isByteOrderMSB  = para.contains("true",  Qt::CaseInsensitive);
            }
            else if(line.contains("CompressedData", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString para = list.last();
                isCompressedData  = para.contains("true",  Qt::CaseInsensitive);
                if(isCompressedData)
                {
                    QMessageBox msgBox;
                    msgBox.setText("MIAlite can not handle compressed data!");
                    msgBox.exec();
                    return false;
                }
            }
            else if(line.contains("TransformMatrix", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString paras = list.last().trimmed();
                QStringList paralist = paras.split(" ", QString::SkipEmptyParts);
                int rows =  (Dimension>3)?3:Dimension;
                for (i = 0; i < rows; i++)
                {
                    for (j = 0; j < 3; j++) // should test whether it should be 3 or Dimension
                    {
                        QString para = paralist.first();
                        orientationmatrix[i*3 + j] = para.toFloat();
                        paralist.removeFirst();
                    }
                }
            }
            else if(line.contains("Offset", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                if(list.first().trimmed().toLower() == "offset")
                {
                    QString paras = list.last().trimmed();
                    QStringList paralist = paras.split(" ", QString::SkipEmptyParts);
                    int n_d = Dimension;
                    if ( n_d < paralist.count() && paralist.count() <= 4 )
                        n_d = paralist.count();
                    qDebug() << "offset " << paralist;
                    for (i = 0; i < n_d; i++)
                    {

                        QString para = paralist.first();
                        offset[i] = para.toFloat();
                        paralist.removeFirst();

                    }
                }
            }
            else if(line.contains("CenterOfRotation", Qt::CaseInsensitive))
            {

            }
            else if(line.contains("AnatomicalOrientation", Qt::CaseInsensitive))
            {

            }
            else if(line.contains("ElementSpacing", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString paras = list.last().trimmed();
                QStringList paralist = paras.split(" ", QString::SkipEmptyParts);

                for (i = 0; i < Dimension; i++)
                {

                    QString para = paralist.first();
                    spacing[i] = para.toFloat();
                    paralist.removeFirst();

                }
            }
            else if(line.contains("DimSize", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString paras = list.last().trimmed();
                QStringList paralist = paras.split(" ", QString::SkipEmptyParts);

                for (i = 0; i < Dimension; i++)
                {

                    QString para = paralist.first();
                    dimSize[i] = para.toInt();
                    paralist.removeFirst();

                }
            }
            else if(line.contains("ElementType", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString para = list.last();
                if(para.contains("MET_UCHAR",  Qt::CaseInsensitive))
                {
                    type = IM_uchar;
                    sizeOfType = sizeof(char);
                }
                else if(para.contains("MET_USHORT",  Qt::CaseInsensitive))
                {
                    type = IM_unsignedshort;
                    sizeOfType = sizeof(short);
                }
                else if(para.contains("MET_SHORT",  Qt::CaseInsensitive))
                {
                    type = IM_short;
                    sizeOfType = sizeof(short);
                }
                else if(para.contains("MET_INT",  Qt::CaseInsensitive))
                {
                    type = IM_int;
                    sizeOfType = sizeof(int);
                }
                else if(para.contains("MET_FLOAT",  Qt::CaseInsensitive))
                {
                    type = IM_float;
                    sizeOfType = sizeof(float);
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText("Unsupported data type! Please convert data to short, int or float");
                    msgBox.exec();
                    return false;
                }
            }
            else if(line.contains("ElementDataFile", Qt::CaseInsensitive))
            {
                QStringList list = line.split("=");
                QString para = list.last().trimmed();
                rawFileName.append(para);
                qDebug() << rawFileName;

            }

         }
    if(isBinary && !isCompressedData)
    {
        if(isByteOrderMSB)
        {
         //change to little endian

        }

        QFile rawfile(rawFileName);
        if(!rawfile.open(QIODevice::ReadOnly))
        {
            QMessageBox msgBox;
            msgBox.setText("can not open raw file %s"+rawFileName);
            msgBox.exec();
        }
        QByteArray* dataarry = new QByteArray();
        dataarry->resize(sizeOfType*dimSize[0]*dimSize[1]*dimSize[2]*dimSize[3]);
        qint64 readbyts = rawfile.read(dataarry->data(),dataarry->size());
        if(readbyts<sizeOfType*dimSize[0]*dimSize[1]*dimSize[2]*dimSize[3])
        {
            delete dataarry;
            QMessageBox msgBox;
            msgBox.setText("Row data array is shorter than it should be. Please check the head file is correct.");
            msgBox.exec();
            return false;

        }
        outputImage = new MiaImageND(dimSize[0], dimSize[1], dimSize[2], dimSize[3], type, dataarry);
        if(outputImage)
        {
            outputImage->retain();
            outputImage->setOffset(offset);
            outputImage->setSpacing(spacing);
            outputImage->set3x3OrientationMatrix(orientationmatrix);
            outputImage->computeTransformMatrix();
            float min, max;
            outputImage->getMaxMin(min,max);
            if(min < - 100) //CT?
            {
                outputImage->defaultWindowCenter = 150;
                outputImage->defaultWindowWidth = 300;
            }
            else
            {
                outputImage->defaultWindowCenter = (min + max) / 2.0;
                outputImage->defaultWindowWidth = (max - min);
            }
            needUpdate = false;
        }
        else
        {
            qDebug() << "Failed to allocate image!";
            return false;
        }

    }
    else
    {
        qDebug() << "Unspported format!";
        return false;
    }
    return true;
}

bool MiaImageReader::loadDICOMImage()
{
    vtkDICOMImageReader* vtkreader = vtkDICOMImageReader::New();
    QByteArray byteArray = fileName.toUtf8();
    const char* cString = byteArray.constData();
    vtkreader->SetDirectoryName(cString);
    vtkreader->Update();

    vtkImageCast* caster = vtkImageCast::New();
    caster->SetInputConnection(vtkreader->GetOutputPort());
    caster->SetOutputScalarTypeToShort ();
    caster->Update();
    vtkImageData* vtkimage = caster->GetOutput();

    int dim[3];

    vtkimage->GetDimensions(dim);


    outputImage = creatEmptyImage(dim[0],dim[1],dim[2],1,IM_short);
    if(outputImage)
    {

        outputImage->retain();



        double spacing[3];
        vtkimage->GetSpacing(spacing);
        float fspacing[3];
        for(int i = 0 ;i < 3; i++)
        {
            fspacing[i] = spacing[i];
            if(fspacing[i] == 0)
                fspacing[i] = 1.0;
        }
        outputImage->setSpacing(fspacing);

        float* origin;
        float offset[3];
        origin = vtkreader->GetImagePositionPatient();

        if(origin)
        {
            offset[0] = origin[0];
            offset[1] = origin[1];
            offset[2] = origin[2];
            outputImage->setOffset(offset);
        }
        float* o, orientationmatrix[6];
        o = vtkreader->GetImageOrientationPatient();
        if(o)
        {
            float sq1 = 0;
            float sq2 = 0;
            for(int i = 0 ;i < 3; i++)
            {
                orientationmatrix[i] = o[i];
                sq1 += o[i]*o[i];
                orientationmatrix[i+3] = o[i+3];
                sq2 += o[i+3]*o[i+3];
            }
            if(sq1 > 0 && sq2 > 0)
                outputImage->set3x2OrientationMatrix(orientationmatrix);
        }

        outputImage->computeTransformMatrix();

        short* indata, *outdata;
        indata = (short*) vtkimage->GetScalarPointer();
        outdata = (short*)outputImage->getDataPtr();

        int slicesize = dim[0]*dim[1];
        for(int z = 0; z < dim[2]; z++)
            for(int y = 0; y < dim[1]; y++)
                for(int x = 0 ;x < dim[0]; x++)
                {
                    outdata[z*slicesize + y*dim[0] + x] = indata[(dim[2]-1-z)*slicesize + (dim[1]-1-y)*dim[0] + x];
                }

        float min,max;
        outputImage->getMaxMin(min, max);
        if(min < - 100) //CT?
        {
            outputImage->defaultWindowCenter = 150;
            outputImage->defaultWindowWidth = 300;
        }
        else
        {
            outputImage->defaultWindowCenter = (min + max) / 2.0;
            outputImage->defaultWindowWidth = (max - min);
        }

    }
    else
    {
        vtkreader->Delete();
        return false;
    }
    vtkreader->Delete();
    return true;
}

bool
MiaImageReader::loadChallengeFolder()
{
    //first the DICOM image is loaded
    QString DICOMfileName = fileName + "/DICOM", aux = fileName;
    QString fname, line;
    QStringList list;
    std::vector<MiaPoint4D> start_markers;
    std::vector<MiaPoint4D> end_markers;
    MiaPoint4D p;


    fileName = DICOMfileName;
    loadDICOMImage();
    fileName = aux;

    //then the start and end points in all segments
    for ( int i = 1; i <= 17; i++ )
    {
        fname = fileName + "/seg_"+ (i<10? "0": "") + "/point_start.txt";
        QFile textfile1(fname);
        if (textfile1.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            line = textfile1.readLine();
            list = line.split(" ");
            p.pos[0] = list[0].toDouble(); p.pos[1] = list[1].toDouble(); p.pos[2] = list[2].toDouble();
            p.pos[3] = i;  //the segment (maybe it is not used at all!)
            start_markers.push_back(p);
            textfile1.close();
        }

        fname = fileName + "/seg_"+ (i<10? "0": "") + "/point_end.txt";
        QFile textfile2(fname);
        if (textfile2.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            line = textfile2.readLine();
            list = line.split(" ");
            p.pos[0] = list[0].toDouble(); p.pos[1] = list[1].toDouble(); p.pos[2] = list[2].toDouble();
            p.pos[3] = i; //the segment (maybe it is not used at all!)
            end_markers.push_back(p);
            textfile1.close();
        }
    }
    return true;
}
bool MiaImageReader::load2DImage()
{
//    return load2DDicomSlice(fileName);
    QImage* img = new QImage(fileName);
    if(img && img->width() > 0 && img->height() > 0)
    {
        int width, height;
        width = img->width();
        height = img->height();

        outputImage = creatEmptyImage(width,height,1,1,IM_short);
        if(outputImage)
        {

            outputImage->retain();

            short *outdata;
            outdata = (short*)outputImage->getDataPtr();
            int i, j;
            for( j = 0; j < height; j++)
                for( i = 0; i < width; i++)
                {
                    QRgb color = img->pixel(i,j);
                    int value = qGray( color );
                    *(outdata + j*width + i) = value;
                }

            float min,max;
            outputImage->getMaxMin(min, max);
            if(min < - 100) //CT?
            {
                outputImage->defaultWindowCenter = 150;
                outputImage->defaultWindowWidth = 300;
            }
            else
            {
                outputImage->defaultWindowCenter = (min + max) / 2.0;
                outputImage->defaultWindowWidth = (max - min);
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

//bool MiaImageReader::load2DDicomSlice(QString filename)
//{
//    DcmFileFormat fileformat;
//    OFCondition status = fileformat.loadFile(filename.toStdString().c_str());
//    if (status.good())
//    {
//       OFString patientsName;
//       if (fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientsName).good())
//       {
//          cout << "Patient's Name: " << patientsName << endl;
//       } else
//         cerr << "Error: cannot access Patient's Name!" << endl;
//    } else
//    cerr << "Error: cannot read DICOM file (" << status.text() << ")" << endl;

//    DicomImage *image = new DicomImage(filename.toStdString().c_str());
//    if (image != NULL && image->getStatus() == EIS_Normal)
//    {
//        int width, height;
//        width = image->getWidth();
//        height = image->getHeight();
//        qDebug()<<"width"<< width <<"height" << height<< "bit per sample" << image->getDepth();

//        outputImage = creatEmptyImage(width,height,1,1,IM_short);
//        if(outputImage)
//        {

//            outputImage->retain();

//            short *outdata;
//            const short *indata;
//            const DiPixel* decompressedPixels = image->getInterData();
//            qDebug() << decompressedPixels->getRepresentation();
//            indata = (const short*) decompressedPixels->getData();
//            outdata = (short*)outputImage->getDataPtr();
//            int i, j;
//            for( j = 0; j < height; j++)
//                for( i = 0; i < width; i++)
//                {
//                    *(outdata + j*width + i) = *(indata + j*width + i);
//                }
//            outputImage->scanMaxMinValue();
//            if(outputImage->minValue < - 100) //CT?
//            {
//                outputImage->defaultWindowCenter = 150;
//                outputImage->defaultWindowWidth = 300;
//            }
//            else
//            {
//                outputImage->defaultWindowCenter = (outputImage->minValue + outputImage->maxValue) / 2.0;
//                outputImage->defaultWindowWidth = (outputImage->maxValue - outputImage->minValue);
//            }
//        }
//    } else
//        qDebug()<< "unable to open the file or the file is not a dicom file";
//    return true;
//}

bool MiaImageReader::loadDICOMImageWithDCMTK()
{
    return false;
}

bool MiaImageReader::loadVTKImage()
{
    vtkDataSetReader* vtkreader = vtkDataSetReader::New();
    QByteArray byteArray = fileName.toUtf8();
    const char* cString = byteArray.constData();
    vtkreader->SetFileName(cString);
    vtkreader->Update();

    vtkImageCast* caster = vtkImageCast::New();
    caster->SetInputData ((vtkDataObject*) vtkreader->GetStructuredPointsOutput());
    caster->SetOutputScalarTypeToShort ();
    caster->Update();
    vtkImageData* vtkimage = caster->GetOutput();

    int dim[3];

    vtkimage->GetDimensions(dim);


    outputImage = creatEmptyImage(dim[0],dim[1],dim[2],1,IM_short);
    if(outputImage)
    {

        outputImage->retain();



        double spacing[3];
        vtkimage->GetSpacing(spacing);
        float fspacing[3];
        for(int i = 0 ;i < 3; i++)
            fspacing[i] = spacing[i];
        outputImage->setSpacing(fspacing);

        float offset[3];
        float orientationmatrix[6];
        double origin[3];
        vtkimage->GetOrigin(origin);
        offset[0] = origin[0];
        offset[1] = origin[1];
        offset[2] = origin[2];
        outputImage->setOffset(offset);

        int pt0[3] = {0,0,0};
        int pt1[3] = {1,0,0};
        int pt2[3] = {0,1,0};
        vtkIdType id0 = vtkimage->ComputePointId(pt0);
        vtkIdType id1 = vtkimage->ComputePointId(pt1);
        vtkIdType id2 = vtkimage->ComputePointId(pt2);
        double pt0_coor[3];
        double pt1_coor[3];
        double pt2_coor[3];
        vtkimage->GetPoint(id0,pt0_coor);
        vtkimage->GetPoint(id1,pt1_coor);
        vtkimage->GetPoint(id2,pt2_coor);

        for(int i = 0 ;i < 3; i++)
        {
            orientationmatrix[i] = pt1_coor[i]-pt0_coor[i];
            orientationmatrix[i+3] = pt2_coor[i]-pt0_coor[i];
        }

        outputImage->set3x2OrientationMatrix(orientationmatrix);

        outputImage->computeTransformMatrix();

        short* indata, *outdata;
        indata = (short*) vtkimage->GetScalarPointer();
        outdata = (short*)outputImage->getDataPtr();

        int slicesize = dim[0]*dim[1];
        for(int z = 0; z < dim[2]; z++)
            for(int y = 0; y < dim[1]; y++)
                for(int x = 0 ;x < dim[0]; x++)
                {
                    outdata[z*slicesize + y*dim[0] + x] = indata[z*slicesize + y*dim[0] + x];
                }

        float min,max;
        outputImage->getMaxMin(min, max);
        if(min < - 100) //CT?
        {
            outputImage->defaultWindowCenter = 150;
            outputImage->defaultWindowWidth = 300;
        }
        else
        {
            outputImage->defaultWindowCenter = (min + max) / 2.0;
            outputImage->defaultWindowWidth = (max - min);
        }
    }
    else
    {
        vtkreader->Delete();
        return false;
    }
    vtkreader->Delete();
    return true;
}

bool MiaImageReader::loadNIFTIImage()
{
    vtkNIFTIImageReader* vtkreader = vtkNIFTIImageReader::New();
    QByteArray byteArray = fileName.toUtf8();
    const char* cString = byteArray.constData();
    vtkreader->SetFileName(cString);
    vtkreader->Update();

    vtkImageCast* caster = vtkImageCast::New();
    caster->SetInputConnection(vtkreader->GetOutputPort());
    caster->SetOutputScalarTypeToShort ();
    caster->Update();
    vtkImageData* vtkimage = caster->GetOutput();

    int dim[3];

    vtkimage->GetDimensions(dim);


    outputImage = creatEmptyImage(dim[0],dim[1],dim[2],1,IM_short);
    if(outputImage)
    {

        outputImage->retain();



        double spacing[3];
        vtkimage->GetSpacing(spacing);
        float fspacing[3];
        for(int i = 0 ;i < 3; i++)
            fspacing[i] = spacing[i];
        outputImage->setSpacing(fspacing);

        vtkMatrix4x4 * qMatrix = vtkreader->GetQFormMatrix ();
        float offset[3];
        float orientationmatrix[6];
        if(qMatrix)
        {
            offset[0] = qMatrix->Element[0][3];
            offset[1] = qMatrix->Element[1][3];
            offset[2] = qMatrix->Element[2][3];
            outputImage->setOffset(offset);

            for(int i = 0 ;i < 3; i++)
            {
                orientationmatrix[i] = qMatrix->Element[0][i];
                orientationmatrix[i+3] = qMatrix->Element[1][i];
            }
        }
        else
        {

            double origin[3];
            vtkimage->GetOrigin(origin);
            offset[0] = origin[0];
            offset[1] = origin[1];
            offset[2] = origin[2];
            outputImage->setOffset(offset);

            int pt0[3] = {0,0,0};
            int pt1[3] = {1,0,0};
            int pt2[3] = {0,1,0};
            vtkIdType id0 = vtkimage->ComputePointId(pt0);
            vtkIdType id1 = vtkimage->ComputePointId(pt1);
            vtkIdType id2 = vtkimage->ComputePointId(pt2);
            double pt0_coor[3];
            double pt1_coor[3];
            double pt2_coor[3];
            vtkimage->GetPoint(id0,pt0_coor);
            vtkimage->GetPoint(id1,pt1_coor);
            vtkimage->GetPoint(id2,pt2_coor);

            for(int i = 0 ;i < 3; i++)
            {
                orientationmatrix[i] = pt1_coor[i]-pt0_coor[i];
                orientationmatrix[i+3] = pt2_coor[i]-pt0_coor[i];
            }
        }

        outputImage->set3x2OrientationMatrix(orientationmatrix);

        outputImage->computeTransformMatrix();

        short* indata, *outdata;
        indata = (short*) vtkimage->GetScalarPointer();
        outdata = (short*)outputImage->getDataPtr();

        int slicesize = dim[0]*dim[1];
        for(int z = 0; z < dim[2]; z++)
            for(int y = 0; y < dim[1]; y++)
                for(int x = 0 ;x < dim[0]; x++)
                {
                    outdata[z*slicesize + y*dim[0] + x] = indata[z*slicesize + y*dim[0] + x];
                }

        float min,max;
        outputImage->getMaxMin(min, max);
        if(min < - 100) //CT?
        {
            outputImage->defaultWindowCenter = 150;
            outputImage->defaultWindowWidth = 300;
        }
        else
        {
            outputImage->defaultWindowCenter = (min + max) / 2.0;
            outputImage->defaultWindowWidth = (max - min);
        }
    }
    else
    {
        vtkreader->Delete();
        return false;
    }
    vtkreader->Delete();
    return true;
    return false;
}
#ifdef Q_WS_MAC
bool MiaImageReader::LoadImageInSharedMemory(QString memID, int size)
{
    const char* memid = memID.toAscii().constData();
    int fd = -1;
    size_t sz = size;
    sharedMemorySize = size;
    fileName = memID;
    fd = shm_open( memid, O_RDWR | O_CREAT, FILE_MODE );
    //	NSLog(@"shm_open fd = %d\n", fd );
    ftruncate(fd, sz);
    int err = 1;
    if ( fd >= 0 )
    {
            try
            {
                    sharedMemory = mmap(0, sz, PROT_READ | PROT_WRITE, MAP_SHARED , fd, 0 );
            }
            catch( const std::exception& e )
            {
                    qDebug("failed to map shared memory.");

            }
            if ( (long)sharedMemory != -1 )
            {
                    qDebug("mmap worked, sharedMemory = 0x%x\n", sharedMemory );
                    err=0;
            }
            else
                    qDebug("mmap failed, errno = %d\n", errno );
            close(fd);
    }
    else
            qDebug("open failed, errno = %d\n", errno );
    if(err)
    {
        QMessageBox msgBox;
        msgBox.setText("can not open the shared memory"+memID);
        msgBox.exec();
        return false;
    }
    float* headInfo = (float *) sharedMemory;


    int dim[3];
    float spacing[3], orient[9],offset[3];
    int i;
    for(i=0;i<3;i++)
            dim[i] = headInfo[i];
    for(i = 0; i < 3; i++)
            spacing[i] = headInfo[i+3];
    for(i = 0; i < 9; i++)
            orient[i] = headInfo[i+6];
    for(i = 0; i < 3; i++)
            offset[i] = headInfo[i+15];
    float wc,ww;
    wc = headInfo[18];
    ww = headInfo[19];
    float max,min;
    min = headInfo[20];
    max = headInfo[21];
    char* dataptr = (char*)sharedMemory+HEADINFO_SIZE*sizeof(float)/sizeof(char);
    outputImage = new ImageND(dim[0], dim[1], dim[2], 1, IM_short, dataptr);
    if(outputImage)
    {

        outputImage->retain();

        outputImage->setSpacing(spacing);
        outputImage->setOffset(offset);

        outputImage->set3x3OrientationMatrix(orient);
        outputImage->computeTransformMatrix();
        outputImage->defaultWindowCenter = wc;
        outputImage->defaultWindowWidth = ww;
        if(min < max)
        {
            outputImage->minValue = min;
            outputImage->maxValue = max;
        }
        else
        {
            outputImage->minValue = -1200;
            outputImage->maxValue = 2000;
        }

        needUpdate = false;
        needReAllocOutputImage = false;


    }
    else
        return false;
    return true;


}
void MiaImageReader::freeSharedMemory()
{
    const char* memid = fileName.toAscii().constData();
    try
    {
            if(munmap(sharedMemory,sharedMemorySize)==-1)
                    qDebug() <<"failed to unmap shared memory. Memory Leaking";
            shm_unlink(memid);
            qDebug() <<"Released shared memory" <<memid;

    }
    catch( const std::exception& e)
    {
            qDebug() <<"failed to unmap shared memory. Memory Leaking";
    }

}

#endif

#ifndef Q_WS_MAC
bool MiaImageReader::LoadImageInSharedMemory(QString memID, int size)
{
    return false;
}
void MiaImageReader::freeSharedMemory()
{
    return;
}

#endif
