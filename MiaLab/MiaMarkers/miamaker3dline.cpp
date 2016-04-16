#include "miamaker3dline.h"
#include <QtGui>
#include "MiaCoreModules/miaimagend.h"

MiaMaker3DLine::MiaMaker3DLine(MiaPoint4D pt, QObject *parent) :
    MiaMarker(pt,parent)
{
    type = MIA_Line;
    name = "3DLine";
    // keyPointList.at(0) is set in base class, we need add the second point to caculate the radius
    keyPointList.append(pt);
    needCrossHairFolowTheFocus = true;
    length = 0;
    visibleDistance = controlNodeSize;
}
void MiaMaker3DLine::mouseButtonPressedAT( MiaPoint4D pt )
{

}

bool MiaMaker3DLine::crossHairNeedMoveAfterClickAt(MiaPoint4D& pt , MiaTransform &transform) const
{
    pt = keyPointList.at(0);
    return true;
}

void MiaMaker3DLine::mouseMovedAT( MiaPoint4D pt )
{
    if(currentMode == MIA_Initializing && mouseButtonPressed )
    {
        keyPointList[1] = pt;

    }
    needCrossHairFolowTheFocus = false;
    emit markerUpdated( this );
}

void MiaMaker3DLine::mouseButtonReleasedAT( MiaPoint4D pt )
{
    mouseButtonPressed = false;
    if(markerCreatingFinished())
    {
        currentMode = MIA_Selected;
        emit markerUpdated( this );
        qDebug() <<"created" << length;
    }
}

bool MiaMaker3DLine::isMarkerValid()
{
    length = keyPointList[1].distance(keyPointList.at(0));
    if(length > 0.0)
        return true;
    else
    {
        qDebug() <<"Invalid" << length;
        return false;
    }
}

MiaPoint4D MiaMaker3DLine::creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance)
{
    MiaPoint4D pt1 = matrix.fromPatientCoordinate(keyPointList.at(0));
    MiaPoint4D pt2 = matrix.fromPatientCoordinate(keyPointList.at(1));

    length = keyPointList[1].distance(keyPointList.at(0));

//    if( fabs(pt1.pos[2]) < visibleDistance && fabs(pt2.pos[2]) < visibleDistance )
    {
        path.moveTo(0,0);
        path.lineTo(pt2.pos[0]-pt1.pos[0],pt2.pos[1]-pt1.pos[1]);

        ControlNode node0;
        node0.projectedPosition.pos[0] = 0;
        node0.projectedPosition.pos[1] = 0;
        node0.index = 0;//index to key Point List

        controlnodes.append(node0);

        ControlNode node1;
        node1.projectedPosition.pos[0] = pt2.pos[0]-pt1.pos[0];
        node1.projectedPosition.pos[1] = pt2.pos[1]-pt1.pos[1];
        node1.index = 1;
        controlnodes.append(node1);


    }
    QPainterPathStroker stroker;
    shape = stroker.createStroke(path);
    return pt1;
}
MiaPoint4D MiaMaker3DLine::getFocus()
{
    return keyPointList.first();
}
