#include "miavesselanalysisnetwork.h"
#include "MiaCommonModules/miathresholding.h"
#include "ui_mainwindow.h"
#include "MiaCommonModules/miaorthogonalslicer.h"
#include "MiaCommonModules/miafastmarchingfilter.h"
#include "MiaLevelSetModules/miasfthresholdlevelsetsegmenter.h"
#include "MiaCommonModules/miaintensitymappingfilter.h"
#include "MiaViewModules/miadrawingmanager.h"
#include "MiaCommonModules/miacentertrackingonarrivalmap.h"
#include "MiaCommonModules/miafocusvaluereadout.h"
#include "MiaCommonModules/miadistancetransformfromcenterline.h"
#include "MiaCommonModules/miamultiinputintensitymappingfilter.h"
#include "MiaCommonModules/mialevelsetfunctionmanualmodifier.h"
#include "MiaCommonModules/miamarkerstatistics.h"
#include "MiaCommonModules/miaimagepropertyspreadingfilter.h"
#include "MiaViewModules/miaimage2vtkisosurface.h"
#include "MiaCommonModules/miazerocrossingfilter.h"
#include "MiaMarkers/miamarker3dpolygon.h"
#include "MiaCommonModules/miaregionstatistics.h"
#include "MiaViewModules/miaimageexporter.h"
#include "MiaViewModules/miaimagereader.h"
#include "MiaCommonModules/miaparameterconnector.h"
#include "MiaCommonModules/miacurvedmprslicer.h"
#include "MiaCommonModules/miaobliquemprslicer.h"
#include "MiaViewModules/miainputeventinterpretorforwindowsetting.h"
#include "miaconstants.h"
#include "MiaMarkers/miamarker3dbspline.h"
#include <algorithm>
#include <QFileDialog>

MiaVesselAnalysisNetwork::MiaVesselAnalysisNetwork(NetWorkEnviromentVariable* var,QObject *parent) :
    MiaBasicMPRandVRTNetwork(var,parent)
{
    networkName = "Vessel analysis network";
    firstDistanceTransformModule = NULL;
    vesselAnalysisGUIOn = false;
    lsVesselSegmenter = NULL;
    narrowBandCreator = NULL;
    vesselWallThresholdMappingFilter = NULL;
    initialCenterlineMarkerManager = NULL;
    firstStepLongitudinalBorderMarkerManagerNo1 = NULL;
    firstStepLongitudinalBorderMarkerManagerNo2 = NULL;
    firstStepLongitudinalBorderMarkerManagerNo3 = NULL;
    secondStepCrossSectionBorderMarkerManager = NULL;
    thirdStepCenterlineMarkerManager = NULL;
    requiredIterations = 0;
    levelsetRunning = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(continueVesselAnalysis()));
    isInAutomtaticProcessing = false;
    currentVesselAnalysisStep = 0;
    rawDataloader = NULL;
    lowerThreshold = 100;
    upperThreshold = 500;

    curvedMPRSlicer = NULL;
    curvedMPRImage = NULL;
    crossSectionMPRSlicer = NULL;
    crossSectionMPRImage = NULL;
    currentStep = MIA_VA_Uninitialized;
    currentCPRAngle = -1;
    currentCrossSectionPostion = -1;
}

void MiaVesselAnalysisNetwork::setupNetwork(MiaModule* input)
{
    MiaBasicMPRandVRTNetwork::setupNetwork(input);

    curvedMPRSlicer = new MiaCurvedMPRSlicer(this);
    crossSectionMPRSlicer = new MiaObliqueMPRSlicer(this);
    curvedMPRImage = new MiaImage2QImage(this);
    crossSectionMPRImage = new MiaImage2QImage(this);

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

    axialViewCentlineMarkerAgent = new MiaMarker2GraphicsItem(this);
    coronalViewCenterlineMarkerAgent = new MiaMarker2GraphicsItem(this);
    saggitalViewCenterlineMarkerAgent = new MiaMarker2GraphicsItem(this);
    cPRViewCenterlineMarkerAgent = new MiaMarker2GraphicsItem(this);

    axialViewLongitudinalBorderMarkerAgent   =new MiaMarker2GraphicsItem(this);
    coronalViewLongitudinalBorderMarkerAgent =new MiaMarker2GraphicsItem(this);
    saggitalViewLongitudinalBorderMarkerAgent=new MiaMarker2GraphicsItem(this);
    cPRViewLongitudinalMarkerAgentNo1           =new MiaMarker2GraphicsItem(this);
    cPRViewLongitudinalMarkerAgentNo2           =new MiaMarker2GraphicsItem(this);
    cPRViewLongitudinalMarkerAgentNo3           =new MiaMarker2GraphicsItem(this);
    crossSectionViewLongitudinalMarkerAgentNo1=new MiaMarker2GraphicsItem(this);
    crossSectionViewLongitudinalMarkerAgentNo2=new MiaMarker2GraphicsItem(this);
    crossSectionViewLongitudinalMarkerAgentNo3=new MiaMarker2GraphicsItem(this);
    crossSectionViewCrossSectionMarkerAgent=new MiaMarker2GraphicsItem(this);


    lsVesselSegmenter = new MiaSFThresholdLevelSetSegmenter(this);
    lsVesselSegmenter->setToUseCoherentPropagation(false);
    intensityReverter = new MiaIntensityMappingFilter(this);
    zeroDistanceTransformModule = new MiaFastMarchingFilter(this);
    firstDistanceTransformModule = new MiaFastMarchingFilter(this);
    intensityExponentialMapper = new MiaIntensityMappingFilter(this);
    secondDistanceTransformModule = new MiaFastMarchingFilter(this);
    centerlineTracer = new MiaCenterTrackingOnArrivalMap(this);
    centerlineDilater = new MiaDistanceTransformFromCenterline(this);
    thirdDistanceTransformModule = new MiaFastMarchingFilter(this);
    intensityCubicMapper = new MiaIntensityMappingFilter(this);
    intensityAndDistanceCombiner = new MiaMultiInputIntensityMappingFilter(this);
    centerRadiusReader = new MiaMarkerStatistics(this);
    centerlinevalueInitialSpreader = new MiaDistanceTransformFromCenterline(this);
    endocentricMapGenerator = new MiaFastMarchingFilter(this);
    centerlinevalueSpreader = new MiaImagePropertySpreadingFilter(this);
    narrowBandCreator = new MiaZeroCrossingFilter(this);
    vesselWallThresholdMappingFilter = new MiaIntensityMappingFilter(this);
    initialCenterlineMarkerManager = new MiaDrawingManager(this);
    firstStepLongitudinalBorderMarkerManagerNo1 = new MiaDrawingManager(this);
    firstStepLongitudinalBorderMarkerManagerNo2 = new MiaDrawingManager(this);
    firstStepLongitudinalBorderMarkerManagerNo3 = new MiaDrawingManager(this);
    secondStepCrossSectionBorderMarkerManager = new MiaDrawingManager(this);
    thirdStepCenterlineMarkerManager = new MiaDrawingManager(this);

    isosurface = new MiaImage2VTKIsosurface(this);
    exporter = new MiaImageExporter(this);
    statistics = new MiaRegionStatistics(this);

// Setting up the CPR view

    curvedMPRSlicer->setInputModule(inputModule);
    curvedMPRImage->setInputModule(curvedMPRSlicer);
    initialCenterlineMarkerManager->connectToDownStreamAgent(curvedMPRSlicer);
    QObject::connect(curvedMPRSlicer, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),initialCenterlineMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));

    crossSectionMPRSlicer->setMPRMode(MIA_MPR_CrossSectionOfCurve);
    crossSectionMPRSlicer->setInputModule(inputModule);
    crossSectionMPRImage->setInputModule(crossSectionMPRSlicer);
    initialCenterlineMarkerManager->connectToDownStreamAgent(crossSectionMPRSlicer);
    QObject::connect(crossSectionMPRSlicer, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),initialCenterlineMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));



// Setting up the vessel analysis network
    /*******************************************************************************************************************************************************
                                    centerline loading                                                                Input image
                                     (initial radius 1.5 unit)                                                          (input)
                                               |                                                                           |
                                               |                                                                       mappingFilter
                                               |                                                                       (thresholding)
                                               |                                                                           |
        centerRadiusReader---------------------=--> centerlineDilater > thirdDistanceTransformModule>intensityCubicMapper -+---> intensityAndDistanceCombiner
    (read radius & regression)                              (                    create cylinder model                    )                (external speed)
              |                                                                       |                                                  |
              |                                                              narrowBandCreator                                           |
              |                                                          (recreating zero leves set)                                     |
              |                                                                       |                                                  |
              +----------------------------------------------+                        +------------------------+-------------------------+
              |                                              |                                                 |
      centerlineTracer<intensityExponentialMapper<firstDistanceTransformModule<intensityReverter<---------lsVesselSegmenter
              (                         centerline tracking                               )              (vessel segmentation)


      *******************************************************************************************************************************************************/

    centerlineDilater->setInputModule(inputModule); // only the dimesnion of the input is used
    initialCenterlineMarkerManager->connectToDownStreamAgent(centerlineDilater);
    QObject::connect(centerlineDilater, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),initialCenterlineMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));


    thirdDistanceTransformModule->setInputModule(centerlineDilater);
    thirdDistanceTransformModule->setInputImageCarrysInitialValue(true);
    thirdDistanceTransformModule->autoUpdateWhenOutputRequested = false;
    thirdDistanceTransformModule->setSeedsThresholds(-0.5f*MAX_FLOAT,0.5f*MAX_FLOAT);//voxels in this range are counted as seeds
    thirdDistanceTransformModule->stopAfterReachValue = 10.0f;
    thirdDistanceTransformModule->operateInSitu = true;

    intensityCubicMapper->setInputModule(thirdDistanceTransformModule);
    intensityCubicMapper->selectedMappingMethod = MIA_Cubic;
    intensityCubicMapper->setMultiplyFactor(1.0);
    intensityCubicMapper->setUpperThreshold(64);
    intensityCubicMapper->operateInSitu = true;

    vesselWallThresholdMappingFilter->setInputModule(inputModule);
