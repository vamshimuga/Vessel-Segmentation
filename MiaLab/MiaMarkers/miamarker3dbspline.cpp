#include "miamarker3dbspline.h"
#include "transform/miapoint4d.h"
#include "vtkCardinalSpline.h"
#include "miaconstants.h"

MiaMarker3DBSpline::MiaMarker3DBSpline(MiaPoint4D pt, QObject *parent) :
    MiaMarker3DPolygon(pt, parent)
{
    type = MIA_Spline;
    name = "3DSpline";
    renderResolution = 0.25; //0.1mm
    needToRecreatRenderPoints = true;
    splineX = vtkCardinalSpline::New();
    splineY = vtkCardinalSpline::New();
    splineZ = vtkCardinalSpline::New();
    currentFocusPointDistanceToStart = -1.0f;
}
MiaMarker3DBSpline::~MiaMarker3DBSpline()
{
    splineX->Delete();
    splineY->Delete();
    splineZ->Delete();
}

MiaPoint4D MiaMarker3DBSpline::getFocus()
{
    if(currentFocusPointDistanceToStart < 0.0f && keyPointList.size()>0)
        return keyPointList.first();
    return currentFocusPoint;
}

void MiaMarker3DBSpline::mouseButtonPressedAT( MiaPoint4D pt )
{
//    moveFocurPointToLastPoint();
    needToRecreatRenderPoints = true;
    MiaMarker3DPolygon::mouseButtonPressedAT(pt);
}

bool MiaMarker3DBSpline::handleMouseButtonPressedIn2DPrjectionView(const MiaPoint4D pt, const MiaTransform &transform)
{
    int insert_index = findCloseProjectedPointFromPointList(pt,transform,renderPoints);
    if(insert_index<0 || insert_index >= renderPoints.size())
        return false;
    insert_index = (int)(renderPointsValues[insert_index].pos[3]);
    keyPointList.insert(insert_index+1,pt);
    needToRecreatRenderPoints = true;
    emit markerUpdated( this );
    return true;

}

void MiaMarker3DBSpline::mouseMovedAT( MiaPoint4D pt )
{
//    moveFocurPointToLastPoint();
    needToRecreatRenderPoints = true;
    MiaMarker3DPolygon::mouseMovedAT(pt);
}

void MiaMarker3DBSpline::mouseButtonReleasedAT( MiaPoint4D pt )
{
//    moveFocurPointToLastPoint();
    needToRecreatRenderPoints = true;
    MiaMarker3DPolygon::mouseButtonReleasedAT(pt);
}
void MiaMarker3DBSpline::mouseDoubleClickedAt( MiaPoint4D pt)
{
//    moveFocurPointToLastPoint();
    needToRecreatRenderPoints = true;
    MiaMarker3DPolygon::mouseDoubleClickedAt(pt);
}

void MiaMarker3DBSpline::keyPointMoved(int index, MiaPoint4D pos)
{
    needToRecreatRenderPoints = true;
    MiaMarker::keyPointMoved(index,pos);
}

void MiaMarker3DBSpline::transformKeyPoints(MiaAffineTransform &matrix)
{
    needToRecreatRenderPoints = true;
    MiaMarker::transformKeyPoints(matrix);
}



//Point4D MiaMarker3DBSpline::evaluateUniformIndexBSplineValueAt( int numpt, QList <Point4D>& keypts, float indexf )
//{
//  float bval;
//  float left_indexf;
//  float right_indexf;
//  float u;
//  Point4D interpolated_pt;
////
////  Find the nearest interval [ left_indexf, right_indexf ] to indexf.
////
//  if(numpt < 2)
//  {
//      throw std::runtime_error("too few points");
//  }
//  else if (numpt == 2)
//  {
//      interpolated_pt = keypts[0]*(1.0f-indexf) + keypts[1]*indexf;
//  }

//  if(indexf <= 0)
//  {
//      left_indexf = 1;
//  }
//  else if(indexf >= numpt-1)
//  {
//      left_indexf = numpt-1;
//  }
//  else
//  {
//      left_indexf = ceil(indexf);
//  }
//  right_indexf = left_indexf + 1.0f;
////
////  Evaluate the 5 nonzero B spline basis functions in the interval,
////  weighted by their corresponding data values.
////
//  u = indexf - (left_indexf-1.0f);
////
////  B function associated with node left_indexf - 1, (or "phantom node"),
////  evaluated in its 4th interval.
////
//  bval = ( ( ( -1.0* u + 3.0 )* u - 3.0 ) * u + 1.0 ) / 6.0;

