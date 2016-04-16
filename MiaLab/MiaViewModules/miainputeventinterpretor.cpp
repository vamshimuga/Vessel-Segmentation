#include "miainputeventinterpretor.h"
#include <QtGui>
#include "miaimageview2d.h"

MiaInputEventInterpretor::MiaInputEventInterpretor(QObject *parent) :
    QObject(parent)
{
     x = 0;
     y = 0;
     wiredParameters.insert("x", x);
     wiredParameters.insert("y", y);
     view = 0;
     notFireUntilButtonReleased = false;
     xUpdateMethod = SETVALUE;
     yUpdateMethod = SETVALUE;
}
//InputEventInterpretor::~InputEventInterpretor()
//{
//    qDebug() << "interpretor being deleted";
//}

void MiaInputEventInterpretor::updateXValue ( float var)
{
    QPoint pt = startpt;
    pt.setX(var);
    if(xUpdateMethod == SETVALUE)
        x = pt.x();
    else if (xUpdateMethod == SETDELTA)
        x = pt.x() - startpt.x();
    else if (xUpdateMethod == ADDDELTA)
    {
        x = pt.x() - startpt.x();
        startpt.setX(pt.x());
    }
    wiredParameters.insert("x",x);
    if(!notFireUntilButtonReleased)
        emit parameterChanged(wiredParameters);
}

void MiaInputEventInterpretor::updateYValue ( float var)
{
    QPoint pt = startpt;
    pt.setY(var);
    if(yUpdateMethod == SETVALUE)
        y = pt.y();
    else if (yUpdateMethod == SETDELTA)
        y = pt.y() - startpt.y();
    else if (yUpdateMethod == ADDDELTA)
    {
        y = pt.y() - startpt.y();
        startpt.setY(pt.y());
    }
    wiredParameters.insert("y",y);
    if(!notFireUntilButtonReleased)
        emit parameterChanged(wiredParameters);
}
void MiaInputEventInterpretor::updateXValue ( int var)
{
    QPoint pt = startpt;
    pt.setX(var);
    updateXValue((float)var);
}

void MiaInputEventInterpretor::updateYValue ( int var)
{
    QPoint pt = startpt;
    pt.setY(var);
    updateYValue((float)var);
}

void MiaInputEventInterpretor::mouseMovedAt ( QPoint pt)
{
    if(xUpdateMethod == FORWARDMESSAGE)
    {
        MiaPoint4D pt4D = view->convertViewPointToPatientCoordinate(pt);
        emit forwardMouseMovedAt(pt4D,view->transform);
        return;
    }
    if(xUpdateMethod == SETVALUE)
        x = pt.x();
    else if (xUpdateMethod == SETDELTA)
        x = pt.x() - startpt.x();
    else if (xUpdateMethod == ADDDELTA)
    {
        x = pt.x() - startpt.x();
        startpt.setX(pt.x());
    }

    if(yUpdateMethod == SETVALUE)
        y = pt.y();
    else if (yUpdateMethod == SETDELTA)
        y = pt.y() - startpt.y();
    else if (yUpdateMethod == ADDDELTA)
    {
        y = pt.y() - startpt.y();
        startpt.setY(pt.y());
    }

    wiredParameters.insert("x",x);
    wiredParameters.insert("y",y);

//    qDebug() << "mouse move" << x << y;
    if(!notFireUntilButtonReleased)
        emit parameterChanged(wiredParameters);
}

void MiaInputEventInterpretor::mouseDoubleClickedAt( QPoint pt )
{
    if(xUpdateMethod == FORWARDMESSAGE)
    {
        MiaPoint4D pt4D = view->convertViewPointToPatientCoordinate(pt);
        emit forwardMouseDoubleClickAt(pt4D,view->transform);
        return;
    }
}

void MiaInputEventInterpretor::setParameters(const QMap<QString, QVariant> &parameters)
{
     QMapIterator<QString, QVariant> i(parameters);
     while (i.hasNext()) {
         i.next();
         if(wiredParameters.contains(i.key()) && wiredParameters.value(i.key()) != i.value())
         {
             wiredParameters.insert(i.key(),i.value());
             if(i.key() == "x")
                 x = i.value().toFloat();
             if(i.key() == "y")
                 y = i.value().toFloat();
         }
//         qDebug() << i.key();
     }
}

void MiaInputEventInterpretor::buttonPressedAt( QPoint pt )
{
    if(xUpdateMethod == FORWARDMESSAGE)
    {
        MiaPoint4D pt4D = view->convertViewPointToPatientCoordinate(pt);
        emit forwardButtonPressedAt(pt4D,view->transform);
        return;
    }
    startpt = pt;
    if(xUpdateMethod == SETVALUE ||xUpdateMethod == SETDELTA)
        mouseMovedAt(pt);
}

void MiaInputEventInterpretor::buttonReleaseAt( QPoint pt )
{
    if(xUpdateMethod == FORWARDMESSAGE)
    {
        MiaPoint4D pt4D = view->convertViewPointToPatientCoordinate(pt);
        emit forwardButtonReleaseAt(pt4D,view->transform);
        return;
    }
    if(notFireUntilButtonReleased)
    {
        mouseMovedAt(pt);
        emit parameterChanged(wiredParameters);

    }
}
void MiaInputEventInterpretor::keyPressed (int key)
{
    emit forwardKeyPressed(key,view->transform);
}

void MiaInputEventInterpretor::keyReleased( int key )
{
    emit forwardKeyReleased(key,view->transform);
}

void MiaInputEventInterpretor::forwardMessageTo(QObject* manager)
{
    this->disconnect();
    if(manager)
    {
        xUpdateMethod = FORWARDMESSAGE;
        QObject::connect(this, SIGNAL(forwardButtonPressedAt(MiaPoint4D,MiaTransform&)), manager, SLOT(buttonPressedAt( MiaPoint4D,MiaTransform& )));
        QObject::connect(this, SIGNAL(forwardMouseMovedAt(MiaPoint4D,MiaTransform&)), manager, SLOT(mouseMovedAt( MiaPoint4D,MiaTransform& )));
        QObject::connect(this, SIGNAL(forwardButtonReleaseAt(MiaPoint4D,MiaTransform&)), manager, SLOT(buttonReleasedAt( MiaPoint4D,MiaTransform& )));
        QObject::connect(this, SIGNAL(forwardMouseDoubleClickAt(MiaPoint4D,MiaTransform&)), manager, SLOT(mouseDoubleClickedAt( MiaPoint4D,MiaTransform& )));
        QObject::connect(this, SIGNAL(forwardKeyPressed(int,MiaTransform&)), manager, SLOT(keyPressed( int,MiaTransform& )));
        QObject::connect(this, SIGNAL(forwardKeyReleased(int,MiaTransform&)), manager, SLOT(keyReleased( int,MiaTransform& )));
    }
}
