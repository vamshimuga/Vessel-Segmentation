#include "miathresholdlevelsetnetwork.h"
#include "ui_mainwindow.h"
#include "MiaCommonModules/miaorthogonalslicer.h"
#include "MiaViewModules/miaImage2qimage.h"
#include "MiaCommonModules/miaparameterconnector.h"
#include "MiaViewModules/miainputeventinterpretor.h"
#include "MiaViewModules/miaimage2vtkvolume.h"
#include "MiaViewModules/miaimage2vtkisosurface.h"
#include "MiaCommonModules/miathresholding.h"
#include "MiaViewModules/mialutmodule.h"
#include "MiaViewModules/miadrawingmanager.h"
#include "MiaMarkers/miamarker3dbox.h"
#include "MiaViewModules/miamarker2graphicsitem.h"
#include "listviewwithkeyboardsupport.h"
#include "MiaLevelSetModules/miasfthresholdlevelsetsegmenter.h"
#include "MiaLevelSetModules/miadistancetransformfrom3dmarkers.h"
#include "MiaCommonModules/miaintensitymappingfilter.h"
#include "MiaViewModules/miaimageexporter.h"
#include "MiaCommonModules/mialevelsetfunctionmanualmodifier.h"
#include "MiaCommonModules/miagradient3dfilter.h"
#include "MiaCommonModules/miaresample3d.h"
#include "MiaViewModules/miainputeventinterpretorforwindowsetting.h"
#include "MiaCommonModules/miaregionstatistics.h"
#include "MiaViewModules/miaimagereader.h"
#include <QtGui>
#include <QFileDialog>
#include <QProgressDialog>
#include <QMessageBox>
MiaThresholdLevelsetNetwork::MiaThresholdLevelsetNetwork(NetWorkEnviromentVariable* var,QObject *parent) :
    MiaBasicMPRandVRTNetwork(var,parent)
{
    networkName = "Threshold base Level set network";
    exporter = NULL;
    axialLevelSetSlicer = NULL;
    coronalLevelSetSlicer  = NULL;
    saggitalLevelSetSlicer  = NULL;
    axialLevelSetQimage  = NULL;
    coronalLevelSetQimage  = NULL;
    saggitalLevelSetQimage  = NULL;

    axialViewThresholder  = NULL;
    axialViewMask  = NULL;
    coronalViewThresholder  = NULL;
    coronalViewMask  = NULL;
    saggitalViewThresholder  = NULL;
    saggitalViewMask  = NULL;
    axialmasklut  = NULL;
    coronalmasklut = NULL;
    saggitalmasklut  = NULL;

    axialMarkerInterpretor  = NULL;
    coronalMarkerInterpretor  = NULL;
    saggitalMarkerInterpretor  = NULL;

    axialStopMarkerInterpretor  = NULL;
    coronalStopMarkerInterpretor  = NULL;
    saggitalStopMarkerInterpretor  = NULL;
    seedMarkerManager  = NULL;
    stopMarkerManager  = NULL;
    axialViewMarkerAgent  = NULL;
    coronalViewMarkerAgent  = NULL;
    saggitalViewMarkerAgent  = NULL;
    axialViewStopMarkerAgent  = NULL;
    coronalViewStopMarkerAgent  = NULL;
    saggitalViewStopMarkerAgent  = NULL;
    distanceTranformer  = NULL;
    lowResolutionDistanceTranformer  = NULL;
    lsSegmenter  = NULL;
    lowResolutionLSSegmenter  = NULL;
    mappingFilter  = NULL;
    lowResolutionMappingFilter  = NULL;
    lsModifier  = NULL;
    lowresolutionLSModifier  = NULL;
    gradientFilter  = NULL;
    downResampler  = NULL;
    upResampler  = NULL;
    isosurface  = NULL;

    //initialize parameters
    isMemoryInitialized = false;
    currentStep = -1;
    currentResolution = 0;
    isDrawingSeeds = true;
    isAllowEditingBlocker = false;

    seedRegionRemappingFilter = NULL;
    seedRegionReader = NULL;
}


