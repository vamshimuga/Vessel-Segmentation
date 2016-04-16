#include "imagepatchgenerator.h"
#include <iostream>
#include "miaimagend.h"
#include "DataPointCollection.h"
#include <QtGlobal>

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{

ImagePatchGenerator::ImagePatchGenerator()
{
    maxRandomOffset = 0;
    maxRandomScaling = 0;
    maxRandomRotationAngle = 0;
}


ImagePatchGenerator::ImagePatchGenerator(Eigen::Vector3i size, float maxoffset, float maxscaling, float maxrotation)
    :patchSize(size), maxRandomOffset(maxoffset),maxRandomScaling(maxscaling),maxRandomRotationAngle(maxrotation)
{

}

void ImagePatchGenerator::SetPatchDimension(Eigen::Vector3i size, float maxoffset, float maxscaling, float maxrotation)
{
    patchSize = size;
    maxRandomOffset = maxoffset;
    maxRandomScaling = maxscaling;
    maxRandomRotationAngle = maxrotation;

}

void ImagePatchGenerator::GetARandomPatchAt(const Eigen::Vector3f &center, const Eigen::Vector3f &spacing, ImagePatch &patch)
{
    Eigen::Vector3f offset = center;
    Eigen::Matrix3f step = Eigen::Matrix3f::Zero();

    for(int dim = 0 ; dim < 3; dim++)
    {
        offset[dim] += NextRandom(-maxRandomOffset,maxRandomOffset);
        step(dim,dim) = spacing[dim] * NextRandom(1.0f-maxRandomScaling,1.0f+maxRandomScaling);
    }

    Eigen::Vector3f axis(0,0,0);
    int axisindex = rand()%3;
    axis[axisindex] = 1;
    float angle = NextRandom(-maxRandomRotationAngle, maxRandomRotationAngle);

//    std::cout << offset-center << std::endl << step << std::endl << angle << std::endl << std::endl;

    Eigen::Transform<float,3, Eigen::Affine> t;
    t = Eigen::AngleAxisf(angle,axis);
    step = t*step;

    GetAObliquePatchAt(offset, step, patch);


}

void ImagePatchGenerator::GetAPatchAt(const Eigen::Vector3f &center, const Eigen::Vector3f &step, ImagePatch &patch)
{
    const Eigen::Vector3f halfsize = patchSize.cast<float>()*0.5;
    const Eigen::Vector3f origin = center - step.cwiseProduct(halfsize);
//    std::cout << step << std::endl << center << std::endl;
#ifdef Q_OS_WIN
    #pragma omp parallel for
#else
    #pragma omp parallel for collapse(3)
#endif
    for(int z = 0; z < patchSize[2]; z++)
        for(int y = 0; y < patchSize[1]; y++)
            for(int x = 0; x < patchSize[0]; x++)
            {
                MiaPoint4D coor;
                coor.pos[2] = origin[2] + step[2]*z;
                coor.pos[1] = origin[1] + step[1]*y;
                coor.pos[0] = origin[0] + step[0]*x;

                int ind = patch.convertPatchCoordinateTo1DimensionIndexOfImage(x,y,z);
                coor = inputImage->convertPatientCoordinateToVoxel(coor);
                patch.setData(ind, inputImage->getValueAt(coor.pos));
            }
}

void ImagePatchGenerator::GetAObliquePatchAt(const Eigen::Vector3f &center, const Eigen::Matrix3f &step, ImagePatch &patch)
{
    const Eigen::Vector3f halfsize = patchSize.cast<float>()*0.5;
    const Eigen::Vector3f origin = center - step*halfsize;
//    std::cout << step << patchSize/2 << step*halfsize;
#ifdef Q_OS_WIN
    #pragma omp parallel for
#else
    #pragma omp parallel for collapse(3)
#endif
    for(int z = 0; z < patchSize[2]; z++)
        for(int y = 0; y < patchSize[1]; y++)
            for(int x = 0; x < patchSize[0]; x++)
            {
                MiaPoint4D coor;
                coor.pos[0] = origin[0] + step.coeff(0,0)*x + step.coeff(0,1)*y + step.coeff(0,2)*z;
                coor.pos[1] = origin[1] + step.coeff(1,0)*x + step.coeff(1,1)*y + step.coeff(1,2)*z;
                coor.pos[2] = origin[2] + step.coeff(2,0)*x + step.coeff(2,1)*y + step.coeff(2,2)*z;

                int ind = patch.convertPatchCoordinateTo1DimensionIndexOfImage(x,y,z);
                coor = inputImage->convertPatientCoordinateToVoxel(coor);
                patch.setData(ind, inputImage->getValueAt(coor.pos));
            }

}

void ImagePatchGenerator::GetAIntegralPatchAt(const Eigen::Vector3f &center, const Eigen::Vector3f &step, ImagePatch &patch)
{
    GetAPatchAt(center, step, patch);
    patch.CreateIntegralImage();
}

void ImagePatchGenerator::GetAIntegralRandomPatchAt(const Eigen::Vector3f &center, const Eigen::Vector3f &spacing, ImagePatch &patch)
{
    GetARandomPatchAt(center, spacing, patch);
    patch.CreateIntegralImage();
}

}}}
