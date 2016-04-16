#include "miainputeventinterpretorforwindowsetting.h"

MiaInputEventInterpretorForWindowSetting::MiaInputEventInterpretorForWindowSetting(QObject *parent) :
    MiaInputEventInterpretor(parent)
{
}

void MiaInputEventInterpretorForWindowSetting::buttonPressedAt( QPoint pt )
{
    startWW = currentWW;
    startWL = currentWL;
    startpt = pt;
}


void MiaInputEventInterpretorForWindowSetting::mouseMovedAt( QPoint pt )
{
    float WWAdapter = startWW / 100.0;

    if( WWAdapter < 0.001) WWAdapter = 0.001;
    currentWL = startWL + (pt.y() -  startpt.y())*WWAdapter;
    currentWW = startWW + (pt.x() -  startpt.x())*WWAdapter;

    wiredParameters.insert("x",currentWW);
    wiredParameters.insert("y",currentWL);
    if(!notFireUntilButtonReleased)
        emit parameterChanged(wiredParameters);
}

void MiaInputEventInterpretorForWindowSetting::buttonReleaseAt( QPoint pt )
{
    mouseMovedAt(pt);
}

void MiaInputEventInterpretorForWindowSetting::setWindowSetting(float wl, float ww)
{
    currentWW = ww;
    currentWL = wl;
    wiredParameters.insert("x",currentWW);
    wiredParameters.insert("y",currentWL);
    if(!notFireUntilButtonReleased)
        emit parameterChanged(wiredParameters);
}
