#include "miamarker3dpolygon.h"

MiaMarker3DPolygon::MiaMarker3DPolygon(MiaPoint4D pt, QObject *parent) :
    MiaMarker(pt, parent)
{
    type = MIA_Polygon;
    name = "3DPolygon";
    // keyPointList.at(0) is set in base class, we need add the second point to caculate the radius
    needCrossHairFolowTheFocus = true;
    length = 0;
    visibleDistance = controlNodeSize;
    currentNode = 0;
    minimumDistanceBetweenNodes = 0.25;
    brushColor = QColor(Qt::transparent);
    preActiveColor = QColor(Qt::transparent);
    mouseButtonPressed = false;
    isClosed = false;
    cursor = Qt::CrossCursor;
}
void MiaMarker3DPolygon::mouseButtonPressedAT( MiaPoint4D pt )
{
    mouseButtonPressed = true;
    if(currentMode == MIA_Initializing&&pt.distance(keyPointList.last())>minimumDistanceBetweenNodes)
    {
        currentNode++;
        keyPointList.push_back(pt);
    }
    qDebug() << "ploygon pressed at" << keyPointList.size();

}

bool MiaMarker3DPolygon::handleMouseButtonPressedIn2DPrjectionView(const MiaPoint4D pt, const MiaTransform &transform)
{
    int insert_index = findCloseProjectedPointFromPointList(pt,transform,keyPointList);
    if(insert_index<0 || insert_index >= keyPointList.size())
        return false;
    keyPointList.insert(insert_index+1,pt);
    emit markerUpdated( this );
    return true;

}

bool MiaMarker3DPolygon::crossHairNeedMoveAfterClickAt( MiaPoint4D& pt, MiaTransform& transform ) const
{
    pt = keyPointList.at(0);
    return true;
}

void MiaMarker3DPolygon::mouseMovedAT( MiaPoint4D pt )
{
    if(currentMode == MIA_Initializing && mouseButtonPressed )
    {
        keyPointList[currentNode] = pt;

    }
    needCrossHairFolowTheFocus = false;
    emit markerUpdated( this );
}

void MiaMarker3DPolygon::mouseButtonReleasedAT( MiaPoint4D pt )
{
    if(currentMode == MIA_Initializing && mouseButtonPressed )
    {
        keyPointList[currentNode] = pt;

    }
    mouseButtonPressed = false;
    emit markerUpdated( this );

}
void MiaMarker3DPolygon::mouseDoubleClickedAt( MiaPoint4D pt)
{
    mouseButtonPressed = false;
    if(markerCreatingFinished())
    {
        currentMode = MIA_Selected;
        emit markerUpdated( this );
        qDebug() <<"created" << length;
    }
}

bool MiaMarker3DPolygon::isMarkerValid()
{
    if(keyPointList.size()<2)
        return false;
    length = keyPointList.last().distance(keyPointList.at(0));
    if(length > 0.0)
        return true;
    else
    {
        qDebug() <<"Invalid" << length;
        return false;
    }
}

MiaPoint4D MiaMarker3DPolygon::creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance)
{

    MiaPoint4D pt0;
    if(keyPointList.size()>0)
        pt0= matrix.fromPatientCoordinate(keyPointList.at(0));
    else
        return pt0;

    length = 0;
    ControlNode node0;
    node0.projectedPosition.pos[0] = 0;
    node0.projectedPosition.pos[1] = 0;
    node0.index = 0;//index to key Point List

    controlnodes.append(node0);
    path.moveTo(0,0);

    for(int i = 1; i < keyPointList.size(); i++ )
    {
        MiaPoint4D pti = matrix.fromPatientCoordinate(keyPointList.at(i));

        path.lineTo(pti.pos[0]-pt0.pos[0],pti.pos[1]-pt0.pos[1]);

        ControlNode node1;
        node1.projectedPosition.pos[0] = pti.pos[0]-pt0.pos[0];
        node1.projectedPosition.pos[1] = pti.pos[1]-pt0.pos[1];
        node1.index = i;
        controlnodes.append(node1);

    }
    QPainterPathStroker stroker;
    shape = stroker.createStroke(path);
    return pt0;
}
MiaPoint4D MiaMarker3DPolygon::getFocus()
{
    return keyPointList.last();
}
bool MiaMarker3DPolygon::resampleKeyPointsWithStepLength(float s)
{
    if(!isMarkerValid()||s <= 0.0)
        return false;
    if(resamplePolygonAndValueWithStepLength(keyPointList,&keyValueList,s))
    {
        emit markerUpdated( this );
        return true;
    }
    else
    {
        return false;
    }
}

