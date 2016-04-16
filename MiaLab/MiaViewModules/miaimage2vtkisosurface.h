#ifndef MIAIMAGE2VTKISOSURFACE_H
#define MIAIMAGE2VTKISOSURFACE_H

#include "miaimage2vtkprop.h"
#include "miamodules_global.h"

class vtkContourFilter;
class vtkPolyDataNormals;
class vtkPolyDataMapper;
class vtkSmoothPolyDataFilter;

class MIAVIEWMODULESSHARED_EXPORT MiaImage2VTKIsosurface : public MiaImage2VTKProp
{
    Q_OBJECT
public:
    explicit MiaImage2VTKIsosurface(MiaNetwork *parent = 0);
    virtual ~MiaImage2VTKIsosurface();
    void saveSTLFile(QString filename);
    void saveVTKFile(QString filename);
    void setIsoSurfaceThreshold(float threshold);

signals:

public slots:
    void setSmoothIterationss(int it);
protected:
    virtual bool runOperation();
    vtkContourFilter* isosurfaceExtractor;
    vtkPolyDataNormals* isosurfaceNormals;
    vtkPolyDataMapper* isosurfaceMapper;
    vtkSmoothPolyDataFilter* isosurfaceSmoother;
    float isoThreshold;
    int smoothIterations;

};

#endif // MIAIMAGE2VTKISOSURFACE_H
