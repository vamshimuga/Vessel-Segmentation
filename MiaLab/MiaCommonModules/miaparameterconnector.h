#ifndef PARAMETERCONNECTOR_H
#define PARAMETERCONNECTOR_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "miamodules_global.h"

class MIACOMMONMODULESSHARED_EXPORT MiaParameterConnector : public QObject
{
    Q_OBJECT
public:
    explicit MiaParameterConnector(QObject *parent = 0);
    void addParameterPairs ( QString name1, QString name2);
    void linkModules( QObject* sender, QObject* receiver, bool bidirection = true);
    void initializeUpperModuleParameter();
signals:
    void upperParameterChanged(const QMap <QString, QVariant> &parameters);
    void lowerParameterChanged(const QMap <QString, QVariant> &parameters);
public slots:
    void upperModuleParameterChanged(const QMap <QString, QVariant> &parameters);
    void lowerModuleParameterChanged(const QMap <QString, QVariant> &parameters);
protected:
    bool isForwardingParameters;
    bool isRebouncingParameters;
    bool lowerModuleResponsed;
    bool upperModuleResponsed;
    QMap <QString, QString> upper2LowerDictionary;
    QMap <QString, QString> lower2UpperDictionary;
    QMap <QString, QVariant> upperParameters;
    QMap <QString, QVariant> lowerParameters;
};

#endif // PARAMETERCONNECTOR_H