//    vesselWallThresholdMappingFilter->selectedMappingMethod = MIA_LUTMapping;
//    QList<QPointF> lut;
//    lut.append(QPointF(lowerThreshold-50,-1.0));
//    lut.append(QPointF(lowerThreshold+50,1.0));
//    vesselWallThresholdMappingFilter->setToLUTMapping(lut);

    intensityAndDistanceCombiner->setInputModule(vesselWallThresholdMappingFilter);
    intensityAndDistanceCombiner->setSecondInputModule(intensityCubicMapper);
    intensityAndDistanceCombiner->setMultiplyFactor(1.0,-1.0);
    intensityAndDistanceCombiner->selectedMappingMethod = MIA_Multi_Add;
    intensityAndDistanceCombiner->operateInSitu = true;

    narrowBandCreator->setInputModule(thirdDistanceTransformModule);
    narrowBandCreator->operateInSitu = true;

    //level set pipeline
    lsVesselSegmenter->setInputModule(intensityAndDistanceCombiner);
    lsVesselSegmenter->setLevelSetFunctionInputModule(narrowBandCreator);
    lsVesselSegmenter->setMaxIteration(300);

    isosurface->setInputModule( lsVesselSegmenter );

    zeroDistanceTransformModule->setInputModule(lsVesselSegmenter);
    zeroDistanceTransformModule->setInputImageCarrysInitialValue(true);
    zeroDistanceTransformModule->autoUpdateWhenOutputRequested = false;
    zeroDistanceTransformModule->setThresholds(-0.5f,MAX_FLOAT); //voxels out of this range are ignored
    zeroDistanceTransformModule->setSeedsThresholds(-0.5f,0.5f);//voxels in this range are counted as seeds
    zeroDistanceTransformModule->stopAfterReachValue = 4.0f;
    zeroDistanceTransformModule->operateInSitu = true;

    intensityReverter->setInputModule(zeroDistanceTransformModule);
    intensityReverter->selectedMappingMethod = MIA_Multiply;
    intensityReverter->setMultiplyFactor(-1.0f);
    intensityReverter->operateInSitu = true;

    firstDistanceTransformModule->setInputModule(intensityReverter);
    firstDistanceTransformModule->setInputImageCarrysInitialValue(true);
    firstDistanceTransformModule->autoUpdateWhenOutputRequested = false;
    firstDistanceTransformModule->setThresholds(-0.5f,MAX_FLOAT); //voxels out of this range are ignored
    firstDistanceTransformModule->setSeedsThresholds(-0.5f,0.5f);//voxels in this range are counted as seeds
    firstDistanceTransformModule->operateInSitu = true;

    intensityExponentialMapper->selectedMappingMethod = MIA_Exponential;
    intensityExponentialMapper->setInputModule(firstDistanceTransformModule);
    intensityExponentialMapper->setMultiplyFactor(0.5f);
//    intensityExponentialMapper->operateInSitu = true;

    secondDistanceTransformModule->setInputModule(intensityExponentialMapper);
    secondDistanceTransformModule->setUseInputImageAsSpeed(true);
    secondDistanceTransformModule->autoUpdateWhenOutputRequested = false;
    secondDistanceTransformModule->setThresholds(1.0e-2,MAX_FLOAT);//voxels out of this range are ignored
    initialCenterlineMarkerManager->connectToDownStreamAgent(secondDistanceTransformModule);
    QObject::connect(secondDistanceTransformModule, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),initialCenterlineMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));


    centerlineTracer->setInputModule(secondDistanceTransformModule);
    centerlineTracer->setTracingToMinimum();
    initialCenterlineMarkerManager->connectToDownStreamAgent(centerlineTracer);
    QObject::connect(centerlineTracer, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),initialCenterlineMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));

    centerRadiusReader->autoUpdateWhenInputMarkerUpdated = false;
    centerRadiusReader->autoUpdateWhenInputChanged = false;
    centerRadiusReader->currentStatisticsMethod = MIA_ReadKeyValues;
    centerRadiusReader->setInputModule(firstDistanceTransformModule);
    centerRadiusReader->setCenterlineSmoothingRange(60);
    initialCenterlineMarkerManager->connectToDownStreamAgent(centerRadiusReader);
    QObject::connect(centerRadiusReader, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),initialCenterlineMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));


    statistics->setMaskImageInputModule(lsVesselSegmenter);
    exporter->setInputModule(lsVesselSegmenter);

    //visualization part
    //Thresholding
    axialViewThresholder->setInputModule( axialslicer );
    axialViewMask->setInputModule( axialViewThresholder );

    coronalViewThresholder->setInputModule( coronalslicer );
    coronalViewMask->setInputModule( coronalViewThresholder );

    saggitalViewThresholder->setInputModule( saggitalslicer );
    saggitalViewMask->setInputModule( saggitalViewThresholder );
    //level set
    axialLevelSetSlicer->setInputModule( lsVesselSegmenter );
    coronalLevelSetSlicer->setInputModule( lsVesselSegmenter );
    saggitalLevelSetSlicer->setInputModule( lsVesselSegmenter );
    axialLevelSetQimage->setInputModule( axialLevelSetSlicer);
    coronalLevelSetQimage->setInputModule( coronalLevelSetSlicer);
    saggitalLevelSetQimage->setInputModule( saggitalLevelSetSlicer );
    MiaLUTModule* lsResultLUT = new MiaLUTModule(LUT_SingleColor, this);
    lsResultLUT->removeAllKeyPoints();
    QColor incolor(Qt::green);
    incolor.setAlphaF(0.5);
    lsResultLUT->insertKeyPoint(0,incolor);
    QColor outcolor(Qt::green);
    outcolor.setAlphaF(0);
    lsResultLUT->insertKeyPoint(1.0,outcolor);
    axialLevelSetQimage->setCurrentLUT(lsResultLUT);
    coronalLevelSetQimage->setCurrentLUT(lsResultLUT);
    saggitalLevelSetQimage->setCurrentLUT(lsResultLUT);



    // testing part
//    centerlinevalueInitialSpreader->setInputModule(inputModule);
//    centerlinevalueInitialSpreader->workingMode = MIA_ReadAllKeyValues;
//    currentCenterlineMarkerManager->connectToDownStreamAgent(centerlinevalueInitialSpreader);
//    QObject::connect(centerlinevalueInitialSpreader, SIGNAL(getInputMarkers(QList<MiaMarker*>&)),currentCenterlineMarkerManager,SLOT(getAllMarkers(QList<MiaMarker*>&)));

//    endocentricMapGenerator->setInputModule(centerlineDilater);
//    endocentricMapGenerator->selectedAlgorith = MIA_ChamferDistance;
//    endocentricMapGenerator->inputImageCarrysInitialValue = true;
//    endocentricMapGenerator->autoUpdateWhenOutputRequested = false;
//    endocentricMapGenerator->setSeedsThresholds(-0.5f*max_float(),0.5f*max_float());//voxels in this range are counted as seeds
//    endocentricMapGenerator->stopAfterReachValue = 10.0f;

//    centerlinevalueSpreader->setInputModule(centerlinevalueInitialSpreader);
//    centerlinevalueSpreader->setDirectionMapInputModule(endocentricMapGenerator,SLOT(getOutputTagImage(ImageND**)));

    miaModuleList.append(curvedMPRSlicer);
    miaModuleList.append(crossSectionMPRSlicer);
    miaModuleList.append(curvedMPRImage);
    miaModuleList.append(crossSectionMPRImage);
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
    miaModuleList.append(axialViewCentlineMarkerAgent);
    miaModuleList.append(coronalViewCenterlineMarkerAgent);
    miaModuleList.append(saggitalViewCenterlineMarkerAgent);
    miaModuleList.append(cPRViewCenterlineMarkerAgent);
    miaModuleList.append(crossSectionViewLongitudinalMarkerAgentNo1);
    miaModuleList.append(crossSectionViewLongitudinalMarkerAgentNo2);
    miaModuleList.append(crossSectionViewLongitudinalMarkerAgentNo3);
    miaModuleList.append(crossSectionViewCrossSectionMarkerAgent);

    miaModuleList.append(axialViewLongitudinalBorderMarkerAgent) ;
    miaModuleList.append(coronalViewLongitudinalBorderMarkerAgent) ;
    miaModuleList.append(saggitalViewLongitudinalBorderMarkerAgent) ;
    miaModuleList.append(cPRViewLongitudinalMarkerAgentNo1);
    miaModuleList.append(cPRViewLongitudinalMarkerAgentNo2);
    miaModuleList.append(cPRViewLongitudinalMarkerAgentNo3);

    miaModuleList.append(exporter);
    outputModule = lsVesselSegmenter;

}

