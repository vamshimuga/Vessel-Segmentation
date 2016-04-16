#ifndef MIAPERSISTANTLABEL_H
#define MIAPERSISTANTLABEL_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaPersistantLabel : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaPersistantLabel(MiaNetwork *parent = 0);
    void reset();
    void setToRemove(bool r){ isRemovingRegion = r; }
    void undo();
signals:

public slots:

protected:
    bool runOperation();
    bool reAllocOutputImage();
    bool isRemovingRegion;
    int startHistoryBit;
    int currentHistoryBit;
};

#endif // MIAPERSISTANTLABEL_H
