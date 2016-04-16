#pragma once

// This file defines types used to illustrate the use of the decision forest
// library in simple multi-class classification task (2D data points).

#include <stdexcept>
#include <algorithm>

#include "Sherwood.h"

#include "StatisticsAggregators.h"
#include "FeatureResponseFunctions.h"
#include "DataPointCollection.h"
#include "ClassificationTrainingContext.h"

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  template<class F>
  class IFeatureResponseFactory
  {
  public:
    virtual F CreateRandom(Random& random)=0;
  };

  class HaarFeatureFactory: public IFeatureResponseFactory<HaarLikeFeature3DResponse>
  {
  public:
    HaarLikeFeature3DResponse CreateRandom(Random& random);
  };
  class LinearFeatureFactory: public IFeatureResponseFactory<LinearFeatureResponse2d>
  {
  public:
    LinearFeatureResponse2d CreateRandom(Random& random);
  };

  class AxisAlignedFeatureResponseFactory : public IFeatureResponseFactory<AxisAlignedFeatureResponse>
  {
  public:
    AxisAlignedFeatureResponse CreateRandom(Random& random);
  };

  template<class F>
  class ClassificationTrainingContext : public ITrainingContext<F,HistogramAggregator> // where F:IFeatureResponse
  {
  private:
    int nClasses_;

    IFeatureResponseFactory<F>* featureFactory_;

  public:
    ClassificationTrainingContext(int nClasses, IFeatureResponseFactory<F>* featureFactory)
    {
      nClasses_ = nClasses;
      featureFactory_ = featureFactory;
    }

  private:
    // Implementation of ITrainingContext
    F GetRandomFeature(Random& random)
    {
      return featureFactory_->CreateRandom(random);
    }

    HistogramAggregator GetStatisticsAggregator()
    {
      return HistogramAggregator(nClasses_);
    }

    double ComputeInformationGain(const HistogramAggregator& allStatistics, const HistogramAggregator& leftStatistics, const HistogramAggregator& rightStatistics)
    {
      double entropyBefore = allStatistics.Entropy();

      unsigned int nTotalSamples = leftStatistics.SampleCount() + rightStatistics.SampleCount();

      if (nTotalSamples <= 1)
        return 0.0;

      double entropyAfter = (leftStatistics.SampleCount() * leftStatistics.Entropy() + rightStatistics.SampleCount() * rightStatistics.Entropy()) / nTotalSamples;

      return entropyBefore - entropyAfter;
    }

    bool ShouldTerminate(const HistogramAggregator& parent, const HistogramAggregator& leftChild, const HistogramAggregator& rightChild, double gain)
    {
      return gain < 0.01;
    }
  };

} } }
