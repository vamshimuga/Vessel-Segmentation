#ifndef IMAGEVIEW3D_H
#define IMAGEVIEW3D_H

#include "QVTKWidget.h"
#include "MiaCoreModules/miaimagend.h"
#include "MiaCoreModules/miamodule.h"
#include "miaimage2vtkprop.h"
#include "miainputeventinterpretor.h"
#include <QtGui>

class vtkRenderer;
class vtkProp3D;
class vtkOrientationMarkerWidget;
struct ModulePropPair
{
    MiaImage2VTKProp* module;
    vtkProp3D* prop;
};

class MIAVIEWMODULESSHARED_EXPORT MiaImageView3D : public QVTKWidget
{
    Q_OBJECT
public:
    explicit MiaImageView3D(QWidget *parent = 0);
    virtual ~MiaImageView3D();
//    void showCone();
    void forceUpdate();
    void addInputModule(MiaImage2VTKProp* module);
    void removeInputModule(MiaImage2VTKProp* module);
    void resetCameraFocus(MiaImage2VTKProp* module);
    void setmouseRightDragInterpretor(MiaInputEventInterpretor* interpretor);

signals:

public slots:
    void removeImageModule();
    void propUpdated();
protected:
    vtkRenderer* Ren1;
    QList <ModulePropPair*> inputModuleList;
    vtkOrientationMarkerWidget* orientationWidget;
    // overloaded mouse press handler
    virtual void mousePressEvent(QMouseEvent* event);
    // overloaded mouse move handler
    virtual void mouseMoveEvent(QMouseEvent* event);
    // overloaded mouse release handler
    virtual void mouseReleaseEvent(QMouseEvent* event);
    MiaInputEventInterpretor* mouseRightDragInterpretor;
    bool rightButtonDown;

};

#endif // IMAGEVIEW3D_H
