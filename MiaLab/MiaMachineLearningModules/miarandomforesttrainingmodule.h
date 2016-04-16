#ifndef MIARANDOMFORESTTRAINNINGMODULE_H
#define MIARANDOMFORESTTRAINNINGMODULE_H

#include <MiaCoreModules/miamodule.h>
#include <memory>
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
class ImagePatchClassificationTrainer;
template<class F, class S>
class Forest;
class HaarLikeFeature3DResponse;
class HistogramAggregator;
}}}

class MIAMACHINELEARNINGMODULESSHARED_EXPORT MiaRandomForestTrainingModule : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaRandomForestTrainingModule(int patchsizex=64, int patchsizey=64, int patchsizez=64, MiaNetwork *parent = 0);
    virtual ~MiaRandomForestTrainingModule();
    void createTrainingPatches(QList<MiaPoint4D>& centers, MiaPoint4D realsizemm, int npos, int nneg);
    void setOutputPatchIndex(int ind) { outputPatchIndex = ind;}
    std::auto_ptr<MicrosoftResearch::Cambridge::Sherwood::Forest<
                  MicrosoftResearch::Cambridge::Sherwood::HaarLikeFeature3DResponse,
                  MicrosoftResearch::Cambridge::Sherwood::HistogramAggregator> > getTrainedForest();
    void saveForestToFile(QString filename);
    void loadForestFromFile(QString filename);
    void addedFalsePositivePatches(QList<MiaPoint4D>& centers, MiaPoint4D size, int nneg);
    void clearAllPatches();
signals:
    void forestTrainingFinished();
public slots:
    void startTraining();
    void updateTrainingProgress(float p);
    void handleTrainingFinished();
protected:
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    MicrosoftResearch::Cambridge::Sherwood::ImagePatchClassificationTrainer* trainer;
    Eigen::Vector3i patchSize;
    int outputPatchIndex;
    float trainingProgress;
};


#endif // MIARANDOMFORESTTRAINNINGMODULE_H
