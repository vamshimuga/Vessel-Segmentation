#include "miacurvedmprtransformalgorithm.h"
#include <stdexcept>

MiaCurvedMPRTransformAlgorithm::MiaCurvedMPRTransformAlgorithm()
{

}
MiaIrregularTransformAlgorithm* MiaCurvedMPRTransformAlgorithm::clone()
{
    MiaCurvedMPRTransformAlgorithm* myclone = new MiaCurvedMPRTransformAlgorithm();
    copyToAnother(myclone);
    return myclone;
}

MiaPoint4D MiaCurvedMPRTransformAlgorithm::toPatientCoordinate(const MiaPoint4D &inpt)
{
    if(needUpdate)
        updateCoefficients();
    MiaPoint4D pt_in_bulkTransform = lookupInPlanePointInBulkTransfomrSpace(inpt);
    return bulkTransform.transform(pt_in_bulkTransform);

}

MiaPoint4D MiaCurvedMPRTransformAlgorithm::fromPatientCoordinate(const MiaPoint4D &inpt)
{
    if(needUpdate)
        updateCoefficients();
    MiaPoint4D pt_in_bulkTransform = bulkTransform.inversedTransform(inpt);
    return lookupNearestInplanePoint(pt_in_bulkTransform);
}
void MiaCurvedMPRTransformAlgorithm::updateCoefficients()
{
    createProjectedCenterlineWithEvenStepLength();
    needUpdate = false;
}

int MiaCurvedMPRTransformAlgorithm::getProjectedLength()
{
    if(needUpdate)
        updateCoefficients();
    return projectedCenterlineWithEvenStepLength.size();
}

void MiaCurvedMPRTransformAlgorithm::createProjectedCenterlineWithEvenStepLength()
{
    projectedCenterlineWithEvenStepLength.clear();
    projectedCenterline.clear();
    yCoordinatesOfProjectedCenterlinePoints.clear();
    for(int i = 0 ; i < keyPoints.size(); i ++)
    {
        MiaPoint4D projectedpt = bulkTransform.inversedTransform(keyPoints.at(i));
        projectedCenterline.push_back(projectedpt);
    }

    projectedCenterlineWithEvenStepLength.append(projectedCenterline.at(0));

    bool lastPointReached = false;
    int nextKeyPointIndex = 1;
    MiaPoint4D currentResamplePt = projectedCenterline.at(0);
    yCoordinatesOfProjectedCenterlinePoints.push_back(0);
    float remainDistance = spacing[1];
    while(!lastPointReached)
    {
        MiaPoint4D nextKeyPoint = projectedCenterline.at(nextKeyPointIndex);
        float dist = distanceIn2DProjectedPlane ( currentResamplePt, nextKeyPoint);
        if(remainDistance > dist)
        {
            currentResamplePt = nextKeyPoint;
            remainDistance -=dist;
            float index_of_next_even_spacing_centerline_point = projectedCenterlineWithEvenStepLength.size();
            yCoordinatesOfProjectedCenterlinePoints.push_back(index_of_next_even_spacing_centerline_point - remainDistance/spacing[1]);
            nextKeyPointIndex++;
            if(nextKeyPointIndex >= projectedCenterline.size())
                lastPointReached = true;
//            continue;
        }
        else
        {
            MiaPoint4D preKeyPoint = projectedCenterline.at(nextKeyPointIndex-1);
            float distToPre = distanceIn2DProjectedPlane (currentResamplePt, preKeyPoint)
                              + remainDistance;

            float segLength = distanceIn2DProjectedPlane (nextKeyPoint, preKeyPoint);
#ifdef QT_DEBUG
            if(segLength == 0)
            {
                qDebug()<<"segment length is zero";
                throw std::runtime_error("segment length is zero");
            }
#endif
            currentResamplePt = preKeyPoint + (nextKeyPoint-preKeyPoint)*(distToPre/segLength);
            projectedCenterlineWithEvenStepLength.append(currentResamplePt);
            remainDistance = spacing[1];
        }
    }

}

MiaPoint4D MiaCurvedMPRTransformAlgorithm::lookupInPlanePointInBulkTransfomrSpace(const MiaPoint4D &inpt) const
{

    MiaPoint4D outpt;
    outpt.pos[0] = inpt.pos[0]*spacing[0]; //bulktranform doesn't consider spacing
    int index_of_corresponding_pt = inpt.pos[1];


    if(index_of_corresponding_pt < 0)
    {
        index_of_corresponding_pt = 0;
    }
    else if(index_of_corresponding_pt >= projectedCenterlineWithEvenStepLength.size() - 1)
    {
        index_of_corresponding_pt = projectedCenterlineWithEvenStepLength.size() - 2;
    }
    // note the offset can be negative or larger than one which allows us to extrapolate points
    float offset = inpt.pos[1] - (float) index_of_corresponding_pt;
    MiaPoint4D interpolatedpt = projectedCenterlineWithEvenStepLength[index_of_corresponding_pt] * (1.0 - offset)
                          +  projectedCenterlineWithEvenStepLength[index_of_corresponding_pt + 1]*offset;


    outpt.pos[1] = interpolatedpt.pos[1];
    outpt.pos[2] = interpolatedpt.pos[2];

    outpt.pos[3] = inpt.pos[3];
    return outpt;

}

