#ifndef MIAMARKER3DBALL_H
#define MIAMARKER3DBALL_H

#include "MiaCoreModules/miamarker.h"

class MIAMARKERSSHARED_EXPORT MiaMarker3DBall : public MiaMarker
{
    Q_OBJECT
public:

    explicit MiaMarker3DBall(MiaPoint4D pt, QObject *parent = 0);
    explicit MiaMarker3DBall(MiaPoint4D pt1, MiaPoint4D pt2, QObject *parent = 0);
    virtual bool isMarkerValid();
    virtual void mouseButtonPressedAT( MiaPoint4D pt );
    virtual void mouseMovedAT( MiaPoint4D pt );
    virtual void mouseButtonReleasedAT( MiaPoint4D pt );
    virtual MiaPoint4D creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance);
    virtual bool crossHairNeedMoveAfterClickAt(MiaPoint4D& pt , MiaTransform &transform) const;
    virtual bool crossHairNeedMoveAfterControlPointSelected(int index, MiaPoint4D&pt) const;
    virtual MiaPoint4D getFocus();
    virtual float getSeedRegionRadius() { return radius;}
    virtual MiaMarker* getANewCopy(QObject *parent )
    {
        MiaMarker* marker = new MiaMarker3DBall(this->keyPointList.first(),parent);
        marker->copyFromAnotherMarker(this);
        return marker;
    }
signals:

public slots:
protected:
//    Point4D ballCenterPoint; use keyPointList.at(0) as the center point
    float radius;

};

#endif // MIAMARKER3DBALL_H
