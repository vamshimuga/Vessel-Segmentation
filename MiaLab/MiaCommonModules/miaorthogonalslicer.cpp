#include "miaorthogonalslicer.h"
#include <QtGui>

MiaOrthogonalSlicer::MiaOrthogonalSlicer(MiaNetwork *parent):MiaModule(parent)
{
    moduleName = "MiaOrthogonalSlicer";
    outWidth = 0, outHeight = 0;
    sliceMode = Axial;
    wiredParameters.insert(QString("pat_x"), pat_coordinate.pos[0]);
    wiredParameters.insert(QString("pat_y"), pat_coordinate.pos[1]);
    wiredParameters.insert(QString("pat_z"), pat_coordinate.pos[2]);
    wiredParameters.insert(QString("pat_t"), pat_coordinate.pos[3]);
    wiredParameters.insert(QString("scroll_y"), 0);
    flipUpDown = true;


}
void MiaOrthogonalSlicer::resetPositionToCenter()
{
    if(!inputImage && !reloadInputImage())
    {
        qDebug() << "No input image";
        return;
    }
    int* dimsize = inputImage->getDimSize();
    int i;
    MiaPoint4D newpos;
    for( i = 0; i<4; i++)
        newpos.pos[i] = dimsize[i]/2;
    setPositionTo(newpos);
}

void MiaOrthogonalSlicer::setPositionTo(MiaPoint4D & apos)
{
     position = apos;
     pat_coordinate = inputImage->convertVoxelToPatientCoordinate(position);
     wiredParameters.insert(QString("pat_x"), pat_coordinate.pos[0]);
     wiredParameters.insert(QString("pat_y"), pat_coordinate.pos[1]);
     wiredParameters.insert(QString("pat_z"), pat_coordinate.pos[2]);
     wiredParameters.insert(QString("pat_t"), pat_coordinate.pos[3]);

     update();

     emit parameterChanged(wiredParameters);
}
void MiaOrthogonalSlicer::setOrientation(Orient_Mode mode )
{
    sliceMode = mode;
    checkIfNeedFlip();
}
void MiaOrthogonalSlicer::dispatchParameters(const QMap<QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
    MiaPoint4D newpos;
     while (i.hasNext()) {
         i.next();
         if(i.key() == "pat_x")
         {
             pat_coordinate.pos[0] = i.value().toFloat();
         }
         else if(i.key() == "pat_y")
         {
             pat_coordinate.pos[1] = i.value().toFloat();
         }
         else if(i.key() == "pat_z")
         {
             pat_coordinate.pos[2] = i.value().toFloat();
         }
         else if(i.key() == "pat_t")
         {
             pat_coordinate.pos[3] = i.value().toFloat();
         }
         else if(i.key() == "scroll_y")
         {
             float scroll_delta = i.value().toFloat();
             if(scroll_delta!=0)
             {
                wiredParameters.insert(QString("scroll_y"), 0);
                scroll((int)scroll_delta);
             }
         }
     }


}