void MiaVesselAnalysisNetwork::setupUserInterface(Ui::MainWindow* ui)
{
    MiaBasicMPRandVRTNetwork::setupUserInterface(ui);
    crossSectionView = new MiaImageView2D();
    userInterface->vesselProgressBar->setVisible(false);
    connect(userInterface->startVesselAnalysisButton, SIGNAL(clicked()),this,SLOT(startVesselAnalysis()));
    connect(userInterface->stopVesselAnalysisButton, SIGNAL(clicked()),this,SLOT(stopVesselAnalysis()));
    connect(userInterface->loadVesselCenterlineButton, SIGNAL(clicked()),this,SLOT(loadVesselCenterline()));
    connect(userInterface->toolBox,SIGNAL(currentChanged(int)), this, SLOT(onToolboxPageChanged(int)));
    connect(userInterface->nextAngleButton,SIGNAL(clicked()),this,SLOT(nextPresetAngle()));
    connect(userInterface->nextPositionButton,SIGNAL(clicked()),this,SLOT(nextPresetPosition()));
    connect(userInterface->exportCrossSectionButton,SIGNAL(clicked()),this,SLOT(saveCrossSectionSegmentation()));
    connect(userInterface->loadCrossSectionButton,SIGNAL(clicked()),this,SLOT(loadCrossSectionSegmentation()));
    connect(userInterface->initializeVesselCenterlineButton,SIGNAL(clicked()),this,SLOT(initializeManuallyCreatedCenterlines()));
    connect(userInterface->viewImageSourceVesselAnalyssisComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(switchViewInputImage(int)));
    userInterface->exportVTKButton->setVisible(false);
    userInterface->loadCrossSectionButton->setVisible(false);
    userInterface->exportCrossSectionButton->setVisible(false);
    userInterface->exportVesselAnalysisButton->setVisible(false);
//    userInterface->exportVesselAnalysisButton->setVisible(false);
    userInterface->toolBox->addItem(userInterface->VesselAnalysisPage,QString("Vessel Analysis"));
    userInterface->VesselAnalysisPage->setVisible(true);
    axialLevelSetSlicer->setInputModule( lsVesselSegmenter );
    coronalLevelSetSlicer->setInputModule( lsVesselSegmenter );
    saggitalLevelSetSlicer->setInputModule( lsVesselSegmenter );
    userInterface->rightLowerView->addInputModule(isosurface);

    initialCenterlineMarkerManager->connectToDownStreamAgent(axialViewCentlineMarkerAgent);
    initialCenterlineMarkerManager->connectToDownStreamAgent(coronalViewCenterlineMarkerAgent);
    initialCenterlineMarkerManager->connectToDownStreamAgent(saggitalViewCenterlineMarkerAgent);
    initialCenterlineMarkerManager->connectToDownStreamAgent(cPRViewCenterlineMarkerAgent);

//    firstStepLongitudinalBorderMarkerManagerNo1->connectToDownStreamAgent(axialViewLongitudinalBorderMarkerAgent);
//    firstStepLongitudinalBorderMarkerManagerNo1->connectToDownStreamAgent(coronalViewLongitudinalBorderMarkerAgent);
//    firstStepLongitudinalBorderMarkerManagerNo1->connectToDownStreamAgent(saggitalViewLongitudinalBorderMarkerAgent);
    firstStepLongitudinalBorderMarkerManagerNo1->connectToDownStreamAgent(cPRViewLongitudinalMarkerAgentNo1);
    firstStepLongitudinalBorderMarkerManagerNo2->connectToDownStreamAgent(cPRViewLongitudinalMarkerAgentNo2);
    firstStepLongitudinalBorderMarkerManagerNo3->connectToDownStreamAgent(cPRViewLongitudinalMarkerAgentNo3);

    firstStepLongitudinalBorderMarkerManagerNo1->connectToDownStreamAgent(crossSectionViewLongitudinalMarkerAgentNo1);
    crossSectionViewLongitudinalMarkerAgentNo1->setMarkersToEditable(false);
    crossSectionViewLongitudinalMarkerAgentNo1->setMarkersAlpha(0.0f);

    firstStepLongitudinalBorderMarkerManagerNo2->connectToDownStreamAgent(crossSectionViewLongitudinalMarkerAgentNo2);
    crossSectionViewLongitudinalMarkerAgentNo2->setMarkersToEditable(false);
    crossSectionViewLongitudinalMarkerAgentNo2->setMarkersAlpha(0.0f);

    firstStepLongitudinalBorderMarkerManagerNo3->connectToDownStreamAgent(crossSectionViewLongitudinalMarkerAgentNo3);
    crossSectionViewLongitudinalMarkerAgentNo3->setMarkersToEditable(false);
    crossSectionViewLongitudinalMarkerAgentNo3->setMarkersAlpha(0.0f);

    secondStepCrossSectionBorderMarkerManager->connectToDownStreamAgent(crossSectionViewCrossSectionMarkerAgent);
    crossSectionViewCrossSectionMarkerAgent->setMarkersAlpha(0.0f);

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


    cPRMarkerInterpretor = new MiaInputEventInterpretor(this);
    cPRMarkerInterpretor->xValueMethod(FORWARDMESSAGE);
    cPRMarkerInterpretor->yValueMethod(FORWARDMESSAGE);

    crossSectionInterpretor = new MiaInputEventInterpretor(this);
    crossSectionInterpretor->xValueMethod(FORWARDMESSAGE);
    crossSectionInterpretor->yValueMethod(FORWARDMESSAGE);

//    QObject::connect(lsVesselSegmenter,SIGNAL(levelsetFunctionUpdated()),this,SLOT(levelsetFunctionUpdated()));
    QObject::connect(lsVesselSegmenter,SIGNAL(levelsetFunctionCompleted()),this,SLOT(levelsetVesselSegmentationCompleted()));
//    QObject::connect(lsVesselSegmenter,SIGNAL(levelsetEnterNewPeriod(int)),this,SLOT(levelsetEnterNewPeriod(int)));

//    userInterface->continueVesselAnalysisButton->setEnabled(false);
    connect(userInterface->smoothFactorSlider_2, SIGNAL(valueChanged(int)), this, SLOT(setVesselSmoothFactor(int)));
    connect(userInterface->smoothFactorSpin_2, SIGNAL(valueChanged(double)), this, SLOT(setVesselSmoothFactor(double)));
    userInterface->smoothFactorSlider_2->setValue(20);
    connect(userInterface->exportVesselAnalysisButton, SIGNAL(clicked()), this, SLOT(exportVesselAnaysisResult()));
//    connect(userInterface->step1_RadioButton, SIGNAL(clicked()), this, SLOT(resetParameterForNextStep()));
//    connect(userInterface->step2_RadioButton, SIGNAL(clicked()), this, SLOT(resetParameterForNextStep()));
//    connect(userInterface->step3_RadioButton, SIGNAL(clicked()), this, SLOT(resetParameterForNextStep()));
    userInterface->loadRawButton->setVisible(false);
    connect(userInterface->loadRawButton, SIGNAL(clicked()), this, SLOT(loadSegmentationResultFromRaw()));
    connect(userInterface->exportVTKButton, SIGNAL(clicked()), this, SLOT(exportVTKFile()));

    //reslicing position connected to crosshair postion
    MiaParameterConnector* connector4 = new MiaParameterConnector(this);
    connector4->addParameterPairs(QString("cross_pat_x"),QString("pat_x"));
    connector4->addParameterPairs(QString("cross_pat_y"),QString("pat_y"));
    connector4->addParameterPairs(QString("cross_pat_z"),QString("pat_z"));
    connector4->addParameterPairs(QString("cross_pat_t"),QString("pat_t"));
    connector4->linkModules(userInterface->leftUpperView, axialLevelSetSlicer);
    connector4->linkModules(userInterface->rightUpperView, coronalLevelSetSlicer);
    connector4->linkModules(userInterface->leftLowerView, saggitalLevelSetSlicer);
    userInterface->leftUpperView->notifyCrossHairChanged();
    //thresholding
    MiaInputEventInterpretor* thresholdingInterpretor = new MiaInputEventInterpretor(this);
    QObject::connect(userInterface->lowerThresholdSlider_4, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateXValue(int)));
    QObject::connect(userInterface->upperThresholdSlider_4, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateYValue(int)));
    QObject::connect(userInterface->lowerThresholdSpin_4, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateXValue(int)));
    QObject::connect(userInterface->upperThresholdSpin_4, SIGNAL(valueChanged(int)), thresholdingInterpretor, SLOT(updateYValue(int)));



    MiaParameterConnector* connector6 = new MiaParameterConnector(this);
    connector6->addParameterPairs( QString("x"), QString("lower_threshold"));
    connector6->addParameterPairs( QString("y"), QString("upper_threshold"));
    connector6->linkModules(thresholdingInterpretor, axialViewThresholder, false);
    connector6->linkModules(thresholdingInterpretor, coronalViewThresholder, false);
    connector6->linkModules(thresholdingInterpretor, saggitalViewThresholder, false);
    connector6->linkModules(thresholdingInterpretor, vesselWallThresholdMappingFilter, false);
    thresholdingInterpretor->updateXValue(userInterface->lowerThresholdSlider_4->value());
    thresholdingInterpretor->updateYValue(userInterface->upperThresholdSlider_4->value());

    MiaParameterConnector* connector44 = new MiaParameterConnector(this);
    connector44->addParameterPairs(QString("pat_x"),QString("pat_x"));
    connector44->addParameterPairs(QString("pat_y"),QString("pat_y"));
    connector44->addParameterPairs(QString("pat_z"),QString("pat_z"));
    connector44->addParameterPairs(QString("pat_t"),QString("pat_t"));

    connector44->linkModules(crossSectionMPRSlicer,axialLevelSetSlicer);

    cPRPanInterpretor = new MiaInputEventInterpretor(this);
    cPRPanInterpretor->xValueMethod(ADDDELTA);
    cPRPanInterpretor->yValueMethod(ADDDELTA);

    cPRZoomInterpretor = new MiaInputEventInterpretor(this);
    cPRZoomInterpretor->xValueMethod(ADDDELTA);
    cPRZoomInterpretor->yValueMethod(ADDDELTA);

    cPRWheelInterpretor = new MiaInputEventInterpretor(this);
    cPRWheelInterpretor->xValueMethod(SETVALUE);
    cPRWheelInterpretor->yValueMethod(SETVALUE);

    crossSectionPanInterpretor = new MiaInputEventInterpretor(this);
    crossSectionPanInterpretor->xValueMethod(ADDDELTA);
    crossSectionPanInterpretor->yValueMethod(ADDDELTA);

    crossSectionZoomInterpretor = new MiaInputEventInterpretor(this);
    crossSectionZoomInterpretor->xValueMethod(ADDDELTA);
    crossSectionZoomInterpretor->yValueMethod(ADDDELTA);

    crossSectionWheelInterpretor = new MiaInputEventInterpretor(this);
    crossSectionWheelInterpretor->xValueMethod(SETVALUE);
    crossSectionWheelInterpretor->yValueMethod(SETVALUE);

    userInterface->curvedMPRV->setmouseRightDragInterpretor( winSettingInterpretor );
    userInterface->curvedMPRV->setWheelInterpretor(cPRWheelInterpretor);

    crossSectionView->setmouseRightDragInterpretor(winSettingInterpretor);
    crossSectionView->setWheelInterpretor(crossSectionWheelInterpretor);

    MiaParameterConnector* connector101 = new MiaParameterConnector(this);
    connector101->addParameterPairs(QString("x"),QString("ww"));
    connector101->addParameterPairs(QString("y"),QString("wl"));
    connector101->linkModules(winSettingInterpretor, curvedMPRImage);
    connector101->linkModules(winSettingInterpretor,crossSectionMPRImage);
    setDefaultWLWW();

    // wheelinterpretor for cpr view
    MiaParameterConnector* connector102 = new MiaParameterConnector(this);
    connector102->addParameterPairs(QString("y"),QString("scroll_y"));
    connector102->linkModules(cPRWheelInterpretor, curvedMPRSlicer, false);

    // paninterpretor for cpr view
    MiaParameterConnector* connector104 = new MiaParameterConnector(this);
    connector104->addParameterPairs(QString("x"),QString("translate_x"));
    connector104->addParameterPairs(QString("y"),QString("translate_y"));
    connector104->linkModules(cPRPanInterpretor, userInterface->curvedMPRV, false);

    // zoominterpretor for cpr view
    MiaParameterConnector* connector103 = new MiaParameterConnector(this);
    connector103->addParameterPairs(QString("y"),QString("scaling_delta"));
    connector103->linkModules(cPRZoomInterpretor, userInterface->curvedMPRV, false);

    MiaParameterConnector* connector105 = new MiaParameterConnector(this);
    connector105->addParameterPairs(QString("y"),QString("scroll_y"));
    connector105->linkModules(crossSectionWheelInterpretor, crossSectionMPRSlicer, false);

    // paninterpretor for cpr view
    MiaParameterConnector* connector106 = new MiaParameterConnector(this);
    connector106->addParameterPairs(QString("x"),QString("translate_x"));
    connector106->addParameterPairs(QString("y"),QString("translate_y"));
    connector106->linkModules(crossSectionPanInterpretor, crossSectionView, false);

    // zoominterpretor for cpr view
    MiaParameterConnector* connector107 = new MiaParameterConnector(this);
    connector107->addParameterPairs(QString("y"),QString("scaling_delta"));
    connector107->linkModules(crossSectionZoomInterpretor, crossSectionView, false);

    userInterface->curvedMPRV->showCrossHair(false);
    userInterface->curvedMPRV->addInputModule(curvedMPRImage);

    crossSectionView->showCrossHair(false);
    crossSectionView->addInputModule(crossSectionMPRImage);
 //   resetParameterForNextStep();
    connect(userInterface->actionLoadCenterline,SIGNAL(triggered()),this,SLOT(loadVesselCenterline()));
}
void MiaVesselAnalysisNetwork::enableInteractions()
{
    MiaBasicMPRandVRTNetwork::enableInteractions();
    userInterface->actionCurve_ROI->setEnabled(true);
    connect(userInterface->actionCurve_ROI, SIGNAL(triggered()), this, SLOT(setInteractionToCurveROI()));
}