void MiaThresholdLevelsetNetwork::setupNetwork(MiaModule* input)
{
    MiaBasicMPRandVRTNetwork::setupNetwork(input);

    exporter = new MiaImageExporter(this);
    intermedium_data_exporter = new MiaImageExporter(this);
    isosurface = new MiaImage2VTKIsosurface(this);

    axialLevelSetSlicer = new MiaOrthogonalSlicer(this) ;
    axialLevelSetSlicer->setOrientation(Axial);
    axialLevelSetSlicer->autoUpdateWhenParameterChanged = false;
    coronalLevelSetSlicer = new MiaOrthogonalSlicer(this);
    coronalLevelSetSlicer->setOrientation(Coronal);
    coronalLevelSetSlicer->autoUpdateWhenParameterChanged = false;
    saggitalLevelSetSlicer = new MiaOrthogonalSlicer(this);
    saggitalLevelSetSlicer->setOrientation(Saggital);
    saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = false;


    axialLevelSetQimage = new MiaImage2QImage(this);
    coronalLevelSetQimage = new MiaImage2QImage(this) ;
    saggitalLevelSetQimage = new MiaImage2QImage(this) ;

    axialViewThresholder = new MiaThresholding(this);
    axialViewMask = new MiaImage2QImage(this);
    coronalViewThresholder = new MiaThresholding(this);
    coronalViewMask = new MiaImage2QImage(this);
    saggitalViewThresholder = new MiaThresholding(this);
    saggitalViewMask = new MiaImage2QImage(this);
    axialmasklut = new MiaLUTModule(LUT_BinaryColor,this);
    axialViewMask->setCurrentLUT(axialmasklut);
    coronalmasklut = new MiaLUTModule(LUT_BinaryColor,this);
    coronalViewMask->setCurrentLUT(coronalmasklut);
    saggitalmasklut = new MiaLUTModule(LUT_BinaryColor,this);
    saggitalViewMask->setCurrentLUT(saggitalmasklut);

    seedMarkerManager = new MiaDrawingManager(this);
    seedMarkerManager->currentType = MIA_3DBall;
    stopMarkerManager = new MiaDrawingManager(this);
    stopMarkerManager->currentColor = QColor(Qt::yellow);
    axialViewMarkerAgent = new MiaMarker2GraphicsItem(this);
    coronalViewMarkerAgent = new MiaMarker2GraphicsItem(this);
    saggitalViewMarkerAgent = new MiaMarker2GraphicsItem(this);
    axialViewStopMarkerAgent = new MiaMarker2GraphicsItem(this);
    coronalViewStopMarkerAgent = new MiaMarker2GraphicsItem(this);
    saggitalViewStopMarkerAgent = new MiaMarker2GraphicsItem(this);
    distanceTranformer = new MiaDistanceTransformFrom3DMarkers(this);
    lowResolutionDistanceTranformer = new MiaDistanceTransformFrom3DMarkers(this);
    lsSegmenter = new MiaSFThresholdLevelSetSegmenter(this);
    lowResolutionLSSegmenter = new MiaSFThresholdLevelSetSegmenter(this);
    lowResolutionLSSegmenter->setInitialMaxStepLength(0.4);
    mappingFilter = new MiaIntensityMappingFilter(this);
    lowResolutionMappingFilter = new MiaIntensityMappingFilter(this);
    lsModifier = new MiaLevelSetFunctionManualModifier(this);
    lowresolutionLSModifier = new MiaLevelSetFunctionManualModifier(this);
    downResampler = new MiaResample3D(this);
//    downResampler->setOutputSize(100,100,100);
    downResampler->setResampleScale(0.5,0.5,0.5);
    upResampler = new MiaResample3D(this);
    statistics = new MiaRegionStatistics(this);
    MiaImageND* im;
    inputModule->getOutputImage(&im);
    upResampler->setOutputSize(im->getWidth(), im->getHeight(), im->getDepth());


    MiaLUTModule* lsResultLUT = new MiaLUTModule(LUT_SingleColor, this);
    lsResultLUT->removeAllKeyPoints();
    QColor incolor(Qt::green);
    incolor.setAlphaF(0.4);
    lsResultLUT->insertKeyPoint(0,incolor);
    QColor outcolor(Qt::green);
    outcolor.setAlphaF(0);
    lsResultLUT->insertKeyPoint(1.0,outcolor);
    axialLevelSetQimage->setCurrentLUT(lsResultLUT);
    coronalLevelSetQimage->setCurrentLUT(lsResultLUT);
    saggitalLevelSetQimage->setCurrentLUT(lsResultLUT);

    //image processing pipeline

    //Thresholding
    axialViewThresholder->setInputModule( axialslicer );
    axialViewMask->setInputModule( axialViewThresholder );

    coronalViewThresholder->setInputModule( coronalslicer );
    coronalViewMask->setInputModule( coronalViewThresholder );

    saggitalViewThresholder->setInputModule( saggitalslicer );
    saggitalViewMask->setInputModule( saggitalViewThresholder );

    //level set pipeline
    lsSegmenter->setInputModule(lsModifier);
    lsSegmenter->setLevelSetFunctionInputModule(distanceTranformer);
//    lsSegmenter->setAdvectionFunctionInputModule(input);
    lsModifier->setInputModule(mappingFilter);
    mappingFilter->setInputModule(inputModule);

    isosurface->setInputModule( lsSegmenter );

    QObject::connect(mappingFilter,SIGNAL(modified()),lsModifier,SLOT(reLatedModuleModified()));
    QObject::connect(lsModifier,SIGNAL(modified()),mappingFilter,SLOT(reLatedModuleModified()));
    distanceTranformer->setInputModule(inputModule);
    seedMarkerManager->connectToDownStreamAgent(distanceTranformer);
    QObject::connect(distanceTranformer, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),seedMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));
    stopMarkerManager->connectToDownStreamAgent(lsModifier);
    QObject::connect(lsModifier, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),stopMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));

    downResampler->setInputModule( inputModule );
    upResampler->setInputModule(lowResolutionLSSegmenter);
    axialLevelSetSlicer->setInputModule( lsSegmenter );
    coronalLevelSetSlicer->setInputModule( lsSegmenter );
    saggitalLevelSetSlicer->setInputModule( lsSegmenter );
    axialLevelSetQimage->setInputModule( axialLevelSetSlicer);
    coronalLevelSetQimage->setInputModule( coronalLevelSetSlicer);
    saggitalLevelSetQimage->setInputModule( saggitalLevelSetSlicer );
    exporter->setInputModule(lsSegmenter);
    intermedium_data_exporter->setInputModule(lsModifier);
    QObject::connect(lsSegmenter,SIGNAL(levelsetFunctionUpdated()),this,SLOT(levelsetFunctionUpdated()));
    QObject::connect(lsSegmenter,SIGNAL(levelsetFunctionCompleted()),this,SLOT(levelsetFunctionCompleted()));
    QObject::connect(lsSegmenter,SIGNAL(levelsetEnterNewPeriod(int)),this,SLOT(levelsetEnterNewPeriod(int)));

    // low Resolution version
    lowResolutionLSSegmenter->setInputModule(lowresolutionLSModifier);
    lowResolutionLSSegmenter->setLevelSetFunctionInputModule(lowResolutionDistanceTranformer);
    lowresolutionLSModifier->setInputModule(lowResolutionMappingFilter);
    lowResolutionMappingFilter->setInputModule(downResampler);

    QObject::connect(lowResolutionMappingFilter,SIGNAL(modified()),lowresolutionLSModifier,SLOT(reLatedModuleModified()));
    QObject::connect(lowresolutionLSModifier,SIGNAL(modified()),lowResolutionMappingFilter,SLOT(reLatedModuleModified()));
    lowResolutionDistanceTranformer->setInputModule(downResampler);
    seedMarkerManager->connectToDownStreamAgent(lowResolutionDistanceTranformer);
    QObject::connect(lowResolutionDistanceTranformer, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),seedMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));
    stopMarkerManager->connectToDownStreamAgent(lowresolutionLSModifier);
    QObject::connect(lowresolutionLSModifier, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),stopMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));



    QObject::connect(lowResolutionLSSegmenter,SIGNAL(levelsetFunctionUpdated()),this,SLOT(levelsetFunctionUpdated()));
    QObject::connect(lowResolutionLSSegmenter,SIGNAL(levelsetFunctionCompleted()),this,SLOT(levelsetFunctionCompleted()));
    QObject::connect(lowResolutionLSSegmenter,SIGNAL(levelsetEnterNewPeriod(int)),this,SLOT(levelsetEnterNewPeriod(int)));

    //statistics
    statistics->setInputModule(inputModule);
    statistics->addRegion("",-MAX_FLOAT,0.0000001f);
    statistics->setMaskImageInputModule(lsSegmenter);


    miaModuleList.append(axialLevelSetSlicer);
    miaModuleList.append(coronalLevelSetSlicer);
    miaModuleList.append(saggitalLevelSetSlicer);
    miaModuleList.append(axialLevelSetQimage);
    miaModuleList.append(coronalLevelSetQimage);
    miaModuleList.append(saggitalLevelSetQimage);
    miaModuleList.append(axialViewMask);
    miaModuleList.append(axialViewThresholder);
    miaModuleList.append(coronalViewMask);
    miaModuleList.append(coronalViewThresholder);
    miaModuleList.append(saggitalViewMask);
    miaModuleList.append(saggitalViewThresholder);
    miaModuleList.append(seedMarkerManager);
