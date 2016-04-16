#ifndef MIAMARKER3DBSPLINE_H
#define MIAMARKER3DBSPLINE_H

#include "miamarker3dpolygon.h"

class vtkCardinalSpline;

class MIAMARKERSSHARED_EXPORT MiaMarker3DBSpline : public MiaMarker3DPolygon
{
    Q_OBJECT
public:
    explicit MiaMarker3DBSpline(MiaPoint4D pt, QObject *parent = 0);
    virtual ~MiaMarker3DBSpline();
    virtual MiaPoint4D getFocus();
    virtual void mouseButtonPressedAT( MiaPoint4D pt );
    virtual bool handleMouseButtonPressedIn2DPrjectionView( const MiaPoint4D pt, const MiaTransform& transform );
    virtual void mouseMovedAT( MiaPoint4D pt );
    virtual void mouseButtonReleasedAT( MiaPoint4D pt );
    virtual void mouseDoubleClickedAt( MiaPoint4D pt);
    virtual void keyPointMoved( int index, MiaPoint4D pos);
    virtual void transformKeyPoints(MiaAffineTransform &matrix);
    virtual MiaPoint4D creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance);
    virtual void getRenderPoints(QList <MiaPoint4D>& pts) {recreatRenderPointsWhenNecessary(); pts.append(renderPoints);}
    void setRenderResolution(float step) { renderResolution = step; }
    virtual MiaMarker* getANewCopy(QObject *parent )
    {
        MiaMarker* marker = new MiaMarker3DBSpline(this->keyPointList.first(),parent);
        marker->copyFromAnotherMarker(this);
        return marker;
    }
    virtual void setKeyPoints(QList <MiaPoint4D>& list);
    float getFocusDistanceToStart(){ return currentFocusPointDistanceToStart;}
    float moveFocusPointAlongCurveToLocation(float dist);
    void moveFocurPointToLastPoint();
    MiaPoint4D getFocusTangentDirection();
signals:

public slots:
protected:
    QList <MiaPoint4D> renderPoints;
    QList <MiaPoint4D> renderPointsValues;
    float renderResolution;
    virtual void recreatRenderPointsWhenNecessary();
    bool needToRecreatRenderPoints;
//    Point4D evaluateUniformIndexBSplineValueAt( int numpt, QList <Point4D>& keypts, float indexf );
    vtkCardinalSpline* splineX;
    vtkCardinalSpline* splineY;
    vtkCardinalSpline* splineZ;
    float currentFocusPointDistanceToStart;
    MiaPoint4D currentFocusPoint;
    MiaPoint4D currentFocusPointTangent;
    float getPointAtDistance(float dist, MiaPoint4D& pos, MiaPoint4D& tan);
};

class MIAMARKERSSHARED_EXPORT MiaMarker3DClosedBSpline: public MiaMarker3DBSpline
{
public:
    explicit MiaMarker3DClosedBSpline(MiaPoint4D pt,QObject *parent = 0);
};

#endif // MIAMARKER3DBSPLINE_H
