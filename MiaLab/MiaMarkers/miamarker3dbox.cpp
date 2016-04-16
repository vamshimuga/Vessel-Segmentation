#include "miamarker3dbox.h"

//         3-----2
//        /|    /|
//       0-+---1 |
//       | 7---+-6
//       |/    |/
//       4-----5
MiaMarker3DBox::MiaMarker3DBox(MiaPoint4D pt, QObject *parent) :
    MiaMarker(pt, parent)
{
    type = MIA_3DBox;
    name = "3DBall";
    // keyPointList.at(0) is the center of a ball, but we need add the second point to caculate the radius
    for(int i = 0; i < 7; i ++)
        keyPointList.append(pt);
    needCrossHairFolowTheFocus = true;
}
void MiaMarker3DBox::mouseButtonPressedAT( MiaPoint4D pt )
{

}

bool MiaMarker3DBox::crossHairNeedMoveAfterClickAt( MiaPoint4D& pt, MiaTransform& transform ) const
{
    pt = keyPointList.at(0);
    return false;
}

bool MiaMarker3DBox::crossHairNeedMoveAfterControlPointSelected(int index, MiaPoint4D &pt) const
{
//    if(index >= keyPointList.count())
//        return false;
//    pt = keyPointList.at(index);
//    return true;
    return false;
}

void MiaMarker3DBox::mouseMovedAT( MiaPoint4D pt )
{
    if(currentMode == MIA_Initializing && mouseButtonPressed )
    {
        keyPointList[5] = pt;
        MiaPoint4D origin = keyPointList[0];
        MiaPoint4D origin_in_scene = transformMatrixOfInitial2DScene.fromPatientCoordinate(origin);
        MiaPoint4D scene_x_axis_end = origin_in_scene;
        scene_x_axis_end.pos[0] += 1.0;
        MiaPoint4D horizontal_axis_end = transformMatrixOfInitial2DScene.toPatientCoordinate(scene_x_axis_end);
        MiaPoint4D scene_y_axis_end = origin_in_scene;
        scene_y_axis_end.pos[1] += 1.0;
        MiaPoint4D vertical_axis_end = transformMatrixOfInitial2DScene.toPatientCoordinate(scene_y_axis_end);
        MiaPoint4D horizontal_orientation = horizontal_axis_end - origin;
        horizontal_orientation.normalize();
        MiaPoint4D vertical_orientation = vertical_axis_end - origin;
        vertical_orientation.normalize();
        MiaPoint4D depth_orientation = horizontal_orientation.cross(vertical_orientation);
        depth_orientation.normalize();
        float width = (keyPointList[5]-keyPointList[0])*horizontal_orientation;
        float height = (keyPointList[5]-keyPointList[0])*vertical_orientation;
        float depth = width < height ? width: height;
        keyPointList[1] = horizontal_orientation*width + keyPointList[0];
        keyPointList[2] = depth_orientation*depth + keyPointList[1];
        keyPointList[3] = depth_orientation*depth + keyPointList[0];
        keyPointList[4] = vertical_orientation*height + keyPointList[0];
        keyPointList[6] = depth_orientation*depth + keyPointList[5];
        keyPointList[7] = depth_orientation*depth + keyPointList[4];


    }
    needCrossHairFolowTheFocus = true;
    emit markerUpdated( this );
}

void MiaMarker3DBox::mouseButtonReleasedAT( MiaPoint4D pt )
{
    mouseButtonPressed = false;
    if(markerCreatingFinished())
    {
        currentMode = MIA_Selected;
        emit markerUpdated( this );
        qDebug() <<"created a 3D box";
    }
}

void MiaMarker3DBox::keyPointMoved(int index, MiaPoint4D pos)
{
    if( index < keyPointList.size() )
    {
        MiaPoint4D delta = pos - keyPointList[index];
        for(int i = 0; i < 6; i++)
        {
            int v_i[4];
            bool faceContainMovingPt = false;
            getFaceVertexIndexes(i, v_i);
            for(int j = 0; j < 4; j++)
            {
                if(v_i[j] == index)
                {
                    faceContainMovingPt = true;
                    break;
                }
            }
            if(faceContainMovingPt)
            {
                MiaPoint4D norm = (keyPointList[v_i[1]] - keyPointList[v_i[0]]).cross(keyPointList[v_i[2]] - keyPointList[v_i[0]]);
                norm.normalize();
                MiaPoint4D projected_delta = norm*(delta*norm);
                for(int j = 0; j < 4; j++)
                {
                    keyPointList[v_i[j]] = keyPointList[v_i[j]] + projected_delta;
                }
            }

        }
    }
    emit markerUpdated(this);
}

