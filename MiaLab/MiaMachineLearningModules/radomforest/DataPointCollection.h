#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Sherwood.h"

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  /// <summary>
  /// Used to describe the expected format of the lines of a data file (used
  /// in DataPointCollection::Load()).
  /// </summary>
  class DataDescriptor
  {
  public:
    enum e
    {
      Unadorned = 0x0,
      HasClassLabels = 0x1,
      HasTargetValues = 0x2
    };
  };
  class ImagePatch
  {
  public:
      ImagePatch(Eigen::Vector3i is, Eigen::Vector3i ps, Eigen::Vector3i offset)
          :imageSize(is),patchSize(ps),patchOffset(offset),pImageData(NULL),usingExternalImageData(false)
      {
          assert(patchSize[0]>0 && patchOffset[0] >= 0 && patchOffset[0] + patchSize[0] <= imageSize[0]);
          assert(patchSize[1]>0 && patchOffset[1] >= 0 && patchOffset[1] + patchSize[1] <= imageSize[1]);
          assert(patchSize[2]>0 && patchOffset[2] >= 0 && patchOffset[2] + patchSize[2] <= imageSize[2]);

          imagedata.resize(imageSize[0]*imageSize[1]*imageSize[2]);
      }
      ImagePatch(Eigen::Vector3i is, Eigen::Vector3i ps, Eigen::Vector3i offset, PATCHDATATYPE* pimage)
          :imageSize(is),patchSize(ps),patchOffset(offset),pImageData(pimage),usingExternalImageData(true)
      {
          assert(patchSize[0]>0 && patchOffset[0] >= 0 && patchOffset[0] + patchSize[0] <= imageSize[0]);
          assert(patchSize[1]>0 && patchOffset[1] >= 0 && patchOffset[1] + patchSize[1] <= imageSize[1]);
          assert(patchSize[2]>0 && patchOffset[2] >= 0 && patchOffset[2] + patchSize[2] <= imageSize[2]);
      }
      ImagePatch createChildrenPatchWithOffset(Eigen::Vector3i offset) const
      {
          ImagePatch childPatch(imageSize,patchSize, offset, (double*)&(imagedata[0]));
          return childPatch;
      }

      void setOffset(int x, int y, int z) { patchOffset << x,y,z;}
      PATCHDATATYPE readOutData(int ind) const
      {
          return usingExternalImageData ? pImageData[ind] : imagedata[ind];
      }

      void setData(int ind, PATCHDATATYPE value)
      {
          assert(!usingExternalImageData);
          imagedata[ind] = value;
      }

      int convertPatchCoordinateTo1DimensionIndexOfImage(int x, int y, int z) const
      {
          assert(x>=0 && x<patchSize[0] && y>=0 && y<patchSize[1] && z>=0 && z<patchSize[2] );
          return (z+patchOffset[2])*imageSize[0]*imageSize[1]
                  + (y+patchOffset[1])*imageSize[0] + (x+patchOffset[0]);
      }
      int convertImageCoordinateTo1DimensionIndexOfImage(int x, int y, int z) const
      {
          assert(x>=0 && x<imageSize[0] && y>=0 && y<imageSize[1] && z>=0 && z<imageSize[2] );
          return z*imageSize[0]*imageSize[1]
                  + y*imageSize[0] + x;
      }
      void CreateIntegralImage()
      {
          assert(!usingExternalImageData);
          //testing intergral function
      //    float total = 0;
      //    for(int i = 0; i < patch.size(); i++)
      //        total += patch[i];
      //    std::cout << total << std::endl;
          //creat test buffer for testing haar-like feature function
//          float testbuffer = imagedata.size();
//          imagedata.resize(testbuffer*2);
//          for(int i = 0; i < testbuffer; i++)
//              imagedata[testbuffer+i] = imagedata[i];
          assert(!usingExternalImageData);
          for(int z = 0; z < imageSize[2]; z++)
          {
              for(int y = 0; y < imageSize[1]; y++)
              {
                  for(int x = 0; x < imageSize[0]; x++)
                  {
                      PATCHDATATYPE a1 = 0, b1 = 0, c1 = 0, d1 = 0, a2 = 0, b2 = 0, d2 = 0;
                      if(x>0 && y>0 && z>0)
                          a1 = imagedata[ convertImageCoordinateTo1DimensionIndexOfImage(x-1, y-1, z-1) ];
                      if(y>0 && z>0)
                          a2 = imagedata[ convertImageCoordinateTo1DimensionIndexOfImage(x, y-1, z-1) ];
                      if(x>0 && z >0)
                          b1 = imagedata[ convertImageCoordinateTo1DimensionIndexOfImage(x-1, y, z-1) ];
                      if(z>0)
                          b2 = imagedata[ convertImageCoordinateTo1DimensionIndexOfImage(x, y, z-1) ];
                      if(x>0)
                          c1 = imagedata[ convertImageCoordinateTo1DimensionIndexOfImage(x-1, y, z) ];
                      if(x>0 && y>0)
                          d1 = imagedata[ convertImageCoordinateTo1DimensionIndexOfImage(x-1, y-1, z) ];
                      if(y>0)
                          d2 = imagedata[ convertImageCoordinateTo1DimensionIndexOfImage(x, y-1, z) ];
                      int index = convertImageCoordinateTo1DimensionIndexOfImage(x, y, z);
      //                std::cout << patch[index] << "(";
                      imagedata[index] += c1 + a1 - b1 - d1 + b2 + d2 - a2; //c2 = patch[index]
      //                std::cout << patch[index] << "),";
                  }
      //            std::cout << std::endl;
              }
      //        std::cout << "Slice" << z << std::endl;
          }
      }
      int totalImageSize() const { return imageSize[0]*imageSize[1]*imageSize[2]; }
  private:
      Eigen::Vector3i imageSize;
      Eigen::Vector3i patchSize;
      bool usingExternalImageData;
      PATCHDATATYPE* pImageData;
      std::vector<PATCHDATATYPE> imagedata;
      Eigen::Vector3i patchOffset;
  };


  class ImagePatchCollection: public IDataPointCollection
  {
      mutable std::vector< ImagePatch > patchArray;
      Eigen::Vector3i patchSize;

      // only for classified data...
      std::vector<int> labels_;

      std::map<std::string, int> labelIndices_; // map string labels to integers

      // only for regression problems...
      std::vector<float> targets_;


  public:
      ImagePatchCollection()
      {

      }
      void SetPatchSize(Eigen::Vector3i size)
      {
          patchSize = size;
      }
      void AddClassLabel(std::string name)
      {
          labelIndices_.insert(std::pair<std::string, int>(name, labelIndices_.size()));
      }
      void Clear()
      {
          patchArray.clear();
          labels_.clear();
          labelIndices_.clear();
          targets_.clear();
      }

//      virtual ~ImagePatchCollection() {};
      unsigned int Count() const {return patchArray.size();}
      static const int UnknownClassLabel = -1;

      /// <summary>
      /// Do these data have class labels?
      /// </summary>
      bool HasLabels() const
      {
        return labels_.size() != 0;
      }

      /// <summary>
      /// How many unique class labels are there?
      /// </summary>
      /// <returns>The number of unique class labels</returns>
      int CountClasses() const
      {
        if (!HasLabels())
          throw std::runtime_error("Unlabelled data.");
        return labelIndices_.size();
      }

      /// <summary>
      /// Do these data have target values (e.g. for regression)?
      /// </summary>
      bool HasTargetValues() const
      {
        return targets_.size() != 0;
      }
      ImagePatch& CreateANewImagePatchWithLable(Eigen::Vector3i is, Eigen::Vector3i ps, Eigen::Vector3i offset, int label)
      {
          labels_.push_back(label);
          return CreateANewImagePatch(is, ps, offset);
      }
      ImagePatch& CreateANewImagePatch(Eigen::Vector3i is, Eigen::Vector3i ps, Eigen::Vector3i offset)
      {
          patchArray.push_back(ImagePatch(is,ps,offset));
          return patchArray.back();
      }
      ImagePatch& AddANewChildImagePatchWithOffset(ImagePatch& patch, Eigen::Vector3i offset)
      {
          patchArray.push_back(patch.createChildrenPatchWithOffset(offset));
          return patchArray.back();
      }

      /// <summary>
      /// Get the specified image patch.
      /// </summary>
      /// <param name="i">Zero-based data point index.</param>
      /// <returns>Pointer to the first element of the data point.</returns>
      const ImagePatch& GetImagePatch(int i) const
      {
//        std::cout <<"Returned patch " << &(patchArray[i][0]) << " "<< patchArray[i][0] << ", "<< patchArray[i][1]<< ", "<< patchArray[i][133152]<< std::endl;
        return patchArray[i];
      }

      /// <summary>
      /// Get the class label for the specified data point (or raise an
      /// exception if these data points do not have associated labels).
      /// </summary>
      /// <param name="i">Zero-based data point index</param>
      /// <returns>A zero-based integer class label.</returns>
      int GetIntegerLabel(int i) const
      {
        if (!HasLabels())
          throw std::runtime_error("Data have no associated class labels.");

        return labels_[i]; // may throw an exception if index is out of range
      }

      /// <summary>
      /// Get the target value for the specified data point (or raise an
      /// exception if these data points do not have associated target values).
      /// </summary>
      /// <param name="i">Zero-based data point index.</param>
      /// <returns>The target value.</returns>
      float GetTarget(int i) const
      {
        if (!HasTargetValues())
          throw std::runtime_error("Data have no associated target values.");

        return targets_[i]; // may throw an exception if index is out of range
      }

  };

  /// <summary>
  /// A collection of data points, each represented by a float[] and (optionally)
  /// associated with a string class label and/or a float target value.
  /// </summary>
  class DataPointCollection: public IDataPointCollection
  {
    std::vector<float> data_;
    int dimension_;

    // only for classified data...
    std::vector<int> labels_;

    std::map<std::string, int> labelIndices_; // map string labels to integers

    // only for regression problems...
    std::vector<float> targets_;

  public:
    static const int UnknownClassLabel = -1;

    /// <summary>
    /// Load a collection of data from a tab-delimited file with one data point
    /// per line. The data may optionally have associated with class labels
    /// (first element on line) and/or target values (last element on line).
    /// </summary>
    /// <param name="path">Path of file to be read.</param>
    /// <param name="bHasClassLabels">Are the data associated with class labels?</param>
    /// <param name="dataDimension">Dimension of the data (excluding class labels and target values).</param>
    /// <param name="bHasTargetValues">Are the data associated with target values.</param>
    static  std::auto_ptr<DataPointCollection> Load(std::istream& r, int dataDimension, DataDescriptor::e descriptor);

    /// <summary>
    /// Generate a 2D dataset with data points distributed in a grid pattern.
    /// Intended for generating visualization images.
    /// </summary>
    /// <param name="rangeX">x-axis range</param>
    /// <param name="nStepsX">Number of grid points in x direction</param>
    /// <param name="rangeY">y-axis range</param>
    /// <param name="nStepsY">Number of grid points in y direction</param>
    /// <returns>A new DataPointCollection</returns>
    static  std::auto_ptr<DataPointCollection> Generate2dGrid(
      std::pair<float, float> rangeX, int nStepsX,
      std::pair<float, float> rangeY, int nStepsY);

    /// <summary>
    /// Generate a 1D dataset containing a given number of data points
    /// distributed at regular intervals within a given range. Intended for
    /// generating visualization images.
    /// </summary>
    /// <param name="range">Range</param>
    /// <param name="nStepsX">Number of grid points</param>
    /// <returns>A new DataPointCollection</returns>
    static std::auto_ptr<DataPointCollection> Generate1dGrid(std::pair<float, float> range, int nSteps);

    /// <summary>
    /// Do these data have class labels?
    /// </summary>
    bool HasLabels() const
    {
      return labels_.size() != 0;
    }

    /// <summary>
    /// How many unique class labels are there?
    /// </summary>
    /// <returns>The number of unique class labels</returns>
    int CountClasses() const
    {
      if (!HasLabels())
        throw std::runtime_error("Unlabelled data.");
      return labelIndices_.size();
    }

    /// <summary>
    /// Do these data have target values (e.g. for regression)?
    /// </summary>
    bool HasTargetValues() const
    {
      return targets_.size() != 0;
    }

    /// <summary>
    /// Count the data points in this collection.
    /// </summary>
    /// <returns>The number of data points</returns>
    unsigned int Count() const
    {
      return data_.size()/dimension_;
    }

    /// <summary>
    /// Get the data range in the specified data dimension.
    /// </summary>
    /// <param name="dimension">The dimension over which to compute min and max</param>
    /// <returns>A tuple containing min and max over the specified dimension of the data</returns>
    std::pair<float, float> GetRange(int dimension) const;

    /// <summary>
    /// Get the range of target values (or raise an exception if these data
    /// do not have associated target values).
    /// </summary>
    /// <returns>A tuple containing the min and max target value for the data</returns>
    std::pair<float, float> GetTargetRange() const;

    /// <summary>
    /// The dimensionality of the data (excluding optional target values).
    /// </summary>
    int Dimensions() const
    {
      return dimension_;
    }

    /// <summary>
    /// Get the specified data point.
    /// </summary>
    /// <param name="i">Zero-based data point index.</param>
    /// <returns>Pointer to the first element of the data point.</returns>
    const float* GetDataPoint(int i) const
    {
      return &data_[i*dimension_];
    }

    /// <summary>
    /// Get the class label for the specified data point (or raise an
    /// exception if these data points do not have associated labels).
    /// </summary>
    /// <param name="i">Zero-based data point index</param>
    /// <returns>A zero-based integer class label.</returns>
    int GetIntegerLabel(int i) const
    {
      if (!HasLabels())
        throw std::runtime_error("Data have no associated class labels.");

      return labels_[i]; // may throw an exception if index is out of range
    }

    /// <summary>
    /// Get the target value for the specified data point (or raise an
    /// exception if these data points do not have associated target values).
    /// </summary>
    /// <param name="i">Zero-based data point index.</param>
    /// <returns>The target value.</returns>
    float GetTarget(int i) const
    {
      if (!HasTargetValues())
        throw std::runtime_error("Data have no associated target values.");

      return targets_[i]; // may throw an exception if index is out of range
    }
  };

  // A couple of file parsing utilities, exposed here for testing only.

  // Split a delimited line into constituent elements.
  void tokenize(
    const std::string& str,
    std::vector<std::string>& tokens,
    const std::string& delimiters = " " );

  // Convert a std::string to a float (or raise an exception).
  float to_float(const std::string& s);
} } }
