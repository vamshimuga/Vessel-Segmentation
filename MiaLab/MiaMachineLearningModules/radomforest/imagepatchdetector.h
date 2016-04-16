#ifndef IMAGEPATCHDETECTOR_H
#define IMAGEPATCHDETECTOR_H
#include <QThread>
#include <Eigen/Dense>
#include "DataPointCollection.h"
#include "FeatureResponseFunctions.h"
#include "StatisticsAggregators.h"

class MiaImageND;
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{

class ImagePatchDetector: public QThread
{
    Q_OBJECT
public:
    ImagePatchDetector( Eigen::Vector3i psize):patchSize(psize)
    {
    }
    bool loadTrainedForest(std::istream& in);
    void setTrainedForest(std::auto_ptr<Forest<HaarLikeFeature3DResponse, HistogramAggregator> >& f) { forest = f;}
    void scaneAImageAndGenerateProbabilityMap(MiaImageND* input, MiaImageND* map);
    void testPatchsWithTheTrainedForest(float* map, int maplength, ImagePatchCollection &testing);
    bool isValid();
signals:
    void reportDetectionProgress(float p);
    void reportDetectionFinished();
public slots:
protected:
    const Eigen::Vector3i patchSize;
    std::auto_ptr<Forest<HaarLikeFeature3DResponse, HistogramAggregator> > forest;
    void copyImageToPatch(MiaImageND* input, ImagePatch& patch);
    template <class S>
    void copyImageDataToPatch(S* inData, int size, ImagePatch& patch);
};
}}}

#endif // IMAGEPATCHDETECTOR_H