//      miaModuleList.append(stopMarkerManager);
    miaModuleList.append(axialViewMarkerAgent);
    miaModuleList.append(coronalViewMarkerAgent);
    miaModuleList.append(saggitalViewMarkerAgent);
    miaModuleList.append(axialViewStopMarkerAgent);
    miaModuleList.append(coronalViewStopMarkerAgent);
    miaModuleList.append(saggitalViewStopMarkerAgent);
    miaModuleList.append(distanceTranformer);
    miaModuleList.append(lowResolutionDistanceTranformer);
    miaModuleList.append(lsModifier);
    miaModuleList.append(lowresolutionLSModifier);
    miaModuleList.append(lsSegmenter);
    miaModuleList.append(lowResolutionLSSegmenter);
    miaModuleList.append(mappingFilter);
    miaModuleList.append(lowResolutionMappingFilter);
    miaModuleList.append(exporter);
    miaModuleList.append(downResampler);
    miaModuleList.append(upResampler);
    outputModule = lsSegmenter;

}

void MiaThresholdLevelsetNetwork::setupUserInterface(Ui::MainWindow* ui)
{
    MiaBasicMPRandVRTNetwork::setupUserInterface(ui);
    userInterface->toolBox->addItem(ui->step2,QString("Step 2: Define Seeds"));
    userInterface->toolBox->addItem(ui->step3,QString("Step 3: Propagation"));
    userInterface->toolBox->addItem(ui->step4,QString("Step 4: Save Results"));
    userInterface->step2->setVisible(true);
    userInterface->step3->setVisible(true);
    userInterface->step4->setVisible(true);
    userInterface->startFuzzyConectednessButton->setVisible(false);
    //basic setups
    userInterface->blockerGroupBox->setVisible(false);
    userInterface->maxiterationSpinBox->setValue(15000);
//    userInterface->maxiterationSpinBox->setVisible(false);
    connect(userInterface->nextstepBotton1,SIGNAL(clicked()), this, SLOT(nextStep()));
    connect(userInterface->nextstepBotton2,SIGNAL(clicked()), this, SLOT(nextStep()));
    connect(userInterface->nextstepBotton3,SIGNAL(clicked()), this, SLOT(nextStep()));
    connect(userInterface->prestepButton2,SIGNAL(clicked()), this, SLOT(previousStep()));
    connect(userInterface->prestepButton3,SIGNAL(clicked()), this, SLOT(previousStep()));
    connect(userInterface->prestepButton4,SIGNAL(clicked()), this, SLOT(previousStep()));
    connect(userInterface->toolBox,SIGNAL(currentChanged(int)), this, SLOT(switchToStep(int)));
    connect(userInterface->showBlockerCheckBox,SIGNAL(clicked(bool)),this, SLOT(showBlockerBox(bool)) );
    connect(userInterface->blockerEditCheckBox,SIGNAL(clicked(bool)),this, SLOT(allowBlockerEditing(bool)) );
    connect(userInterface->startButton, SIGNAL(clicked()), this, SLOT(startLevelSetSegmentation()));
    connect(userInterface->stopButton, SIGNAL(clicked()), this, SLOT(stopLevelSetSegmentation()));
    connect(userInterface->saveButton, SIGNAL(clicked()), this, SLOT(saveSegmentedResult()));
    connect(userInterface->saveTextButton, SIGNAL(clicked()), this, SLOT(saveRegionToTextFile()));
    connect(userInterface->saveSTLButton, SIGNAL(clicked()), this, SLOT(exportSTLFile()));
    connect(userInterface->seedButton, SIGNAL(clicked()), this, SLOT(switchDrawingMode()));
    connect(userInterface->smoothFactorSlider, SIGNAL(valueChanged(int)), this, SLOT(setSmoothFactor(int)));
    connect(userInterface->smoothFactorSpin, SIGNAL(valueChanged(double)), this, SLOT(setSmoothFactor(double)));
    connect(userInterface->saveSeedButton, SIGNAL(clicked()),this,SLOT(saveMarkers()));
    connect(userInterface->loadSeedButton, SIGNAL(clicked()),this,SLOT(loadMarkers()));
    connect(userInterface->speedBoostingHelpButton, SIGNAL(clicked()),this,SLOT(showMessageBoxOfSpeedBoosting()));
// link between modules
    //mouse to drawing
    axialMarkerInterpretor = new MiaInputEventInterpretor(this);
    axialMarkerInterpretor->xValueMethod(FORWARDMESSAGE);
    axialMarkerInterpretor->yValueMethod(FORWARDMESSAGE);

    coronalMarkerInterpretor = new MiaInputEventInterpretor(this);
    coronalMarkerInterpretor->xValueMethod(FORWARDMESSAGE);
    coronalMarkerInterpretor->yValueMethod(FORWARDMESSAGE);

    saggitalMarkerInterpretor = new MiaInputEventInterpretor(this);
    saggitalMarkerInterpretor->xValueMethod(FORWARDMESSAGE);
    saggitalMarkerInterpretor->yValueMethod(FORWARDMESSAGE);

   axialStopMarkerInterpretor = new MiaInputEventInterpretor(this);
   axialStopMarkerInterpretor->xValueMethod(FORWARDMESSAGE);
   axialStopMarkerInterpretor->yValueMethod(FORWARDMESSAGE);

   coronalStopMarkerInterpretor = new MiaInputEventInterpretor(this);
   coronalStopMarkerInterpretor->xValueMethod(FORWARDMESSAGE);
   coronalStopMarkerInterpretor->yValueMethod(FORWARDMESSAGE);

   saggitalStopMarkerInterpretor = new MiaInputEventInterpretor(this);
   saggitalStopMarkerInterpretor->xValueMethod(FORWARDMESSAGE);
   saggitalStopMarkerInterpretor->yValueMethod(FORWARDMESSAGE);

//parameter exchanging pipeline
   // connecting drawing manager
   axialMarkerInterpretor->forwardMessageTo(seedMarkerManager);
   coronalMarkerInterpretor->forwardMessageTo(seedMarkerManager);
   saggitalMarkerInterpretor->forwardMessageTo(seedMarkerManager);

   axialStopMarkerInterpretor->forwardMessageTo(stopMarkerManager);
   coronalStopMarkerInterpretor->forwardMessageTo(stopMarkerManager);
   saggitalStopMarkerInterpretor->forwardMessageTo(stopMarkerManager);

   seedMarkerManager->connectToDownStreamAgent(axialViewMarkerAgent);
   seedMarkerManager->connectToDownStreamAgent(coronalViewMarkerAgent);
   seedMarkerManager->connectToDownStreamAgent(saggitalViewMarkerAgent);

   stopMarkerManager->connectToDownStreamAgent(axialViewStopMarkerAgent);
   stopMarkerManager->connectToDownStreamAgent(coronalViewStopMarkerAgent);
   stopMarkerManager->connectToDownStreamAgent(saggitalViewStopMarkerAgent);

   //reslicing position connected to crosshair postion
   MiaParameterConnector* connector4 = new MiaParameterConnector(this);
   connector4->addParameterPairs(QString("cross_pat_x"),QString("pat_x"));
   connector4->addParameterPairs(QString("cross_pat_y"),QString("pat_y"));
   connector4->addParameterPairs(QString("cross_pat_z"),QString("pat_z"));
   connector4->addParameterPairs(QString("cross_pat_t"),QString("pat_t"));
   connector4->linkModules(userInterface->leftUpperView, axialLevelSetSlicer);
   connector4->linkModules(userInterface->rightUpperView, coronalLevelSetSlicer);
   connector4->linkModules(userInterface->leftLowerView, saggitalLevelSetSlicer);
   connector4->linkModules(seedMarkerManager, saggitalslicer);
   connector4->linkModules(stopMarkerManager, saggitalslicer);
//   connector4->initializeUpperModuleParameter();
   userInterface->leftUpperView->notifyCrossHairChanged();

   //thresholding
   MiaInputEventInterpretor* thresholdingInterpretor = new MiaInputEventInterpretor(this);
   QObject::connect(userInterface->lowerThresholdSlider_2, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateXValue(int)));
   QObject::connect(userInterface->upperThresholdSlider_2, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateYValue(int)));
   QObject::connect(userInterface->lowerThresholdSpin_2, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateXValue(int)));
   QObject::connect(userInterface->upperThresholdSpin_2, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateYValue(int)));
   QObject::connect(userInterface->lowerThresholdSlider, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateXValue(int)));
   QObject::connect(userInterface->upperThresholdSlider, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateYValue(int)));
   QObject::connect(userInterface->lowerThresholdSpin, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateXValue(int)));
   QObject::connect(userInterface->upperThresholdSpin, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateYValue(int)));


   MiaParameterConnector* connector6 = new MiaParameterConnector(this);
   connector6->addParameterPairs( QString("x"), QString("lower_threshold"));
   connector6->addParameterPairs( QString("y"), QString("upper_threshold"));
   connector6->linkModules(thresholdingInterpretor, axialViewThresholder, false);
   connector6->linkModules(thresholdingInterpretor, coronalViewThresholder, false);
   connector6->linkModules(thresholdingInterpretor, saggitalViewThresholder, false);
   connector6->linkModules(thresholdingInterpretor, mappingFilter, false);
   connector6->linkModules(thresholdingInterpretor, lowResolutionMappingFilter, false);


   userInterface->markerListView->setModel(seedMarkerManager);
   seedMarkerManager->setSelectionModel( userInterface->markerListView->selectionModel() );

   userInterface->stopMarkerListView->setModel(stopMarkerManager);
   stopMarkerManager->setSelectionModel( userInterface->stopMarkerListView->selectionModel() );

   userInterface->stopMarkerListView_2->setModel(stopMarkerManager);
   userInterface->stopMarkerListView_2->setSelectionModel(userInterface->stopMarkerListView->selectionModel());




   thresholdingInterpretor->updateXValue(userInterface->lowerThresholdSlider->value());
   thresholdingInterpretor->updateYValue(userInterface->upperThresholdSlider->value());

   MiaImageND* im;
   inputModule->getOutputImage(&im);
   if(im)
   {   if(im->getDimension()==3)
       {
           userInterface->rightLowerView->addInputModule(isosurface);
       }
       else if(im->getDimension()==2)
       {
            userInterface->multi_Resolution_CheckBox->setEnabled(false);
       }
   }

   connect(userInterface->actionCurve_ROI, SIGNAL(triggered()), this, SLOT(onDrawingToolSelected()));
   connect(userInterface->smoothingIterationSpinBox, SIGNAL(valueChanged(int)), isosurface, SLOT(setSmoothIterationss(int)));

}

