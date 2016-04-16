#ifndef MIAVESSELANALYSISNETWORK_H
#define MIAVESSELANALYSISNETWORK_H

#include "miabasicmprandvrtnetwork.h"
class MiaThresholding;
class MiaLUTModule;
class MiaDrawingManager;
class MiaMarker2GraphicsItem;
class MiaImageExporter;
class MiaImage2VTKIsosurface;
class MiaIntensityMappingFilter;
class MiaCenterTrackingOnArrivalMap;
class MiaDistanceTransformFromCenterline;
class MiaMultiInputIntensityMappingFilter;
class MiaMarkerStatistics;
class MiaImagePropertySpreadingFilter;
class MiaSparseFieldLevelSetSegmenter;
class MiaZeroCrossingFilter;
class MiaImageReader;
class MiaRegionStatistics;
class MiaCurvedMPRSlicer;
class MiaObliqueMPRSlicer;
class MiaImageView2D;
class MiaFastMarchingFilter;
enum VesselAnalysisMode {MIA_VA_Uninitialized, MIA_VA_Manual_Centerline,MIA_VA_Manual_Border,MIA_VA_Auto_Segmentation};
class MiaVesselAnalysisNetwork : public MiaBasicMPRandVRTNetwork
{
    Q_OBJECT
public:
    explicit MiaVesselAnalysisNetwork(NetWorkEnviromentVariable* var,QObject *parent = 0);
    virtual void setupNetwork(MiaModule* input);
    virtual void setupUserInterface(Ui::MainWindow* ui);
    virtual void activeUserInteraction();
    virtual void deactiveUserInteraction();
    bool  loadVesselCenterlineFromFile(QString file);
    void startVesselAnalysisAutomatically();
    QString resultSavingPath;
    void setThreshold(float lower, float upper);
    void saveCrossSectionSegmentationToFile(QString filename);
    void loadCrossSectionSegmentationFromFile(QString filename);

signals:

public slots:
    void startVesselAnalysis();
    void stopVesselAnalysis();
    void loadVesselCenterline();
    void continueVesselAnalysis();
    void onToolboxPageChanged(int index);
    void switchViewInputImage(int source);
    void levelsetVesselSegmentationCompleted();
    void setVesselSmoothFactor(int var);
    void setVesselSmoothFactor(double var);
    void exportVesselAnaysisResult();
    void resetParameterForNextStep();
    void loadSegmentationResultFromRaw();
    void exportVTKFile();
    virtual void setInteractionToCurveROI(){changeUserInteractionMode(MIA_Interaction_ROI_Open_Curve);}
    void nextPresetAngle();
    void nextPresetPosition();
    void saveCrossSectionSegmentation();
    void loadCrossSectionSegmentation();
    void initializeManuallyCreatedCenterlines();
protected:
    //MIA Networkmodules
    MiaImageExporter* exporter;
    MiaImage2VTKIsosurface* isosurface ;
    //cpr parts
    MiaImageView2D* crossSectionView;

    MiaCurvedMPRSlicer* curvedMPRSlicer;
    MiaObliqueMPRSlicer* crossSectionMPRSlicer;
    MiaImage2QImage* curvedMPRImage;
    MiaImage2QImage* crossSectionMPRImage;

    MiaInputEventInterpretor* cPRPanInterpretor;
    MiaInputEventInterpretor* cPRZoomInterpretor;
    MiaInputEventInterpretor* cPRWheelInterpretor;

    MiaInputEventInterpretor* crossSectionPanInterpretor;
    MiaInputEventInterpretor* crossSectionZoomInterpretor;
    MiaInputEventInterpretor* crossSectionWheelInterpretor;

    MiaMarker2GraphicsItem* axialViewLongitudinalBorderMarkerAgent ;
    MiaMarker2GraphicsItem* coronalViewLongitudinalBorderMarkerAgent ;
    MiaMarker2GraphicsItem* saggitalViewLongitudinalBorderMarkerAgent ;
    MiaMarker2GraphicsItem* cPRViewLongitudinalMarkerAgentNo1;
    MiaMarker2GraphicsItem* cPRViewLongitudinalMarkerAgentNo2;
    MiaMarker2GraphicsItem* cPRViewLongitudinalMarkerAgentNo3;
    MiaMarker2GraphicsItem* crossSectionViewLongitudinalMarkerAgentNo1;
    MiaMarker2GraphicsItem* crossSectionViewLongitudinalMarkerAgentNo2;
    MiaMarker2GraphicsItem* crossSectionViewLongitudinalMarkerAgentNo3;
    MiaMarker2GraphicsItem* crossSectionViewCrossSectionMarkerAgent;



