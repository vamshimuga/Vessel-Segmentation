#include "mianetwork.h"

MiaNetwork::MiaNetwork(NetWorkEnviromentVariable* var,QObject *parent) :
    QObject(parent)
{
    inputModule = NULL;
    outputModule = NULL;
    userInterface = NULL;
    networkName = "Abstract Network";
    enviromentSetting = var;
    connect(this, SIGNAL(procesingFinished(bool)), parent, SLOT(workStepFinished(bool)));
}
void MiaNetwork::destroyNetwork()
{
    while (!miaModuleList.isEmpty())
         delete miaModuleList.takeFirst();
    //Mia Modules must be deleted before 2D views to allow modules delete image items created by them.
}

float MiaNetwork::getGlobalParameterValue(QString savedname)
{
    QMap<QString,QVariant> savedparameter;
    savedparameter.insert(savedname,0);
    enviromentSetting->readParametersFromXMLDoc(savedparameter);
    qDebug() << savedparameter;
    return savedparameter[savedname].toFloat();
}
