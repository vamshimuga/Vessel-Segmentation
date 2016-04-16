#ifndef MiaImage2QImage_H
#define MiaImage2QImage_H

#include "MiaCoreModules/miamodule.h"
#include <QtGui>
#include "MiaViewModules/mialutmodule.h"
#include "miamodules_global.h"

class MiaGraphics2DImageItem;

class MIAVIEWMODULESSHARED_EXPORT MiaImage2QImage : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaImage2QImage(MiaNetwork *parent = 0);
    virtual ~MiaImage2QImage();
    void setWLWW(float wl, float ww);
    void setCurrentLUT(MiaLUTModule* lut) { currentLut = lut; currentLut->getDefaultWindowSetting(curWinWidth, curWinLevel);}
    QImage* getQImage();
    MiaGraphics2DImageItem* getQImageItem();
signals:
    void qimageWillRemoved(MiaGraphics2DImageItem* img);
    void qimageAdded(MiaGraphics2DImageItem* img);
    void qimageGeometryChanged(MiaGraphics2DImageItem* img);
public slots:
    virtual void inputUpdated();
protected:
    QImage* qimage;
    MiaGraphics2DImageItem* qimageItem;
    virtual bool reloadInputImage();
    virtual bool runOperation();
    virtual bool checkIfNeedReAllocOutputImage(){ return true;}
    virtual bool reAllocOutputImage();
    void convertRGBAImageToQImage();
    void dispatchParameters(const QMap<QString, QVariant> &parameters);
    MiaLUTModule* currentLut;
    float curWinWidth,curWinLevel;
    bool inputGeometryChanged;


};

#endif // MiaImage2QImage_H