void MiaThresholdLevelsetNetwork::activeUserInteraction()
{
    this->switchToStep(0);
}

void MiaThresholdLevelsetNetwork::deactiveUserInteraction()
{
    this->switchToStep(-1);

}

void MiaThresholdLevelsetNetwork::startSegmentationAutomatically(QString seedFile, float seedThreshld, QList<QVariant> &lut, float curvaturef)
{
    mappingFilter->setToLUTMapping(lut);
    mappingFilter->update();
    setSmoothFactor(curvaturef);
    lsSegmenter->setCurvatureWeight(curvaturef);
    qDebug() << "try to load seed" << seedFile;
    if(seedFile == "FullVolume")
    {
        MiaImageND* im;
        inputModule->getOutputImage(&im);
        int* dimsize = im->getDimSize();
        int margin_size = 2;
        MiaPoint4D pt0(margin_size,margin_size,margin_size, 0);
        MiaPoint4D pt = im->convertVoxelToPatientCoordinate(pt0);
        MiaMarker3DBox seedbox(pt,seedMarkerManager);
        pt0.pos[1] = dimsize[1]-1-margin_size;
        pt = im->convertVoxelToPatientCoordinate(pt0);
        seedbox.keyPointList[1] = pt;

        pt0.pos[0] = dimsize[0]-1-margin_size;
        pt = im->convertVoxelToPatientCoordinate(pt0);
        seedbox.keyPointList[2] = pt;

        pt0.pos[1] = margin_size;
        pt = im->convertVoxelToPatientCoordinate(pt0);
        seedbox.keyPointList[3] = pt;

        pt0.pos[0] = margin_size;
        pt0.pos[2] = dimsize[2]-1-margin_size;

        pt = im->convertVoxelToPatientCoordinate(pt0);
        seedbox.keyPointList[4] = pt;

        pt0.pos[1] = dimsize[1]-1-margin_size;
        pt = im->convertVoxelToPatientCoordinate(pt0);
        seedbox.keyPointList[5] = pt;

        pt0.pos[0] = dimsize[0]-1-margin_size;
        pt = im->convertVoxelToPatientCoordinate(pt0);
        seedbox.keyPointList[6] = pt;

        pt0.pos[1] = margin_size;
        pt = im->convertVoxelToPatientCoordinate(pt0);
        seedbox.keyPointList[7] = pt;

        for(int i = 0; i < seedbox.keyPointList.count(); i++)
        {
            qDebug() << seedbox.keyPointList[i].pos[0] << seedbox.keyPointList[i].pos[1] << seedbox.keyPointList[i].pos[2];
        }

        seedMarkerManager->addMarker(&seedbox);
    }
    else
    {
        seedRegionReader = new MiaImageReader(this);
        seedRegionRemappingFilter = new MiaIntensityMappingFilter(this);
        seedRegionRemappingFilter->setMultiplyFactor(1.0, seedThreshld);
        seedRegionRemappingFilter->selectedMappingMethod = MIA_Multiply;
        seedRegionRemappingFilter->setInputModule(seedRegionReader);
        lsSegmenter->setLevelSetFunctionInputModule(seedRegionRemappingFilter);
        seedRegionReader->setFileName(seedFile,MIA_MHD);
        seedRegionReader->update();
        seedRegionRemappingFilter->update();
    }
//    lsSegmenter->setAdvectionFunctionInputModule(inputModule);
    isMemoryInitialized = true;
    lsSegmenter->update();
    switchToStep(2);
    startLevelSetSegmentation();

}

