#include "miamarker.h"

MiaMarker::MiaMarker(MiaPoint4D pt, QObject *parent) :
    QObject(parent)
{
    type = MIA_Unknown;
    currentMode = MIA_Initializing;
    mouseButtonPressed = true;
    keyPointList.append(pt);
    QObject::connect(this, SIGNAL(markerUpdated( MiaMarker*)), parent, SLOT(forwardMarkerUpdatedSignal(MiaMarker*)));
    QObject::connect(this, SIGNAL(markerWillBeRemoved( MiaMarker*)), parent, SLOT(markerRemoved(MiaMarker*)));
    QObject::connect(this, SIGNAL(markerSelected(MiaMarker*)), parent, SLOT(selectAnotherMarker( MiaMarker*))); // check if it can be selected
    QObject::connect(this, SIGNAL(markerModeChanged(MiaMarker*)), parent, SLOT(forwardMarkerModeChanged( MiaMarker*))); //if it is selected then notifiy drawer

    QObject::connect(this, SIGNAL(markerCompleted(MiaMarker*)), parent, SLOT(currentMarkerCompleted( MiaMarker* )));
    color = QColor(Qt::green);
    brushColor = QColor(Qt::green);
    brushColor.setAlpha(75);
    preActiveColor = QColor(Qt::yellow);
    preActiveColor.setAlpha(50);
    nodeRestingColor = QColor(Qt::cyan);
    nodeActiveColor = QColor(Qt::red);
    initStateColor = QColor(Qt::red);
    cursor = Qt::OpenHandCursor;
    lineWidth = 1.0;
    controlNodeSize = 3.0;
    needCrossHairFolowTheFocus = false;
    justSelectedAndNeedResetFocus = false;
    isFrozen = false;
    isVisible = true;
    isShowingHighlights = true;

}

MiaMarker::~MiaMarker()
{
    currentMode = MIA_Destroying;
    emit markerWillBeRemoved(this);
}

bool MiaMarker::crossHairNeedMoveAfterClickAt(MiaPoint4D &pt, MiaTransform &transform) const
{
    pt = transform.toPatientCoordinate(pt);
    return true;
}

bool MiaMarker::crossHairNeedMoveAfterControlPointSelected(int index, MiaPoint4D &pt) const
{
    pt = keyPointList.at(index);
    return true;
}
void MiaMarker::translateOriginTo(const MiaPoint4D& pt, bool unconditional)
{
    //origin is the first key point
    MiaAffineTransform t;
    t.toPatientSystemMatrix[3] = pt.pos[0] - keyPointList.at(0).pos[0];
    t.toPatientSystemMatrix[7] = pt.pos[1] - keyPointList.at(0).pos[1];
    t.toPatientSystemMatrix[11] = pt.pos[2] - keyPointList.at(0).pos[2];
    //check is the new position is far away from the computation error 0.01
    if(t.toPatientSystemMatrix[3]*t.toPatientSystemMatrix[3] + t.toPatientSystemMatrix[7]*t.toPatientSystemMatrix[7] + t.toPatientSystemMatrix[11]*t.toPatientSystemMatrix[11]>0.0001 || unconditional)
        transformKeyPoints(t);
}

void MiaMarker::transformKeyPoints(MiaAffineTransform& matrix)
{
    for(int i  = 0 ; i < keyPointList.size(); i++)
    {
        keyPointList[i] = matrix.transform(keyPointList.at(i));
    }
    emit markerUpdated(this);
}

//void MiaMarker::mouseButtonPressedAT( Point4D pt )
//{

//}

//void MiaMarker::mouseMovedAT( Point4D pt )
//{

//}

//void MiaMarker::mouseButtonReleasedAT( Point4D pt )
//{

//}

void MiaMarker::setSelected(bool isSelected)
{
    if(isSelected)
    {
        currentMode = MIA_Selected;
        emit markerSelected(this);
        if(currentMode == MIA_Selected)
        {
            emit markerModeChanged(this);
        }
    }
    else
    {
        if(isMarkerValid())
        {
            currentMode = MIA_Resting;
            emit markerModeChanged(this);
        }
        else
        {
             delete this;
        }
    }
}

void MiaMarker::setPreActive(bool active)
{
    if(active && currentMode == MIA_Resting)
    {
        currentMode = MIA_PreActive;
        emit markerModeChanged(this);
    }
    else if( !active && currentMode == MIA_PreActive  )
    {
        currentMode = MIA_Resting;
        emit markerModeChanged(this);
    }

}


void MiaMarker::keyPointMoved( int index, MiaPoint4D pos)
{
    if( index < keyPointList.size() )
    {
        keyPointList[index] = pos;
    }
    emit markerUpdated(this);
}
void MiaMarker::setKeyPoints(QList <MiaPoint4D>& list)
{
    keyPointList.clear();
    keyPointList.append(list);
    emit markerUpdated(this);
}
void MiaMarker::setKeyValues(QList <MiaPoint4D>& list)
{
    keyValueList.clear();
    keyValueList.append(list);
    emit markerUpdated(this);
}
void MiaMarker::copyFromAnotherMarker(MiaMarker* marker)
{
    color = marker->color;
    brushColor = marker->brushColor;
    preActiveColor = marker->preActiveColor;
    nodeRestingColor = marker->nodeRestingColor;
    nodeActiveColor = marker->nodeActiveColor;
    lineWidth = marker->lineWidth;
    controlNodeSize = marker->controlNodeSize;
    needCrossHairFolowTheFocus = marker->needCrossHairFolowTheFocus;
    setKeyPoints(marker->keyPointList);
    setKeyValues(marker->keyValueList);
}

bool MiaMarker::markerCreatingFinished()
{
    mouseButtonPressed = false;
    emit markerCompleted(this);
    if(isMarkerValid())
    {
        this->currentMode = MIA_Resting;
        emit markerUpdated(this);
        return true;
    }
    else
        delete this;
    return false;
}

bool MiaMarker::setMarkerColor(const QColor *contourcolor, const QColor *brushcolor)
{
    if(contourcolor)
    {
        color = *contourcolor;
    }
    if(brushcolor)
    {
        brushColor = *brushcolor;
    }
    emit markerUpdated(this);
    return true;
}

void MiaMarker::showHighlights(bool show)
{
    isShowingHighlights = show;
    emit markerUpdated(this);
}

QDataStream& operator<<(QDataStream& stream, const MiaMarker& marker )
{
    stream<<marker.name;
    stream<<(qint32)marker.lineWidth;
    stream<<marker.color;
    stream<<marker.keyPointList;
    return stream;
}
QDataStream& operator>>(QDataStream& stream, MiaMarker& marker )
{
    QString markername;
    qint32 markerlinewidth;
    QColor markercolor;
    stream>>markername;
    stream>>markerlinewidth;
    stream>>markercolor;
    marker.name = markername;
    marker.lineWidth = markerlinewidth;
    marker.color = markercolor;
    stream>>marker.keyPointList;
    return stream;

}