void MiaVesselAnalysisNetwork::changeUserInteractionMode(InteractionMode mode)
{
    MiaBasicMPRandVRTNetwork::changeUserInteractionMode(mode);

    switch(mode)
    {
    case MIA_Interaction_None:
        userInterface->curvedMPRV->setmouseLeftDragInterpretor( NULL );
        crossSectionView->setmouseLeftDragInterpretor( NULL );
        userInterface->curvedMPRV->setCursor(Qt::ArrowCursor);
        crossSectionView->setCursor(Qt::ArrowCursor);
        break;
    case MIA_Interaction_Window_Setting:
        userInterface->curvedMPRV->setmouseLeftDragInterpretor( winSettingInterpretor );
        userInterface->curvedMPRV->setCursor(MiaImageView2D::getWinCursor());

        crossSectionView->setmouseLeftDragInterpretor(winSettingInterpretor);
        crossSectionView->setCursor(MiaImageView2D::getWinCursor());
        break;
    case MIA_Interaction_Pan:
        userInterface->curvedMPRV->setmouseLeftDragInterpretor( cPRPanInterpretor );
        userInterface->curvedMPRV->setCursor(MiaImageView2D::getPanCursor());

        crossSectionView->setmouseLeftDragInterpretor(crossSectionPanInterpretor);
        crossSectionView->setCursor(MiaImageView2D::getPanCursor());
        break;
    case MIA_Interaction_Zoom:
        userInterface->curvedMPRV->setmouseLeftDragInterpretor(cPRZoomInterpretor);
        userInterface->curvedMPRV->setCursor(MiaImageView2D::getZoomCursor());

        crossSectionView->setmouseLeftDragInterpretor(crossSectionZoomInterpretor);
        crossSectionView->setCursor(MiaImageView2D::getZoomCursor());
        break;
     case MIA_Interaction_ROI_Open_Curve:
        initialCenterlineMarkerManager->currentType =MIA_Spline;
        firstStepLongitudinalBorderMarkerManagerNo1->currentType = MIA_Spline;
        firstStepLongitudinalBorderMarkerManagerNo2->currentType = MIA_Spline;
        firstStepLongitudinalBorderMarkerManagerNo3->currentType = MIA_Spline;
        secondStepCrossSectionBorderMarkerManager->currentType = MIA_ClosedSpline;
        userInterface->leftUpperView->setmouseLeftDragInterpretor( axialMarkerInterpretor );
        userInterface->leftUpperView->setKeyboardInterpretor(axialMarkerInterpretor);
        userInterface->leftUpperView->setCursor(Qt::ArrowCursor);

        userInterface->rightUpperView->setmouseLeftDragInterpretor( coronalMarkerInterpretor );
        userInterface->rightUpperView->setKeyboardInterpretor(coronalMarkerInterpretor);
        userInterface->rightUpperView->setCursor(Qt::ArrowCursor);

        userInterface->leftLowerView->setmouseLeftDragInterpretor( saggitalMarkerInterpretor );
        userInterface->leftLowerView->setKeyboardInterpretor(saggitalMarkerInterpretor);
        userInterface->leftLowerView->setCursor(Qt::ArrowCursor);

        userInterface->curvedMPRV->setmouseLeftDragInterpretor(cPRMarkerInterpretor);
        userInterface->curvedMPRV->setKeyboardInterpretor(cPRMarkerInterpretor);
        userInterface->curvedMPRV->setCursor(Qt::ArrowCursor);

        crossSectionView->setmouseLeftDragInterpretor(crossSectionInterpretor);
        crossSectionView->setKeyboardInterpretor(crossSectionInterpretor);
        crossSectionView->setCursor(Qt::ArrowCursor);
        break;
    default:
        qDebug()<<"unsupported interaction mode";
        throw std::runtime_error("unsupported interaction mode");
    }

}

void MiaVesselAnalysisNetwork::activeUserInteraction()
{

}

void MiaVesselAnalysisNetwork::deactiveUserInteraction()
{

}

