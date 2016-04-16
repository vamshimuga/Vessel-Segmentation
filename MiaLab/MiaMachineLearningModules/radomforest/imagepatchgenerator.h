#ifndef IMAGEPATCHGENERATOR_H
#define IMAGEPATCHGENERATOR_H
#include <QList>
#include <Eigen/Dense>
#include <vector>
#include "Random.h"
class MiaImageND;
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
class ImagePatch;
class ImagePatchGenerator
{
public:
    ImagePatchGenerator();
    ImagePatchGenerator( Eigen::Vector3i size, float maxoffset, float maxscaling, float maxrotation);
    void SetPatchDimension(Eigen::Vector3i size, float maxoffset, float maxscaling, float maxrotation);
    void SetInputImage(MiaImageND* image) { inputImage = image;}
    void GetARandomPatchAt(const Eigen::Vector3f& center, const Eigen::Vector3f& spacing, ImagePatch& patch);
    void GetAPatchAt(const Eigen::Vector3f& center, const Eigen::Vector3f& step, ImagePatch& patch);
    void GetAObliquePatchAt(const Eigen::Vector3f& center, const Eigen::Matrix3f &step, ImagePatch& patch);
    void GetAIntegralPatchAt(const Eigen::Vector3f& center, const Eigen::Vector3f& step, ImagePatch &patch);
    void GetAIntegralRandomPatchAt(const Eigen::Vector3f& center, const Eigen::Vector3f& spacing, ImagePatch& patch);
    /// <summary>
    /// Generate a random number in the range [min, max).
    /// </summary>
    static double NextRandom(float min, float max)
    {
      return min + (max-min)*(rand())/RAND_MAX;
    }
protected:
    MiaImageND* inputImage;
    Eigen::Vector3i patchSize;
    float maxRandomOffset;
    float maxRandomScaling;
    float maxRandomRotationAngle;

};
}}}
#endif // IMAGEPATCHGENERATOR_H
