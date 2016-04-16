#include "MiaMarkers/miamarker3dball.h"
#include <QtGui>
#include "MiaCoreModules/miaimagend.h"

MiaMarker3DBall::MiaMarker3DBall(MiaPoint4D pt, QObject *parent) :
    MiaMarker(pt, parent)
{
    type = MIA_3DBall;
    name = "3DBall";
    // keyPointList.at(0) is the center of a ball, but we need add the second point to caculate the radius
    keyPointList.append(pt);
    needCrossHairFolowTheFocus = true;

}

MiaMarker3DBall::MiaMarker3DBall(MiaPoint4D pt1, MiaPoint4D pt2, QObject *parent) :
    MiaMarker(pt1, parent)
{
    type = MIA_3DBall;
    name = "3DBall";
    // keyPointList.at(0) is the center of a ball, but we need add the second point to caculate the radius
    keyPointList.append(pt2);
    needCrossHairFolowTheFocus = true;

}

void MiaMarker3DBall::mouseButtonPressedAT( MiaPoint4D pt )
{
    Q_UNUSED(pt);
}

bool MiaMarker3DBall::crossHairNeedMoveAfterClickAt( MiaPoint4D& pt, MiaTransform& transform ) const
{
    Q_UNUSED(transform);
    pt = keyPointList.at(0);
    return true;
}

bool MiaMarker3DBall::crossHairNeedMoveAfterControlPointSelected(int index, MiaPoint4D &pt) const
{
    Q_UNUSED(index);
    Q_UNUSED(pt);
    return false;
}

void MiaMarker3DBall::mouseMovedAT( MiaPoint4D pt )
{
    if(currentMode == MIA_Initializing && mouseButtonPressed )
    {
        keyPointList[1] = pt;

    }
    needCrossHairFolowTheFocus = false;
    emit markerUpdated( this );
}

void MiaMarker3DBall::mouseButtonReleasedAT( MiaPoint4D pt )
{
    Q_UNUSED(pt);
    mouseButtonPressed = false;
    if(markerCreatingFinished())
    {
        currentMode = MIA_Selected;
        emit markerUpdated( this );
        qDebug() <<"created" << radius;
    }
}

bool MiaMarker3DBall::isMarkerValid()
{
    radius = keyPointList[1].distance(keyPointList.at(0));
    if(radius > 0.0)
        return true;
    else
    {
        qDebug() <<"Invalid" << radius;
        return false;
    }
}

MiaPoint4D MiaMarker3DBall::creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance)
{
    Q_UNUSED(highlights);
    Q_UNUSED(clipdistance);
    MiaPoint4D origin = matrix.fromPatientCoordinate(keyPointList.at(0));
    radius = keyPointList[1].distance(keyPointList.at(0));

    if( fabs(origin.pos[2]) < radius )
    {
        float intersectRadius = sqrt( radius*radius - origin.pos[2]*origin.pos[2]  );

        path.addEllipse(QPointF(0,0),intersectRadius,intersectRadius);


        intersectRadius *= sqrt(2.0)/2.0;

        ControlNode node0;
        node0.projectedPosition.pos[0] = -intersectRadius;
        node0.projectedPosition.pos[1] = -intersectRadius;
        node0.index = 1;

        controlnodes.append(node0);

        ControlNode node1;
        node1.projectedPosition.pos[0] = +intersectRadius;
        node1.projectedPosition.pos[1] = -intersectRadius;
        node1.index = 1;
        controlnodes.append(node1);

        ControlNode node2;
        node2.projectedPosition.pos[0] = -intersectRadius;
        node2.projectedPosition.pos[1] = +intersectRadius;
        node2.index = 1;
        controlnodes.append(node2);

        ControlNode node3;
        node3.projectedPosition.pos[0] = intersectRadius;
        node3.projectedPosition.pos[1] = intersectRadius;
        node3.index = 1;
        controlnodes.append(node3);

    }

    shape = path;



    return origin;
}
MiaPoint4D MiaMarker3DBall::getFocus()
{
    return keyPointList.first();
}