//  if ( 0 < left_indexf-1.0f )
//  {
//    interpolated_pt = interpolated_pt + (keypts[(int)left_indexf-2] * bval);
//  }
//  else
//  {
//    interpolated_pt = interpolated_pt + ((keypts[0]*2.0 - keypts[1] ) * bval);
//  }
////
////  B function associated with node left_indexf,
////  evaluated in its third interval.
////
//  bval = ( ( ( 3.0 * u - 6.0 ) * u + 0.0 ) * u + 4.0 ) / 6.0;

//  interpolated_pt = interpolated_pt + (keypts[(int)left_indexf-1] * bval);
////
////  B function associated with node right_indexf,
////  evaluated in its second interval.
////
//  bval = ( ( ( -3.0 * u + 3.0 ) * u + 3.0 ) * u + 1.0 ) / 6.0;

//  interpolated_pt = interpolated_pt + (keypts[(int)right_indexf-1] * bval);
////
////  B function associated with node right_indexf+1, (or "phantom node"),
////  evaluated in its first interval.
////
//  bval = pow ( u, 3 ) / 6.0;

//  if ( (int)right_indexf+1 <= numpt )
//  {
//    interpolated_pt = interpolated_pt + (keypts[(int)right_indexf] * bval);
//  }
//  else
//  {
//    interpolated_pt = interpolated_pt + ((keypts[numpt-1]*2.0 - keypts[numpt-2] ) * bval);
//  }

//  return interpolated_pt;
//}
void MiaMarker3DBSpline::recreatRenderPointsWhenNecessary()
{
    if(!needToRecreatRenderPoints)
        return;
    renderPoints.clear();
    renderPointsValues.clear();
    if(keyPointList.size()<2)
        return;
    int number_key_points = keyPointList.size();
    double total_length = 0.0;
    for(int i = 0; i < number_key_points; i++)
    {
        if(i > 0)
        {
            total_length += keyPointList[i].distance(keyPointList[i-1]);
        }
        splineX->AddPoint(i,keyPointList[i].pos[0]);
        splineY->AddPoint(i,keyPointList[i].pos[1]);
        splineZ->AddPoint(i,keyPointList[i].pos[2]);
    }
    if(isClosed)
    {
        total_length += keyPointList.last().distance(keyPointList.first());
    }
    int number_render_points = total_length/renderResolution;

    float tstep = (number_key_points-1.0f)/(number_render_points-1.0f);
    if(isClosed)
        tstep = number_key_points/(number_render_points-1.0f);
    for(int i = 0; i < number_render_points; i++)
    {
        float tval = tstep*(float)i;
        float x = splineX->Evaluate(tval);
        float y = splineY->Evaluate(tval);
        float z = splineZ->Evaluate(tval);
        MiaPoint4D pt;
        pt.pos[0] = x; pt.pos[1] = y; pt.pos[2] = z;
        renderPoints.push_back(pt);
        MiaPoint4D ptvalue;
        ptvalue.pos[3] = tval;
        renderPointsValues.push_back(ptvalue);
    }

    resamplePolygonAndValueWithStepLength(renderPoints,&renderPointsValues,renderResolution);
    needToRecreatRenderPoints = false;
}

float MiaMarker3DBSpline::getPointAtDistance(float dist, MiaPoint4D &pos, MiaPoint4D &tan)
{
    recreatRenderPointsWhenNecessary();
    if(renderPoints.size() == 0)
        return -1.0f;
    else if(renderPoints.size() == 1)
    {
        pos = renderPoints[0];
        tan.pos[0] = tan.pos[1] = 0.0f;
        tan.pos[2] = 1.0f;
        return 0.0f;
    }
    else if(dist < 0)
        dist = 0;
    else if(dist > (renderPoints.size()-1)*renderResolution)
        dist = (renderPoints.size()-1)*renderResolution;
    MiaPoint4D pt1,pt2;
    int pt1_index = dist/renderResolution;
    if(pt1_index == renderPoints.size() - 1)
        pt1_index--;
    pt1 = renderPoints[pt1_index];
    pt2 = renderPoints[pt1_index+1];

    float fraction = dist/renderResolution - pt1_index;
    pos = pt1 + (pt2-pt1)*fraction;
    tan = pt2 - pt1;
    tan.normalize();
    return dist;
}

