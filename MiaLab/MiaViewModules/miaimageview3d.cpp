#include "miaimageview3d.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkCommand.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkConeSource.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProp3D.h"
#include "vtkActor.h"
#include "vtkInteractorStyle.h"
#include "vtkTDxInteractorStyleCamera.h"
#include "vtkTDxInteractorStyleSettings.h"
#include "vtkCamera.h"
#include "vtkAnnotatedCubeActor.h"
#include "vtkProperty.h"
#include "vtkOrientationMarkerWidget.h"


MiaImageView3D::MiaImageView3D(QWidget *parent) :
    QVTKWidget(parent)
{
    vtkRenderWindow* renwin = vtkRenderWindow::New();
//    renwin->StereoCapableWindowOn();

//    // Activate 3DConnexion device only on the left render window.
//    SetUseTDx(true);
//    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
//    iren->SetRenderWindow(renwin);
    SetRenderWindow(renwin);
    renwin->Delete();
    // add a renderer
    Ren1 = vtkRenderer::New();
    GetRenderWindow()->AddRenderer(Ren1);
//    Ren1->SetBackground(1.0,1.0,1.0);



    vtkAnnotatedCubeActor* cube = vtkAnnotatedCubeActor::New();
    cube->SetXPlusFaceText ( "L" );
    cube->SetXMinusFaceText( "R" );
    cube->SetYPlusFaceText ( "P" );
    cube->SetYMinusFaceText( "A" );
    cube->SetZPlusFaceText ( "S" );
    cube->SetZMinusFaceText( "I" );
    cube->SetFaceTextScale( 0.67 );


    vtkProperty* property = cube->GetXPlusFaceProperty();
    property->SetColor(0, 0, 1);
    property = cube->GetXMinusFaceProperty();
    property->SetColor(0, 0, 1);
    property = cube->GetYPlusFaceProperty();
    property->SetColor(0, 1, 0);
    property = cube->GetYMinusFaceProperty();
    property->SetColor(0, 1, 0);
    property = cube->GetZPlusFaceProperty();
    property->SetColor(1, 0, 0);
    property = cube->GetZMinusFaceProperty();
    property->SetColor(1, 0, 0);

    cube->SetTextEdgesVisibility( 1);
    cube->SetCubeVisibility( 1);
    cube->SetFaceTextVisibility( 1);

    orientationWidget = vtkOrientationMarkerWidget::New();
    orientationWidget->SetOrientationMarker( cube );
    orientationWidget->SetInteractor(  renwin->GetInteractor() );
    orientationWidget->SetViewport( 0.90, 0.90, 1, 1);
    orientationWidget->SetEnabled( 1 );
    orientationWidget->InteractiveOff();
    cube->Delete();

    vtkCamera *camera = Ren1->GetActiveCamera();
    camera->SetFocalPoint(0, 0,0);
    camera->SetPosition(600, - 600, 200);

    mouseRightDragInterpretor = NULL;
    rightButtonDown = false;

}
MiaImageView3D::~MiaImageView3D()
{
  if(Ren1)
      Ren1->Delete();
  if(orientationWidget)
      orientationWidget->Delete();
}
void MiaImageView3D::addInputModule(MiaImage2VTKProp* module)
{
    vtkProp3D* aprop = module->getProp();
    if(aprop)
    {
        ModulePropPair* anewpair = new ModulePropPair();
        anewpair->module = module;
        anewpair->prop = aprop;
        Ren1->AddViewProp(aprop);

        inputModuleList.append(anewpair);

        resetCameraFocus( module );
        QObject::connect(module, SIGNAL(imageModuleWillBeDeleted()), this, SLOT(removeImageModule()));
        QObject::connect(module, SIGNAL(outputUpdated()), this, SLOT(propUpdated()));

    }
}
void MiaImageView3D::propUpdated()
{
    update();
}

