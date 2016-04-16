#include "imagepatchclassificationtrainer.h"
#include "MiaMachineLearningModules/radomforest/TrainingParameters.h"
#include "MiaMachineLearningModules/radomforest/ClassificationTrainingContext.h"
#include "MiaMachineLearningModules/radomforest/DataPointCollection.h"
#include "MiaMachineLearningModules/radomforest/imagepatchgenerator.h"
#include "MiaMachineLearningModules/radomforest/ParallelForestTrainer.h"
#include "miaimagend.h"
#include "miaconstants.h"
#include <qdebug.h>
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{

ImagePatchClassificationTrainer::ImagePatchClassificationTrainer(Eigen::Vector3i psize, int ntree, int nlevel, int nfeatures, int nthresholds)
    :patchSize(psize),numberOfTrees(ntree),numberOfLevels(nlevel),numberOfFeatures(nfeatures),numberOfFeatureThresholds(nthresholds)
{
    trainingData.SetPatchSize(patchSize);
    trainingData.AddClassLabel("background");
    trainingData.AddClassLabel("target");
    patchGenerator.SetPatchDimension(patchSize,5,0.1,0.05*E_PI);
}

void ImagePatchClassificationTrainer::ValidateForest()
{
    std::vector<std::vector<int> > leafNodeIndices;
    forest->Apply(trainingData, leafNodeIndices);

    // For each patch
    int index = 0;
    int tpcount = 0, tncount = 0, fpcount = 0,fncount = 0;
    int mudycount = 0;
    float maxmudy = 0, averagemudy = 0;

    for (int i = 0; i < trainingData.Count(); i++)
    {
//        std::cout << "Trainning patch " << i << " lable " << trainingData.GetIntegerLabel(i);
        // Aggregate statistics for this sample over all leaf nodes reached
        HistogramAggregator h(trainingData.CountClasses());
        for (int t = 0; t < forest->TreeCount(); t++)
        {
            int leafIndex = leafNodeIndices[t][index];
            h.Aggregate(forest->GetTree((t)).GetNode(leafIndex).TrainingDataStatistics);
        }

        // Let's muddy the colors with grey where the entropy is high.
        float mudiness = 0.5f*(float)(h.Entropy());
        if(mudiness > 0.5)
            mudycount++;
        if(mudiness > maxmudy)
            maxmudy = mudiness;
        averagemudy += mudiness;
//         std::cout << " mudiness " << mudiness << " probability: ";
        float probability[2];
        assert(trainingData.CountClasses() == 2);
        for (int b = 0; b < trainingData.CountClasses(); b++)
        {
            probability[b] = h.GetProbability(b);
        }
        if(trainingData.GetIntegerLabel(i) == 0 && probability[0] > probability[1])
            tncount++;
        if(trainingData.GetIntegerLabel(i) == 1 && probability[0] < probability[1])
            tpcount++;
        if(trainingData.GetIntegerLabel(i) == 1 && probability[0] > probability[1])
            fncount++;
        if(trainingData.GetIntegerLabel(i) == 0 && probability[0] < probability[1])
            fpcount++;
//        std::cout << std::endl;
        index++;
    }
    qDebug() << "Total training patches:" << trainingData.Count();
    qDebug() << "True Positive:" << tpcount << "True Negative:" << tncount << "False Positive:" << fpcount
             << "False Negative:" << fncount;
    qDebug() << "Mudiness: max " << maxmudy << "average"<< averagemudy << ">0.5" << mudycount;
}

void ImagePatchClassificationTrainer::clearAllPatches()
{
    trainingData.Clear();
    trainingData.AddClassLabel("background");
    trainingData.AddClassLabel("target");

}

void ImagePatchClassificationTrainer::PrintForest()
{
    for(int i = 0; i < forest->TreeCount(); i++)
    {
        Tree<HaarLikeFeature3DResponse, HistogramAggregator>& tree = forest->GetTree(i);
        std::cout << "Tree No." << i << std::endl;
        int level_nodes = 1;
        int level_bound = 0;
        for(int j = 0; j < tree.NodeCount(); j++)
        {
            if(j>level_bound)
            {
                std::cout << std::endl;
                level_nodes *= 2;
                level_bound += level_nodes;
            }
            Node<HaarLikeFeature3DResponse, HistogramAggregator>& node = tree.GetNode(j);
            if(node.IsNull())
                std::cout << "(null) ";
            else if(node.IsLeaf())
            {
                std::cout << "(**"
                          << node.TrainingDataStatistics.ToString()
                          << "**)";
            }
            else if(node.IsSplit())
                std::cout << "(" << node.Feature.ToString()<<")";
        }
        std::cout << std::endl;
    }
}

void ImagePatchClassificationTrainer::run()
{
    TrainingParameters trainingParameters;
    trainingParameters.MaxDecisionLevels = numberOfLevels-1;//D.Value-1;
    trainingParameters.NumberOfCandidateFeatures = numberOfFeatures;
    if(numberOfFeatureThresholds > 0)
    {
        trainingParameters.NumberOfCandidateThresholdsPerFeature = numberOfFeatureThresholds;
        trainingParameters.UseZeroAsThreshold = false;
    }
    else
    {
        trainingParameters.NumberOfCandidateThresholdsPerFeature = 1;
        trainingParameters.UseZeroAsThreshold = true;
    }
    trainingParameters.NumberOfTrees = numberOfTrees;
    trainingParameters.Verbose = false;

    HaarFeatureFactory featureFactory;
    Random random(patchSize);
    ClassificationTrainingContext<HaarLikeFeature3DResponse> classificationContext(trainingData.CountClasses(), &featureFactory);
    forest = ParallelForestTrainer<HaarLikeFeature3DResponse, HistogramAggregator>::TrainForest (
                random, trainingParameters, classificationContext, trainingData );

    //validate the forest
    ValidateForest();
    PrintForest();
    emit reportTrainingFinished();
}

void ImagePatchClassificationTrainer::saveForestToStream(std::ostream &out)
{
    forest->Serialize(out);
}

void ImagePatchClassificationTrainer::loadForestFromStream(std::istream &in)
{
    forest.reset();
    forest = Forest<HaarLikeFeature3DResponse, HistogramAggregator>::Deserialize(in);
    ValidateForest();
}

void ImagePatchClassificationTrainer::createAPositivePatche(MiaImageND *image, const Eigen::Vector3f& roiscenter, const Eigen::Vector3f& spacing, bool userandom)
{
    createATrainingPatche(1, image, roiscenter, spacing, userandom);

}

void ImagePatchClassificationTrainer::createANegitativePatche(MiaImageND *image, const Eigen::Vector3f &roiscenter, const Eigen::Vector3f &spacing, bool userandom)
{
    createATrainingPatche(0, image, roiscenter, spacing, userandom);
}

void ImagePatchClassificationTrainer::createNegitativePatches(MiaImageND *image, const std::vector<Eigen::Vector3f>& rois, const Eigen::Vector3f& spacing, int numberofpatches)
{
    patchGenerator.SetInputImage(image);
    const Eigen::Vector3f voxelSize = patchSize.cast<float>();
    for(int i = 0; i < numberofpatches; i++)
    {
        ImagePatch& anewpatch = trainingData.CreateANewImagePatchWithLable(patchSize,patchSize,Eigen::Vector3i::Zero(), 0);
        Eigen::Vector3f center = creatAValideNegativePatchCenter(image, rois, spacing);
        patchGenerator.GetAIntegralRandomPatchAt(center,spacing,anewpatch);
    }

}

void ImagePatchClassificationTrainer::getAPatchAsImage(MiaImageND* image, int index)
{
    assert(image->getType() == IM_float && index < trainingData.Count());
    float* im = (float*) image->getDataPtr();
    const ImagePatch patch = trainingData.GetImagePatch(index);
    int total = patch.totalImageSize();
    for(int i = 0; i < total; i++)
            im[i] = patch.readOutData(i);
    std::cout << "label" << trainingData.GetIntegerLabel(index) << std::endl;
}

Eigen::Vector3f ImagePatchClassificationTrainer::creatAValideNegativePatchCenter(MiaImageND *image, const std::vector<Eigen::Vector3f> &rois, const Eigen::Vector3f& spacing)
{
    Eigen::Vector3f patchRealSize = patchSize.cast<float>().cwiseProduct(spacing);
    float safedistance = patchRealSize.mean()*0.5f;
    float* imagespacing = image->getSpacing();
    float boundingsize[3];
    int* imagesize = image->getDimSize();
    for(int dim = 0 ; dim < 3; dim++)
    {
        boundingsize[dim] = patchRealSize[dim]*0.5f / imagespacing[dim];
    }

    Eigen::Vector3f centerpt;
    bool isvalid = false;
    while(!isvalid)
    {
        MiaPoint4D acenter;
        for(int dim = 0 ; dim < 3; dim++)
        {
            acenter.pos[dim] = ImagePatchGenerator::NextRandom(boundingsize[dim], imagesize[dim]-boundingsize[dim]);
        }
        acenter = image->convertVoxelToPatientCoordinate(acenter);
        for(int dim = 0 ; dim < 3; dim++)
        {
            centerpt[dim] = acenter.pos[dim];
        }
        isvalid = true;
        for(int i = 0; i < rois.size(); i++)
        {
            if((rois[i] - centerpt).squaredNorm() < safedistance*safedistance)
                isvalid = false;
        }
    }
    return centerpt;
}

void ImagePatchClassificationTrainer::createATrainingPatche(int label, MiaImageND *image, const Eigen::Vector3f &roiscenter, const Eigen::Vector3f &spacing, bool userandom)
{
    patchGenerator.SetInputImage(image);
    ImagePatch& anewpatch = trainingData.CreateANewImagePatchWithLable(patchSize,patchSize,Eigen::Vector3i::Zero(), label);

    if(userandom)
    {
        patchGenerator.GetAIntegralRandomPatchAt(roiscenter,spacing,anewpatch);
    }
    else
    {
        patchGenerator.GetAIntegralPatchAt(roiscenter, spacing, anewpatch);
    }
}

}}}
