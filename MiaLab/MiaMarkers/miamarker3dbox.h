#ifndef MIAMARKER3DBOX_H
#define MIAMARKER3DBOX_H

#include "MiaCoreModules/miamarker.h"

class MIAMARKERSSHARED_EXPORT MiaMarker3DBox : public MiaMarker
{
    Q_OBJECT
public:
    explicit MiaMarker3DBox(MiaPoint4D pt, QObject *parent = 0);
    virtual bool isMarkerValid();
    virtual void mouseButtonPressedAT( MiaPoint4D pt );
    virtual void mouseMovedAT( MiaPoint4D pt );
    virtual void mouseButtonReleasedAT( MiaPoint4D pt );
    virtual void keyPointMoved( int index, MiaPoint4D pos);
    virtual MiaPoint4D creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance);
    virtual bool crossHairNeedMoveAfterClickAt(MiaPoint4D& pt , MiaTransform &transform) const;
    virtual bool crossHairNeedMoveAfterControlPointSelected(int index, MiaPoint4D&pt) const;
    virtual MiaPoint4D getFocus();
    virtual float getSeedRegionRadius() { return 0;}
    virtual float getDistanceToSurface(MiaPoint4D &pt);
    virtual MiaMarker* getANewCopy(QObject *parent )
    {
        MiaMarker* marker = new MiaMarker3DBox(this->keyPointList.first(),parent);
        marker->copyFromAnotherMarker(this);
        return marker;
    }
signals:
    
public slots:
protected:
    void getFaceVertexIndexes(int faceid, int* indexes);
    
};

#endif // MIAMARKER3DBOX_H
