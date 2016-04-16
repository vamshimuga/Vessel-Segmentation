#ifndef NETWORKENVIROMENTVARIABLE_H
#define NETWORKENVIROMENTVARIABLE_H

#include <QtGui>
#include "miamodules_global.h"
class QDomDocument;
class MIACOREMODULESSHARED_EXPORT NetWorkEnviromentVariable : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkEnviromentVariable(QObject *parent = 0);
    ~NetWorkEnviromentVariable();
    float windowLevel;
    float windowWidth;
    int logging_level; // 0: no log;
                       // 1: system level log;
                       // 2: network level log;
                       // 3: Module level log;
                       // 4: ROI level log;
                       // 5: Profiling information
                       // 6: All detail information level
    void setLogFileName(QString filename);
    void setNetworkSettingFile(QString filename);
    void logMessage(QString message);
    bool saveParametersToXMLDoc(const QMap <QString, QVariant> &parameters);
    bool readParametersFromXMLDoc(QMap<QString, QVariant> &parameters);
    
signals:
    
public slots:

protected:
    QTextStream* logStream;
    QFile logFile;
    QFile networkSettingFile;
    QDomDocument* networkSettingXMLDoc;
    bool networkSettingFileIsSet;

};

#endif // NETWORKENVIROMENTVARIABLE_H