bool MiaOrthogonalSlicer::runOperation()
{
    bool result = false;
    ImageType sourcetype = inputImage->getType();
    float* out = (float*)outputImage->getDataPtr();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            result = runOperation<uchar>(in,out);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            result = runOperation<short>(in,out);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            result = runOperation<unsigned short>(in,out);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            result = runOperation<int>(ini,out);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            result = runOperation<float>(inf,out);
        }
            break;
        case IM_RGBA:
        {
            QRgb* inr = (QRgb*) inputImage->getDataPtr();
            result = runOperation<QRgb>(inr,out);
        }
            break;
    }
    if(!result && outputImage != NULL)
    {
        outputImage->release();
        outputImage = NULL;
    }
    return result;
}
template <class S>
bool MiaOrthogonalSlicer::runOperation(S* inData, float* outdata)
{
    int* dimsize = inputImage->getDimSize();
    float* inputspacing = inputImage->getSpacing();
    position = inputImage->convertPatientCoordinateToVoxel(pat_coordinate);
    int baseindex = (int)(position.pos[3])*dimsize[0]*dimsize[1]*dimsize[2];
    if(baseindex >= inputImage->getImageSize() || baseindex < 0 )
        return false;

    int xstep = 0, ystep = 0;
    float newspacing[4];
    newspacing[3] = 1.0f;
    MiaPoint4D leftuppercorner;
    MiaPoint4D firstrow;
    MiaPoint4D firstcolomn;
    switch(sliceMode)
    {
        case Axial:
        {
              if(position.pos[2] < 0 || position.pos[2] >= dimsize[2])
                  return false;
              baseindex += (int)(position.pos[2])*dimsize[0]*dimsize[1];
              xstep = 1;
              ystep = dimsize[0];
              newspacing[0] = inputspacing[0];
              newspacing[1] = inputspacing[1];
              newspacing[2] = inputspacing[2];
              leftuppercorner.pos[0] = 0; leftuppercorner.pos[1] = 0; leftuppercorner.pos[2] = position.pos[2]; leftuppercorner.pos[3] = position.pos[3];
              firstrow.pos[0] = 1; firstrow.pos[1] = 0; firstrow.pos[2] = 0; firstrow.pos[3] = 0;
              firstcolomn.pos[0] = 0; firstcolomn.pos[1] = 1; firstcolomn.pos[2] = 0; firstcolomn.pos[3] = 0;

        }
        break;
        case Coronal:
        {
            if(position.pos[1] < 0 || position.pos[1] >= dimsize[1])
                return false;
            baseindex += (int)(position.pos[1])*dimsize[0];
            xstep = 1;
            ystep = dimsize[0]*dimsize[1];
            newspacing[0] = inputspacing[0];
            newspacing[1] = inputspacing[2];
            newspacing[2] = inputspacing[1];
            leftuppercorner.pos[0] = 0; leftuppercorner.pos[1] = position.pos[1]; leftuppercorner.pos[2] = 0; leftuppercorner.pos[3] = position.pos[3];
            firstrow.pos[0] = 1; firstrow.pos[1] = 0; firstrow.pos[2] = 0; firstrow.pos[3] = 0;
            firstcolomn.pos[0] = 0; firstcolomn.pos[1] = 0; firstcolomn.pos[2] = 1; firstcolomn.pos[3] = 0;

        }
        break;
        case Saggital:
        {
            if(position.pos[0] >= dimsize[0] || position.pos[0] < 0)
                return false;
            baseindex += position.pos[0];
            xstep = dimsize[0];
            ystep = dimsize[0]*dimsize[1];
            newspacing[0] = inputspacing[1];
            newspacing[1] = inputspacing[2];
            newspacing[2] = inputspacing[0];
            leftuppercorner.pos[0] = position.pos[0]; leftuppercorner.pos[1] = 0; leftuppercorner.pos[2] = 0; leftuppercorner.pos[3] = position.pos[3];
            firstrow.pos[0] = 0; firstrow.pos[1] = 1; firstrow.pos[2] = 0; firstrow.pos[3] = 0;
            firstcolomn.pos[0] = 0; firstcolomn.pos[1] = 0; firstcolomn.pos[2] = 1; firstcolomn.pos[3] = 0;


        }
        break;
    }
    int i,j;
    if(inputImage->getType() != IM_RGBA)
    {
        for(i = 0; i < outHeight; i++)
            for(j = 0; j < outWidth; j++)
            {
                *(outdata + i*outWidth + j) = (float) *(inData + baseindex + i*ystep + j*xstep);
            }
    }
    else
    {
        QRgb* rgbOutData = (QRgb*)outdata;
        for(i = 0; i < outHeight; i++)
            for(j = 0; j < outWidth; j++)
            {
                *(rgbOutData + i*outWidth + j) = *(inData + baseindex + i*ystep + j*xstep);
            }
    }
    float* origin = inputImage->getOffset();
    MiaPoint4D newoffset = inputImage->convertVoxelToPatientCoordinate(leftuppercorner);
    MiaPoint4D newxorientation = inputImage->convertVoxelToPatientCoordinate(firstrow);
    MiaPoint4D newyorientation = inputImage->convertVoxelToPatientCoordinate(firstcolomn);
    outputImage->setOffset( newoffset.pos );
    outputImage->setSpacing(newspacing);
    float neworientation[9];
    for(i = 0 ; i < 3 ; i++)
    {
        neworientation[i] = newxorientation.pos[i]-origin[i];
        neworientation[i+3] = newyorientation.pos[i] - origin[i];
        neworientation[i+6] = 0;
    }
    outputImage->set3x2OrientationMatrix( neworientation );

    outputImage->computeTransformMatrix();

    outputImage->needFlipUpDown = flipUpDown;
    outputImage->needFlipLeftRight = flipLeftRight;

    return true;
}

