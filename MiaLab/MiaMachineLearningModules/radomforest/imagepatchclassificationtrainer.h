#ifndef IMAGEPATCHCLASSIFICATIONTRAINER_H
#define IMAGEPATCHCLASSIFICATIONTRAINER_H
#include <QThread>
#include <Eigen/Dense>
#include "DataPointCollection.h"
#include "imagepatchgenerator.h"
#include "FeatureResponseFunctions.h"
#include "StatisticsAggregators.h"
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{

class ImagePatchClassificationTrainer: public QThread
{
    Q_OBJECT
public:
    //if nthresholds is set to 0, then the threshold will be constant 0;
    ImagePatchClassificationTrainer(Eigen::Vector3i psize, int ntree = 100, int nlevel = 6, int nfeatures=5000, int nthresholds=0);
    virtual void run();
    void saveForestToStream(std::ostream& out);
    void loadForestFromStream(std::istream& in);
    std::auto_ptr<Forest<HaarLikeFeature3DResponse, HistogramAggregator> > getTrainedForest(){return forest->Clone();}
    void ValidateForest();
    void clearAllPatches();
    void PrintForest();
signals:
    void reportTrainingProgress(float p);
    void reportTrainingFinished();

public slots:
    void createAPositivePatche(MiaImageND* image, const Eigen::Vector3f &roiscenter, const Eigen::Vector3f &spacing, bool userandom);
    void createANegitativePatche(MiaImageND* image, const Eigen::Vector3f &roiscenter, const Eigen::Vector3f &spacing, bool userandom);
    void createNegitativePatches(MiaImageND* image, const std::vector<Eigen::Vector3f>& rois, const Eigen::Vector3f& spacing, int numberofpatches = 1);
    void getAPatchAsImage(MiaImageND *image, int index);

protected:
    const int numberOfTrees;
    const int numberOfLevels;
    const int numberOfFeatures;
    const int numberOfFeatureThresholds;
    const Eigen::Vector3i patchSize;
    ImagePatchCollection trainingData;
    ImagePatchGenerator patchGenerator;
    Eigen::Vector3f creatAValideNegativePatchCenter(MiaImageND* image, const std::vector<Eigen::Vector3f>& rois, const Eigen::Vector3f &spacing);
    std::auto_ptr<Forest<HaarLikeFeature3DResponse, HistogramAggregator> > forest;
    void createATrainingPatche(int label, MiaImageND* image, const Eigen::Vector3f &roiscenter, const Eigen::Vector3f &spacing, bool userandom);
};
}}}
#endif // IMAGEPATCHCLASSIFICATIONTRAINER_H