void MiaThresholdLevelsetNetwork::setLevelSetToUsingMultiResolution(bool seton)
{
    if(seton)
    {
        lsSegmenter->setLevelSetFunctionInputModule(upResampler);
        if(lowResolutionLSSegmenter!= NULL)
        {
            lowResolutionDistanceTranformer->update();
            lowResolutionLSSegmenter->update();
            lowResolutionLSSegmenter->setMaxIteration(userInterface->maxiterationSpinBox->value());
        }
    }
    else
    {
        lsSegmenter->setLevelSetFunctionInputModule(distanceTranformer);
    }
}

void MiaThresholdLevelsetNetwork::startLevelSetSegmentation()
{
      switchToSegmentingMode(true);
      userInterface->LSStatus->setText(QString("Starting..."));
      userInterface->progressBar->setValue(0);
      lsSegmenter->setMaxIteration(userInterface->maxiterationSpinBox->value());
      if(userInterface->multi_Resolution_CheckBox->isChecked() && lowResolutionLSSegmenter!= NULL)
      {
          setLevelSetToUsingMultiResolution(true);
          axialslicer->setInputModule( downResampler );
          coronalslicer->setInputModule( downResampler );
          saggitalslicer->setInputModule( downResampler );
          axialLevelSetSlicer->setInputModule( lowResolutionLSSegmenter );
          coronalLevelSetSlicer->setInputModule( lowResolutionLSSegmenter );
          saggitalLevelSetSlicer->setInputModule( lowResolutionLSSegmenter );
          currentResolution = 1;
          lowResolutionLSSegmenter->update();
          lowResolutionLSSegmenter->start();
      }
      else
      {
          currentResolution = 0;
          lsSegmenter->update();
          lsSegmenter->start();
      }

}
void MiaThresholdLevelsetNetwork::stopLevelSetSegmentation()
{

    if(currentResolution == 0)
    {
        lsSegmenter->stop();
    }
    else
    {
        if(lowResolutionLSSegmenter)
        {
            lowResolutionLSSegmenter->stop();

        }
    }
}
void MiaThresholdLevelsetNetwork::switchToHigherResolution()
{
    upResampler->update();
    lsSegmenter->update();
    axialslicer->setInputModule( inputModule );
    coronalslicer->setInputModule( inputModule );
    saggitalslicer->setInputModule( inputModule );
    axialLevelSetSlicer->setInputModule( lsSegmenter );
    coronalLevelSetSlicer->setInputModule( lsSegmenter );
    saggitalLevelSetSlicer->setInputModule( lsSegmenter );

}
void MiaThresholdLevelsetNetwork::switchToStep( int step )
{
    if(step == currentStep)
        return;
    else
    {
        int lastStep = currentStep;
        currentStep = step;
        if(userInterface->toolBox->currentIndex() != currentStep)
            userInterface->toolBox->setCurrentIndex ( currentStep );

        switch(lastStep)
        {
        case 0:

            setToNavigatingMode( false );
            break;
        case 1:
            setToMarkerDrawingMode( false );
            break;
        case 2:
            setToLevelsetPropagationMode( false );
            break;
        case 3:
            setToResultMode( false );
            break;
        default:
            break;
        }


    }

    switch(step)
    {
    case 0:

        setToNavigatingMode( true );
        break;
    case 1:
        setToMarkerDrawingMode( true );
        break;
    case 2:
        setToLevelsetPropagationMode( true );
        break;
    case 3:
        setToResultMode( true );
        break;
    default:
        break;
    }
}