void MiaOrthogonalSlicer::checkIfNeedFlip()
{
    if(!inputImage)
        return;
    float* inputOrient = inputImage->getOrientationMatrix();
    int i;
    float sliceOrient [6];
    switch(sliceMode)
    {
        case Axial:

            for( i = 0 ; i < 6 ; i++)
            {
                sliceOrient[i] = inputOrient[i];
            }
            break;
        case Coronal:
            for( i = 0 ; i < 3 ; i++)
            {
                sliceOrient[i] = inputOrient[i];
            }
            for( i = 0 ; i < 3 ; i++)
            {
                sliceOrient[i+3] = inputOrient[i+6];
            }
            break;
        case Saggital:
            for( i = 0 ; i < 3 ; i++)
            {
                sliceOrient[i] = inputOrient[i+3];
            }
            for( i = 0 ; i < 3 ; i++)
            {
                sliceOrient[i+3] = inputOrient[i+6];
            }
            break;
    }
    //check x direction to be increase from left to right
    int maxindex = 0;
    float maxvalue = fabs(sliceOrient[maxindex]);
    for( i = 0 ; i < 3 ; i++)
    {
        if (maxvalue < fabs( sliceOrient[i] ))
        {
            maxindex = i;
            maxvalue = fabs( sliceOrient[i] );
        }
    }
    if( sliceOrient[maxindex] < 0 )
        flipLeftRight = true;
    else
        flipLeftRight = false;

    // check x direction to be increaseing
    maxvalue = fabs(sliceOrient[maxindex+3]);
    for( i = 0 ; i < 3 ; i++)
    {
        if (maxvalue < fabs( sliceOrient[i+3] ))
        {
            maxindex = i;
            maxvalue = fabs( sliceOrient[i+3] );
        }
    }
    if( maxindex != 1) // check y direction to be decreasing
    {
        if( sliceOrient[maxindex+3] > 0 )
            flipUpDown = true;
        else
            flipUpDown = false;
    }
    else // if y axis is the direction of y axis of patient reverse it
    {
        if( sliceOrient[maxindex+3] < 0 )
            flipUpDown = true;
        else
            flipUpDown = false;
    }




}

bool MiaOrthogonalSlicer::reAllocOutputImage()
{
    outWidth = 0, outHeight = 0;
    int x,y,z;
    x = inputImage->getWidth();
    y = inputImage->getHeight();
    z = inputImage->getDepth();
    switch(sliceMode)
    {
        case Axial:
            outWidth = x;
            outHeight = y;
            break;
        case Coronal:
            outWidth = x;
            outHeight = z;
            break;
        case Saggital:
            outWidth = y;
            outHeight = z;
            break;
    }
    if(outputImage && (outputImage->getWidth()!= outWidth || outputImage->getHeight() != outHeight) )
    {
        outputImage->release();
        outputImage = NULL;
    }
    if(outputImage == NULL)
    {
        if(inputImage->getType() == IM_RGBA)
            outputImage = new MiaImageND( outWidth, outHeight, 1, 1, IM_RGBA);
        else
            outputImage = new MiaImageND( outWidth, outHeight, 1, 1, IM_float);
        if(outputImage)
        {
            outputImage->retain();

        }
        else
            return false;
    }

 checkIfNeedFlip();
 return true;

}

void MiaOrthogonalSlicer::scroll(int delta)
{
    MiaPoint4D newpos = inputImage->convertPatientCoordinateToVoxel(pat_coordinate);
    int* dimsize = inputImage->getDimSize();
    switch(sliceMode)
    {
    case Axial:
        newpos.pos[2] = ((int)roundf(newpos.pos[2] + delta) + dimsize[2]) % dimsize[2];
        break;
    case Coronal:
        newpos.pos[1] = ((int)roundf(newpos.pos[1] + delta) + dimsize[1]) % dimsize[1];
        break;
    case Saggital:
        newpos.pos[0] = ((int)roundf(newpos.pos[0] + delta) + dimsize[0]) % dimsize[0];
        break;
    }
    setPositionTo(newpos);

}
