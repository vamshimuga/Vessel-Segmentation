#ifndef MIAMAKER3DLINE_H
#define MIAMAKER3DLINE_H

#include "MiaCoreModules/miamarker.h"

class MIAMARKERSSHARED_EXPORT MiaMaker3DLine : public MiaMarker
{
    Q_OBJECT
public:
    explicit MiaMaker3DLine(MiaPoint4D pt, QObject *parent = 0);
    virtual bool isMarkerValid();
    virtual void mouseButtonPressedAT( MiaPoint4D pt );
    virtual void mouseMovedAT( MiaPoint4D pt );
    virtual void mouseButtonReleasedAT( MiaPoint4D pt );
    virtual MiaPoint4D creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance);
    virtual bool crossHairNeedMoveAfterClickAt( MiaPoint4D& pt, MiaTransform& transform ) const;
    virtual MiaPoint4D getFocus();
    virtual float getSeedRegionRadius() { return 3.0f;}
    virtual MiaMarker* getANewCopy(QObject *parent)
    {
        MiaMarker* marker = new MiaMaker3DLine(this->keyPointList.first(),parent);
        marker->copyFromAnotherMarker(this);
        return marker;
    }
signals:

public slots:
protected:
//    Point4D ballCenterPoint; use keyPointList.at(0) as the center point
    float length;
    float visibleDistance;
};

#endif // MIAMAKER3DLINE_H