void MiaThresholdLevelsetNetwork::setToNavigatingMode( bool seton )
{
    if( seton )
    {
        userInterface->leftUpperView->addInputModule(axialViewMask);
        userInterface->rightUpperView->addInputModule(coronalViewMask);
        userInterface->leftLowerView->addInputModule(saggitalViewMask);
        userInterface->upperThresholdSlider_2->setValue(axialViewThresholder->upperThreshold);
        userInterface->upperThresholdSpin_2->setValue(axialViewThresholder->upperThreshold);
        userInterface->lowerThresholdSlider_2->setValue(axialViewThresholder->lowerThreshold);
        userInterface->lowerThresholdSpin_2->setValue(axialViewThresholder->lowerThreshold);
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialCrossInterpretor );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalCrossInterpretor );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalCrossInterpretor );
    }
    else
    {
        userInterface->leftUpperView->removInputModule(axialViewMask);
        userInterface->rightUpperView->removInputModule(coronalViewMask);
        userInterface->leftLowerView->removInputModule(saggitalViewMask);
        userInterface->leftUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( NULL );
    }
}

void MiaThresholdLevelsetNetwork::setToMarkerDrawingMode( bool seton )
{
    if( seton )
    {
        userInterface->leftUpperView->addInputModule(axialViewMask);
        userInterface->rightUpperView->addInputModule(coronalViewMask);
        userInterface->leftLowerView->addInputModule(saggitalViewMask);
        userInterface->leftUpperView->addMarkerModule(axialViewMarkerAgent);
        userInterface->rightUpperView->addMarkerModule(coronalViewMarkerAgent);
        userInterface->leftLowerView->addMarkerModule(saggitalViewMarkerAgent);
        userInterface->leftUpperView->addMarkerModule(axialViewStopMarkerAgent);
        userInterface->rightUpperView->addMarkerModule(coronalViewStopMarkerAgent);
        userInterface->leftLowerView->addMarkerModule(saggitalViewStopMarkerAgent);
        userInterface->actionCurve_ROI->setEnabled(true);

        switchToSeedingMode(isDrawingSeeds);
    }
    else
    {
        userInterface->leftUpperView->removInputModule(axialViewMask);
        userInterface->rightUpperView->removInputModule(coronalViewMask);
        userInterface->leftLowerView->removInputModule(saggitalViewMask);
        userInterface->leftUpperView->removeMarkerModule(axialViewMarkerAgent);
        userInterface->rightUpperView->removeMarkerModule(coronalViewMarkerAgent);
        userInterface->leftLowerView->removeMarkerModule(saggitalViewMarkerAgent);
        userInterface->leftUpperView->removeMarkerModule(axialViewStopMarkerAgent);
        userInterface->rightUpperView->removeMarkerModule(coronalViewStopMarkerAgent);
        userInterface->leftLowerView->removeMarkerModule(saggitalViewStopMarkerAgent);
        userInterface->actionCurve_ROI->setEnabled(false);

        userInterface->leftUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftUpperView->setKeyboardInterpretor(NULL);

        userInterface->rightUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->rightUpperView->setKeyboardInterpretor(NULL);

        userInterface->leftLowerView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftLowerView->setKeyboardInterpretor(NULL);
//        isDrawingSeeds = false;
//        switchToSeedingMode(isDrawingSeeds);
        //reset the level set framework to initial without multiple support
        MiaImageND* im;
        inputModule->getOutputImage(&im);
        if(!isMemoryInitialized && im && im->getImageSize() > 52428800)
        {
            QProgressDialog progress("Intializing Memory...", 0, 0, 100,0,Qt::WindowStaysOnTopHint );
            progress.setWindowModality(Qt::WindowModal);
            progress.setValue(20);
            progress.show();
            distanceTranformer->update();
        }
        else
            distanceTranformer->update();
        userInterface->LSStatus->setText(QString("Intializing..."));
        lsSegmenter->setLevelSetFunctionInputModule(distanceTranformer);

        if(lowResolutionLSSegmenter)
        {
            userInterface->multi_Resolution_CheckBox->setEnabled(true);
            userInterface->multi_Resolution_CheckBox->setChecked(false);
        }

    }


}

