#ifndef MIARANDOMFORESTDETECTIONMODULE_H
#define MIARANDOMFORESTDETECTIONMODULE_H

#include <MiaCoreModules/miamodule.h>
#include <memory>
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
class ImagePatchDetector;
template<class F, class S>
class Forest;
class HaarLikeFeature3DResponse;
class HistogramAggregator;
}}}

class MIAMACHINELEARNINGMODULESSHARED_EXPORT MiaRandomForestDetectionModule : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaRandomForestDetectionModule(int patchsizex=64, int patchsizey=64, int patchsizez=64, MiaNetwork *parent = 0);
    virtual ~MiaRandomForestDetectionModule();
    bool loadForestFromFile(QString filename);
    void setTrainedForest(std::auto_ptr<MicrosoftResearch::Cambridge::Sherwood::Forest<MicrosoftResearch::Cambridge::Sherwood::HaarLikeFeature3DResponse, MicrosoftResearch::Cambridge::Sherwood::HistogramAggregator> > f);
    void getDetectionGroups(QList<MiaPoint4D>& out);

signals:

public slots:
    void startDetection();
    void updateDetectionProgress(float p);
    void handleDetectionFinished();
protected:
    virtual bool runOperation();
    virtual bool reAllocOutputImage();
    MicrosoftResearch::Cambridge::Sherwood::ImagePatchDetector* detector;
    Eigen::Vector3i patchSize;
    float detectionProgress;
    float scanDensity;
};

#endif // MIARANDOMFORESTDETECTIONMODULE_H
