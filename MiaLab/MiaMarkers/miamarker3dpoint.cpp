#include "miamarker3dpoint.h"
#include <QtGui>
#include "MiaCoreModules/miaimagend.h"

MiaMarker3DPoint::MiaMarker3DPoint(MiaPoint4D pt, QObject *parent) :
    MiaMarker(pt, parent)
{
    type = MIA_Point;
    name = "3DPoint";
    // keyPointList.at(0) is the center of a ball, but we need add the second point to caculate the radius
//    keyPointList.append(pt);
    needCrossHairFolowTheFocus = true;
    radius = controlNodeSize;
}

void MiaMarker3DPoint::mouseButtonPressedAT( MiaPoint4D pt )
{

}

void MiaMarker3DPoint::mouseMovedAT( MiaPoint4D pt )
{
    if(currentMode == MIA_Initializing && mouseButtonPressed )
    {
        keyPointList[0] = pt;

    }
    needCrossHairFolowTheFocus = false;
    emit markerUpdated( this );
}

void MiaMarker3DPoint::mouseButtonReleasedAT( MiaPoint4D pt )
{
    mouseButtonPressed = false;
    if(markerCreatingFinished())
    {
        currentMode = MIA_Selected;
        emit markerUpdated( this );
        qDebug() <<"created" << radius;
    }
}

MiaPoint4D MiaMarker3DPoint::creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance)
{
    MiaPoint4D origin = matrix.fromPatientCoordinate(keyPointList.at(0));

    if( fabs(origin.pos[2]) < radius )
    {
        float intersectRadius = sqrt( radius*radius - origin.pos[2]*origin.pos[2]  );

        path.addEllipse(QPointF(0,0),intersectRadius,intersectRadius);


        intersectRadius *= sqrt(2.0)/2.0;

        ControlNode node0;
        node0.projectedPosition.pos[0] = 0;
        node0.projectedPosition.pos[1] = 0;
        node0.index = 0;

        controlnodes.append(node0);

    }
    QPainterPathStroker stroker;
    shape = stroker.createStroke(path);
    return origin;

}

bool MiaMarker3DPoint::crossHairNeedMoveAfterClickAt( MiaPoint4D& pt, MiaTransform& transform ) const
{
    pt = keyPointList.at(0);
    return true;
}

MiaPoint4D MiaMarker3DPoint::getFocus()
{
    return keyPointList.first();
}

bool MiaMarker3DPoint::isMarkerValid()
{
    return true;
}