void MiaImageView3D::resetCameraFocus(MiaImage2VTKProp* module)
{
    vtkCamera *camera = Ren1->GetActiveCamera();
    MiaPoint4D centerpt =  module->getImageCenter();
    double x,y,z;
    camera->GetFocalPoint(x,y,z);
    if( (float)x != centerpt.pos[0] || (float)y != centerpt.pos[1] || (float)z != centerpt.pos[2] )
    {
        camera->SetFocalPoint(centerpt.pos[0], centerpt.pos[1],centerpt.pos[2]);
        camera->SetPosition(centerpt.pos[0] + 600, centerpt.pos[1] - 600, centerpt.pos[2] + 200);
    }
    camera->ComputeViewPlaneNormal();
    camera->SetViewUp(0, 0, 1);
    camera->OrthogonalizeViewUp();
    Ren1->ResetCamera();

    mRenWin->Render();

}

void MiaImageView3D::removeImageModule()
{
    QObject* deletedImageModue = QObject::sender();
    removeInputModule((MiaImage2VTKProp*)deletedImageModue);
}

void MiaImageView3D::removeInputModule(MiaImage2VTKProp* module)
{
    int i;
    for(i = 0; i < inputModuleList.size(); i++)
    {
        ModulePropPair* apair = inputModuleList.at(i);
        if(module == apair->module)
        {
            Ren1->RemoveViewProp(apair->prop);
            delete inputModuleList.takeAt(i);
            break;
        }
    }
}

//void ImageView3D::showCone()
//{

//    const double angleSensitivity=0.02;
//    const double translationSensitivity=0.001;

//    QVTKInteractor *iren=GetInteractor();
//    vtkInteractorStyle *s=
//      static_cast<vtkInteractorStyle *>(iren->GetInteractorStyle());
//    vtkTDxInteractorStyleCamera *t=
//      static_cast<vtkTDxInteractorStyleCamera *>(s->GetTDxStyle());

//    t->GetSettings()->SetAngleSensitivity(angleSensitivity);
//    t->GetSettings()->SetTranslationXSensitivity(translationSensitivity);
//    t->GetSettings()->SetTranslationYSensitivity(translationSensitivity);
//    t->GetSettings()->SetTranslationZSensitivity(translationSensitivity);





//    // add a popup menu for the window and connect it to our slot
////    QMenu* popup1 = new QMenu(ui->qvktview);
////    popup1->addAction("Background White");
////    popup1->addAction("Background Black");
////    popup1->addAction("Stereo Rendering");
////    connect(popup1, SIGNAL(triggered(QAction*)), this, SLOT(color1(QAction*)));

//    // put cone in one window
//    vtkConeSource* cone = vtkConeSource::New();
//    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
//    mapper->SetInput(cone->GetOutput());
//    vtkActor* actor = vtkActor::New();
//    actor->SetMapper(mapper);
//    Ren1->AddViewProp(actor);
//    actor->Delete();
//    mapper->Delete();
//    cone->Delete();

//    update();
//}

void MiaImageView3D::forceUpdate()
{
//    mRenWin->MakeCurrent();
    QObject* module = QObject::sender();
    ModulePropPair* apair = inputModuleList.at(0);
    if(apair)
    resetCameraFocus( apair->module );

}

// overloaded mouse press handler
void MiaImageView3D::mousePressEvent(QMouseEvent* event)
{
    if( event->button() == Qt::RightButton )
    {
        if(mouseRightDragInterpretor)
        {
            mouseRightDragInterpretor->buttonPressedAt( event->pos() );
            rightButtonDown = true;
        }
    }
    else
    {
        QVTKWidget::mousePressEvent(event);
    }
}

// overloaded mouse move handler
void MiaImageView3D::mouseMoveEvent(QMouseEvent* event)
{
    if( event->button() == Qt::RightButton || rightButtonDown)
    {
        if(mouseRightDragInterpretor)
            mouseRightDragInterpretor->mouseMovedAt( event->pos() );
    }
    else
    {
        QVTKWidget::mouseMoveEvent(event);
    }
}
// overloaded mouse release handler
void MiaImageView3D::mouseReleaseEvent(QMouseEvent* event)
{
    if( event->button() == Qt::RightButton )
    {
        if(mouseRightDragInterpretor)
        {
            mouseRightDragInterpretor->buttonReleaseAt( event->pos() );
            rightButtonDown = false;
        }
    }
    else
    {
        QVTKWidget::mouseReleaseEvent(event);
    }
}
void MiaImageView3D::setmouseRightDragInterpretor(MiaInputEventInterpretor* interpretor)
{
    mouseRightDragInterpretor = interpretor;
}

