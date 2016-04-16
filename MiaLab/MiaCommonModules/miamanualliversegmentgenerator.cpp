#include "miamanualliversegmentgenerator.h"
#include "miaconstants.h"

MiaManualLiverSegmentGenerator::MiaManualLiverSegmentGenerator(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Marker Statistics";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenOutputRequested = false;
    autoUpdateWhenParameterChanged = false;
    autoUpdateWhenInputMarkerUpdated = false;
    upperThreshold = 0;
    lowerThreshold = -100000000;
    startAngle = -E_PI/2.0;
}

bool MiaManualLiverSegmentGenerator::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();
    short* out = (short*)outputImage->getDataPtr();

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
//            QRgb* inr = (QRgb*) inputImage->getDataPtr();
//            runOperation<QRgb>(inr,out);
            qDebug() << "RGB image is not supported yet";
            return false;
        }
            break;
    }


    return result;

}

template <class S>
bool MiaManualLiverSegmentGenerator::runOperation(S* inData, short *outdata)
{
    if(markerList.count() != 4)
    {
        int totalsize = inputImage->getImageSize();
        for(int i = 0; i < totalsize; i++)
        {
            if(inData[i] >= lowerThreshold && inData[i] <= upperThreshold)
            {
                outdata[i] = 1;
            }
            else
            {
                outdata[i] = 0;
            }
        }

        return true;
    }
    int* imsize = inputImage->getDimSize();
    int slicesize = imsize[1]*imsize[0];
    MiaPoint4D clipPlaneNorms[4];
    MiaPoint4D clipPlaneOrigin[4];
    QList<float> angles;

    MiaPoint4D zAxis(0,0,1,0);
    MiaPoint4D yAxis(0,1,0,0);
    MiaAffineTransform rotationAroundZ(zAxis, E_PI/2.0f);
    MiaAffineTransform rotationAroundY(yAxis, E_PI/2.0f);
    for(int i = 0; i < 4; i++)
    {
        MiaPoint4D pt1 = markerList[i]->getKeyPointAt(0);
        MiaPoint4D pt2 = markerList[i]->getKeyPointAt(1);
        pt1 = inputImage->convertPatientCoordinateToVoxel(pt1);
        pt2 = inputImage->convertPatientCoordinateToVoxel(pt2);
        clipPlaneOrigin[i] = pt2;
        MiaPoint4D vector = pt1 - pt2;
        if( i < 3)
        {
            float angle = convertToAngle(vector.pos[0], vector.pos[1]);
            angles.push_back(angle);
            clipPlaneNorms[i] = rotationAroundZ.transform(vector);
        }
        else
        {
            clipPlaneNorms[i] = rotationAroundY.transform(vector);
        }

    }
    qSort(angles.begin(), angles.end());
    qDebug() << angles;
    float lastanlge = (angles.first() + 2.0*E_PI + angles.last())/2.0;
    angles.push_back(lastanlge);

    for(int z = 0; z < imsize[2]; z++)
        for(int y = 0; y < imsize[1]; y++)
            for(int x = 0; x < imsize[0]; x++)
            {
                int ind = z*slicesize + y*imsize[0] + x;
                short output = 0;
                if(inData[ind] >= lowerThreshold && inData[ind] <= upperThreshold)
                {
                    MiaPoint4D pt(x,y,z,0);

                    MiaPoint4D axialvector = pt - clipPlaneOrigin[0];
                    float angle = convertToAngle(axialvector.pos[0], axialvector.pos[1]);
                    if(angle < angles.first())
                        angle += 2.0*E_PI;
                    if(angle > angles.last())
                        output = 2;
                    else
                    {
                        for(int i = 0; i < 4; i++)
                        {
                            if(angle < angles[i])
                            {
                                output = (i+1)*2;
                                break;
                            }
                        }
                    }
                    MiaPoint4D coronalvector = pt - clipPlaneOrigin[3];
                    if(coronalvector*clipPlaneNorms[3] >= 0)
                    {
                            output --;
                    }
                }
                outdata[ind] = output;
            }
    return true;
}

bool MiaManualLiverSegmentGenerator::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        emit getInputMarkers(markerList);

    }
    else
        return false;
    return true;
}

bool MiaManualLiverSegmentGenerator::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    outputImage = creatEmptyImageFrom( inputImage, IM_short);
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;
    return true;
}

float MiaManualLiverSegmentGenerator::convertToAngle(float x, float y)
{
    float angle = atan2(-y, x);
    if(angle < startAngle)
        angle += 2*E_PI;
    return angle;
}
