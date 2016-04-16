#include "miaparameterconnector.h"
#include <QtGui>
MiaParameterConnector::MiaParameterConnector(QObject *parent) :
    QObject(parent)
{
    isForwardingParameters = false;
    isRebouncingParameters = false;
    lowerModuleResponsed = false;
    upperModuleResponsed = false;
}

void MiaParameterConnector::addParameterPairs ( QString name1, QString name2)
{
    upper2LowerDictionary.insert(name1,name2);
    lower2UpperDictionary.insert(name2,name1);
}

void MiaParameterConnector::linkModules( QObject* sender, QObject* receiver, bool bidirection)
{
    connect(sender, SIGNAL(parameterChanged(const QMap <QString, QVariant> &)), this, SLOT(upperModuleParameterChanged(const QMap <QString, QVariant> &)), Qt::UniqueConnection);
    connect(this, SIGNAL(upperParameterChanged(const QMap <QString, QVariant> &)), receiver, SLOT(setParameters(const QMap <QString, QVariant> &)), Qt::UniqueConnection);

    if(bidirection)
    {
        connect(receiver, SIGNAL(parameterChanged(const QMap <QString, QVariant> &)), this, SLOT(lowerModuleParameterChanged(const QMap <QString, QVariant> &)), Qt::UniqueConnection);
        connect(this, SIGNAL(lowerParameterChanged(const QMap <QString, QVariant> &)), sender, SLOT(setParameters(const QMap <QString, QVariant> &)), Qt::UniqueConnection);

    }
}

void MiaParameterConnector::upperModuleParameterChanged(const QMap <QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if( upper2LowerDictionary.contains(i.key()) )
         {
             lowerParameters.insert(upper2LowerDictionary.value(i.key()),i.value());
//             qDebug() << "update" << i.key() << i.value();
         }
     }
     if(isRebouncingParameters)
     {
         upperModuleResponsed = true;
     }
     else if ( !isForwardingParameters )
     {
         isForwardingParameters = true;
         emit upperParameterChanged(lowerParameters);
         if(lowerModuleResponsed)
         {
             emit lowerParameterChanged(upperParameters);
             lowerModuleResponsed = false;
         }
         isForwardingParameters = false;
     }



}
void MiaParameterConnector::lowerModuleParameterChanged(const QMap <QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if( lower2UpperDictionary.contains(i.key()) )
         {
             upperParameters.insert(lower2UpperDictionary.value(i.key()),i.value());
//            qDebug() <<"reverse update" << i.key() << i.value();
         }
     }
     if(isForwardingParameters)
     {
         lowerModuleResponsed = true;
     }
     else if ( !isRebouncingParameters)
     {
         isRebouncingParameters = true;
         emit lowerParameterChanged(upperParameters);
         if(upperModuleResponsed)
         {
             emit upperParameterChanged(lowerParameters);
             upperModuleResponsed = false;
         }
         isRebouncingParameters = false;

     }

}
void MiaParameterConnector::initializeUpperModuleParameter()
{
    lowerParameters.clear();
    isForwardingParameters = true;
    emit upperParameterChanged(lowerParameters);
    isForwardingParameters = false;

    isRebouncingParameters = true;
    emit lowerParameterChanged(upperParameters);
    isRebouncingParameters = false;
}
