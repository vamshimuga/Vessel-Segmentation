#include "miacentertrackingonarrivalmap.h"
#include "MiaMarkers/miamarker3dpolygon.h"
#include "miaconstants.h"


MiaCenterTrackingOnArrivalMap::MiaCenterTrackingOnArrivalMap(MiaNetwork *parent) :
    MiaModule(parent)
{
        moduleName = "centerline tracking module";
        imDimension = 3;
        stepLength = 0.1;
        direction = -1.0;
        m_MinimumStepLength = 1e-3;
        m_GradientMagnitudeTolerance = 1e-4;
        m_CurrentStepLength   =   0;
        m_RelaxationFactor = 0.5;

}


void MiaCenterTrackingOnArrivalMap::getOutputMarkers(QList <MiaMarker*>& list)
{
    if(needUpdate && autoUpdateWhenOutputRequested)
        update();
    list.clear();
    for(int i = 0; i < centerlinesList.size(); i++)
        list.append( centerlinesList.at(i) );
    return;
}

bool MiaCenterTrackingOnArrivalMap::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        imDimension = inputImage->getDimension();
        numberOfNeighbors = 1<<imDimension;
        centerlinesList.clear();
        QList <MiaMarker*> markerList;
        emit getInputMarkers(markerList);
        if(markerList.size()<1)
            return false;
        centerlinesList.append(markerList);
//        for( i = 0; i < markerList.size(); i++ )
//        {
//            Point4D pt = markerList.at(i)->getKeyPointAt(0);
//            MiaMarker* amarker = new MiaMarker3DPolygon(pt, this);
//            pt = markerList.at(i)->getKeyPointAt(markerList.at(i)->getKeyPointNumber()-1);
//            amarker->mouseButtonPressedAT(pt);
//            centerlinesList.append(amarker);
//        }
    }
    else
        return false;
    return true;
}

bool MiaCenterTrackingOnArrivalMap::runOperation()
{
    inData = (float*)(inputImage->getDataPtr());
    int* size = inputImage->getDimSize();
    float* spacing = inputImage->getSpacing();
    for(int i = 0 ;  i < imDimension ; i++)
    {
        inputSize[i] = size[i];
        inputSpacing[i] = spacing[i];
    }
    QList <MiaPoint4D> centerPoints;
    for( int i = 0; i < centerlinesList.size(); i++ )
    {
        if(centerlinesList.at(i)->keyPointList.size() < 2)
            continue;
//        qDebug()<<"Start branch" << i;
        centerPoints.clear();
        int currentPos = centerlinesList.at(i)->keyPointList.size() - 1;
        MiaPoint4D pt1 = centerlinesList.at(i)->keyPointList.first();
        MiaPoint4D pt2 = centerlinesList.at(i)->keyPointList.last();
        pt1 = inputImage->convertPatientCoordinateToVoxel(pt1);
        pt2 = inputImage->convertPatientCoordinateToVoxel(pt2);
        QList <MiaPoint4D> temppts;
        temppts.clear();
        traceCenterPoints(pt1,pt2,temppts);
        while(currentCondition != MIA_StoppedAtSeed)
        {
            if(temppts.size()==0)
            {
                currentPos--;
                centerPoints.insert(0,pt2);
            }
            else
            {
                temppts.append(centerPoints);
                centerPoints.clear();
                centerPoints.append(temppts);
                temppts.clear();
                MiaPoint4D newpt = centerPoints.first();
                newpt = inputImage->convertVoxelToPatientCoordinate(newpt);
                int closest = 0;
                float dist= newpt.distance(pt1);
                for(int s = 0; s < currentPos; s++)
                {
                    if(newpt.distance(centerlinesList.at(i)->keyPointList.at(s)) < dist)
                    {
                        closest = s;
                        dist = newpt.distance(centerlinesList.at(i)->keyPointList.at(s));
                    }
                }
                currentPos = closest;
            }

            pt2 = centerlinesList.at(i)->keyPointList.at(currentPos);
            pt2 = inputImage->convertPatientCoordinateToVoxel(pt2);
            traceCenterPoints(pt1,pt2,temppts);

        }
        temppts.append(centerPoints);
        centerPoints.clear();
        centerPoints.append(temppts);

        centerlinesList.at(i)->keyPointList.clear();
        for( int j = 0; j < centerPoints.size(); j++ )
        {
            MiaPoint4D pt = inputImage->convertVoxelToPatientCoordinate(centerPoints.at(j));
            centerPoints[j] = pt;
        }
//        qDebug()<<"finished centerline tracing"<<centerPoints.count();
        centerlinesList.at(i)->setKeyPoints(centerPoints);

    }
    return true;

}