bool MiaMarker3DBox::isMarkerValid()
{
    float distance = keyPointList[1].distance(keyPointList.at(5));
    if(distance > 0.0)
        return true;
    else
    {
        qDebug() <<"Invalid" << distance;
        return false;
    }
}

MiaPoint4D MiaMarker3DBox::creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance)
{
    MiaPoint4D origin = matrix.fromPatientCoordinate(keyPointList.at(0));
    QVector<QPointF> projectedvertex;

    for(int i = 0; i < 8; i++)
    {
        MiaPoint4D projectpt = matrix.fromPatientCoordinate(keyPointList.at(i));
        projectpt.pos[0] -= origin.pos[0];
        projectpt.pos[1] -= origin.pos[1];
        QPointF scencept(projectpt.pos[0], projectpt.pos[1]);
        projectedvertex.append(scencept);

        ControlNode node;
        node.projectedPosition = projectpt;
        node.index = i;
        controlnodes.append(node);

    }

    for(int i = 0; i < 6; i++)
    {
        int v_i[4];
        getFaceVertexIndexes(i, v_i);
        if(projectedvertex[v_i[0]] != projectedvertex[v_i[1]] && projectedvertex[v_i[1]] != projectedvertex[v_i[2]])
        {
            QPolygonF polygon;
            polygon << projectedvertex[v_i[0]] << projectedvertex[v_i[1]] << projectedvertex[v_i[2]] << projectedvertex[v_i[3]] << projectedvertex[v_i[0]];
            path.addPolygon(polygon);
        }
    }

    for(int i = 0; i < 8; i++)
    {

    };

    shape = path;
    shape.setFillRule(Qt::WindingFill);

    return origin;
}
MiaPoint4D MiaMarker3DBox::getFocus()
{
    return keyPointList.first();
}

float MiaMarker3DBox::getDistanceToSurface(MiaPoint4D &pt)
{
    float distance_pos = 0;
    float distance_neg = - keyPointList.at(0).distance(keyPointList.at(6));
    for(int i = 0 ; i < 6; i ++)
    {
        int v_i[4];
        getFaceVertexIndexes(i, v_i);
        MiaPoint4D norm = (keyPointList[v_i[1]] - keyPointList[v_i[0]]).cross(keyPointList[v_i[2]] - keyPointList[v_i[0]]);
        norm.normalize();
        float dist_to_face = (pt-keyPointList[v_i[0]])*norm;
        if(dist_to_face > 0)
        {
            distance_pos += dist_to_face*dist_to_face;
        }
        else
        {
            if(dist_to_face > distance_neg)
                distance_neg = dist_to_face;
        }

    }
    if(distance_pos > 0) // point is outside the box
    {
        return sqrt(distance_pos);
    }
    return distance_neg;
}

void MiaMarker3DBox::getFaceVertexIndexes(int faceid, int *indexes)
{
    switch(faceid)
    {
    case 0:
        indexes[0] = 0; indexes[1] = 1; indexes[2] = 2; indexes[3] = 3;
        break;
    case 1:
        indexes[0] = 0; indexes[1] = 4; indexes[2] = 5; indexes[3] = 1;
        break;
    case 2:
        indexes[0] = 1; indexes[1] = 5; indexes[2] = 6; indexes[3] = 2;
        break;
    case 3:
        indexes[0] = 4; indexes[1] = 7; indexes[2] = 6; indexes[3] = 5;
        break;
    case 4:
        indexes[0] = 3; indexes[1] = 2; indexes[2] = 6; indexes[3] = 7;
        break;
    case 5:
        indexes[0] = 0; indexes[1] = 3; indexes[2] = 7; indexes[3] = 4;
        break;
    }
}