void MiaThresholdLevelsetNetwork::setToLevelsetPropagationMode( bool seton )
{
    if( seton )
    {
        userInterface->leftUpperView->addInputModule(axialViewMask);
        userInterface->rightUpperView->addInputModule(coronalViewMask);
        userInterface->leftLowerView->addInputModule(saggitalViewMask);
        userInterface->upperThresholdSlider->setValue(axialViewThresholder->upperThreshold);
        userInterface->upperThresholdSpin->setValue(axialViewThresholder->upperThreshold);
        userInterface->lowerThresholdSlider->setValue(axialViewThresholder->lowerThreshold);
        userInterface->lowerThresholdSpin->setValue(axialViewThresholder->lowerThreshold);
        userInterface->leftUpperView->addInputModule(axialLevelSetQimage);
        userInterface->rightUpperView->addInputModule(coronalLevelSetQimage);
        userInterface->leftLowerView->addInputModule(saggitalLevelSetQimage);
        userInterface->leftUpperView->addMarkerModule(axialViewStopMarkerAgent);
        userInterface->rightUpperView->addMarkerModule(coronalViewStopMarkerAgent);
        userInterface->leftLowerView->addMarkerModule(saggitalViewStopMarkerAgent);


        axialLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        coronalLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        switchToSegmentingMode(false);

        allowBlockerEditing( isAllowEditingBlocker );
        MiaImageND* im;
        inputModule->getOutputImage(&im);
        if(!isMemoryInitialized && im && im->getImageSize() > 52428800)
        {
            QProgressDialog progress("Intializing Memory...", 0, 0, 100,0,Qt::WindowStaysOnTopHint );
            progress.setWindowModality(Qt::WindowModal);
            progress.setValue(70);
            progress.show();
            lsSegmenter->update();
            progress.setValue(100);
            isMemoryInitialized = true;
        }
        else
            lsSegmenter->update();
//        lsSegmenter->setMaxIteration(userInterface->maxiterationSpinBox->value());
        if(lowResolutionLSSegmenter == NULL)
            userInterface->multi_Resolution_CheckBox->setHidden(true);
        userInterface->LSStatus->setText(QString("Intialized"));
    }
    else
    {
        userInterface->leftUpperView->removInputModule(axialViewMask);
        userInterface->rightUpperView->removInputModule(coronalViewMask);
        userInterface->leftLowerView->removInputModule(saggitalViewMask);
        userInterface->leftUpperView->removInputModule(axialLevelSetQimage);
        userInterface->rightUpperView->removInputModule(coronalLevelSetQimage);
        userInterface->leftLowerView->removInputModule(saggitalLevelSetQimage);
        userInterface->leftUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftUpperView->removeMarkerModule(axialViewStopMarkerAgent);
        userInterface->rightUpperView->removeMarkerModule(coronalViewStopMarkerAgent);
        userInterface->leftLowerView->removeMarkerModule(saggitalViewStopMarkerAgent);
        axialLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        coronalLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = false;

        userInterface->leftUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftUpperView->setKeyboardInterpretor(NULL);

        userInterface->rightUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->rightUpperView->setKeyboardInterpretor(NULL);

        userInterface->leftLowerView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftLowerView->setKeyboardInterpretor(NULL);
    }
}
void MiaThresholdLevelsetNetwork::setToResultMode( bool seton )
{
    if( seton )
    {

        userInterface->leftUpperView->addInputModule(axialLevelSetQimage);
        userInterface->rightUpperView->addInputModule(coronalLevelSetQimage);
        userInterface->leftLowerView->addInputModule(saggitalLevelSetQimage);
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialCrossInterpretor );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalCrossInterpretor );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalCrossInterpretor );
        axialLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        coronalLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        statistics->update();
        userInterface->resultSummary->setText(statistics->getSummaryString());

    }
    else
    {
        userInterface->leftUpperView->removInputModule(axialLevelSetQimage);
        userInterface->rightUpperView->removInputModule(coronalLevelSetQimage);
        userInterface->leftLowerView->removInputModule(saggitalLevelSetQimage);
        userInterface->leftUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( NULL );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( NULL );
        axialLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        coronalLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = false;

    }
}

void MiaThresholdLevelsetNetwork::saveSegmentedResult()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
        exporter->setFileName(fileName, MIA_MHD);
        exporter->update();
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}

void MiaThresholdLevelsetNetwork::levelsetFunctionUpdated()
{
    int progress = 0;
    if(currentResolution == 0)
    {
        int its = lsSegmenter->getIteration();
        QVariant status_str(its);
        userInterface->LSStatus->setText(status_str.toString().append("iterations"));
        int p = lsSegmenter->getPeriod();

        if(its > 1000)
            its = 1000;
        if(p > 0)
        {
            progress +=50 + (p-1)*16 + its*10/1000;

        }
        else
        {
            progress += its*50/1000;
            if(its == 0)
            {
                userInterface->LSStatus->setText(QString("updating 3D Mesh"));
            }
        }
    }
    else
    {
        int its = lowResolutionLSSegmenter->getIteration();
        QVariant status_str(its);
        userInterface->LSStatus->setText(status_str.toString().append("Low resolution iterations"));
        int p = lowResolutionLSSegmenter->getPeriod();

        if(its > 1000)
            its = 1000;
        if(p > 0)
        {
            progress +=50 + (p-1)*16 + its*10/1000;

        }
        else
        {
            progress += its*50/1000;
            if(its == 0)
            {
                userInterface->LSStatus->setText(QString("Starting high resolution"));
            }
        }
    }

    userInterface->progressBar->setValue(progress);

}

void MiaThresholdLevelsetNetwork::levelsetEnterNewPeriod(int n) //n is the nth period
{
    userInterface->LSStatus->setText(QString("Starting new period"));
}

void MiaThresholdLevelsetNetwork::levelsetFunctionCompleted()
{
    if(currentResolution == 0)
    {
        userInterface->progressBar->setValue(100);
        userInterface->LSStatus->setText(QString("Finished"));
        switchToSegmentingMode(false);
        userInterface->multi_Resolution_CheckBox->setChecked(false);
        userInterface->multi_Resolution_CheckBox->setEnabled(false);
        isosurface->update();

        if(!autoSaveResultPath.isEmpty())
        {
            exporter->setFileName(autoSaveResultPath, MIA_MHD);
            exporter->update();
            emit procesingFinished(true);
        }
    }
    else
    {
        switchToHigherResolution();
        currentResolution = 0;
        userInterface->multi_Resolution_CheckBox->setChecked(false);
        userInterface->multi_Resolution_CheckBox->setEnabled(false);
        lsSegmenter->start();

    }

}
void MiaThresholdLevelsetNetwork::setSmoothFactor(int var)
{
    double factor = (double)var/100;
    if( factor !=  userInterface->smoothFactorSpin->value() )
        userInterface->smoothFactorSpin->setValue(factor);
    lsSegmenter->setCurvatureWeight(factor);
    if(lowResolutionLSSegmenter)
    {
        factor = factor*0.5/(1 - factor*0.5);
        lowResolutionLSSegmenter->setCurvatureWeight(factor);
    }
}