void MiaVesselAnalysisNetwork::startVesselAnalysis()
{
//    axialslicer->setInputModule(inputModule);
//    saggitalslicer->setInputModule(inputModule);
//    statusReader->setInputModule(inputModule);
    requiredIterations = 20;
    centerRadiusReader->setCenterlineSmoothingRange(300);

    centerlineDilater->update();
    thirdDistanceTransformModule->update();
    intensityCubicMapper->update();
    vesselWallThresholdMappingFilter->update();
    intensityAndDistanceCombiner->update();
    lsVesselSegmenter->update();
    userInterface->startVesselAnalysisButton->setEnabled(false);
    statusReader->setInputModule(intensityAndDistanceCombiner);
//    lsVesselSegmenter->update();
    levelsetRunning = true;
    lsVesselSegmenter->start();
    timer->start(1000);
    userInterface->vesselProgressBar->setVisible(true);
    userInterface->vesselProgressBar->setValue(0);
    userInterface->vesselStatusLabel->setText("Starting...");


}
void MiaVesselAnalysisNetwork::startVesselAnalysisAutomatically()
{
    isInAutomtaticProcessing = true;
    switchToVesselAnalysisPage(MIA_VA_Auto_Segmentation);
    startVesselAnalysis();
    levelsetRunning = true;
    timer->start(1000);
}


void MiaVesselAnalysisNetwork::stopVesselAnalysis()
{
    requiredIterations = 0;
    lsVesselSegmenter->stop();
}

