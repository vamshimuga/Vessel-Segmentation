#ifndef MIAMARKER3DPOLYGON_H
#define MIAMARKER3DPOLYGON_H

#include "MiaCoreModules/miamarker.h"

class MIAMARKERSSHARED_EXPORT MiaMarker3DPolygon : public MiaMarker
{
    Q_OBJECT
public:
    explicit MiaMarker3DPolygon(MiaPoint4D pt, QObject *parent = 0);
    virtual bool isMarkerValid();
    virtual void mouseButtonPressedAT( MiaPoint4D pt );
    virtual bool handleMouseButtonPressedIn2DPrjectionView( const MiaPoint4D pt, const MiaTransform& transform );
    virtual void mouseMovedAT( MiaPoint4D pt );
    virtual void mouseButtonReleasedAT( MiaPoint4D pt );
    virtual void mouseDoubleClickedAt( MiaPoint4D pt);
    virtual MiaPoint4D creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance);
    virtual bool crossHairNeedMoveAfterClickAt(MiaPoint4D& pt , MiaTransform &transform) const;
    virtual MiaPoint4D getFocus();
    virtual float getSeedRegionRadius() { return 3.0f;}
    bool resampleKeyPointsWithStepLength(float s);
    void setDirectionVectorAsKeyValues(MiaImageND* im);
    virtual void getRenderPoints(QList <MiaPoint4D>& pts) { pts = keyPointList; }
    virtual MiaMarker* getANewCopy(QObject *parent )
    {
        MiaMarker* marker = new MiaMarker3DPolygon(this->keyPointList.first(),parent);
        marker->copyFromAnotherMarker(this);
        return marker;
    }
    QList <int> bifurcationNodeList;
signals:

public slots:
protected:
    int currentNode;
    float length;
    float visibleDistance;
    float minimumDistanceBetweenNodes;
    bool isClosed;
    virtual int findCloseProjectedPointFromPointList(const MiaPoint4D& pt, const MiaTransform& transform, const QList<MiaPoint4D>& list) const;
    float distanceFromPointToLineSegment(const MiaPoint4D& pt, const MiaPoint4D& line_start, const MiaPoint4D& line_end, double& offset) const;
    bool resamplePolygonAndValueWithStepLength(QList<MiaPoint4D>& polygon, QList<MiaPoint4D>* values, double stepLength) const;
};

#endif // MIAMARKER3DPOLYGON_H
