#ifndef MIAIMAGEPROPERTYSPREADINGFILTER_H
#define MIAIMAGEPROPERTYSPREADINGFILTER_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaImagePropertySpreadingFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaImagePropertySpreadingFilter(MiaNetwork *parent = 0);
    virtual ~MiaImagePropertySpreadingFilter();
    void setDirectionMapInputModule( MiaModule* input, const char * method);
signals:
    void requestUpdateDirectionMap(MiaImageND** img);
public slots:
protected:
    bool runOperation();
    template <class A>
    bool runOperation(A* inData,A* outData);
    template <class A>
    A getPropertyAt(int currentindex, A* inData,A* outData);
    bool reloadInputImage();
    bool reAllocOutputImage();
    MiaImageND* directionMap;
    MiaModule* directionMapInputModule;
    int width, height, depth,sliceSize;
    uchar* dmap;
};

#endif // MIAIMAGEPROPERTYSPREADINGFILTER_H
