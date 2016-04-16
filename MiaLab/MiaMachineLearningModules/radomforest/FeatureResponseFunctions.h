#pragma once

// This file defines some IFeatureResponse implementations used by the example code in
// Classification.h, DensityEstimation.h, etc. Note we represent IFeatureResponse
// instances using simple structs so that all tree data can be stored
// contiguously in a linear array.

#include <string>

#include "Sherwood.h"

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  class Random;
  class ImagePatch;
  class HaarLikeFeature3DResponse
  {
      int box1[6];
      int box2[6];
      Eigen::Vector3i patchSize;
    public:
      HaarLikeFeature3DResponse()
      {
          patchSize = Eigen::Vector3i::Zero();
          for(int i = 0; i < 6; i++)
          {
              box1[i] = 0;
              box2[i] = 0;
          }
      }

      HaarLikeFeature3DResponse(Eigen::Vector3i size, int b1[6], int b2[6])
      :patchSize(size)
      {
        for(int i = 0; i < 6; i++)
        {
            box1[i] = b1[i];
            box2[i] = b2[i];
        }
      }

      /// <summary>
      /// Create an AxisAlignedFeatureResponse instance with a random choice of axis.
      /// </summary>
      /// <param name="randomNumberGenerator">A random number generator.</param>
      /// <returns>A new AxisAlignedFeatureResponse instance.</returns>
      static HaarLikeFeature3DResponse CreateRandom(Random& random);

      // IFeatureResponse implementation
      float GetResponse(const IDataPointCollection& data, unsigned int sampleIndex) const;

      PATCHDATATYPE GetAreaMean(const ImagePatch& patch, const int box[6]) const;

      std::string ToString() const;
  };

  /// <summary>
  /// A feature that orders data points using one of their coordinates,
  /// i.e. by projecting them onto a coordinate axis.
  /// </summary>
  class AxisAlignedFeatureResponse
  {
    int axis_;

  public:
    AxisAlignedFeatureResponse()
    {
      axis_ = -1;
    }

    /// <summary>
    /// Create an AxisAlignedFeatureResponse instance for the specified axis.
    /// </summary>
    /// <param name="axis">The zero-based index of the axis.</param>
    AxisAlignedFeatureResponse(int axis)
    {
      axis_ = axis;
    }

    /// <summary>
    /// Create an AxisAlignedFeatureResponse instance with a random choice of axis.
    /// </summary>
    /// <param name="randomNumberGenerator">A random number generator.</param>
    /// <returns>A new AxisAlignedFeatureResponse instance.</returns>
    static AxisAlignedFeatureResponse CreateRandom(Random& random);

    int Axis() const
    {
      return axis_;
    }

    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int sampleIndex) const;

    std::string ToString() const;
  };

  /// <summary>
  /// A feature that orders data points using a linear combination of their
  /// coordinates, i.e. by projecting them onto a given direction vector.
  /// </summary>
  class LinearFeatureResponse2d
  {
    float dx_, dy_;

  public:
    LinearFeatureResponse2d()
    {
      dx_ = 0.0;
      dy_ = 0.0;
    }

    /// <summary>
    /// Create a LinearFeatureResponse2d instance for the specified direction vector.
    /// </summary>
    /// <param name="dx">The first element of the direction vector.</param>
    /// <param name="dx">The second element of the direction vector.</param> 
    LinearFeatureResponse2d(float dx, float dy)
    {
      dx_ = dx; dy_ = dy;
    }

    /// <summary>
    /// Create a LinearFeatureResponse2d instance with a random direction vector.
    /// </summary>
    /// <param name="randomNumberGenerator">A random number generator.</param>
    /// <returns>A new LinearFeatureResponse2d instance.</returns>
    static LinearFeatureResponse2d CreateRandom(Random& random);

    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int index) const;

    std::string ToString()  const;
  };	
} } }
