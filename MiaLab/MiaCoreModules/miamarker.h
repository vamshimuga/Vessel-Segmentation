#ifndef MIAMARKER_H
#define MIAMARKER_H

#include <QObject>
#include "miaimagend.h"
#include <QtGui>
enum MarkerUpdateType {MIA_MarkerAdde, MIA_MarkerUpdated, MIA_MarkerRemoved};
enum MarkerMode {MIA_Selected, MIA_Initializing, MIA_Resting, MIA_PreActive, MIA_Destroying};
struct ControlNode
{
    MiaPoint4D projectedPosition;
    int index;
};
enum MarkerType { MIA_Unknown, MIA_Point,MIA_Line, MIA_Circle, MIA_Rectangle, MIA_Polygon,MIA_ClosedPolygon, MIA_Spline,MIA_ClosedSpline, MIA_Text, MIA_3DBall, MIA_3DBox};

class MIACOREMODULESSHARED_EXPORT MiaMarker : public QObject
{
    Q_OBJECT
public:
    explicit MiaMarker( MiaPoint4D pt, QObject *parent = 0);
    virtual ~MiaMarker();
    MarkerMode currentMode;
    virtual bool isMarkerValid() = 0;
    virtual void mouseButtonPressedAT( MiaPoint4D pt ) = 0;
    virtual bool handleMouseButtonPressedIn2DPrjectionView( const MiaPoint4D pt, const MiaTransform& transform ) {Q_UNUSED (pt); Q_UNUSED(transform); return false;}
    virtual void mouseMovedAT( MiaPoint4D pt ) = 0;
    virtual void mouseButtonReleasedAT( MiaPoint4D pt ) = 0;
    virtual void mouseDoubleClickedAt( MiaPoint4D pt) {Q_UNUSED(pt);}
    virtual bool crossHairNeedMoveAfterClickAt( MiaPoint4D& pt, MiaTransform& transform ) const;
    virtual bool crossHairNeedMoveAfterControlPointSelected(int index, MiaPoint4D&pt) const;
    QString name;
    float lineWidth;
    void setSelected(bool isSelected);
    virtual MiaPoint4D creatShapePath(QPainterPath& path, QPainterPath& shape, QPainterPath& highlights, QList<ControlNode>&controlnodes, MiaTransform& matrix, float clipdistance) = 0;
    virtual void translateOriginTo(const MiaPoint4D &pt, bool unconditional = false); //origin is the first key point
    virtual void transformKeyPoints(MiaAffineTransform &matrix);
    virtual void setPreActive(bool active);
    virtual void keyPointMoved( int index, MiaPoint4D pos);
    virtual MiaMarker* getANewCopy(QObject *parent ){Q_UNUSED(parent); return NULL;}
    virtual void copyFromAnotherMarker(MiaMarker*);
    virtual bool markerCreatingFinished();
    virtual bool setMarkerColor(const QColor* contourcolor, const QColor* brushcolor = NULL);
    virtual void showHighlights(bool show);
    QColor color;
    QColor brushColor;
    QColor preActiveColor;
    QColor initStateColor;
    QCursor cursor;
    bool needCrossHairFolowTheFocus;
    bool justSelectedAndNeedResetFocus;
    bool isFrozen;
    bool isShowingHighlights;
    bool isVisible;
    virtual MiaPoint4D getFocus() = 0;
    QColor nodeRestingColor;
    QColor nodeActiveColor;
    float controlNodeSize;
    MarkerType type;
    MiaPoint4D getKeyPointAt( int index ) { return keyPointList.at( index );}
    virtual void setKeyPoints(QList <MiaPoint4D>& list);
    void setKeyValues(QList <MiaPoint4D>& list);
    int getKeyPointNumber() {return keyPointList.size();}
    QList <MiaPoint4D> keyPointList;
    QList <MiaPoint4D> keyValueList;
    virtual float getSeedRegionRadius() { return 0;}
    virtual float getDistanceToSurface(MiaPoint4D &pt) {  return pt.distance(keyPointList.at(0)); }
    void setTransformMatrixOfInitial2DSlice(MiaTransform& originslicetr)
    {
        transformMatrixOfInitial2DScene = originslicetr;
    }

    MiaTransform transformMatrixOfInitial2DScene;


signals:
    void markerWillBeRemoved( MiaMarker* marker );
    void markerModeChanged( MiaMarker* marker );
    void markerUpdated( MiaMarker* marker );
    void markerSelected( MiaMarker* marker );
    void markerCompleted( MiaMarker* marker );

public slots:

protected:
    bool mouseButtonPressed;


};
MIACOREMODULESSHARED_EXPORT QDataStream& operator<<(QDataStream& stream, const MiaMarker& marker );
MIACOREMODULESSHARED_EXPORT QDataStream& operator>>(QDataStream& stream, MiaMarker& marker );


#endif // MIAMARKER_H
