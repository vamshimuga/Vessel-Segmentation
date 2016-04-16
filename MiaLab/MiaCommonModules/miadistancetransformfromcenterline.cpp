#include "miadistancetransformfromcenterline.h"
#include "miaconstants.h"

MiaDistanceTransformFromCenterline::MiaDistanceTransformFromCenterline(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Distance Transform based on centerlines";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    workingMode = MIA_DistanceTransformOnly;

}

bool MiaDistanceTransformFromCenterline::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        imDimension = inputImage->getDimension();
        numberOfNeighbors = 1<<imDimension;
        emit getInputMarkers(markerList);
        int i;
        if(markerList.size()<1)
            return false;
        for( i = 0; i < markerList.size(); i++ )
        {
            if ( markerList.at(i)->type != MIA_Polygon && markerList.at(i)->type != MIA_Spline)
            {
                markerList.removeAt(i);
                i--;
            }
        }
    }
    else
        return false;
    return true;
}

bool MiaDistanceTransformFromCenterline::runOperation()
{
    reset();
    QList <MiaPoint4D> centerPoints;
    QList <float> centerDistances;
    for( int i = 0; i < markerList.size(); i++ )
    {
        centerPoints.clear();
        centerDistances.clear();
//        if(markerList.at(i)->keyPointList.size()!=markerList.at(i)->keyValueList.size())
//        {
//            qDebug()<< "centerline radius is not defined on every key points";
//            return false;
//        }
        for( int j = 0; j < markerList.at(i)->keyPointList.size(); j++ )
        {
            MiaPoint4D pt = inputImage->convertPatientCoordinateToVoxel(markerList.at(i)->keyPointList.at(j));
            centerPoints.append(pt);
//            centerDistances[j] = markerList.at(i)->keyValueList.at(j).pos[3];
            float radius = markerList.at(i)->keyValueList.at(j).pos[3];
            centerDistances.append( radius );
        }
        if(!distanceTransformNear(centerPoints,centerDistances))
            return false;
        QMap<int, int>::const_iterator iter = closestNeighborLUT.constBegin();
        switch(workingMode)
        {
        case MIA_DistanceTransformOnly:
            break;
        case MIA_ReadAllKeyValues:
            outDataRGB = (QRgb*)outData;
            while (iter != closestNeighborLUT.constEnd()) {
                int v[4];
                for(int dim = 0; dim < 4; dim++)
                {
                    v[dim] = 128 + (int)(127.0 * markerList.at(i)->keyValueList[iter.value()].pos[dim]);
                    if(v[dim]<0)
                        v[dim] = 0;
                    else if( v[dim] > 255)
                        v[dim] = 255;
                }
                outDataRGB[iter.key()]  = qRgba(v[0],v[1],v[2],v[3]);
                ++iter;
            }
            break;
        case MIA_ReadKeyValue_Pos3:
            while (iter != closestNeighborLUT.constEnd()) {
                outData[iter.key()]  = markerList.at(i)->keyValueList[iter.value()].pos[3];
                ++iter;
            }
            break;
        }
    }

    return true;
}
bool MiaDistanceTransformFromCenterline::distanceTransformNear(QList <MiaPoint4D> &centerpts,QList <float> &initdis)
{
    closestNeighborLUT.clear();
    for( int i = 0; i < centerpts.size(); i++ )
    {
        MiaPoint4D voxelCoordinate = centerpts[i];
        float initialDistance = initdis[i];
        unsigned int dim;  // index over dimension

        /**
         * Compute base index = closet index below point
         * Compute distance from point to base index
         */
        int baseIndex[3];
        float distance[3];
        int tIndex;

        for( dim = 0; dim < imDimension; dim++ )
        {
            // The following "if" block is equivalent to the following line without
            // having to call floor.
            //    baseIndex[dim] = (long) vcl_floor(index[dim] );
            if (voxelCoordinate.pos[dim] >= 0.0)
            {
                baseIndex[dim] = (int) voxelCoordinate.pos[dim];
            }
            else
            {
                tIndex = (int) voxelCoordinate.pos[dim];
                if (float(tIndex) != voxelCoordinate.pos[dim])
                {
                  tIndex--;
                }
                baseIndex[dim] = tIndex;
            }
            distance[dim] = voxelCoordinate.pos[dim] - float( baseIndex[dim] );
        }

        /**
         * Interpolated value is the weighted sum of each of the surrounding
         * neighbors. The weight for each neighbor is the fraction overlap
         * of the neighbor pixel with respect to a pixel centered on point.
         */


        for( unsigned int counter = 0; counter < numberOfNeighbors; counter++ )
        {
          unsigned int upper = counter;  // each bit indicates upper/lower neighbour
          int neighIndex[3];
          float distSqr = 0;
          // get neighbor index and overlap fraction
          for( dim = 0; dim < imDimension; dim++ )
          {

            if ( upper & 1 )
            {
              neighIndex[dim] = baseIndex[dim] + 1;
              distSqr += (1.0 - distance[dim]) * (1.0 - distance[dim]) * spacing[dim] * spacing[dim];
            }
            else
            {
              neighIndex[dim] = baseIndex[dim];
              distSqr += distance[dim] * distance[dim] * spacing[dim] * spacing[dim];
            }

            upper >>= 1;

          }

          bool isNeighIndexValid = true;
          for(int j = 0; j < imDimension; j++)
              if(neighIndex[j]<0 || neighIndex[j]>= size[j])
                  isNeighIndexValid = false;
          if(isNeighIndexValid)
          {
              int ind = neighIndex[2] * size[0] * size[1] + neighIndex[1] * size[0] + neighIndex[0];
              float newdist = initialDistance + sqrt(distSqr);
              if(outData[ind] > newdist)
              {
                  outData[ind] = newdist;
                  if(workingMode != MIA_DistanceTransformOnly)
                    closestNeighborLUT[ind] = i;
              }
          }

        }

    }

    return true;
}

bool MiaDistanceTransformFromCenterline::reAllocOutputImage()
{
    if(outputImage)
        outputImage->release();
    if(workingMode == MIA_ReadAllKeyValues)
        outputImage = creatEmptyImageFrom(inputImage, IM_RGBA);
    else
        outputImage = creatEmptyImageFrom(inputImage, IM_float);

    if(outputImage)
    {
        outputImage->retain();
        float* sp = outputImage->getSpacing();
        int* si = outputImage->getDimSize();
        float minSpacing = MAX_FLOAT;
        for(int i = 0; i<3; i++)
        {
            if(sp[i] < minSpacing)
                minSpacing = sp[i];
        }
        for(int i = 0; i<3; i++)
        {
            spacing[i] = sp[i]/minSpacing;
            size[i] = si[i];
        }
        outData = (float*) outputImage->getDataPtr();
        return true;
    }
    else
        return false;
}

void MiaDistanceTransformFromCenterline::reset()
{
    if(outputImage == NULL)
        return;
    float* distmap = (float*) outputImage->getDataPtr();
    int totalSize = outputImage->getImageSize();
    int i;
    #pragma omp parallel for
    for( i = 0 ; i < totalSize;  i++ )
    {
        distmap [i] = MAX_FLOAT;
    }
}
