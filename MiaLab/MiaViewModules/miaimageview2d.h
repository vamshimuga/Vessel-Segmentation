#ifndef IMAGEVIEW2D_H
#define IMAGEVIEW2D_H

#include <QGraphicsView>
#include <QPoint>
#include "miaImage2qimage.h"
#include "miagraphics2dimageitem.h"
#include "miainputeventinterpretor.h"
#include "miagraphicscrosshairitem.h"
#include "miamarker2graphicsitem.h"

#define IMAGEMINZVALUE  0
#define IMAGEMAXZVALUE  100000.0
#define MARKERMINZVALUE 100001.0
#define MARKERMAXZVALUE 1000000.0
#define CROSSHAIRZVALUE 1000001.0

enum ViewParameterGroup {MIA_Parameter_None,
                         MIA_Parameter_SettingCrossPatientCoordinate,
                         MIA_Parameter_SettingCrossViewCoordinate,
                         MIA_Parameter_Panning,
                         MIA_Parameter_Zooming,
                         MIA_Parameter_ScrollCrossViewCoordinate};

class MIAVIEWMODULESSHARED_EXPORT MiaImageView2D : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MiaImageView2D(QWidget *parent = 0);
    virtual ~MiaImageView2D(){}
    void addInputModule(MiaImage2QImage* imagemodule);
    void removInputModule(MiaImage2QImage* imagemodule);
    void addMarkerModule(MiaMarker2GraphicsItem* module);
    void setWheelInterpretor(MiaInputEventInterpretor* interpretor) {  wheelInterpretor = interpretor; if(interpretor) interpretor->view = this; }
    void setmouseRightDragInterpretor(MiaInputEventInterpretor* interpretor) {  mouseRightDragInterpretor = interpretor; if(interpretor) interpretor->view = this;}
    void setmouseLeftDragInterpretor(MiaInputEventInterpretor* interpretor) {  mouseLeftDragInterpretor = interpretor; if(interpretor) interpretor->view = this;}
    void setmouseMoveInterpretor(MiaInputEventInterpretor* interpretor) { mouseMoveInterpretor = interpretor; if(interpretor) interpretor->view = this;}
    void setKeyboardInterpretor( MiaInputEventInterpretor* interpretor ) {keyboardInterpretor = interpretor; if(interpretor) interpretor->view = this;}
    void resetViewSize();
    void notifyCrossHairChanged();
    QPointF crosshairMoved( QPointF scene_pt );
    void showCrossHair(bool show);
    void moveCrossHairTo( MiaPoint4D pt );
    MiaPoint4D convertViewPointToPatientCoordinate( QPoint viewpt);
    MiaPoint4D convertScenePointToPatientCoordinate(QPointF scenept);
    QPointF convertPatientCoordinateToScenePoint( MiaPoint4D pt);
    QPoint convertPatientCoordinateToViewPoint( MiaPoint4D pt);
    QString viewName;
    MiaTransform transform;//Transform Matrix from patient to scene
    bool blockLeftButtonEvents;

    float getSliceThickness();

    static QCursor getZoomCursor();
    static QCursor getPanCursor();
    static QCursor getWinCursor();

signals:
    void parameterChanged(const QMap <QString, QVariant> &parameters);
public slots:
    void removeQImage(MiaGraphics2DImageItem* img);
    void addQImage(MiaGraphics2DImageItem* img);
    void imageGeometryChanged(MiaGraphics2DImageItem* img);
    void setParameters(const QMap<QString, QVariant> &parameters);
    void removeMarkerModule(MiaMarker2GraphicsItem* module);
protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void mouseDoubleClickEvent ( QMouseEvent * event );
    virtual void	keyPressEvent ( QKeyEvent * event );
    virtual void	keyReleaseEvent ( QKeyEvent * event );
    virtual void wheelEvent( QWheelEvent *event );
    virtual void resizeEvent ( QResizeEvent * event );
    void repositionImages();

    QGraphicsScene *imageScene;
    QList <MiaGraphics2DImageItem*> imageItemList;
    MiaInputEventInterpretor* mouseMoveInterpretor;
    MiaInputEventInterpretor* mouseLeftDragInterpretor;
    MiaInputEventInterpretor* mouseRightDragInterpretor;
    MiaInputEventInterpretor* wheelInterpretor;
    MiaInputEventInterpretor* keyboardInterpretor;

    MiaGraphicsCrossHairItem* crossHair;
    bool leftButtonDown;
    bool rightButtonDown;
    QRectF previousSceneRect;
    MiaGraphics2DImageItem* referenceImage;//the rest image will be position according to this image
    QMap <QString, QVariant> wiredParameters;
    QList <MiaMarker2GraphicsItem*> markerModuleList;
    float topMostImageZValue;
    bool settingCrossHairBySelf;
    float expectedSliceThickness;


};

#endif // IMAGEVIEW2D_H