void MiaVesselAnalysisNetwork::loadVesselCenterline()
{
    QString fileName = QFileDialog::getOpenFileName((QWidget*)parent(), tr("Open File"), 0, 0);
    if (fileName != "")
    {
        loadVesselCenterlineFromFile(fileName);
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}
bool  MiaVesselAnalysisNetwork::loadVesselCenterlineFromFile(QString file)
{
    QList <MiaMarker*> list;
    initialCenterlineMarkerManager->getAllMarkers(list);
    for(int j = 0; j < list.size(); j++)
    {
        MiaMarker3DPolygon* marker = (MiaMarker3DPolygon*)list.at(j);
        for(int i = 0 ; i < marker->getKeyPointNumber(); i++)
        {
            MiaPoint4D v;
            v.pos[3] = -1.0;
            marker->keyValueList.append(v);
        }
//        marker->resampleKeyPointsWithStepLength(0.1);
    }


    qDebug()<<"load centerlines"<<file;
    MiaImageND* im;
    inputModule->getOutputImage(&im);
//    initialCenterlineMarkerManager->loadCenterLineFromMICCAIChallenge(file,im->getTransformMatrix());
    initialCenterlineMarkerManager->loadAllCenterLinesFromMICCAIChallenge(file,im->getTransformMatrix());
//    firstStepCenterlineMarkerManager->loadMarkersFromAnotherManager(initialCenterlineMarkerManager);
//    secondStepCenterlineMarkerManager->loadMarkersFromAnotherManager(initialCenterlineMarkerManager);
//    thirdStepCenterlineMarkerManager->loadMarkersFromAnotherManager(initialCenterlineMarkerManager);
//    cropCenterlines(firstStepCenterlineMarkerManager,20);
    initialCenterlineMarkerManager->freezeAllMarkers(true);
//    changeUserInteractionMode(MIA_Interaction_ROI_Open_Curve);
//    nextPresetAngle();
//    nextPresetPosition();
    if(initialCenterlineMarkerManager->rowCount() > 0)
        switchToVesselAnalysisPage(MIA_VA_Auto_Segmentation);
    return true;
}

void MiaVesselAnalysisNetwork::onToolboxPageChanged(int index)
{
//    if(index = userInterface->toolBox->indexOf(userInterface->VesselAnalysisPage))
//    {
//    }
//    switchToVesselAnalysisPage(MIA_VA_Manual_Centerline);

}
void MiaVesselAnalysisNetwork::switchViewInputImage(int source)
{
    switch(source)
    {
    case 0:
        axialslicer->setInputModule(inputModule);
        statusReader->setInputModule(inputModule);
        break;
    case 1:
        axialslicer->setInputModule(thirdDistanceTransformModule);
        statusReader->setInputModule(thirdDistanceTransformModule);
        break;
    case 2:
        axialslicer->setInputModule(vesselWallThresholdMappingFilter);
        statusReader->setInputModule(vesselWallThresholdMappingFilter);
        break;
    case 3:
        axialslicer->setInputModule(intensityAndDistanceCombiner);
        statusReader->setInputModule(intensityAndDistanceCombiner);
        break;

    default:
        axialslicer->setInputModule(inputModule);
        statusReader->setInputModule(inputModule);
        break;
    }
}
void MiaVesselAnalysisNetwork::switchToVesselAnalysisPage(VesselAnalysisMode step)
{
    QList<int> defaultSpliterSizeList;
    //turn off the modules that is related to the current step;
    switch(currentStep)
    {
    case MIA_VA_Uninitialized:
        userInterface->gridLayout->removeWidget(userInterface->rightLowerView);
        userInterface->rightLowerView->setVisible(false);
        axialLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        coronalLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        axialViewThresholder->autoUpdateWhenInputChanged = false;
        saggitalViewThresholder->autoUpdateWhenInputChanged = false;
        coronalViewThresholder->autoUpdateWhenInputChanged = false;
        statusReader->setInputModule(inputModule);

        break;
    case MIA_VA_Manual_Centerline:
        userInterface->loadCrossSectionButton->setVisible(false);
        userInterface->exportCrossSectionButton->setVisible(false);
        userInterface->exportVesselAnalysisButton->setVisible(false);
        userInterface->gridLayout->removeWidget(crossSectionView);
        crossSectionView->setVisible(false);
        userInterface->leftUpperView->removeMarkerModule(axialViewCentlineMarkerAgent);
        userInterface->rightUpperView->removeMarkerModule(coronalViewCenterlineMarkerAgent);
        userInterface->leftLowerView->removeMarkerModule(saggitalViewCenterlineMarkerAgent);
        userInterface->curvedMPRV->removeMarkerModule(cPRViewCenterlineMarkerAgent);

        axialMarkerInterpretor->forwardMessageTo(NULL);
        coronalMarkerInterpretor->forwardMessageTo(NULL);
        saggitalMarkerInterpretor->forwardMessageTo(NULL);
        cPRMarkerInterpretor->forwardMessageTo(NULL);
        crossSectionInterpretor->forwardMessageTo(NULL);

        break;
    case MIA_VA_Manual_Border:
        userInterface->loadCrossSectionButton->setVisible(false);
        userInterface->exportCrossSectionButton->setVisible(false);
        userInterface->exportVesselAnalysisButton->setVisible(false);
        userInterface->gridLayout->removeWidget(crossSectionView);
        crossSectionView->setVisible(false);
        userInterface->leftUpperView->removeMarkerModule(axialViewCentlineMarkerAgent);
        userInterface->rightUpperView->removeMarkerModule(coronalViewCenterlineMarkerAgent);
        userInterface->leftLowerView->removeMarkerModule(saggitalViewCenterlineMarkerAgent);
        userInterface->curvedMPRV->removeMarkerModule(cPRViewCenterlineMarkerAgent);

        userInterface->leftUpperView->removeMarkerModule(axialViewLongitudinalBorderMarkerAgent);
        userInterface->rightUpperView->removeMarkerModule(coronalViewLongitudinalBorderMarkerAgent);
        userInterface->leftLowerView->removeMarkerModule(saggitalViewLongitudinalBorderMarkerAgent);
        userInterface->curvedMPRV->removeMarkerModule(cPRViewLongitudinalMarkerAgentNo1);
        userInterface->curvedMPRV->removeMarkerModule(cPRViewLongitudinalMarkerAgentNo2);
        userInterface->curvedMPRV->removeMarkerModule(cPRViewLongitudinalMarkerAgentNo3);

        crossSectionView->removeMarkerModule(crossSectionViewLongitudinalMarkerAgentNo1);
        crossSectionView->removeMarkerModule(crossSectionViewLongitudinalMarkerAgentNo2);
        crossSectionView->removeMarkerModule(crossSectionViewLongitudinalMarkerAgentNo3);
        crossSectionView->removeMarkerModule(crossSectionViewCrossSectionMarkerAgent);
        nextPresetAngle();
        break;
    case MIA_VA_Auto_Segmentation:
        userInterface->exportVTKButton->setVisible(false);
        userInterface->exportVesselAnalysisButton->setVisible(false);
        userInterface->gridLayout->removeWidget(userInterface->rightLowerView);
        userInterface->rightLowerView->setVisible(false);
        userInterface->leftUpperView->removeMarkerModule(axialViewCentlineMarkerAgent);
        userInterface->rightUpperView->removeMarkerModule(coronalViewCenterlineMarkerAgent);
        userInterface->leftLowerView->removeMarkerModule(saggitalViewCenterlineMarkerAgent);
        userInterface->curvedMPRV->removeMarkerModule(cPRViewCenterlineMarkerAgent);

        userInterface->leftUpperView->removInputModule(axialViewMask);
        userInterface->rightUpperView->removInputModule(coronalViewMask);
        userInterface->leftLowerView->removInputModule(saggitalViewMask);

        userInterface->leftUpperView->removInputModule(axialLevelSetQimage);
        userInterface->rightUpperView->removInputModule(coronalLevelSetQimage);
        userInterface->leftLowerView->removInputModule(saggitalLevelSetQimage);

        axialLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        coronalLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = false;
        axialViewThresholder->autoUpdateWhenInputChanged = false;
        saggitalViewThresholder->autoUpdateWhenInputChanged = false;
        coronalViewThresholder->autoUpdateWhenInputChanged = false;
        statusReader->setInputModule(inputModule);
        break;
    }
    currentStep = step;
    //turn on the modules that is related to the set step;
    switch(currentStep)
    {
    case MIA_VA_Uninitialized:
        userInterface->gridLayout->addWidget(userInterface->rightLowerView,1,1);
        userInterface->rightLowerView->setVisible(true);
        break;
    case MIA_VA_Manual_Centerline:
        userInterface->loadCrossSectionButton->setVisible(true);
        userInterface->exportCrossSectionButton->setVisible(true);
        userInterface->exportVesselAnalysisButton->setVisible(true);
        userInterface->gridLayout->addWidget(crossSectionView,1,1);
        crossSectionView->setVisible(true);
        userInterface->curvedMPRPage->setVisible(true);

        defaultSpliterSizeList.append(3);
        defaultSpliterSizeList.append(1);
        userInterface->mPRSplitter->setSizes(defaultSpliterSizeList);
        userInterface->leftUpperView->addMarkerModule(axialViewCentlineMarkerAgent);
        userInterface->rightUpperView->addMarkerModule(coronalViewCenterlineMarkerAgent);
        userInterface->leftLowerView->addMarkerModule(saggitalViewCenterlineMarkerAgent);
        userInterface->curvedMPRV->addMarkerModule(cPRViewCenterlineMarkerAgent);

        axialMarkerInterpretor->forwardMessageTo(initialCenterlineMarkerManager);
        coronalMarkerInterpretor->forwardMessageTo(initialCenterlineMarkerManager);
        saggitalMarkerInterpretor->forwardMessageTo(initialCenterlineMarkerManager);
        cPRMarkerInterpretor->forwardMessageTo(initialCenterlineMarkerManager);
        break;
    case MIA_VA_Manual_Border:
        userInterface->loadCrossSectionButton->setVisible(true);
        userInterface->exportCrossSectionButton->setVisible(true);
        userInterface->exportVesselAnalysisButton->setVisible(true);
        userInterface->gridLayout->addWidget(crossSectionView,1,1);
        crossSectionView->setVisible(true);
        userInterface->curvedMPRPage->setVisible(true);
        defaultSpliterSizeList.append(3);
        defaultSpliterSizeList.append(1);
        userInterface->mPRSplitter->setSizes(defaultSpliterSizeList);
        userInterface->leftUpperView->addMarkerModule(axialViewCentlineMarkerAgent);
        userInterface->rightUpperView->addMarkerModule(coronalViewCenterlineMarkerAgent);
        userInterface->leftLowerView->addMarkerModule(saggitalViewCenterlineMarkerAgent);
        userInterface->curvedMPRV->addMarkerModule(cPRViewCenterlineMarkerAgent);

        userInterface->leftUpperView->addMarkerModule(axialViewLongitudinalBorderMarkerAgent);
        userInterface->rightUpperView->addMarkerModule(coronalViewLongitudinalBorderMarkerAgent);
        userInterface->leftLowerView->addMarkerModule(saggitalViewLongitudinalBorderMarkerAgent);
        userInterface->curvedMPRV->addMarkerModule(cPRViewLongitudinalMarkerAgentNo1);
        userInterface->curvedMPRV->addMarkerModule(cPRViewLongitudinalMarkerAgentNo2);
        userInterface->curvedMPRV->addMarkerModule(cPRViewLongitudinalMarkerAgentNo3);
        crossSectionView->addMarkerModule(crossSectionViewLongitudinalMarkerAgentNo1);
        crossSectionView->addMarkerModule(crossSectionViewLongitudinalMarkerAgentNo2);
        crossSectionView->addMarkerModule(crossSectionViewLongitudinalMarkerAgentNo3);
        crossSectionView->addMarkerModule(crossSectionViewCrossSectionMarkerAgent);

        axialMarkerInterpretor->forwardMessageTo(initialCenterlineMarkerManager);
        coronalMarkerInterpretor->forwardMessageTo(initialCenterlineMarkerManager);
        saggitalMarkerInterpretor->forwardMessageTo(initialCenterlineMarkerManager);
        cPRMarkerInterpretor->forwardMessageTo(firstStepLongitudinalBorderMarkerManagerNo1);
        crossSectionInterpretor->forwardMessageTo(secondStepCrossSectionBorderMarkerManager);
        break;
    case MIA_VA_Auto_Segmentation:
        userInterface->exportVTKButton->setVisible(true);
        userInterface->exportVesselAnalysisButton->setVisible(true);
        userInterface->gridLayout->addWidget(userInterface->rightLowerView,1,1);
        userInterface->rightLowerView->setVisible(true);
        userInterface->curvedMPRPage->setVisible(false);
        defaultSpliterSizeList.append(1);
        defaultSpliterSizeList.append(0);
        userInterface->mPRSplitter->setSizes(defaultSpliterSizeList);
        userInterface->leftUpperView->addMarkerModule(axialViewCentlineMarkerAgent);
        userInterface->rightUpperView->addMarkerModule(coronalViewCenterlineMarkerAgent);
        userInterface->leftLowerView->addMarkerModule(saggitalViewCenterlineMarkerAgent);
        userInterface->curvedMPRV->addMarkerModule(cPRViewCenterlineMarkerAgent);

        userInterface->leftUpperView->addInputModule(axialViewMask);
        userInterface->rightUpperView->addInputModule(coronalViewMask);
        userInterface->leftLowerView->addInputModule(saggitalViewMask);

        userInterface->leftUpperView->addInputModule(axialLevelSetQimage);
        userInterface->rightUpperView->addInputModule(coronalLevelSetQimage);
        userInterface->leftLowerView->addInputModule(saggitalLevelSetQimage);

        axialLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        coronalLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        saggitalLevelSetSlicer->autoUpdateWhenParameterChanged = true;
        axialViewThresholder->autoUpdateWhenInputChanged = true;
        saggitalViewThresholder->autoUpdateWhenInputChanged = true;
        coronalViewThresholder->autoUpdateWhenInputChanged = true;
        break;
    }
//    if(vesselAnalysisGUIOn != step)
//    {
//        vesselAnalysisGUIOn = step;
//        if(vesselAnalysisGUIOn)
//        {

//        }
//        else
//        {


//        }
//    }

}

void MiaVesselAnalysisNetwork::continueVesselAnalysis()
{
    if(levelsetRunning)
    {
        return;
    }
    timer->stop();
    requiredIterations--;
    userInterface->vesselProgressBar->setValue((20-requiredIterations)*5);
    QVariant status_str(20-requiredIterations);
    userInterface->vesselStatusLabel->setText(status_str.toString().append("iterations"));
    qDebug()<<"iteration remain"<<requiredIterations;
    if(requiredIterations<=0)
    {
        if(isInAutomtaticProcessing)
        {
//            QVariant  s(currentVesselAnalysisStep);
//            QString resultFileName = resultSavingPath;
//            resultFileName.append(s.toString());
//            resultFileName.append("_");
//            exportVesselAnalysisResultToPath(resultFileName);
//            exporter->setFileName(resultFileName+"mask");
//            exporter->update();

            QString stlFileName = resultSavingPath;
            isosurface->saveSTLFile(stlFileName.append("mesh.stl"));
            QString centerlineFileName = resultSavingPath;
            exportVesselAnalysisResultToPath(centerlineFileName.append("centerline.txt"));
            QApplication::exit(0);
            return;
//            statistics->update();
//            resultFileName.append("statistics");
//            QFile file(resultFileName);
//            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//                return;
//            QTextStream out(&file);
//            out << statistics->summaryStr;
//            QApplication::exit(0);
//            if(currentVesselAnalysisStep == 0)
//            {
////                userInterface->step2_RadioButton->setChecked(true);
//                secondStepCrossSectionBorderMarkerManager->loadMarkersFromAnotherManager(firstStepLongitudinalBorderMarkerManagerNo1);

//            }
//            else if(currentVesselAnalysisStep == 1)
//            {
////                userInterface->step3_RadioButton->setChecked(true);
//                thirdStepCenterlineMarkerManager->loadMarkersFromAnotherManager(secondStepCrossSectionBorderMarkerManager);
//                combineFirstAndSecondStepCenterlines();

//            }
//            else if(currentVesselAnalysisStep >= 2)
//            {
//                QString vtkFileName = resultSavingPath;
//                vtkFileName.remove("result_step", Qt::CaseInsensitive);
//                isosurface->saveVTKFile(vtkFileName.append("segmentation.vtk"));
//                QApplication::exit(0);
//                return;
//            }
//            currentVesselAnalysisStep++;
//            resetParameterForNextStep();
//            startVesselAnalysisAutomatically();
        }
        userInterface->startVesselAnalysisButton->setEnabled(true);
        userInterface->vesselProgressBar->setVisible(false);
        userInterface->vesselStatusLabel->setText("Not running");
        return;
    }

//    userInterface->continueVesselAnalysisButton->setEnabled(false);
//    userInterface->vesselAnalysisGroupBox->setEnabled(false);
    QTime t;
     t.start();

    zeroDistanceTransformModule->update();

    qDebug() << "*********************zero DT************ time elapsed: " << t.elapsed();
    intensityReverter->update();
    firstDistanceTransformModule->update();
    qDebug() << "*********************1s DT************ time elapsed: " << t.elapsed();
//    intensityReverter->update(); // set the level set back to original
    intensityExponentialMapper->update();
    secondDistanceTransformModule->update();
    qDebug() << "*********************2nd DT************ time elapsed: " << t.elapsed();
    centerRadiusReader->update();
    centerlineDilater->update();
//    endocentricMapGenerator->update();//this must run before thirdDistanceTransformModule, because the latter will change the value
    thirdDistanceTransformModule->update();
    qDebug() << "*********************3rd DT************ time elapsed: " << t.elapsed();
    intensityCubicMapper->update();
    vesselWallThresholdMappingFilter->update();
    intensityAndDistanceCombiner->update();


/***** testing part for principle curvatue. Does not help, setting curvature force lower instead***/
//    MiaImageND* im;
//    inputModule->getOutputImage(&im);

//    QList <MiaMarker*> centerlineList;
//    currentCenterlineMarkerManager->getAllMarkers(centerlineList);
//    for(int i = 0; i < centerlineList.size() ; i++)
//    {
//        MiaMarker3DPolygon* marker = (MiaMarker3DPolygon*)centerlineList.at(i);
//        marker->setDirectionVectorAsKeyValues(im);
//    }
//    centerlinevalueInitialSpreader->update();
//    centerlinevalueSpreader->update();
//    centerlinevalueSpreader->getOutputImage(&im);


//    axialslicer->setInputModule(firstDistanceTransformModule);
//    saggitalslicer->setInputModule(firstDistanceTransformModule);
//    statusReader->setInputModule(firstDistanceTransformModule);

//    userInterface->leftUpperView->removInputModule(axialViewMask);
//    userInterface->rightUpperView->removInputModule(coronalViewMask);
//    userInterface->leftLowerView->removInputModule(saggitalViewMask);

//    userInterface->leftUpperView->removInputModule(axialLevelSetQimage);
//    userInterface->rightUpperView->removInputModule(coronalLevelSetQimage);
//    userInterface->leftLowerView->removInputModule(saggitalLevelSetQimage);

//    lsVesselSegmenter->resetExternalForce();
    lsVesselSegmenter->update();
    lsVesselSegmenter->start();
    levelsetRunning = true;
    timer->start(1000);
    qDebug() << "*********************centerline update************ time elapsed: " << t.elapsed();


}
void MiaVesselAnalysisNetwork::levelsetVesselSegmentationCompleted()
{
//    userInterface->continueVesselAnalysisButton->setEnabled(true);
    isosurface->update();
    levelsetRunning = false;

}

void MiaVesselAnalysisNetwork::setVesselSmoothFactor(int var)
{
    double factor = (double)var/100;
    if( factor !=  userInterface->smoothFactorSpin_2->value() )
        userInterface->smoothFactorSpin_2->setValue(factor);
    lsVesselSegmenter->setCurvatureWeight(factor);

}

void MiaVesselAnalysisNetwork::setVesselSmoothFactor(double var)
{
    double factor = var*100;
    if( (int)factor !=  userInterface->smoothFactorSlider_2->value() )
        userInterface->smoothFactorSlider_2->setValue((int)(factor+0.5));
    lsVesselSegmenter->setCurvatureWeight(var);
}
void MiaVesselAnalysisNetwork::exportVesselAnaysisResult()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
        exportVesselAnalysisResultToPath(fileName);
    }
    else
    {
        qDebug() << "User canceled save file";
    }


}
void MiaVesselAnalysisNetwork::exportVesselAnalysisResultToPath(QString path)
{
    QList <MiaMarker*> markerList;
    initialCenterlineMarkerManager->getAllMarkers(markerList);
    MiaImageND* im;
    inputModule->getOutputImage(&im);
    MiaTransform transform = im->getTransformMatrix();

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    for(int i =0; i < markerList.size(); i++)
    {
        MiaMarker* marker = markerList.at(i);

        QVariant index(i);

        for(int j = 0; j < marker->getKeyPointNumber(); j++)
        {
            MiaPoint4D pt = marker->keyPointList.at(j);
//            pt = transform.fromPatientCoordinate(pt);
            float radius = marker->keyValueList.at(j).pos[3];
            out << index.toString()
                << " " << pt.pos[0]
                << " " << pt.pos[1]
                << " " << pt.pos[2]
                << " "  << -radius+0.5
                << "\n";
        }
    }
    file.close();
}

