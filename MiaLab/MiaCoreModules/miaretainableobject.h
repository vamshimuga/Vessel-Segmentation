#ifndef MIARETAINABLEOBJECT_H
#define MIARETAINABLEOBJECT_H

#include <QObject>
#include "miamodules_global.h"

class MIACOREMODULESSHARED_EXPORT MiaRetainableObject : public QObject
{
    Q_OBJECT
public:
    explicit MiaRetainableObject():QObject(0)
    {
        usedByModules = 0;
    }
    virtual ~MiaRetainableObject()
    {

    }
    virtual void retain()
    {
        usedByModules++;
    }

    virtual void release()
    {
        usedByModules--;
        if(usedByModules <= 0)
            delete this;
    }
protected:
    int usedByModules;
    
};

#endif // MIARETAINABLEOBJECT_H