MiaPoint4D MiaCurvedMPRTransformAlgorithm::lookupNearestInplanePoint(const MiaPoint4D &inpt) const
{
    MiaPoint4D outpt;
    outpt.pos[0] = inpt.pos[0]/spacing[0]; //bulktranform doesn't consider spacing
    int nearest_index = 0;
    double nearest_dist =inpt.distance(projectedCenterline[0]);
    for(int i = 0; i <projectedCenterline.size(); i++)
    {
        double dist = inpt.distance(projectedCenterline[i]) ;
        if(dist < nearest_dist)
        {
            nearest_dist = dist;
            nearest_index = i;
        }
    }
    //now search nearest neighbor in the curved plan but only around the closest 3d key pt
    nearest_dist = distanceIn2DProjectedPlane(inpt,projectedCenterline[nearest_index]);
    double left_nearest_dist = nearest_dist;
    double left_nearest_index = nearest_index;

    double right_nearest_dist = nearest_dist;
    double right_nearest_index = nearest_index;
    if( nearest_index>0 )
    {
        //search on th left side
        left_nearest_dist = searchShortestDistanceFromIndex(nearest_index, inpt, -1, left_nearest_index);
    }
    if(nearest_index < projectedCenterline.size()-1)
    {
        //search on the right side
        right_nearest_dist = searchShortestDistanceFromIndex(nearest_index, inpt, 1, right_nearest_index);
    }

    if(right_nearest_dist < left_nearest_dist)
    {
        outpt.pos[1] = right_nearest_index;
        outpt.pos[2] = right_nearest_dist;
    }
    else
    {
        outpt.pos[1] = left_nearest_index;
        outpt.pos[2] = left_nearest_dist;
    }
    outpt.pos[3] = inpt.pos[3];

    return outpt;
}

double MiaCurvedMPRTransformAlgorithm::distanceIn2DProjectedPlane(const MiaPoint4D &inpt1, const MiaPoint4D &inpt2)
{
    return sqrt((inpt1.pos[2]-inpt2.pos[2])*(inpt1.pos[2]-inpt2.pos[2]) + (inpt1.pos[1]-inpt2.pos[1])*(inpt1.pos[1]-inpt2.pos[1]));
}

double MiaCurvedMPRTransformAlgorithm::shortestDistanceToLineSegment(const MiaPoint4D &start, const MiaPoint4D &end, const MiaPoint4D &pt, double& offset) const
{
    MiaPoint4D vector1 = end - start;
    MiaPoint4D vector2 = pt - start;
    double segment_length = distanceIn2DProjectedPlane(start,end);
    double ratio = ((vector1.pos[2]*vector2.pos[2] + vector1.pos[1]* vector2.pos[1])/segment_length)/segment_length;

    if(ratio <= 0)
    {
        offset = 0;
        return distanceIn2DProjectedPlane(pt,start);
    }
    else if(ratio >= 1.0)
    {
        offset = 1.0;
        return distanceIn2DProjectedPlane(pt,end);
    }
    else
    {
        offset = ratio;
        MiaPoint4D projectedpt = (end - start)*ratio + start;
        return distanceIn2DProjectedPlane(pt, projectedpt);
    }
}

double MiaCurvedMPRTransformAlgorithm::searchShortestDistanceFromIndex(int ind, const MiaPoint4D &inpt, int direction, double &projected_index) const
{
    int start_index = ind;
    int nearest_index;
    double nearest_dist;
    int total_size = projectedCenterline.size();
    int step = 1;
    if(direction < 0)
        step = -1;
    //search the nearest point on coarse level
    nearest_dist = distanceIn2DProjectedPlane(inpt, projectedCenterline[start_index]);
    nearest_index = start_index;
    for(int i = start_index + step; i >= 0 && i < total_size; i += step)
    {
        double dist = distanceIn2DProjectedPlane(inpt,projectedCenterline[i]);
        if(dist < nearest_dist)
        {
            nearest_dist = dist;
            nearest_index = i;
        }
        else
        {
            break;
        }
    }
    projected_index = (double)yCoordinatesOfProjectedCenterlinePoints[nearest_index];
    // get the shortest distance to left line segment
    if(nearest_index > 0)
    {
        double offset_from_start;
        double dist = shortestDistanceToLineSegment(projectedCenterline[nearest_index],
                                                    projectedCenterline[nearest_index - 1],
                                                    inpt,
                                                    offset_from_start);
        if(dist < nearest_dist)
        {
            nearest_dist = dist;
            projected_index = (double)yCoordinatesOfProjectedCenterlinePoints[nearest_index] +
                              (double)(yCoordinatesOfProjectedCenterlinePoints[nearest_index - 1] -
                                       yCoordinatesOfProjectedCenterlinePoints[nearest_index])
                                       *offset_from_start;
        }

    }
    if(nearest_index < projectedCenterline.size() - 1)
    {
        double offset_from_start;
        double dist = shortestDistanceToLineSegment(projectedCenterline[nearest_index],
                                                    projectedCenterline[nearest_index + 1],
                                                    inpt,
                                                    offset_from_start);
        if(dist < nearest_dist)
        {
            nearest_dist = dist;
            projected_index = (double)yCoordinatesOfProjectedCenterlinePoints[nearest_index] +
                    (double)(yCoordinatesOfProjectedCenterlinePoints[nearest_index + 1] -
                             yCoordinatesOfProjectedCenterlinePoints[nearest_index])
                             *offset_from_start;
        }

    }
    return nearest_dist;

}
