#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeType,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#include "miaimage2vtkisosurface.h"
#include "vtkImageImport.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkProperty.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkSTLWriter.h"
#include "vtkPolyDataWriter.h"
#include "vtkTessellatorFilter.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkSmoothPolyDataFilter.h"

MiaImage2VTKIsosurface::MiaImage2VTKIsosurface(MiaNetwork *parent) :
    MiaImage2VTKProp(parent)
{
    isosurfaceExtractor = vtkContourFilter::New();
    isosurfaceExtractor->SetInputConnection(importer->GetOutputPort());
    isosurfaceExtractor->SetValue(0, 0);
    isoThreshold = 0;

    smoothIterations = 10;
    isosurfaceSmoother = vtkSmoothPolyDataFilter::New();
    isosurfaceSmoother->SetInputConnection(isosurfaceExtractor->GetOutputPort());
    isosurfaceSmoother->SetNumberOfIterations(smoothIterations);

    isosurfaceNormals = vtkPolyDataNormals::New();
    isosurfaceNormals->SetInputConnection(isosurfaceSmoother->GetOutputPort());
    isosurfaceNormals->SetFeatureAngle(60.0);
    isosurfaceNormals->FlipNormalsOn();

    isosurfaceMapper = vtkPolyDataMapper::New();
    isosurfaceMapper->SetInputConnection(isosurfaceNormals->GetOutputPort());
    isosurfaceMapper->ScalarVisibilityOff();

    prop = vtkActor::New();
    ((vtkActor*)prop)->SetMapper(isosurfaceMapper);
    ((vtkActor*)prop)->GetProperty()->SetColor(0,1,0);

    autoUpdateWhenInputChanged = false;

}

MiaImage2VTKIsosurface::~MiaImage2VTKIsosurface()
{
    if(prop)
        prop->Delete();
    if(isosurfaceMapper)
        isosurfaceMapper->Delete();
    if(isosurfaceNormals)
        isosurfaceNormals->Delete();
    if(isosurfaceExtractor)
        isosurfaceExtractor->Delete();
}
bool MiaImage2VTKIsosurface::runOperation()
{
    if(isosurfaceExtractor)
        isosurfaceExtractor->Delete();
    isosurfaceExtractor = vtkContourFilter::New();
    isosurfaceExtractor->SetInputConnection(importer->GetOutputPort());
    isosurfaceExtractor->SetValue(0, isoThreshold);
    isosurfaceExtractor->Update();
    isosurfaceSmoother->SetInputConnection(isosurfaceExtractor->GetOutputPort());

    return true;
}
void MiaImage2VTKIsosurface::saveSTLFile(QString filename)
{
    if(!filename.endsWith(".stl", Qt::CaseInsensitive))
        filename.append(".stl");
    vtkSTLWriter  *exporter = vtkSTLWriter::New();
    vtkTransformPolyDataFilter* translateFilter = vtkTransformPolyDataFilter::New();

    vtkTransform* localToPatientTransform = vtkTransform::New();
    vtkMatrix4x4* matrixvtk =  getTranformMatrix();
    localToPatientTransform->SetMatrix(matrixvtk);
    matrixvtk->Delete();

    translateFilter->SetTransform(localToPatientTransform);
    translateFilter->SetInputConnection(isosurfaceExtractor->GetOutputPort());
    exporter->SetInputConnection(translateFilter->GetOutputPort());
    QByteArray byteArray = filename.toUtf8();
    const char* cString = byteArray.constData();
    exporter->SetFileName( cString);
    exporter->Write();
    exporter->Delete();
}
void MiaImage2VTKIsosurface::saveVTKFile(QString filename)
{
    if(!filename.endsWith(".vtk", Qt::CaseInsensitive))
        filename.append(".vtk");
    vtkPolyDataWriter  *exporter = vtkPolyDataWriter::New();
    vtkTransformPolyDataFilter* translateFilter = vtkTransformPolyDataFilter::New();

    vtkTransform* localToPatientTransform = vtkTransform::New();
    vtkMatrix4x4* matrixvtk =  getTranformMatrix();
    localToPatientTransform->SetMatrix(matrixvtk);
    matrixvtk->Delete();

    translateFilter->SetTransform(localToPatientTransform);
    translateFilter->SetInputConnection(isosurfaceExtractor->GetOutputPort());

    vtkTessellatorFilter* tf = vtkTessellatorFilter::New();
    tf->SetInputConnection( translateFilter->GetOutputPort() );
    tf->MergePointsOn();
    tf->SetChordError(0.001);
    tf->Update();
    vtkDataSetSurfaceFilter * dataSetSurfaceFilter = vtkDataSetSurfaceFilter::New();
    dataSetSurfaceFilter->SetInputConnection( tf->GetOutputPort() );

    exporter->SetInputConnection(dataSetSurfaceFilter->GetOutputPort());
    QByteArray byteArray = filename.toUtf8();
    const char* cString = byteArray.constData();
    exporter->SetFileName( cString );
    exporter->Write();
    exporter->Delete();
    dataSetSurfaceFilter->Delete();
    tf->Delete();
}
void MiaImage2VTKIsosurface::setIsoSurfaceThreshold(float threshold)
{
    isoThreshold = threshold;
    isosurfaceExtractor->SetValue(0, isoThreshold);
    needUpdate = true;
    if(autoUpdateWhenParameterChanged)
        update();
    return;
}

void MiaImage2VTKIsosurface::setSmoothIterationss(int it)
{
    smoothIterations = it;
    isosurfaceSmoother->SetNumberOfIterations( smoothIterations );

    needUpdate = true;
    if(autoUpdateWhenParameterChanged)
        update();
    return;
}
