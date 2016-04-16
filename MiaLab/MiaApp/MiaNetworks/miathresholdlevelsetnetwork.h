#ifndef MIATHRESHOLDLEVELSETNETWORK_H
#define MIATHRESHOLDLEVELSETNETWORK_H

#include "miabasicmprandvrtnetwork.h"

class MiaThresholding;
class MiaLUTModule;
class MiaDrawingManager;
class MiaMarker2GraphicsItem;
class listViewWithKeyBoardSupport;
class MiaSparseFieldLevelSetSegmenter;
class MiaDistanceTransformFrom3DMarkers;
class MiaIntensityMappingFilter;
class MiaImageExporter;
class MiaLevelSetFunctionManualModifier;
class MiaGradient3DFilter;
class MiaResample3D;
class MiaRegionStatistics;
class MiaImage2VTKIsosurface;
class MiaImageReader;

class MiaThresholdLevelsetNetwork : public MiaBasicMPRandVRTNetwork
{
    Q_OBJECT
public:
    explicit MiaThresholdLevelsetNetwork(NetWorkEnviromentVariable* var,QObject *parent = 0);
    virtual void setupNetwork(MiaModule* input);
    virtual void setupUserInterface(Ui::MainWindow* ui);
    virtual void activeUserInteraction();
    virtual void deactiveUserInteraction();
    void startSegmentationAutomatically(QString seedFile, float seedThreshld,QList<QVariant> &lut, float curvaturef);
    QString autoSaveResultPath;
signals:

public slots:
    void switchToStep( int step );
    void startLevelSetSegmentation();
    void stopLevelSetSegmentation();
    void saveSegmentedResult();
    void levelsetFunctionUpdated();
    void levelsetEnterNewPeriod(int n); //n is the nth period
    void levelsetFunctionCompleted();
    void setSmoothFactor(int var);
    void setSmoothFactor(double var);
    void exportSTLFile();
    void switchDrawingMode();
    void nextStep();
    void previousStep();
    void showBlockerBox(bool show);
    void allowBlockerEditing(bool editable);
    void saveMarkers();
    void loadMarkers();
    void setLevelSetToUsingMultiResolution(bool seton);
    void showMessageBoxOfSpeedBoosting();
    void saveRegionToTextFile();
    void onDrawingToolSelected();

protected:
    void setToNavigatingMode( bool seton );
    void setToMarkerDrawingMode( bool seton );
    void setToLevelsetPropagationMode( bool seton );
    void switchToSegmentingMode(bool isSegmenting);
    void switchToSeedingMode( bool seeding);
    void setToResultMode( bool seton );
    void switchToHigherResolution();
    int currentStep;
    bool isDrawingSeeds;
    bool isAllowEditingBlocker;
    int currentResolution;
    bool isMemoryInitialized;

//MIA Networkmodules
    MiaImageExporter* exporter;
    MiaImageExporter* intermedium_data_exporter;
    MiaImage2VTKIsosurface* isosurface ;

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

    MiaInputEventInterpretor* thresholdingInterpretor;

    MiaInputEventInterpretor* axialStopMarkerInterpretor;
    MiaInputEventInterpretor* coronalStopMarkerInterpretor;
    MiaInputEventInterpretor* saggitalStopMarkerInterpretor;
    MiaDrawingManager* seedMarkerManager ;
    MiaDrawingManager* stopMarkerManager ;
    MiaMarker2GraphicsItem* axialViewMarkerAgent ;
    MiaMarker2GraphicsItem* coronalViewMarkerAgent ;
    MiaMarker2GraphicsItem* saggitalViewMarkerAgent ;
    MiaMarker2GraphicsItem* axialViewStopMarkerAgent ;
    MiaMarker2GraphicsItem* coronalViewStopMarkerAgent ;
    MiaMarker2GraphicsItem* saggitalViewStopMarkerAgent ;
    MiaDistanceTransformFrom3DMarkers* distanceTranformer;
    MiaDistanceTransformFrom3DMarkers* lowResolutionDistanceTranformer;
    MiaSparseFieldLevelSetSegmenter* lsSegmenter;
    MiaSparseFieldLevelSetSegmenter* lowResolutionLSSegmenter;
    MiaIntensityMappingFilter* mappingFilter;
    MiaIntensityMappingFilter* lowResolutionMappingFilter;
    MiaLevelSetFunctionManualModifier* lsModifier;
    MiaLevelSetFunctionManualModifier* lowresolutionLSModifier;
    MiaGradient3DFilter* gradientFilter;
    MiaResample3D* downResampler;
    MiaResample3D* upResampler;
    MiaRegionStatistics* statistics;
    MiaIntensityMappingFilter* seedRegionRemappingFilter;
    MiaImageReader* seedRegionReader;

};

#endif // MIATHRESHOLDLEVELSETNETWORK_H
