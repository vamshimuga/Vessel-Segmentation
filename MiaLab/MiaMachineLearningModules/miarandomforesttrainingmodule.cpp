#include "miarandomforesttrainingmodule.h"
#include "radomforest/imagepatchclassificationtrainer.h"

MiaRandomForestTrainingModule::MiaRandomForestTrainingModule(int patchsizex, int patchsizey, int patchsizez, MiaNetwork *parent) : MiaModule(parent)
{
    moduleName = "Random forest trainer";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    outputPatchIndex = 0;
    patchSize << patchsizex,patchsizez,patchsizez;
    trainer = new MicrosoftResearch::Cambridge::Sherwood::ImagePatchClassificationTrainer(patchSize);
    connect(trainer, SIGNAL(reportTrainingProgress(float)), this, SLOT(updateTrainingProgress(float)));
    connect(trainer, SIGNAL(reportTrainingFinished()), this, SLOT(handleTrainingFinished()));
}

MiaRandomForestTrainingModule::~MiaRandomForestTrainingModule()
{
    if(trainer != NULL)
        delete trainer;
}

void MiaRandomForestTrainingModule::createTrainingPatches(QList<MiaPoint4D> &centers, MiaPoint4D realsizemm, int npos, int nneg)
{
    if(reloadInputImage())
    {
        std::vector<Eigen::Vector3f> rois;
        for(int i = 0 ; i < centers.size(); i++)
        {
            Eigen::Vector3f c(centers[i].pos[0],centers[i].pos[1],centers[i].pos[2]);
            rois.push_back(c);
        }
        const Eigen::Vector3f spacing(realsizemm.pos[0]/patchSize[0], realsizemm.pos[1]/patchSize[1], realsizemm.pos[2]/patchSize[2]);
        for(int i = 0; i < npos; i++)
            trainer->createAPositivePatche(inputImage,rois[i%centers.size()],spacing,true);
        trainer->createNegitativePatches(inputImage,rois,spacing,nneg);
    }
    else
    {
        qDebug() << "Cannot get input image!";
        throw std::runtime_error( "Cannot get input image!");
    }
}

std::auto_ptr<MicrosoftResearch::Cambridge::Sherwood::Forest<MicrosoftResearch::Cambridge::Sherwood::HaarLikeFeature3DResponse, MicrosoftResearch::Cambridge::Sherwood::HistogramAggregator> > MiaRandomForestTrainingModule::getTrainedForest()
{
    return trainer->getTrainedForest();
}

void MiaRandomForestTrainingModule::saveForestToFile(QString filename)
{
    std::ofstream out (filename.toStdString().c_str(),std::ofstream::binary);
    trainer->saveForestToStream(out);

}

void MiaRandomForestTrainingModule::loadForestFromFile(QString filename)
{
    std::ifstream in (filename.toStdString().c_str(),std::ifstream::binary);
    trainer->loadForestFromStream(in);

}

void MiaRandomForestTrainingModule::addedFalsePositivePatches(QList<MiaPoint4D> &centers, MiaPoint4D size, int nneg)
{
    if(reloadInputImage())
    {
        std::vector<Eigen::Vector3f> rois;
        for(int i = 0 ; i < centers.size(); i++)
        {
            Eigen::Vector3f c(centers[i].pos[0],centers[i].pos[1],centers[i].pos[2]);
            rois.push_back(c);
        }
        Eigen::Vector3f spacing(size.pos[0]/patchSize[0], size.pos[1]/patchSize[1], size.pos[2]/patchSize[2]);
        for(int i = 0; i < nneg; i++)
            trainer->createANegitativePatche(inputImage,rois[i%centers.size()],spacing,true);
    }
    else
    {
        qDebug() << "Cannot get input image!";
        throw std::runtime_error("Cannot get input image!");
    }
}

void MiaRandomForestTrainingModule::clearAllPatches()
{
    trainer->clearAllPatches();
}

void MiaRandomForestTrainingModule::startTraining()
{
    trainingProgress = 0;
    trainer->start();
    qDebug() << "Training started";
}

void MiaRandomForestTrainingModule::updateTrainingProgress(float p)
{
    if(p > trainingProgress)
    {
        qDebug() << "Training running " << trainingProgress;
    }
}

void MiaRandomForestTrainingModule::handleTrainingFinished()
{
    qDebug() << "Training finished ";
    emit forestTrainingFinished();
}

bool MiaRandomForestTrainingModule::runOperation()
{
//    trainer->start();
    trainer->getAPatchAsImage(outputImage, outputPatchIndex);
    qDebug() << "Output patch number" << outputPatchIndex;
    return true;
}
bool MiaRandomForestTrainingModule::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    outputImage = creatEmptyImage(patchSize[0], patchSize[1], patchSize[2], 1, IM_float);
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;
    return true;
}
