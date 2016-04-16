#ifndef INPUTEVENTINTERPRETORFORWINDOWSETTING_H
#define INPUTEVENTINTERPRETORFORWINDOWSETTING_H
#include "miainputeventinterpretor.h"

class MIAVIEWMODULESSHARED_EXPORT MiaInputEventInterpretorForWindowSetting : public MiaInputEventInterpretor
{
    Q_OBJECT
public:
    explicit MiaInputEventInterpretorForWindowSetting(QObject *parent = 0);
    virtual void buttonPressedAt( QPoint pt );
    virtual void buttonReleaseAt( QPoint pt );
    virtual void mouseMovedAt( QPoint pt );
    void setWindowSetting(float wl, float ww);
signals:

public slots:

protected:
    float startWW, startWL;
    float currentWW, currentWL;
};

#endif // INPUTEVENTINTERPRETORFORWINDOWSETTING_H
