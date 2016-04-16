#ifndef MIAMODULE_H
#define MIAMODULE_H

#include <QObject>
#include "miaimagend.h"
#include "miamarker.h"

class MiaNetwork;
class MIACOREMODULESSHARED_EXPORT MiaModule : public QObject
{
    Q_OBJECT
public:
    explicit MiaModule(MiaNetwork *parent = 0);
    virtual ~MiaModule();
    void SetInputImage(MiaImageND* img); //if called, inputimage is considered as constant.
    void setInputModule( MiaModule* input ); // this is provide for convenience when connecting standard modules
    //it is not necessary for all module be connected this way as long as the signal and slot are correctly connected.
    bool update();
    QString moduleName;
    bool autoUpdateWhenInputChanged;
    bool autoUpdateWhenParameterChanged;
    bool autoUpdateWhenOutputRequested;
    bool autoUpdateWhenInputMarkerUpdated;
    bool operateInSitu;

signals:
    void outputUpdated();
    void outputMarkerUpdated(MarkerUpdateType msg);
    void requestUpdateInput(MiaImageND** img);
    void parameterChanged(const QMap <QString, QVariant> &parameters);
    void modified(); //this is designed for modules that do not allow automatic update. or modules using input data directly (inline)
    // for non-automatic-update modules, their need let the upper module know their input has changed
    // for inline modules, they need ask for a fresh input to do modifications

public slots:
    void inputUpdated();
    void getOutputImage(MiaImageND** img);
    void setParameters(const QMap <QString, QVariant> &parameters);
    void reLatedModuleModified();
    void inputMarkerUpdated(MarkerUpdateType msg);
protected:
    MiaImageND* creatEmptyImageFrom(MiaImageND* img, ImageType targettype);
    MiaImageND* creatEmptyImage(int x, int y, int z, int t, ImageType type);
    MiaImageND* castImageToAnotherType(MiaImageND* img, ImageType targettype, float delta = 0.0, float scale = 1.0);
    virtual void dispatchParameters(const QMap <QString, QVariant> &parameters);
    virtual bool reloadInputImage();
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    void changeWiredParameter(QString &name, float value);
    void logMessage(QString msg);
    bool needUpdate;
    virtual bool checkIfNeedReAllocOutputImage();
    MiaImageND* inputImage;
    MiaImageND* outputImage; 
    bool inputImageIsSet;
    QMap <QString, QVariant> wiredParameters;
    ImageType requiredInputImageType;
    MiaModule* inputModule;
    MiaNetwork* parentNetwork;
    int logging_level; // 0: no log;
                       // 1: system level log;
                       // 2: network level log;
                       // 3: Module level log;
                       // 4: ROI level log;
                       // 5: Profiling information
                       // 6: All detail information level

};

#endif // MIAMODULE_H
