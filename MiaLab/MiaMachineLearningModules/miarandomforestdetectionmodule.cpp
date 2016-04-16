#include "miarandomforestdetectionmodule.h"
#include "radomforest/imagepatchdetector.h"
#define SCALEFORVISUAL 1.0f

MiaRandomForestDetectionModule::MiaRandomForestDetectionModule(int patchsizex, int patchsizey, int patchsizez, MiaNetwork *parent) : MiaModule(parent)
{
    moduleName = "Random forest detector";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    patchSize << patchsizex,patchsizey,patchsizez;
    scanDensity = 16;
    detector = new MicrosoftResearch::Cambridge::Sherwood::ImagePatchDetector(patchSize);
    connect(detector, SIGNAL(reportDetectionProgress(float)), this, SLOT(updateDetectionProgress(float)));
    connect(detector, SIGNAL(reportDetectionFinished()), this, SLOT(handleDetectionFinished()));
}

MiaRandomForestDetectionModule::~MiaRandomForestDetectionModule()
{
    if(detector != NULL)
        delete detector;
}

bool MiaRandomForestDetectionModule::loadForestFromFile(QString filename)
{
    std::ifstream in (filename.toStdString().c_str(),std::ifstream::binary);
    return detector->loadTrainedForest(in);
}

void MiaRandomForestDetectionModule::setTrainedForest(std::auto_ptr<
                                                      MicrosoftResearch::Cambridge::Sherwood::Forest<
                                                      MicrosoftResearch::Cambridge::Sherwood::HaarLikeFeature3DResponse,
                                                      MicrosoftResearch::Cambridge::Sherwood::HistogramAggregator> > f)
{
    detector->setTrainedForest(f);
}

void MiaRandomForestDetectionModule::getDetectionGroups(QList<MiaPoint4D> &out)
{
    if(!outputImage)
    {
        return;
    }
    int* size = outputImage->getDimSize();
    QList<MiaPoint4D> centerpts;
    QList<float> groupweights;
    QList<float> groupCertainty;
    float* imagedata = (float*)outputImage->getDataPtr();
    float mergeDistance = 3; //in image space it is 3*scanDensity = 24
    for(int z = 0; z < size[2]; z++)
        for(int y = 0; y < size[1]; y++)
            for(int x = 0; x < size[0]; x++)
            {
                int ind = z*size[0]*size[1] + y*size[0] + x;

                if(imagedata[ind]/SCALEFORVISUAL > 0.5)
                {
                    qDebug() << imagedata[ind]/SCALEFORVISUAL << x << y << z;
                    //find a new hit
                    MiaPoint4D apt(x,y,z,0);
                    centerpts.push_back(apt);
                    groupweights.push_back(imagedata[ind]);
                    groupCertainty.push_back(imagedata[ind]/SCALEFORVISUAL);
                }
            }
    //merge pts into groups if close enought
    bool pointMerged = true;
    while(pointMerged)
    {
        qDebug() << "Starting new merging process";
        pointMerged = false;
        for(int i = 0; i < centerpts.size(); i++)
        {
            MiaPoint4D acenter = centerpts[i];
            float weight = groupweights[i];
            float certainty = groupCertainty[i];
            qDebug() << i << acenter.pos[0] << acenter.pos[1] << acenter.pos[2];
            for(int j = i+1; j < centerpts.size(); j++)
            {
                MiaPoint4D  apt = centerpts[j];
//                qDebug() << "checking" << j << apt.pos[0] << apt.pos[1] << apt.pos[2];
                if(acenter.distance(apt) < mergeDistance)
                {
                    pointMerged = true;
                    //perform the merge
                    acenter = (acenter*weight + apt*groupweights[j])*(1.0f/(weight+groupweights[j]));
                    weight += groupweights[j];
                    if(groupCertainty[i]>certainty)
                        certainty = groupCertainty[i];
                    centerpts.removeAt(j);
                    groupweights.removeAt(j);
                    groupCertainty.removeAt(j);
                    j--;
//                    qDebug() << "merged";
                }
            }
//            qDebug() << "update" << i << acenter.pos[0] << acenter.pos[1] << acenter.pos[2];
            centerpts[i] = acenter;
            groupweights[i] = weight;
            groupCertainty[i] = certainty;
        }
    }

    for(int i = 0; i < centerpts.size(); i++) {
        MiaPoint4D acenter = outputImage->convertVoxelToPatientCoordinate(centerpts[i]);
        acenter.pos[3] = groupCertainty[i];
        qDebug() << "final" << acenter.pos[0] << acenter.pos[1] << acenter.pos[2] << "certainty" << acenter.pos[3];
        out.push_back(acenter);
    }
}

void MiaRandomForestDetectionModule::startDetection()
{

}

void MiaRandomForestDetectionModule::updateDetectionProgress(float p)
{

}

void MiaRandomForestDetectionModule::handleDetectionFinished()
{

}

bool MiaRandomForestDetectionModule::runOperation()
{
    if(detector->isValid())
    {
        detector->scaneAImageAndGenerateProbabilityMap(inputImage,outputImage);
        int totalsize = outputImage->getImageSize();
        float* imagedata = (float*) outputImage->getDataPtr();
        for(int i = 0; i < totalsize; i++)
            imagedata[i] *= SCALEFORVISUAL;
        return true;
    }
    return false;
}

bool MiaRandomForestDetectionModule::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    float* inputspacing = inputImage->getSpacing();
    int* inputdim = inputImage->getDimSize();


    int dim;
    int newdimension[3];
    float newspacing[4];

    for(dim = 0 ; dim < 3; dim ++)
    {
        newspacing[dim] =  inputspacing[dim]*patchSize[dim]/scanDensity;
        newdimension[dim] = (float)inputdim[dim]*inputspacing[dim]/ newspacing[dim];

    }
    newspacing[3] = 1.0;
    outputImage =  new MiaImageND(newdimension[0], newdimension[1], newdimension[2], 1, IM_float);
    if(outputImage)
    {
        outputImage->setOffset(inputImage->getOffset());
        outputImage->setSpacing(newspacing);

        outputImage->set3x3OrientationMatrix(inputImage->getOrientationMatrix());
        outputImage->computeTransformMatrix();
        outputImage->retain();

        int totalsize = outputImage->getImageSize();
        float* imagedata = (float*) outputImage->getDataPtr();
        for(int i = 0; i < totalsize; i++)
            imagedata[i] = -1;

    }
    else
        return false;

    return true;
}