    MiaOrthogonalSlicer* axialLevelSetSlicer ;
    MiaOrthogonalSlicer* coronalLevelSetSlicer ;
    MiaOrthogonalSlicer* saggitalLevelSetSlicer ;

    MiaImage2QImage* axialLevelSetQimage ;
    MiaImage2QImage* coronalLevelSetQimage ;
    MiaImage2QImage* saggitalLevelSetQimage ;

    MiaThresholding* axialViewThresholder ;
    MiaImage2QImage* axialViewMask ;
    MiaThresholding* coronalViewThresholder ;
    MiaImage2QImage* coronalViewMask ;
    MiaThresholding* saggitalViewThresholder ;
    MiaImage2QImage* saggitalViewMask ;
    MiaLUTModule* axialmasklut ;
    MiaLUTModule* coronalmasklut;
    MiaLUTModule* saggitalmasklut ;


    MiaInputEventInterpretor* axialMarkerInterpretor;
    MiaInputEventInterpretor* coronalMarkerInterpretor;
    MiaInputEventInterpretor* saggitalMarkerInterpretor;
    MiaInputEventInterpretor* cPRMarkerInterpretor;
    MiaInputEventInterpretor* crossSectionInterpretor;

    MiaInputEventInterpretor* thresholdingInterpretor;

    MiaMarker2GraphicsItem* axialViewCentlineMarkerAgent ;
    MiaMarker2GraphicsItem* coronalViewCenterlineMarkerAgent ;
    MiaMarker2GraphicsItem* saggitalViewCenterlineMarkerAgent ;
    MiaMarker2GraphicsItem* cPRViewCenterlineMarkerAgent;

    MiaFastMarchingFilter* zeroDistanceTransformModule; //distance transform from level set contour outward to avoid bug when the vessel diameter is too small
    MiaFastMarchingFilter* firstDistanceTransformModule; //distance transform from level set contour inward
    MiaFastMarchingFilter* secondDistanceTransformModule; //wave propagating from seed point for centerline tracking
    MiaFastMarchingFilter* thirdDistanceTransformModule; //distance transform from the centerline
    MiaFastMarchingFilter* endocentricMapGenerator;
    MiaIntensityMappingFilter* intensityReverter;
    MiaIntensityMappingFilter* intensityExponentialMapper;
    MiaIntensityMappingFilter* intensityCubicMapper;
    MiaCenterTrackingOnArrivalMap* centerlineTracer;
    MiaDistanceTransformFromCenterline* centerlineDilater;
    MiaMultiInputIntensityMappingFilter* intensityAndDistanceCombiner;
    MiaMarkerStatistics* centerRadiusReader;
    MiaDistanceTransformFromCenterline* centerlinevalueInitialSpreader;
    MiaImagePropertySpreadingFilter* centerlinevalueSpreader;
    MiaSparseFieldLevelSetSegmenter* lsVesselSegmenter;
    MiaIntensityMappingFilter* vesselWallThresholdMappingFilter;
    MiaZeroCrossingFilter* narrowBandCreator;
    MiaDrawingManager* initialCenterlineMarkerManager;
    MiaDrawingManager* firstStepLongitudinalBorderMarkerManagerNo1;
    MiaDrawingManager* firstStepLongitudinalBorderMarkerManagerNo2;
    MiaDrawingManager* firstStepLongitudinalBorderMarkerManagerNo3;
    MiaDrawingManager* secondStepCrossSectionBorderMarkerManager;
    MiaDrawingManager* thirdStepCenterlineMarkerManager;
    MiaRegionStatistics* statistics;
    bool vesselAnalysisGUIOn;
    void switchToVesselAnalysisPage(VesselAnalysisMode step);
    int requiredIterations;
    bool levelsetRunning;
    QTimer *timer;
    void cropCenterlines(MiaDrawingManager* manager,float threshold);
    void exportVesselAnalysisResultToPath(QString path);
    bool isInAutomtaticProcessing;
    int currentVesselAnalysisStep;
    void combineFirstAndSecondStepCenterlines();
    MiaImageReader* rawDataloader;
    float lowerThreshold;
    float upperThreshold;
    VesselAnalysisMode currentStep;
    int currentCPRAngle;
    int currentCrossSectionPostion;

    virtual void enableInteractions();
    virtual void changeUserInteractionMode(InteractionMode mode);
};

#endif // MIAVESSELANALYSISNETWORK_H
