#include "miabasicmprandvrtnetwork.h"
#include "ui_mainwindow.h"
#include "MiaCommonModules/miaorthogonalslicer.h"
#include "MiaViewModules/miaImage2qimage.h"
#include "MiaCommonModules/miaparameterconnector.h"
#include "MiaViewModules/miainputeventinterpretor.h"
#include "MiaViewModules/miainputeventinterpretorforwindowsetting.h"
#include "MiaViewModules/miaimage2vtkvolume.h"
#include "MiaCommonModules/miafocusvaluereadout.h"

MiaBasicMPRandVRTNetwork::MiaBasicMPRandVRTNetwork(NetWorkEnviromentVariable* var,QObject *parent) :
    MiaNetwork(var,parent)
{
    networkName = "Basic MPR and VRT";
    axialslicer = NULL;
    coronalslicer = NULL;
    saggitalslicer = NULL;
    axialqimage  = NULL;
    coronalqimage  = NULL;
    saggitalqimage  = NULL;
    volumetransformer  = NULL;
    axialCrossInterpretor  = NULL;
    coronalCrossInterpretor  = NULL;
    saggitalCrossInterpretor  = NULL;
    winSettingInterpretor  = NULL;
    win3DSettingInterpretor  = NULL;
    statusReader = NULL;

}

MiaBasicMPRandVRTNetwork::~MiaBasicMPRandVRTNetwork()
{
}

void MiaBasicMPRandVRTNetwork::setupNetwork(MiaModule* input)
{
    MiaNetwork::setupNetwork(input);
    axialslicer = new MiaOrthogonalSlicer(this);
    axialslicer->setOrientation(Axial);
    axialslicer->moduleName.append("axial");
    coronalslicer = new MiaOrthogonalSlicer(this);
    coronalslicer->moduleName.append("coronal");
    coronalslicer->setOrientation(Coronal);
    saggitalslicer = new MiaOrthogonalSlicer(this);
    saggitalslicer->setOrientation(Saggital);
    saggitalslicer->moduleName.append("saggital");

    axialqimage = new MiaImage2QImage(this);
    coronalqimage = new MiaImage2QImage(this);
    saggitalqimage = new MiaImage2QImage(this);
    volumetransformer = new MiaImage2VTKVolume(this);

//image processing pipeline
    //axial view
    axialslicer->setInputModule( inputModule );
    axialqimage->setInputModule( axialslicer );
    //coronal view
    coronalslicer->setInputModule( inputModule );
    coronalqimage->setInputModule( coronalslicer );
    //saggital view
    saggitalslicer->setInputModule( inputModule );
    saggitalqimage->setInputModule( saggitalslicer );

    //3D view
    volumetransformer->setInputModule( inputModule );

    statusReader = new MiaFocusValueReadOut(this);
    statusReader->setInputModule(inputModule);


    miaModuleList.append(axialslicer);
    miaModuleList.append(coronalslicer);
    miaModuleList.append(saggitalslicer);
    miaModuleList.append(axialqimage);
    miaModuleList.append(coronalqimage);
    miaModuleList.append(saggitalqimage);
    miaModuleList.append(statusReader);

}