void MiaVesselAnalysisNetwork::resetParameterForNextStep()
{
    return;
//    if(userInterface->step1_RadioButton->isChecked())
    {
        requiredIterations = 20;
        centerRadiusReader->setCenterlineSmoothingRange(300);
        float ww = (upperThreshold-lowerThreshold)/2.0;
        QList<QVariant> lut;
        lut.append(QPointF(lowerThreshold-ww,-1.0));
        lut.append(QPointF(lowerThreshold+ww,1.0));
//        lut.append(QPointF(600-50,1.0));
//        lut.append(QPointF(600+50,-1.0));
        vesselWallThresholdMappingFilter->setToLUTMapping(lut);
//        switchCenterlineManager(firstStepLongitudinalBorderMarkerManager);
    }
//    else if(userInterface->step2_RadioButton->isChecked())
    {
        requiredIterations = 10;
        centerRadiusReader->setCenterlineSmoothingRange(60);
        float ww = (upperThreshold-lowerThreshold)/2.0;
        QList<QVariant> lut;
        lut.append(QPointF(lowerThreshold-ww,-1.0));
        lut.append(QPointF(lowerThreshold+ww,1.0));
//        lut.append(QPointF(upperThreshold-50,1.0));
        lut.append(QPointF(upperThreshold+ww,-1.0));
        vesselWallThresholdMappingFilter->setToLUTMapping(lut);
//        switchCenterlineManager(secondStepCrossSectionBorderMarkerManager);
    }
//    else if(userInterface->step3_RadioButton->isChecked())
    {
        requiredIterations = 0;
        centerRadiusReader->setCenterlineSmoothingRange(60);
        float ww = (upperThreshold-lowerThreshold)/2.0;
        QList<QVariant> lut;
        lut.append(QPointF(lowerThreshold-ww,-1.0));
        lut.append(QPointF(lowerThreshold+ww,1.0));
//        lut.append(QPointF(upperThreshold-50,1.0));
        lut.append(QPointF(upperThreshold+ww,-1.0));
        vesselWallThresholdMappingFilter->setToLUTMapping(lut);
//        switchCenterlineManager(thirdStepCenterlineMarkerManager);
    }
}
void MiaVesselAnalysisNetwork::cropCenterlines(MiaDrawingManager* manager,float threshold)
{
    QList <MiaMarker*> markerList;
    manager->getAllMarkers(markerList);
    for(int i =0; i < markerList.size(); i++)
    {
        float len = 0;
        MiaMarker* marker = markerList.at(i);
        QList <MiaPoint4D> newCenterline;
        newCenterline.append(marker->getKeyPointAt(0));
        for(int j = 1; j < marker->getKeyPointNumber(); j++)
        {
            len += marker->getKeyPointAt(j).distance(marker->getKeyPointAt(j-1));
            if(len>threshold)
            {
                break;
            }
            newCenterline.append(marker->getKeyPointAt(j));

        }
        marker->setKeyPoints(newCenterline);
    }
}
void MiaVesselAnalysisNetwork::combineFirstAndSecondStepCenterlines()
{
    QList <MiaMarker*> fistStepCenterlineList;
    QList <MiaMarker*> secondStepCenterlineList;
    QList <MiaMarker*> thirdStepCenterlineList;
    firstStepLongitudinalBorderMarkerManagerNo1->getAllMarkers(fistStepCenterlineList);
    secondStepCrossSectionBorderMarkerManager->getAllMarkers(secondStepCenterlineList);
    thirdStepCenterlineMarkerManager->getAllMarkers(thirdStepCenterlineList);
    if(fistStepCenterlineList.size()!=secondStepCenterlineList.size()||secondStepCenterlineList.size()!=thirdStepCenterlineList.size())
    {
        qDebug()<<"fail to generate centerline for step 3";
        return;
    }
    for(int i =0; i < fistStepCenterlineList.size(); i++)
    {
        MiaMarker3DPolygon* centerline1 = (MiaMarker3DPolygon*)fistStepCenterlineList.at(i);
        MiaMarker3DPolygon* centerline2 = (MiaMarker3DPolygon*)secondStepCenterlineList.at(i);
        MiaMarker3DPolygon* centerline3 = (MiaMarker3DPolygon*)thirdStepCenterlineList.at(i);
        centerline1->resampleKeyPointsWithStepLength(0.05);
        centerline2->resampleKeyPointsWithStepLength(0.05);
        centerline3->resampleKeyPointsWithStepLength(0.05);
        int keypointNumber = centerline1->getKeyPointNumber();
        if(keypointNumber > centerline2->getKeyPointNumber())
            keypointNumber = centerline2->getKeyPointNumber();
        if(keypointNumber > centerline3->getKeyPointNumber())
            keypointNumber = centerline3->getKeyPointNumber();
        for(int j =0; j<keypointNumber; j++)
        {
            centerline3->keyValueList[j].pos[3] = std::max(centerline1->keyValueList[j].pos[3],centerline2->keyValueList[j].pos[3]);
//            qDebug()<<centerline3->keyValueList[j].pos[3] << centerline1->keyValueList[j].pos[3] << centerline2->keyValueList[j].pos[3];
        }
    }

}