void MiaThresholdLevelsetNetwork::setSmoothFactor(double var)
{
    double factor = var*100;
    if( (int)factor !=  userInterface->smoothFactorSlider->value() )
        userInterface->smoothFactorSlider->setValue((int)(factor+0.5));
    lsSegmenter->setCurvatureWeight(var);
    if(lowResolutionLSSegmenter)
    {
        factor = factor*0.5/(1 - factor*0.5);
        lowResolutionLSSegmenter->setCurvatureWeight(factor);
    }
}
void MiaThresholdLevelsetNetwork::switchToSegmentingMode(bool isSegmenting)
{
    if(isSegmenting)
    {
        userInterface->startButton->setDisabled(true);
        userInterface->stopButton->setDisabled(false);
        userInterface->levelSetTabWidget->setDisabled(true);
        userInterface->progressBar->setVisible(true);
    }
    else
    {
        userInterface->startButton->setDisabled(false);
        userInterface->stopButton->setDisabled(true);
        userInterface->levelSetTabWidget->setDisabled(false);
        userInterface->progressBar->setVisible(false);
    }
}
void MiaThresholdLevelsetNetwork::exportSTLFile()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
        isosurface->saveSTLFile(fileName);
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}
void MiaThresholdLevelsetNetwork::switchToSeedingMode( bool seeding)
{
    if(seeding)
    {
        seedMarkerManager->freezeAllMarkers(false);
        userInterface->markerListView->setEnabled(true);
        stopMarkerManager->freezeAllMarkers( true );
        userInterface->stopMarkerListView->setEnabled(false);
        userInterface->seedButton->setText("Draw Blocking Region");
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialMarkerInterpretor );
        userInterface->leftUpperView->setKeyboardInterpretor(axialMarkerInterpretor);

        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalMarkerInterpretor );
        userInterface->rightUpperView->setKeyboardInterpretor(coronalMarkerInterpretor);

        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalMarkerInterpretor );
        userInterface->leftLowerView->setKeyboardInterpretor(saggitalMarkerInterpretor);

    }
    else
    {
        seedMarkerManager->freezeAllMarkers(true);
        userInterface->markerListView->setEnabled(false);
        stopMarkerManager->freezeAllMarkers( false );
        userInterface->stopMarkerListView->setEnabled(true);
        userInterface->seedButton->setText("Draw Seed Segion");

        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialStopMarkerInterpretor );
        userInterface->leftUpperView->setKeyboardInterpretor(axialStopMarkerInterpretor);

        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalStopMarkerInterpretor );
        userInterface->rightUpperView->setKeyboardInterpretor(coronalStopMarkerInterpretor);

        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalStopMarkerInterpretor );
        userInterface->leftLowerView->setKeyboardInterpretor(saggitalStopMarkerInterpretor);
    }
}

void MiaThresholdLevelsetNetwork::switchDrawingMode()
{
    isDrawingSeeds = !isDrawingSeeds;

    switchToSeedingMode(isDrawingSeeds);
}

void MiaThresholdLevelsetNetwork::nextStep()
{
    if(currentStep<3)
    {
        userInterface->toolBox->setCurrentIndex(currentStep+1);
    }
}

void MiaThresholdLevelsetNetwork::previousStep()
{
    if(currentStep>0)
    {
        userInterface->toolBox->setCurrentIndex(currentStep-1);
    }
}
void MiaThresholdLevelsetNetwork::showBlockerBox(bool show)
{
    if(show)
    {
        userInterface->blockerGroupBox->setVisible(true);
        switchToSeedingMode(false);
    }
    else
    {
        userInterface->blockerGroupBox->setVisible(false);
        switchToSeedingMode(true);
    }
}
void MiaThresholdLevelsetNetwork::allowBlockerEditing(bool editable)
{
    isAllowEditingBlocker = editable;
    if(editable)
    {
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialStopMarkerInterpretor );
        userInterface->leftUpperView->setKeyboardInterpretor(axialStopMarkerInterpretor);

        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalStopMarkerInterpretor );
        userInterface->rightUpperView->setKeyboardInterpretor(coronalStopMarkerInterpretor);

        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalStopMarkerInterpretor );
        userInterface->leftLowerView->setKeyboardInterpretor(saggitalStopMarkerInterpretor);

        stopMarkerManager->freezeAllMarkers( false );
        userInterface->levelSetTabWidget->addTab(userInterface->blockerTab,QString("Blockers"));
        userInterface->levelSetTabWidget->setCurrentIndex(1);
    }
    else
    {
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialCrossInterpretor );
        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalCrossInterpretor );
        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalCrossInterpretor );

        userInterface->leftUpperView->setKeyboardInterpretor(NULL);
        userInterface->rightUpperView->setKeyboardInterpretor(NULL);
        userInterface->leftLowerView->setKeyboardInterpretor(NULL);

        stopMarkerManager->freezeAllMarkers( true );
        userInterface->levelSetTabWidget->removeTab(1);
    }
}

void MiaThresholdLevelsetNetwork::saveMarkers()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        // we will serialize the data into the file
        out << QString("SavedMarkersVer0.2");   // serialize a string
        out << QString("Seeds");
        seedMarkerManager->saveMarkers(out);
        out << QString("StopSeeds");
        stopMarkerManager->saveMarkers(out);
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}
void MiaThresholdLevelsetNetwork::loadMarkers()
{

    QString fileName = QFileDialog::getOpenFileName((QWidget*)parent(), tr("Open File"), 0, 0);
    if (fileName != "")
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        // we deserialize the data from the file
        QString versionstr;
        in >> versionstr;   // deserialize a string
        if(versionstr != "SavedMarkersVer0.2")
        {
            qDebug()<<"Wrong marker file";
            return;
        }
        while(!in.atEnd())
        {
            QString labelstr;
            in >> labelstr;
            if(labelstr == "Seeds")
            {
                seedMarkerManager->loadMarkers(in);
            }
            else if(labelstr == "StopSeeds")
            {
                stopMarkerManager->loadMarkers(in);
            }


        }

        seedMarkerManager->loadMarkers(in);
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}
void MiaThresholdLevelsetNetwork::showMessageBoxOfSpeedBoosting()
{
    QMessageBox msgBox;
    msgBox.setText("Speed boosting uses a multi-resolution approach to process image. NOTE: it may cause region-leaking problem as the boudary between organs may be blur at lower resolution.");
    msgBox.exec();
}
void MiaThresholdLevelsetNetwork::saveRegionToTextFile()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
        statistics->exportSamplesToPlainText(fileName);
//        intermedium_data_exporter->setFileName(fileName);
//        intermedium_data_exporter->update();
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}

void MiaThresholdLevelsetNetwork::onDrawingToolSelected()
{
    userInterface->leftLowerView->setCursor(Qt::ArrowCursor);
    userInterface->leftUpperView->setCursor(Qt::ArrowCursor);
    userInterface->rightLowerView->setCursor(Qt::ArrowCursor);
    switchToSeedingMode(isDrawingSeeds);
}
