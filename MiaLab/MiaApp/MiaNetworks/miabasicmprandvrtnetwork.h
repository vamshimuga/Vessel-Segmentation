#ifndef MIABASICMPRANDVRTNETWORK_H
#define MIABASICMPRANDVRTNETWORK_H

#include "mianetwork.h"
enum InteractionMode {MIA_Interaction_None, MIA_Interaction_Window_Setting,MIA_Interaction_Zoom, MIA_Interaction_Pan,
                      MIA_Interaction_ROI_Closed_Curve, MIA_Interaction_ROI_Open_Curve};

class MiaOrthogonalSlicer;
class MiaImage2QImage;
class MiaInputEventInterpretor;
class MiaInputEventInterpretorForWindowSetting;
class MiaImage2VTKVolume;
class MiaFocusValueReadOut;

class MiaBasicMPRandVRTNetwork : public MiaNetwork
{
    Q_OBJECT
public:
    explicit MiaBasicMPRandVRTNetwork(NetWorkEnviromentVariable* var,QObject *parent = 0);
    virtual ~MiaBasicMPRandVRTNetwork();
    virtual void setupNetwork(MiaModule* input);
    virtual void setupUserInterface(Ui::MainWindow* ui);
    virtual void setWLWW(float wl, float ww);
    virtual void setDefaultWLWW();
    virtual void destroyNetwork();
signals:

public slots:
    virtual void setInteractionToWindowSetting(){changeUserInteractionMode(MIA_Interaction_Window_Setting);}
    virtual void setInteractionToZoom(){changeUserInteractionMode(MIA_Interaction_Zoom);}
    virtual void setInteractionToPan(){changeUserInteractionMode(MIA_Interaction_Pan);}

protected:
    //MiaModules
    MiaOrthogonalSlicer* axialslicer ;
    MiaOrthogonalSlicer* coronalslicer ;
    MiaOrthogonalSlicer* saggitalslicer ;

    MiaImage2QImage* axialqimage ;
    MiaImage2QImage* coronalqimage ;
    MiaImage2QImage* saggitalqimage ;

    MiaImage2VTKVolume* volumetransformer ;

    //interactions and parameters
    MiaInputEventInterpretor* axialCrossInterpretor;
    MiaInputEventInterpretor* coronalCrossInterpretor;
    MiaInputEventInterpretor* saggitalCrossInterpretor;

    MiaInputEventInterpretor* axialPanInterpretor;
    MiaInputEventInterpretor* coronalPanInterpretor;
    MiaInputEventInterpretor* saggitalPanInterpretor;

    MiaInputEventInterpretor* axialZoomInterpretor;
    MiaInputEventInterpretor* coronalZoomInterpretor;
    MiaInputEventInterpretor* saggitalZoomInterpretor;

    MiaInputEventInterpretor* axialWheelInterpretor;
    MiaInputEventInterpretor* coronalWheelInterpretor;
    MiaInputEventInterpretor* saggitalWheelInterpretor;

    MiaInputEventInterpretorForWindowSetting* winSettingInterpretor;
    MiaInputEventInterpretorForWindowSetting* win3DSettingInterpretor;

    MiaFocusValueReadOut* statusReader;
    virtual void enableInteractions();
    virtual void changeUserInteractionMode(InteractionMode mode);
};

#endif // MIABASICMPRANDVRTNETWORK_H
