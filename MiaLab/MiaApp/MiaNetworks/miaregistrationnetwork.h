#ifndef MIAREGISTRATIONNETWORK_H
#define MIAREGISTRATIONNETWORK_H

#include "miabasicmprandvrtnetwork.h"

class MiaImageRegistrationFilter;
class MiaImageReader;
class MiaImage2QImage;
class MiaOrthogonalSlicer;
class MiaImageExporter;

class MiaRegistrationNetwork : public MiaBasicMPRandVRTNetwork
{
    Q_OBJECT
public:
    explicit MiaRegistrationNetwork(NetWorkEnviromentVariable* var,QObject *parent = 0);
    virtual void setupNetwork(MiaModule* input);
    virtual void setupUserInterface(Ui::MainWindow* ui);
    void openMovingImageWithName(QString filename);
    void loadTransformParametersFromFile(QString filename);
    void saveTransformParametersToFile(QString filename);
    void setBackgroundValue(float value);
    void setMovingImageThreshold(float value);
    void setMaximumIteration(int ite);
    QString autoSaveTransformParameterPath;

    
signals:
    
public slots:
    void openMovingImage();
    void startRegistration();
    void registrationFinished();
    void saveTransformParameters();
    void loadTransformParameters();
    void saveRegistrationResults();
    void resetRegistrationFilter();
protected:
    MiaImageRegistrationFilter* registrationFilter;
    MiaImageReader* movingImageReader;
    MiaOrthogonalSlicer* axialRegisteredVolumeSlicer;
    MiaOrthogonalSlicer* saggitalRegisteredVolumeSlicer;
    MiaOrthogonalSlicer* coronalRegisteredVolumeSlicer;
    MiaImage2QImage* axialViewRegisteredMask;
    MiaImage2QImage* saggitalViewRegisteredMask;
    MiaImage2QImage* coronalViewRegisteredMask;
    MiaImageExporter* exporter;
};

#endif // MIAREGISTRATIONNETWORK_H