float MiaMarker3DPolygon::distanceFromPointToLineSegment(const MiaPoint4D& pt, const MiaPoint4D& line_start, const MiaPoint4D& line_end, double& offset) const
{
    MiaPoint4D vector1 = line_end - line_start;
    MiaPoint4D vector2 = pt - line_start;
    double segment_length = line_end.distance(line_start);
    double ratio = ((vector1*vector2)/segment_length)/segment_length;

    if(ratio <= 0)
    {
        offset = 0;
        return pt.distance(line_start);
    }
    else if(ratio >= 1.0)
    {
        offset = 1.0;
        return pt.distance(line_end);
    }
    else
    {
        offset = ratio;
        MiaPoint4D projectedpt = (line_end - line_start)*ratio + line_start;
        return pt.distance(projectedpt);
    }
    return 0.0f;
}

bool MiaMarker3DPolygon::resamplePolygonAndValueWithStepLength(QList<MiaPoint4D>& polygon, QList<MiaPoint4D>* values, double stepLength) const
{

    QList <MiaPoint4D> newPointList;
    QList <MiaPoint4D> newValueList;
    newPointList.append(polygon.at(0));
    if(values != NULL)
        newValueList.append(values->at(0));
    bool lastPointReached = false;
    int nextKeyPointIndex = 1;
    MiaPoint4D currentResamplePt = polygon.at(0);
    float remainDistance = stepLength;
    while(!lastPointReached)
    {
        MiaPoint4D nextKeyPoint = polygon.at(nextKeyPointIndex);
        float dist = currentResamplePt.distance(nextKeyPoint);
        if(remainDistance > dist)
        {
            currentResamplePt = nextKeyPoint;
            remainDistance -=dist;
            nextKeyPointIndex++;
            if(nextKeyPointIndex >= polygon.size())
                lastPointReached = true;
//            continue;
        }
        else
        {
            MiaPoint4D preKeyPoint = polygon.at(nextKeyPointIndex-1);
            float distToPre = currentResamplePt.distance(preKeyPoint) + remainDistance;
            float segLength = preKeyPoint.distance(nextKeyPoint);
#ifdef QT_DEBUG
            if(segLength == 0)
            {
                throw std::runtime_error("Segment length cannot be zero");
            }
#endif
            currentResamplePt = preKeyPoint + (nextKeyPoint-preKeyPoint)*(distToPre/segLength);
            newPointList.append(currentResamplePt);
            if(values != NULL)
            {
                MiaPoint4D preKeyValue = values->at(nextKeyPointIndex-1);
                MiaPoint4D nextKeyValue = values->at(nextKeyPointIndex);
                MiaPoint4D currentResampleValue;
                currentResampleValue = preKeyValue + (nextKeyValue-preKeyValue)*(distToPre/segLength);
                newValueList.append(currentResampleValue);
            }
            remainDistance = stepLength;
        }
    }
    if(newPointList.size()<2)
        return false;

    polygon.clear();

    polygon.append(newPointList);
    if(values != NULL)
    {
        values->clear();
        values->append(newValueList);
    }

    return true;
}

void MiaMarker3DPolygon::setDirectionVectorAsKeyValues(MiaImageND* im)
{
    if(keyPointList.size()<2)
        return;
    keyValueList.clear();
    keyValueList.append(keyPointList);
    keyValueList[0] = im->convertPatientCoordinateToVoxel(keyPointList[1]) - im->convertPatientCoordinateToVoxel(keyPointList[0]);
//    keyValueList[0] = im->convertPatientCoordinateToVoxel(keyValueList[0]);
    keyValueList[0].normalize();
    for(int i = 1; i<keyValueList.size(); i++ )
    {
        keyValueList[i] = im->convertPatientCoordinateToVoxel(keyPointList[i])- im->convertPatientCoordinateToVoxel(keyPointList[i-1]);
//        keyValueList[i] = im->convertPatientCoordinateToVoxel(keyValueList[i]);
        keyValueList[i].normalize();
    }
}

int MiaMarker3DPolygon::findCloseProjectedPointFromPointList(const MiaPoint4D &pt, const MiaTransform &transform, const QList<MiaPoint4D> &list) const
{
    if(list.size() == 0)
        return -1;
    MiaPoint4D projected_pt = transform.fromPatientCoordinate(pt);
    projected_pt.pos[2] = 0;
    projected_pt.pos[3] = 0;
    MiaPoint4D line_start = transform.fromPatientCoordinate(list[0]);
    line_start.pos[2] = 0;
    line_start.pos[3] = 0;

    int closest_index = 0;
    float closest_dist = projected_pt.distance(line_start);
    int number_line_segment = list.size();
    int number_points = list.size();
    if(isClosed)
        number_line_segment++;
    for(int i = 1; i < number_line_segment; i++)
    {
        MiaPoint4D projected_next_pt = transform.fromPatientCoordinate(list[i%number_points]);
        projected_next_pt.pos[2] = 0;
        projected_next_pt.pos[3] = 0;
        if(line_start == projected_pt)
            continue;
        double offset;
        float dist = distanceFromPointToLineSegment(projected_pt,line_start,projected_next_pt,offset);
        if(dist < closest_dist)
        {
            closest_dist = dist;
            closest_index = i - 1;
        }
        line_start = projected_next_pt;
    }
    return closest_index;

}