MiaPoint4D MiaMarker3DBSpline::creatShapePath(QPainterPath& path,QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance)
{
    recreatRenderPointsWhenNecessary();
    MiaPoint4D pt0;
    if(keyPointList.size() == 0)
        return pt0;
    pt0 = matrix.fromPatientCoordinate(keyPointList.at(0));
    length = 0;
    path.moveTo(0,0);

    for(int i = 0; i < keyPointList.size(); i++ )
    {
        MiaPoint4D pti = matrix.fromPatientCoordinate(keyPointList.at(i));
        if(pti.pos[2] <= clipdistance & pti.pos[2] >= -clipdistance)
        {
            ControlNode node1;
            node1.projectedPosition.pos[0] = pti.pos[0]-pt0.pos[0];
            node1.projectedPosition.pos[1] = pti.pos[1]-pt0.pos[1];
            node1.index = i;
            controlnodes.append(node1);
        }

    }

    if(renderPoints.size()==0)
        return pt0;

    for(int i = 1; i < renderPoints.size(); i++ )
    {
        MiaPoint4D pti = matrix.fromPatientCoordinate(renderPoints.at(i));

        path.lineTo(pti.pos[0]-pt0.pos[0],pti.pos[1]-pt0.pos[1]);

    }

    MiaPoint4D lastpt = pt0;
    int no_of_line_segments = renderPoints.size()-1;
    int no_of_render_points = renderPoints.size();
    if(isClosed)
        no_of_line_segments++;
    if(clipdistance <= 0.0f)
        clipdistance = visibleDistance;
    for(int i = 1; i <= no_of_line_segments; i++ )
    {
        MiaPoint4D pti = matrix.fromPatientCoordinate(renderPoints.at(i%no_of_render_points));
        MiaPoint4D currentpt = pti;
        MiaPoint4D seg_start, seg_end;

        if((lastpt.pos[2] > clipdistance && currentpt.pos[2] > clipdistance) || (lastpt.pos[2] < -clipdistance && currentpt.pos[2] < -clipdistance))
        {

        }
        else
        {
            //set the point to increasing order
            if(lastpt.pos[2] > currentpt.pos[2])
            {
                MiaPoint4D temppt = currentpt;
                currentpt = lastpt;
                lastpt = temppt;
            }
            if(lastpt.pos[2] < -clipdistance)
            {
                float fraction = (-clipdistance - lastpt.pos[2])/(currentpt.pos[2] - lastpt.pos[2]);
                seg_start = (currentpt - lastpt)*fraction + lastpt;
            }
            else
            {
                seg_start = lastpt;
            }

            if(currentpt.pos[2] < clipdistance)
            {
                 seg_end = currentpt;
            }
            else
            {
                float fraction = (clipdistance - lastpt.pos[2])/(currentpt.pos[2] - lastpt.pos[2]);
                seg_end = (currentpt - lastpt)*fraction + lastpt;
            }
            highlights.moveTo(seg_start.pos[0]-pt0.pos[0],seg_start.pos[1]-pt0.pos[1]);
            highlights.lineTo(seg_end.pos[0]-pt0.pos[0],seg_end.pos[1]-pt0.pos[1]);
        }


        lastpt = pti;

    }

    if(currentFocusPointDistanceToStart>=0.0f)
    {
        MiaPoint4D fc = matrix.fromPatientCoordinate(currentFocusPoint);
        highlights.moveTo(fc.pos[0] - pt0.pos[0] - 1.0, fc.pos[1] - pt0.pos[1] - 1.0);
        highlights.lineTo(fc.pos[0] - pt0.pos[0] + 1.0, fc.pos[1] - pt0.pos[1] + 1.0);
        highlights.moveTo(fc.pos[0] - pt0.pos[0] - 1.0, fc.pos[1] - pt0.pos[1] + 1.0);
        highlights.lineTo(fc.pos[0] - pt0.pos[0] + 1.0, fc.pos[1] - pt0.pos[1] - 1.0);
    }
    QPainterPathStroker stroker;
    shape = stroker.createStroke(highlights);
    return pt0;
}

void MiaMarker3DBSpline::setKeyPoints(QList<MiaPoint4D> &list)
{
    needToRecreatRenderPoints = true;
    MiaMarker::setKeyPoints(list);
}


float MiaMarker3DBSpline::moveFocusPointAlongCurveToLocation(float dist)
{
    if(currentFocusPointDistanceToStart < 0.0f)
        currentFocusPointDistanceToStart = 0.0f;
    float new_dist = getPointAtDistance(dist,currentFocusPoint,currentFocusPointTangent);
    if(new_dist != currentFocusPointDistanceToStart)
    {
        currentFocusPointDistanceToStart = new_dist;
        emit markerUpdated(this);
    }
    return currentFocusPointDistanceToStart;
}

void MiaMarker3DBSpline::moveFocurPointToLastPoint()
{
    currentFocusPointDistanceToStart = getPointAtDistance(MAX_FLOAT,currentFocusPoint,currentFocusPointTangent);
}

MiaPoint4D MiaMarker3DBSpline::getFocusTangentDirection()
{
    return currentFocusPointTangent;
}

MiaMarker3DClosedBSpline::MiaMarker3DClosedBSpline(MiaPoint4D pt,QObject *parent):MiaMarker3DBSpline(pt,parent)
{
    type = MIA_ClosedSpline;
    name = "3DClosedSpline";
    splineX->ClosedOn();
    splineY->ClosedOn();
    splineZ->ClosedOn();
    isClosed = true;
}
