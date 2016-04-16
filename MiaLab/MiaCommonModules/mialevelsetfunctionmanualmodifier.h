#ifndef MIALEVELSETFUNCTIONMANUALMODIFIER_H
#define MIALEVELSETFUNCTIONMANUALMODIFIER_H

#include "MiaCoreModules/miamodule.h"
#include "MiaCoreModules/miamarker.h"

class MIACOMMONMODULESSHARED_EXPORT MiaLevelSetFunctionManualModifier : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaLevelSetFunctionManualModifier(MiaNetwork *parent = 0);

signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;emit modified();}
    void markerUpdated(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;emit modified();}
    void markerRemoved(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;emit modified();}
protected:
    virtual bool reloadInputImage();
    virtual bool runOperation();
    bool reAllocOutputImage();
    QList <MiaMarker*> markerList;

};

#endif // MIALEVELSETFUNCTIONMANUALMODIFIER_H