void MiaVesselAnalysisNetwork::loadSegmentationResultFromRaw()
{
    QString fileName = QFileDialog::getOpenFileName((QWidget*)parent(), tr("Open File"), 0, 0);
    if (fileName != "")
    {
        rawDataloader = new MiaImageReader(this);
        rawDataloader->setFileName(fileName,MIA_MHD);
        int result = rawDataloader->update();
        if(result)
        {
            MiaImageND* im;
            rawDataloader->getOutputImage(&im);
            MiaImageND* levelsetResult;
            lsVesselSegmenter->getOutputImage(&levelsetResult);
            if(im && levelsetResult && im->getImageSize() == levelsetResult->getImageSize()&&im->getType()==IM_short&&levelsetResult->getType()==IM_float)
            {
                float* out = (float*)levelsetResult->getDataPtr();
                short* in = (short*)im->getDataPtr();
                int totalsize =im->getImageSize();
                for(int i =0 ; i < totalsize; i++)
                {
                    out[i] = -(float)(in[i])*0.01f;
                }
                lsVesselSegmenter->update();
            }
        }
        delete rawDataloader;
    }
    else
    {
        qDebug() << "User canceled open file";
    }

}

void MiaVesselAnalysisNetwork::exportVTKFile()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
        isosurface->saveSTLFile(fileName);
        exporter->setFileName(fileName+"mask", MIA_MHD);
        exporter->update();
    }
    else
    {
        qDebug() << "User canceled save file";
    }
}

void MiaVesselAnalysisNetwork::nextPresetAngle()
{
    currentCPRAngle = (currentCPRAngle+1)%3;
    curvedMPRSlicer->setRotationAngle(currentCPRAngle*E_PI/3.0f);
    firstStepLongitudinalBorderMarkerManagerNo1->selectAnotherMarker(NULL);
    firstStepLongitudinalBorderMarkerManagerNo2->selectAnotherMarker(NULL);
    firstStepLongitudinalBorderMarkerManagerNo3->selectAnotherMarker(NULL);
    userInterface->curvedMPRV->removeMarkerModule(cPRViewLongitudinalMarkerAgentNo1);
    userInterface->curvedMPRV->removeMarkerModule(cPRViewLongitudinalMarkerAgentNo2);
    userInterface->curvedMPRV->removeMarkerModule(cPRViewLongitudinalMarkerAgentNo3);
    switch(currentCPRAngle)
    {
    case 0:
        cPRMarkerInterpretor->forwardMessageTo(firstStepLongitudinalBorderMarkerManagerNo1);
        firstStepLongitudinalBorderMarkerManagerNo1->freezeAllMarkers(false);
        firstStepLongitudinalBorderMarkerManagerNo2->freezeAllMarkers(true);
        firstStepLongitudinalBorderMarkerManagerNo3->freezeAllMarkers(true);
        userInterface->curvedMPRV->addMarkerModule(cPRViewLongitudinalMarkerAgentNo1);

        break;
    case 1:
        cPRMarkerInterpretor->forwardMessageTo(firstStepLongitudinalBorderMarkerManagerNo2);
        firstStepLongitudinalBorderMarkerManagerNo1->freezeAllMarkers(true);
        firstStepLongitudinalBorderMarkerManagerNo2->freezeAllMarkers(false);
        firstStepLongitudinalBorderMarkerManagerNo3->freezeAllMarkers(true);
        userInterface->curvedMPRV->addMarkerModule(cPRViewLongitudinalMarkerAgentNo2);
        break;
    case 2:
        cPRMarkerInterpretor->forwardMessageTo(firstStepLongitudinalBorderMarkerManagerNo3);
        firstStepLongitudinalBorderMarkerManagerNo1->freezeAllMarkers(true);
        firstStepLongitudinalBorderMarkerManagerNo2->freezeAllMarkers(true);
        firstStepLongitudinalBorderMarkerManagerNo3->freezeAllMarkers(false);
        userInterface->curvedMPRV->addMarkerModule(cPRViewLongitudinalMarkerAgentNo3);
        break;
    }
}

void MiaVesselAnalysisNetwork::nextPresetPosition()
{
    secondStepCrossSectionBorderMarkerManager->selectAnotherMarker(NULL);
    currentCrossSectionPostion = (currentCrossSectionPostion+1)%10;
    crossSectionMPRSlicer->scrollTo(15.0f+currentCrossSectionPostion*1.5f);

}

void MiaVesselAnalysisNetwork::saveCrossSectionSegmentation()
{
    if(!resultSavingPath.isEmpty())
    {
        saveCrossSectionSegmentationToFile(resultSavingPath);
        QApplication::exit(0);
        return;
    }
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
//        secondStepCrossSectionBorderMarkerManager->saveClosedSplineToVTKPolyData(fileName);
        saveCrossSectionSegmentationToFile(fileName);
    }
    else
    {
        qDebug() << "User canceled save file";
    }
}

void MiaVesselAnalysisNetwork::loadCrossSectionSegmentation()
{
    QString fileName = QFileDialog::getOpenFileName((QWidget*)parent(), tr("Open File"), 0, 0);
    if (fileName != "")
    {
        loadCrossSectionSegmentationFromFile(fileName);
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}

void MiaVesselAnalysisNetwork::initializeManuallyCreatedCenterlines()
{
    if(currentStep == MIA_VA_Uninitialized)
    {
        userInterface->initializeVesselCenterlineButton->setText("Initialize Centerlines");
        switchToVesselAnalysisPage(MIA_VA_Manual_Centerline);
    }
    else if(currentStep == MIA_VA_Manual_Centerline)
    {
        if(currentStep != MIA_VA_Auto_Segmentation)
        {
            switchToVesselAnalysisPage(MIA_VA_Auto_Segmentation);
        }
        QList <MiaMarker*> centerlineList;
        initialCenterlineMarkerManager->getAllMarkers(centerlineList);
        for(int mi = 0 ; mi < centerlineList.count(); mi ++)
        {
            MiaMarker3DBSpline* marker = (MiaMarker3DBSpline*)centerlineList.at(mi);
            marker->keyValueList.clear();
            for(int i = 0 ; i < marker->getKeyPointNumber(); i++)
            {
                MiaPoint4D v;
                v.pos[3] = -1.5;
                marker->keyValueList.append(v);
            }
            marker->resampleKeyPointsWithStepLength(0.1);
        }
    }
}


void MiaVesselAnalysisNetwork::setThreshold(float lower, float upper)
{
    lowerThreshold = lower;
    upperThreshold = upper;
    if(userInterface)
    {
        userInterface->lowerThresholdSlider_4->setValue(lowerThreshold);
        userInterface->upperThresholdSlider_4->setValue(upperThreshold);
    }
    resetParameterForNextStep();
}

void MiaVesselAnalysisNetwork::saveCrossSectionSegmentationToFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    // we will serialize the data into the file
    out << QString("SavedMarkersVer0.2");   // serialize a string
    out << QString("Centerline");
    initialCenterlineMarkerManager->saveMarkers(out);
    out << QString("LongitudinalSegmentationNo1");
    firstStepLongitudinalBorderMarkerManagerNo1->saveMarkers(out);
    out << QString("LongitudinalSegmentationNo2");
    firstStepLongitudinalBorderMarkerManagerNo2->saveMarkers(out);
    out << QString("LongitudinalSegmentationNo3");
    firstStepLongitudinalBorderMarkerManagerNo3->saveMarkers(out);
    out << QString("CrossSectionSegmentation");
    secondStepCrossSectionBorderMarkerManager->saveMarkers(out);
    file.close();
}

void MiaVesselAnalysisNetwork::loadCrossSectionSegmentationFromFile(QString filename)
{
    QFile file(filename);
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
    initialCenterlineMarkerManager->deleteAllMarkers();
    firstStepLongitudinalBorderMarkerManagerNo1->deleteAllMarkers();
    firstStepLongitudinalBorderMarkerManagerNo2->deleteAllMarkers();
    firstStepLongitudinalBorderMarkerManagerNo3->deleteAllMarkers();
    secondStepCrossSectionBorderMarkerManager->deleteAllMarkers();
    while(!in.atEnd())
    {
        QString labelstr;
        in >> labelstr;
        if(labelstr == "Centerline")
        {
            initialCenterlineMarkerManager->loadMarkers(in);
        }
        else if(labelstr == "LongitudinalSegmentationNo1")
        {
            firstStepLongitudinalBorderMarkerManagerNo1->loadMarkers(in);
        }
        else if(labelstr == "LongitudinalSegmentationNo2")
        {
            firstStepLongitudinalBorderMarkerManagerNo2->loadMarkers(in);
        }
        else if(labelstr == "LongitudinalSegmentationNo3")
        {
            firstStepLongitudinalBorderMarkerManagerNo3->loadMarkers(in);
        }
        else if(labelstr == "CrossSectionSegmentation")
        {
            secondStepCrossSectionBorderMarkerManager->loadMarkers(in);
        }


    }
    initialCenterlineMarkerManager->freezeAllMarkers(true);
    file.close();

    firstStepLongitudinalBorderMarkerManagerNo1->setAllMarkersToColor(QColor(Qt::yellow));
    firstStepLongitudinalBorderMarkerManagerNo2->setAllMarkersToColor(QColor(Qt::yellow));
    firstStepLongitudinalBorderMarkerManagerNo3->setAllMarkersToColor(QColor(Qt::yellow));
    secondStepCrossSectionBorderMarkerManager->setAllMarkersToColor(QColor(Qt::yellow));

    firstStepLongitudinalBorderMarkerManagerNo1->setAllMarkersToShowHighlights(false);
    firstStepLongitudinalBorderMarkerManagerNo2->setAllMarkersToShowHighlights(false);
    firstStepLongitudinalBorderMarkerManagerNo3->setAllMarkersToShowHighlights(false);
    secondStepCrossSectionBorderMarkerManager->setAllMarkersToShowHighlights(false);
    nextPresetAngle();
    nextPresetPosition();

}
