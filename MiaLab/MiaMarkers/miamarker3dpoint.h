#ifndef MIAMARKER3DPOINT_H
#define MIAMARKER3DPOINT_H

#include "MiaCoreModules/miamarker.h"

class MIAMARKERSSHARED_EXPORT MiaMarker3DPoint : public MiaMarker
{
    Q_OBJECT
public:
    explicit MiaMarker3DPoint(MiaPoint4D pt, QObject *parent);
    virtual bool isMarkerValid();
    virtual void mouseButtonPressedAT( MiaPoint4D pt );
    virtual void mouseMovedAT( MiaPoint4D pt );
    virtual void mouseButtonReleasedAT( MiaPoint4D pt );
    virtual MiaPoint4D creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance);
    virtual bool crossHairNeedMoveAfterClickAt(MiaPoint4D& pt , MiaTransform &transform) const;
    virtual MiaPoint4D getFocus();
    virtual float getSeedRegionRadius() { return 3.0f;}
    virtual MiaMarker* getANewCopy(QObject *parent )
    {
        MiaMarker* marker = new MiaMarker3DPoint(this->keyPointList.first(),parent);
        marker->copyFromAnotherMarker(this);
        return marker;
    }
signals:

public slots:

protected:
    float radius;

};

#endif // MIAMARKER3DPOINT_H