void MiaBasicMPRandVRTNetwork::setupUserInterface(Ui::MainWindow* ui)
{
    MiaNetwork::setupUserInterface(ui);
    connect(statusReader,SIGNAL(UpdateSummaryText(QString)), userInterface->statusBar, SLOT(showMessage(QString)));
//initial interpretors
    axialCrossInterpretor = new MiaInputEventInterpretor(this);
    axialCrossInterpretor->xValueMethod(SETVALUE);
    axialCrossInterpretor->yValueMethod(SETVALUE);

    coronalCrossInterpretor = new MiaInputEventInterpretor(this);
    coronalCrossInterpretor->xValueMethod(SETVALUE);
    coronalCrossInterpretor->yValueMethod(SETVALUE);

    saggitalCrossInterpretor = new MiaInputEventInterpretor(this);
    saggitalCrossInterpretor->xValueMethod(SETVALUE);
    saggitalCrossInterpretor->yValueMethod(SETVALUE);

    axialPanInterpretor = new MiaInputEventInterpretor(this);
    axialPanInterpretor->xValueMethod(ADDDELTA);
    axialPanInterpretor->yValueMethod(ADDDELTA);

    coronalPanInterpretor = new MiaInputEventInterpretor(this);
    coronalPanInterpretor->xValueMethod(ADDDELTA);
    coronalPanInterpretor->yValueMethod(ADDDELTA);

    saggitalPanInterpretor = new MiaInputEventInterpretor(this);
    saggitalPanInterpretor->xValueMethod(ADDDELTA);
    saggitalPanInterpretor->yValueMethod(ADDDELTA);

    axialZoomInterpretor = new MiaInputEventInterpretor(this);
    axialZoomInterpretor->xValueMethod(ADDDELTA);
    axialZoomInterpretor->yValueMethod(ADDDELTA);

    coronalZoomInterpretor = new MiaInputEventInterpretor(this);
    coronalZoomInterpretor->xValueMethod(ADDDELTA);
    coronalZoomInterpretor->yValueMethod(ADDDELTA);

    saggitalZoomInterpretor = new MiaInputEventInterpretor(this);
    saggitalZoomInterpretor->xValueMethod(ADDDELTA);
    saggitalZoomInterpretor->yValueMethod(ADDDELTA);


    axialWheelInterpretor = new MiaInputEventInterpretor(this);
    axialWheelInterpretor->xValueMethod(SETVALUE);
    axialWheelInterpretor->yValueMethod(SETVALUE);

    coronalWheelInterpretor = new MiaInputEventInterpretor(this);
    coronalWheelInterpretor->xValueMethod(SETVALUE);
    coronalWheelInterpretor->yValueMethod(SETVALUE);

    saggitalWheelInterpretor = new MiaInputEventInterpretor(this);
    saggitalWheelInterpretor->xValueMethod(SETVALUE);
    saggitalWheelInterpretor->yValueMethod(SETVALUE);

    winSettingInterpretor = new MiaInputEventInterpretorForWindowSetting(this);
    win3DSettingInterpretor = new MiaInputEventInterpretorForWindowSetting(this);


    userInterface->leftUpperView->setmouseRightDragInterpretor( winSettingInterpretor );
    userInterface->leftLowerView->setmouseRightDragInterpretor( winSettingInterpretor );
    userInterface->rightUpperView->setmouseRightDragInterpretor( winSettingInterpretor );
    userInterface->rightLowerView->setmouseRightDragInterpretor( win3DSettingInterpretor);
    userInterface->leftUpperView->viewName = "axial view";
    userInterface->rightUpperView->viewName = "coronal view";
    userInterface->leftLowerView->viewName = "saggital view";

//parameter exchanging pipeline
    // crossinterpretor for axial view
    MiaParameterConnector* connector1 = new MiaParameterConnector(this);
    connector1->addParameterPairs(QString("x"),QString("cross_view_x"));
    connector1->addParameterPairs(QString("y"),QString("cross_view_y"));
    connector1->linkModules(axialCrossInterpretor, userInterface->leftUpperView, false);
    connector1->initializeUpperModuleParameter();

    // crossinterpretor for coronal view
    MiaParameterConnector* connector2 = new MiaParameterConnector(this);
    connector2->addParameterPairs(QString("x"),QString("cross_view_x"));
    connector2->addParameterPairs(QString("y"),QString("cross_view_y"));
    connector2->linkModules(coronalCrossInterpretor, userInterface->rightUpperView, false);
    connector2->initializeUpperModuleParameter();

    // crossinterpretor for saggital view
    MiaParameterConnector* connector3 = new MiaParameterConnector(this);
    connector3->addParameterPairs(QString("x"),QString("cross_view_x"));
    connector3->addParameterPairs(QString("y"),QString("cross_view_y"));
    connector3->linkModules(saggitalCrossInterpretor, userInterface->leftLowerView, false);
    connector3->initializeUpperModuleParameter();

    // paninterpretor for axial view
    MiaParameterConnector* connector1p = new MiaParameterConnector(this);
    connector1p->addParameterPairs(QString("x"),QString("translate_x"));
    connector1p->addParameterPairs(QString("y"),QString("translate_y"));
    connector1p->linkModules(axialPanInterpretor, userInterface->leftUpperView, false);


    // paninterpretor for coronal view
    MiaParameterConnector* connector2p = new MiaParameterConnector(this);
    connector2p->addParameterPairs(QString("x"),QString("translate_x"));
    connector2p->addParameterPairs(QString("y"),QString("translate_y"));
    connector2p->linkModules(coronalPanInterpretor, userInterface->rightUpperView, false);

    // paninterpretor for saggital view
    MiaParameterConnector* connector3p = new MiaParameterConnector(this);
    connector3p->addParameterPairs(QString("x"),QString("translate_x"));
    connector3p->addParameterPairs(QString("y"),QString("translate_y"));
    connector3p->linkModules(saggitalPanInterpretor, userInterface->leftLowerView, false);


    // zoominterpretor for axial view
    MiaParameterConnector* connector1z = new MiaParameterConnector(this);
    connector1z->addParameterPairs(QString("y"),QString("scaling_delta"));
    connector1z->linkModules(axialZoomInterpretor, userInterface->leftUpperView, false);


    // zoominterpretor for coronal view
    MiaParameterConnector* connector2z = new MiaParameterConnector(this);
    connector2z->addParameterPairs(QString("y"),QString("scaling_delta"));
    connector2z->linkModules(coronalZoomInterpretor, userInterface->rightUpperView, false);

    // zoominterpretor for saggital view
    MiaParameterConnector* connector3z = new MiaParameterConnector(this);
    connector3z->addParameterPairs(QString("y"),QString("scaling_delta"));
    connector3z->linkModules(saggitalZoomInterpretor, userInterface->leftLowerView, false);

    // wheelinterpretor for axial view
    MiaParameterConnector* connector1w = new MiaParameterConnector(this);
    connector1w->addParameterPairs(QString("y"),QString("scroll_y"));
    connector1w->linkModules(axialWheelInterpretor, axialslicer, false);

    // wheelinterpretor for coronal view
    MiaParameterConnector* connector2w = new MiaParameterConnector(this);
    connector2w->addParameterPairs(QString("y"),QString("scroll_y"));
    connector2w->linkModules(coronalWheelInterpretor, coronalslicer, false);

    // wheelinterpretor for saggital view
    MiaParameterConnector* connector3w = new MiaParameterConnector(this);
    connector3w->addParameterPairs(QString("y"),QString("scroll_y"));
    connector3w->linkModules(saggitalWheelInterpretor, saggitalslicer, false);


    //reslicing position connected to crosshair postion
    MiaParameterConnector* connector4 = new MiaParameterConnector(this);
    connector4->addParameterPairs(QString("cross_pat_x"),QString("pat_x"));
    connector4->addParameterPairs(QString("cross_pat_y"),QString("pat_y"));
    connector4->addParameterPairs(QString("cross_pat_z"),QString("pat_z"));
    connector4->addParameterPairs(QString("cross_pat_t"),QString("pat_t"));
    connector4->linkModules(userInterface->leftUpperView, axialslicer);
    connector4->linkModules(userInterface->rightUpperView, coronalslicer);
    connector4->linkModules(userInterface->leftLowerView, saggitalslicer);
    connector4->linkModules(userInterface->leftLowerView, statusReader);
    connector4->initializeUpperModuleParameter();

    //right dragging connected to window seting
    MiaParameterConnector* connector5 = new MiaParameterConnector(this);
    connector5->addParameterPairs(QString("x"),QString("ww"));
    connector5->addParameterPairs(QString("y"),QString("wl"));
    connector5->linkModules(winSettingInterpretor, axialqimage);
    connector5->linkModules(winSettingInterpretor, coronalqimage);
    connector5->linkModules(winSettingInterpretor, saggitalqimage);
    connector5->linkModules(winSettingInterpretor, statusReader);
    winSettingInterpretor->setWindowSetting(128,256);

    MiaParameterConnector* connector8 = new MiaParameterConnector(this);
    connector8->addParameterPairs(QString("x"),QString("ww"));
    connector8->addParameterPairs(QString("y"),QString("wl"));
    connector8->linkModules(win3DSettingInterpretor, volumetransformer);
    win3DSettingInterpretor->setWindowSetting(300,400);

    MiaImageND* im;
    inputModule->getOutputImage(&im);
    if(im)
    {
        setDefaultWLWW();
        float min, max;
        im->getMaxMin(min,max);
        userInterface->upperThresholdSlider->setMinimum(min-100);
        userInterface->upperThresholdSlider_2->setMinimum(min-100);
        userInterface->upperThresholdSpin->setMinimum(min-100);
        userInterface->upperThresholdSpin_2->setMinimum(min-100);

        userInterface->upperThresholdSlider->setMaximum(max+100);
        userInterface->upperThresholdSlider_2->setMaximum(max+100);
        userInterface->upperThresholdSpin->setMaximum(max+100);
        userInterface->upperThresholdSpin_2->setMaximum(max+100);

        userInterface->lowerThresholdSlider->setMinimum(min-100);
        userInterface->lowerThresholdSlider_2->setMinimum(min-100);
        userInterface->lowerThresholdSpin->setMinimum(min-100);
        userInterface->lowerThresholdSpin_2->setMinimum(min-100);

        userInterface->lowerThresholdSlider->setMaximum(max+100);
        userInterface->lowerThresholdSlider_2->setMaximum(max+100);
        userInterface->lowerThresholdSpin->setMaximum(max+100);
        userInterface->lowerThresholdSpin_2->setMaximum(max+100);


        if(im->getDimension()==3)
        {
            userInterface->leftUpperView->addInputModule(axialqimage);
            userInterface->rightUpperView->addInputModule(coronalqimage);
            userInterface->leftLowerView->addInputModule(saggitalqimage);
            userInterface->rightLowerView->addInputModule(volumetransformer);
            userInterface->leftLowerView->resetViewSize();
            userInterface->leftUpperView->resetViewSize();
            userInterface->rightUpperView->resetViewSize();
            axialslicer->resetPositionToCenter();
            coronalslicer->resetPositionToCenter();
            saggitalslicer->resetPositionToCenter();
            userInterface->rightLowerView->forceUpdate();
        }
        else if(im->getDimension()==2)
        {
            userInterface->leftUpperView->addInputModule(axialqimage);
            userInterface->leftUpperView->showCrossHair(false);
            userInterface->leftLowerView->setVisible(false);
            userInterface->rightUpperView->setVisible(false);
            userInterface->rightLowerView->setVisible(false);
            userInterface->gridLayout->removeWidget(userInterface->leftLowerView);
            userInterface->gridLayout->removeWidget(userInterface->rightUpperView);
            userInterface->gridLayout->removeWidget(userInterface->rightLowerView);
            axialslicer->resetPositionToCenter();
            userInterface->leftUpperView->resetViewSize();
        }
    }
    enableInteractions();

}
void MiaBasicMPRandVRTNetwork::enableInteractions()
{
    userInterface->actionWindow_Setting->setEnabled(true);
    userInterface->actionPan->setEnabled(true);
    userInterface->actionZoom->setEnabled(true);
    //connect toolbar
    connect(userInterface->actionWindow_Setting, SIGNAL(triggered()), this, SLOT(setInteractionToWindowSetting()));
    connect(userInterface->actionPan, SIGNAL(triggered()), this, SLOT(setInteractionToPan()));
    connect(userInterface->actionZoom, SIGNAL(triggered()), this, SLOT(setInteractionToZoom()));

    // wheel interaction is set to scroll
    userInterface->leftUpperView->setWheelInterpretor( axialWheelInterpretor );
    userInterface->leftLowerView->setWheelInterpretor( saggitalWheelInterpretor );
    userInterface->rightUpperView->setWheelInterpretor( coronalWheelInterpretor );
}