void MiaCenterTrackingOnArrivalMap::traceCenterPoints(MiaPoint4D start,MiaPoint4D end, QList <MiaPoint4D> &centerpts)
{
    currentCondition = MIA_Unfinished;
    m_CurrentStepLength = stepLength;
    MiaPoint4D currentPoint = end;
    MiaPoint4D currentGradient;
    MiaPoint4D previousGradient = getGradientAt(currentPoint);;
    while(currentCondition == MIA_Unfinished)
    {
        if(start.distance(currentPoint) < 1.5)
        {
            currentCondition = MIA_StoppedAtSeed;
//            qDebug()<<"stopped at seed point";
            break;
        }
        float transformedGradient[3];
        float previousTransformedGradient[3];
//        float scales[3];
        currentGradient = getGradientAt(currentPoint);
//        float limit = sqrt(max_float());
//        for(unsigned int i = 0;  i < imDimension; i++)
//        {
//            if(currentGradient.pos[i]>limit || currentGradient.pos[i] < - limit)
//            {
//                for(unsigned int j = 0;  j < imDimension; j++)
//                {
//                    currentGradient.pos[j] = currentGradient.pos[j]/currentGradient.pos[i];
//                }
//            }
//        }

        float magnitudeSquare = 0;
        float scalarProduct = 0;
        for(unsigned int i = 0;  i < imDimension; i++)
        {
            transformedGradient[i]  = currentGradient.pos[i];
            previousTransformedGradient[i] = previousGradient.pos[i];
            magnitudeSquare += transformedGradient[i] * transformedGradient[i];
            scalarProduct += transformedGradient[i] * previousTransformedGradient[i];
        }

        const float gradientMagnitude = sqrt( magnitudeSquare );

        if( gradientMagnitude < m_GradientMagnitudeTolerance )
        {
          currentCondition = MIA_StoppedAtLocalMINorMAX;
          qDebug()<<"Gradient too small";
          break;
        }

        // If there is a direction change
        if( scalarProduct < 0 )
        {
          m_CurrentStepLength *= m_RelaxationFactor;
        }

        if( m_CurrentStepLength < m_MinimumStepLength )
        {
            currentCondition = MIA_StoppedAtLocalMINorMAX;
            qDebug()<<"Step lenght too small";
            break;
        }

        const float factor = direction * m_CurrentStepLength / gradientMagnitude;
        for(unsigned int i = 0;  i < imDimension; i++)
        {
            currentPoint.pos[i] += factor*currentGradient.pos[i];
        }
        centerpts.insert(0,currentPoint);
        previousGradient = currentGradient;
        if(centerpts.size()>29980)
        {
            qDebug()<<currentGradient.pos[0] << currentGradient.pos[1] << currentGradient.pos[0];
        }
        if(centerpts.size()>30000)
        {
            qDebug()<<"too long centerline "<<m_CurrentStepLength;
            break;
        }

    }
    if(currentCondition == MIA_StoppedAtSeed)
    {
        MiaPoint4D pt;
        for(int i = 1; i <= 10; i++)
        {
            for(int dim = 0 ; dim < 3 ; dim ++)
            {
                pt.pos[dim] = currentPoint.pos[dim] + (float)i * 0.1 * (start.pos[dim] - currentPoint.pos[dim]);
            }
            centerpts.insert(0,pt);
        }
    }
    qDebug()<<centerpts.size();

}

float MiaCenterTrackingOnArrivalMap::getValueAt(MiaPoint4D voxelCoordinate)
{
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
    float value = 0.0f;
    float totalOverlap = 0.0f;


    for( unsigned int counter = 0; counter < numberOfNeighbors; counter++ )
    {

      float overlap = 1.0;          // fraction overlap
      unsigned int upper = counter;  // each bit indicates upper/lower neighbour
      int neighIndex[3];

      // get neighbor index and overlap fraction
      for( dim = 0; dim < imDimension; dim++ )
      {

        if ( upper & 1 )
        {
          neighIndex[dim] = baseIndex[dim] + 1;
          overlap *= distance[dim];
        }
        else
        {
          neighIndex[dim] = baseIndex[dim];
          overlap *= 1.0 - distance[dim];
        }

        upper >>= 1;

      }

      // get neighbor value only if overlap is not zero
      if( overlap )
      {
          bool isNeighIndexValid = true;
          for(int j = 0; j < imDimension; j++)
              if(neighIndex[j]<0 || neighIndex[j]>= inputSize[j])
                  isNeighIndexValid = false;
          if(isNeighIndexValid)
            value += overlap * inData[ neighIndex[2] * inputSize[0] * inputSize[1] + neighIndex[1] * inputSize[0] + neighIndex[0]];
          //else
          // value += 0;
          totalOverlap += overlap;
      }

      if( totalOverlap == 1.0 )
      {
          // finished
           break;
      }

    }

    return value;
}

MiaPoint4D MiaCenterTrackingOnArrivalMap::getGradientAt(MiaPoint4D voxelCoordinate)
{
    MiaPoint4D derivative;
    bool ifReachBorder = false;

    for(unsigned int dim=0; dim<imDimension; dim++)
    {
      // Get the left neighbor
      MiaPoint4D pointLeft;
      pointLeft = voxelCoordinate;
      pointLeft.pos[dim] += -1.0;
      float valueLeft = getValueAt( pointLeft );

      // Get the right neighbor
      MiaPoint4D pointRight;
      pointRight = voxelCoordinate;
      pointRight.pos[dim] += 1.0;
      float valueRight = getValueAt( pointRight );
      float limit = sqrt(MAX_FLOAT);
      if(valueLeft>limit || valueLeft < -limit || valueRight>limit || valueRight < -limit)
      {
          ifReachBorder = true;
          break;
      }

      // Compute derivative
      derivative.pos[dim] = (valueRight - valueLeft) *
                        (0.5 / inputSpacing[dim]);
    }
    if(ifReachBorder)
    {
        derivative.pos[0] = 0;
        derivative.pos[1] = 0;
        derivative.pos[2] = 0;
    }

    return derivative;
}

