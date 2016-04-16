#ifndef INPUTEVENTINTERPRETOR_H
#define INPUTEVENTINTERPRETOR_H

#include <QtGui>
#include "MiaCoreModules/miaimagend.h"

enum UpdateMethod { SETVALUE, SETDELTA, ADDDELTA, FORWARDMESSAGE };
//SETDELTA caculate the delta distance to the position where the
//SETVALUE
//ADDDELTA
class MiaImageView2D;
class MIAVIEWMODULESSHARED_EXPORT MiaInputEventInterpretor : public QObject
{
    Q_OBJECT
public:
    explicit MiaInputEventInterpretor(QObject *parent = 0);
    virtual ~MiaInputEventInterpretor(){}
    void xValueMethod ( UpdateMethod type) { xUpdateMethod = type; }
    void yValueMethod ( UpdateMethod type) { yUpdateMethod = type; };
    virtual void buttonPressedAt( QPoint pt );
    virtual void buttonReleaseAt( QPoint pt );
    virtual void mouseMovedAt( QPoint pt );
    virtual void mouseDoubleClickedAt( QPoint pt );
    void keyPressed (int key);
    void keyReleased( int key );
    void forwardMessageTo(QObject* manager);

    bool notFireUntilButtonReleased;
    MiaImageView2D* view;

signals:
    void parameterChanged(const QMap <QString, QVariant> &parameters);
    void forwardButtonPressedAt( MiaPoint4D pt, MiaTransform& sceneTransform);
    void forwardButtonReleaseAt( MiaPoint4D pt, MiaTransform& sceneTransform);
    void forwardMouseMovedAt( MiaPoint4D pt, MiaTransform& sceneTransform);
    void forwardMouseDoubleClickAt( MiaPoint4D pt, MiaTransform& sceneTransform);
    void forwardKeyPressed( int key, MiaTransform& sceneTransform);
    void forwardKeyReleased( int key, MiaTransform& sceneTransform);


public slots:
    void updateXValue ( float var);
    void updateYValue ( float var);
    void updateXValue ( int var);
    void updateYValue ( int var);
    void setParameters(const QMap <QString, QVariant> &parameters);
protected:
    UpdateMethod xUpdateMethod;
    UpdateMethod yUpdateMethod;
    QPoint startpt;
    float x,y;
    QMap <QString, QVariant> wiredParameters;


};

#endif // INPUTEVENTINTERPRETOR_H