void MiaBasicMPRandVRTNetwork::changeUserInteractionMode(InteractionMode mode)
{
    switch(mode)
    {
    case MIA_Interaction_None:
        userInterface->leftUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( NULL );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftUpperView->setCursor(Qt::ArrowCursor);
        userInterface->leftLowerView->setCursor(Qt::ArrowCursor);
        userInterface->rightUpperView->setCursor(Qt::ArrowCursor);
        break;
    case MIA_Interaction_Window_Setting:
        userInterface->leftUpperView->setmouseLeftDragInterpretor( winSettingInterpretor );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( winSettingInterpretor );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( winSettingInterpretor );
        userInterface->leftUpperView->setCursor(MiaImageView2D::getWinCursor());
        userInterface->leftLowerView->setCursor(MiaImageView2D::getWinCursor());
        userInterface->rightUpperView->setCursor(MiaImageView2D::getWinCursor());
//        userInterface->rightLowerView->setmouseLeftDragInterpretor( win3DSettingInterpretor);
        break;
    case MIA_Interaction_Pan:
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialPanInterpretor );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalPanInterpretor );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalPanInterpretor );
        userInterface->leftUpperView->setCursor(MiaImageView2D::getPanCursor());
        userInterface->leftLowerView->setCursor(MiaImageView2D::getPanCursor());
        userInterface->rightUpperView->setCursor(MiaImageView2D::getPanCursor());
