#ifndef MIAIMAGE2VTKPROP_H
#define MIAIMAGE2VTKPROP_H

#include "MiaCoreModules/miamodule.h"
#include "MiaCoreModules/miaimagend.h"
#include "miamodules_global.h"

class vtkProp3D;
class vtkImageImport;
class vtkAbstractMapper3D;
class vtkMatrix4x4;

class MIAVIEWMODULESSHARED_EXPORT MiaImage2VTKProp : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaImage2VTKProp(MiaNetwork *parent = 0);
    virtual ~MiaImage2VTKProp();
    vtkProp3D* getProp();
    MiaPoint4D getImageCenter();

signals:
    void imageModuleWillBeDeleted();
public slots:
protected:
    virtual bool reloadInputImage();
    virtual bool reAllocOutputImage();
    virtual bool checkIfNeedReAllocOutputImage() {return true;}
    vtkProp3D* prop;
//    vtkAbstractMapper3D* mapper;
    vtkImageImport* importer;
    MiaImageND* sourceImage;
    unsigned short dataSample[27];
    vtkMatrix4x4* getTranformMatrix();

};

#endif // MIAIMAGE2VTKPROP_H
