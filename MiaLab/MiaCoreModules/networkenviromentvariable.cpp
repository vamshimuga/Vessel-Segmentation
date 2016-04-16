#include "networkenviromentvariable.h"
#include <QApplication>
#include <QDomDocument>
const int IndentSize = 4;

NetWorkEnviromentVariable::NetWorkEnviromentVariable(QObject *parent) :
    QObject(parent)
{
    logging_level = 1;
    logStream = NULL;
    networkSettingFileIsSet = false;
    networkSettingXMLDoc = new QDomDocument();
}

NetWorkEnviromentVariable::~NetWorkEnviromentVariable()
{
    if(logStream != NULL)
    {
        logMessage("Logging End");
        delete logStream;
    }
    if(logFile.isOpen())
        logFile.close();
    if(networkSettingFile.isOpen())
        networkSettingFile.close();
    if(networkSettingXMLDoc)
        delete networkSettingXMLDoc;
}

void NetWorkEnviromentVariable::setLogFileName(QString filename)
{
    if(logStream)
    {
        delete logStream;
        logStream = NULL;
    }
    if(logFile.isOpen())
        logFile.close();
    logFile.setFileName(filename);
    qDebug()<<QApplication::applicationDirPath() + "/log.txt";
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug() << "Fail to open log file" << filename;
        return;
    }
    logStream = new QTextStream(&logFile);
    logMessage("Logging Start");
}

void NetWorkEnviromentVariable::setNetworkSettingFile(QString filename)
{
    if(networkSettingFile.isOpen())
        networkSettingFile.close();
    networkSettingFileIsSet = false;
    networkSettingXMLDoc->clear();
    if(filename.isEmpty())
    { // set networksetting filename to null will disable read/saveParameterToXMLDoc function
        return;
    }
    networkSettingFile.setFileName(filename);

    if(!networkSettingFile.exists())
    {
        QDomElement root = networkSettingXMLDoc->createElement("MiaML");
        networkSettingXMLDoc->appendChild(root);
        QDomElement parameterSection = networkSettingXMLDoc->createElement("Parameters");
        root.appendChild(parameterSection);
        networkSettingFileIsSet = true;
    }
    else
    {
        if (!networkSettingFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Fail to open setting File" << filename;
            return;
        }
        //check if it is a MiaML
        if(networkSettingXMLDoc->setContent(&networkSettingFile) && networkSettingXMLDoc->elementsByTagName("MiaML").count() > 0)
        {
            networkSettingFileIsSet = true;
        }
        else
            qDebug() << filename << "is not a MiaML File";
        networkSettingFile.close();
    }

}

void NetWorkEnviromentVariable::logMessage(QString message)
{
    if(logStream && logging_level > 0)
    {
        (*logStream)<<QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " : " <<message<<"\n";
        logStream->flush();
    }
}

bool NetWorkEnviromentVariable::saveParametersToXMLDoc(const QMap <QString, QVariant> &parameters)
{
    if(!networkSettingFileIsSet)
    {
        return false;
    }
    if(networkSettingFile.isOpen())
        networkSettingFile.close();
    if(!networkSettingFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    QDomNodeList list = networkSettingXMLDoc->elementsByTagName("Parameters");

    if(list.count() == 0)
    {
        return false;
    }
    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement parameterElements = list.at(0).toElement();

    QMapIterator<QString, QVariant> i(parameters);
    while (i.hasNext()) {
        i.next();
        QDomElement aparameter;
        QDomNodeList matchingList = parameterElements.elementsByTagName(i.key());

        if(matchingList.count()>0)
            aparameter = matchingList.at(0).toElement();
        else
            aparameter = networkSettingXMLDoc->createElement(i.key());

        parameterElements.appendChild(aparameter);
        while(aparameter.childNodes().count() > 0)
        {
            aparameter.removeChild(aparameter.lastChild());
        }
        if(i.value().type() == QVariant::List)
        {
            QList<QVariant> valuelist = i.value().toList();
            for(int j = 0; j < valuelist.count(); j ++)
            {
                QDomElement listitemx = networkSettingXMLDoc->createElement("valuelistx");
                aparameter.appendChild(listitemx);
                QPointF apoint = valuelist.at(j).toPointF();
                QDomText xvalue = networkSettingXMLDoc->createTextNode(QVariant(apoint.x()).toString());
                listitemx.appendChild(xvalue);
                QDomElement listitemy = networkSettingXMLDoc->createElement("valuelisty");
                aparameter.appendChild(listitemy);
                QDomText yvalue = networkSettingXMLDoc->createTextNode(QVariant(apoint.y()).toString());
                listitemy.appendChild(yvalue);
            }
        }
        else
        {
            QDomText value = networkSettingXMLDoc->createTextNode(i.value().toString());
            aparameter.appendChild(value);
        }
     }

//    qDebug() << networkSettingXMLDoc->toString();
    QTextStream out(&networkSettingFile);
    networkSettingXMLDoc->save(out,IndentSize);
    networkSettingFile.close();
    return true;
}

bool NetWorkEnviromentVariable::readParametersFromXMLDoc(QMap <QString, QVariant> &parameters)
{
    if(!networkSettingFileIsSet)
    {
        return false;
    }
    QDomNodeList list = networkSettingXMLDoc->elementsByTagName("Parameters");

    if(list.count() == 0)
    {
        return false;
    }
    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement parameterElements = list.at(0).toElement();

    QDomNode n = parameterElements.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
//            qDebug() << "reading" << e.tagName();
            if(parameters.contains(e.tagName()))
            {
                if(n.childNodes().count() == 1)
                {
                    parameters[e.tagName()] = QVariant(e.text());
//                    qDebug() << parameters[e.tagName()] <<  parameters[e.tagName()].toFloat();
                }
                else
                {
                    QList<QVariant> valuelist;
                    QDomNode vnode = e.firstChild();
                    while(!vnode.isNull())
                    {
                        float x,y;
                        QDomElement evalue = vnode.toElement();
                        x = QVariant(evalue.text()).toFloat();
                        vnode = vnode.nextSibling();
                        evalue = vnode.toElement();
                        y = QVariant(evalue.text()).toFloat();
                        valuelist.push_back(QPointF(x,y));
                        vnode = vnode.nextSibling();
                    }
                    parameters[e.tagName()] = QVariant(valuelist);
                }

            }
        }
        n = n.nextSibling();
    }
    return true;
}