//        userInterface->rightLowerView->setmouseLeftDragInterpretor( NULL );
        break;
    case MIA_Interaction_Zoom:
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialZoomInterpretor );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalZoomInterpretor );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalZoomInterpretor );
        userInterface->leftUpperView->setCursor(MiaImageView2D::getZoomCursor());
        userInterface->leftLowerView->setCursor(MiaImageView2D::getZoomCursor());
        userInterface->rightUpperView->setCursor(MiaImageView2D::getZoomCursor());
//        userInterface->rightLowerView->setmouseLeftDragInterpretor( NULL );
        break;
    default:
        qDebug()<<"unsupported interaction mode";
    }
}

void MiaBasicMPRandVRTNetwork::setWLWW(float wl, float ww)
{
    winSettingInterpretor->setWindowSetting(wl,ww);
}

void MiaBasicMPRandVRTNetwork::setDefaultWLWW()
{
    MiaImageND* im;
    inputModule->getOutputImage(&im);
    if(im)
    {

        winSettingInterpretor->setWindowSetting(im->defaultWindowCenter,im->defaultWindowWidth);
        qDebug()<< im->defaultWindowCenter << im->defaultWindowWidth;
    }
}

void MiaBasicMPRandVRTNetwork::destroyNetwork()
{
    changeUserInteractionMode(MIA_Interaction_None);
    MiaNetwork::destroyNetwork();
}
