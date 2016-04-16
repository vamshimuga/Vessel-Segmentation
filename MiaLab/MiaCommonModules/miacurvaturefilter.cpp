#include "miacurvaturefilter.h"

MiaCurvatureFilter::MiaCurvatureFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Curvature 3D Filter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    requiredInputImageType = IM_float;

    sliceSize = 0;
    for(int i = 0 ;i < 3; i++)
    {
        dimension[i] = 0;
        dimoffset[i] = 0;
    }
    imDimension = 0;
    phi = NULL;
    output = NULL;
}

bool MiaCurvatureFilter::runOperation()
{
    for(int ind = 0; ind < totalSize; ind++)
    {
        int cor[3];
        cor[2]=ind/sliceSize;
        cor[1]=(ind-cor[2]*sliceSize)/dimension[0];
        cor[0]=ind-cor[2]*sliceSize-cor[1]*dimension[0];
        output[ind] = getCurvature(cor);
    }
    return true;
}

bool MiaCurvatureFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    outputImage = creatEmptyImageFrom( inputImage, IM_float);
    int* imsize = inputImage->getDimSize();
    for(int i = 0; i < 3; i++)
    {
        dimension[i] = imsize[i];
    }
    sliceSize = dimension[0]*dimension[1];
    totalSize = sliceSize*dimension[2];
    imDimension = inputImage->getDimension();
    assert(imDimension == 3);
    dimoffset[0] = 1;
    dimoffset[1] = dimension[0];
    dimoffset[2] = sliceSize;
    phi = (float*)inputImage->getDataPtr();
    if(outputImage)
    {
        outputImage->retain();
        output = (float*)outputImage->getDataPtr();

    }
    else
        return false;

    return true;
}
float MiaCurvatureFilter::getCurvature(const int cor[3])
{
    // small loop code
     float dx_phi[3];
     float dx_phiplus[3], dx_phiminus[3];
     int ind = cor[2]*sliceSize + cor[1]*dimension[0] + cor[0];
     bool isOnBoundingBoxEdges = false;

     for(int dim = 0; dim < imDimension; dim++)
     {
         if(cor[dim]==0||cor[dim]==dimension[dim]-1)
         {
             dx_phi[dim]=0;
             isOnBoundingBoxEdges = true;
         }
         else
             dx_phi[dim]=(phi[ind+dimoffset[dim]]-phi[ind-dimoffset[dim]])/2.0f;

         if(cor[dim]==0)
             dx_phiminus[dim]=0;
         else
             dx_phiminus[dim]=phi[ind]-phi[ind-dimoffset[dim]];

         if(cor[dim]==dimension[dim]-1)
             dx_phiplus[dim]=0;
         else
             dx_phiplus[dim]=(phi[ind+dimoffset[dim]]-phi[ind]);

     }

     float curvature= 0.0f;

     for(int dim = 0; dim < imDimension; dim++)
     {
         double normplus = 1.192092896e-07F;
         normplus += dx_phiplus[dim]*dx_phiplus[dim];
         double normminus = 1.192092896e-07F;
         double normalplus,normalminus;
         normminus += dx_phiminus[dim]*dx_phiminus[dim];

         for(int dim2 = 0; dim2 < imDimension; dim2 ++)
         {
             if(dim == dim2)
                 continue;
             float dx_phi_at_plus,dx_phi_at_minus;
             if(cor[dim2]==0||cor[dim2]==dimension[dim2]-1||cor[dim] == dimension[dim]-1)
             {
                 dx_phi_at_plus = 0;
             }
             else
             {
                 dx_phi_at_plus = (phi[ind+dimoffset[dim]+dimoffset[dim2]]-phi[ind+dimoffset[dim]-dimoffset[dim2]])/2.0f;
             }

             normplus += (dx_phi_at_plus + dx_phi[dim2])*(dx_phi_at_plus + dx_phi[dim2])*0.25;

             if(cor[dim2]==0||cor[dim2]==dimension[dim2]-1||cor[dim] == 0)
             {
                 dx_phi_at_minus = 0;
             }
             else
             {
                 dx_phi_at_minus = (phi[ind-dimoffset[dim]+dimoffset[dim2]]-phi[ind-dimoffset[dim]-dimoffset[dim2]])/2.0f;
             }

             normminus += (dx_phi_at_minus + dx_phi[dim2])*(dx_phi_at_minus + dx_phi[dim2])*0.25;

         }
         normplus = sqrt(normplus);
         normalplus = dx_phiplus[dim]/normplus;

         normminus = sqrt(normminus);
         normalminus = dx_phiminus[dim]/normminus;

         curvature += normalplus-normalminus;
     }
     curvature /= 2.0f;//may also be (float)imDimension;

     return curvature;
}
