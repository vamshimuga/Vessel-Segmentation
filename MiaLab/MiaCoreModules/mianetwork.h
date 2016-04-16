#ifndef MIANETWORK_H
#define MIANETWORK_H

#include <QObject>
#include "miamodule.h"
#include "networkenviromentvariable.h"
namespace Ui {
    class MainWindow;
}
class MIACOREMODULESSHARED_EXPORT MiaNetwork : public QObject
{
    Q_OBJECT
public:
    explicit MiaNetwork(NetWorkEnviromentVariable* var, QObject *parent = 0);
    virtual void destroyNetwork();
    virtual void setupNetwork(MiaModule* input){ inputModule = input;}
    virtual void setupUserInterface(Ui::MainWindow* ui){ userInterface = ui; }
    virtual MiaModule* getOutputModule(){ return outputModule;}
    virtual bool update() { return outputModule->update();}
    virtual void activeUserInteraction() { }
    virtual void deactiveUserInteraction() {}
    NetWorkEnviromentVariable* enviromentSetting;
signals:
    void procesingFinished(bool sucess);
public slots:
protected:
    QList <QObject*> miaModuleList;
    MiaModule* inputModule;
    MiaModule* outputModule;
    Ui::MainWindow* userInterface;
    QString networkName;
    void logMessage(QString msg) {if(enviromentSetting->logging_level>=2) enviromentSetting->logMessage(msg);}
    float getGlobalParameterValue(QString savedname);
    // private modules
    //......

};

#endif // MIANETWORK_H
