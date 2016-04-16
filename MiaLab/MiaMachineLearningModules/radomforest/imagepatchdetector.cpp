#include "imagepatchdetector.h"
#include "DataPointCollection.h"
#include "FeatureResponseFunctions.h"
#include "StatisticsAggregators.h"
#include "miaimagend.h"
#define NUMBEROFCLASS 2
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{

bool ImagePatchDetector::loadTrainedForest(std::istream &in)
{
    forest.reset();
    forest = Forest<HaarLikeFeature3DResponse, HistogramAggregator>::Deserialize(in);
    if(forest.get() && forest->TreeCount()>0)
        return true;
    else
        return false;

}

void ImagePatchDetector::scaneAImageAndGenerateProbabilityMap(MiaImageND *input, MiaImageND *map)
{
    int* ims = input->getDimSize();
    Eigen::Vector3i imageSize(ims[0],ims[1],ims[2]);
    ImagePatch mainPatch(imageSize,patchSize,Eigen::Vector3i::Zero());
    copyImageToPatch(input,mainPatch);
    mainPatch.CreateIntegralImage();

    ImagePatchCollection testingDataCollection;
    float*outputmap = (float*)map->getDataPtr();
    MiaPoint4D halfPatchSize(patchSize[0]/2,patchSize[1]/2,patchSize[2]/2,0);

    int* outputSize = map->getDimSize();
    for(int z = 0 ; z < outputSize[2]; z++)
        for(int y = 0; y < outputSize[1]; y++)
            for(int x = 0; x < outputSize[0]; x++)
            {
                int ind = z*outputSize[0]*outputSize[1] + y*outputSize[0] + x;
                MiaPoint4D pt(x,y,z,0);
                pt = map->convertVoxelToPatientCoordinate(pt);
                pt = input->convertPatientCoordinateToVoxel(pt);
                pt.pos[0] =  (int)(pt.pos[0]+0.5f);
                pt.pos[1] =  (int)(pt.pos[1]+0.5f);
                pt.pos[2] =  (int)(pt.pos[2]+0.5f);
                MiaPoint4D pt1 = pt - halfPatchSize;
                MiaPoint4D pt2 = pt + halfPatchSize;
                if(pt1.pos[0]>=0 && pt2.pos[0] < ims[0] && pt1.pos[1]>=0 && pt2.pos[1] < ims[1] && pt1.pos[2] >= 0 && pt2.pos[2] < ims[2])
                {
                    Eigen::Vector3i offset(pt1.pos[0], pt1.pos[1], pt1.pos[2]);
                    testingDataCollection.AddANewChildImagePatchWithOffset(mainPatch,offset);
                    outputmap[ind] = 0;
                }
                else {
                    outputmap[ind] = -1.0f;
                }
            }
    assert(map->getType() == IM_float);
    testPatchsWithTheTrainedForest(outputmap, input->getImageSize(), testingDataCollection);

}

void ImagePatchDetector::testPatchsWithTheTrainedForest(float *map,int maplength, ImagePatchCollection &testing)
{
    std::vector<std::vector<int> > leafNodeIndices;
    forest->Apply(testing, leafNodeIndices);

    // For each patch
    int index = 0;
    while(map[index]!=0 && index < maplength)
        index++;
    for (int i = 0; i < testing.Count(); i++)
    {
//        std::cout << "Trainning patch " << i;
        // Aggregate statistics for this sample over all leaf nodes reached
        HistogramAggregator h(NUMBEROFCLASS);
        for (int t = 0; t < forest->TreeCount(); t++)
        {
            int leafIndex = leafNodeIndices[t][i];
            h.Aggregate(forest->GetTree((t)).GetNode(leafIndex).TrainingDataStatistics);
        }

        // Let's muddy the colors with grey where the entropy is high.
        float mudiness = 0.5f*(float)(h.Entropy());
//         std::cout << " mudiness " << mudiness << " probability: ";

        for (int b = 0; b < NUMBEROFCLASS; b++)
        {
            float p = (1.0f-mudiness)*h.GetProbability(b); // NB probabilities sum to 1.0 over the classes
//            std::cout << p << " ";
            if(b==1)
            {
                map[index] = p;
            }

        }
//        std::cout << std::endl;
        index++;
        while(map[index]!=0 && index < maplength)
            index++;
    }
}

bool ImagePatchDetector::isValid()
{
    if(forest.get())
    {
        return forest->TreeCount() > 0;
    }
    return false;
}

void ImagePatchDetector::copyImageToPatch(MiaImageND *input, ImagePatch &patch)
{
    int totalsize = input->getImageSize();
    switch(input->getType())
    {
    case IM_short:
        copyImageDataToPatch<short>((short*) input->getDataPtr(),totalsize, patch);
        break;
    case IM_unsignedshort:
        copyImageDataToPatch<unsigned short>((unsigned short*) input->getDataPtr(),totalsize, patch);
        break;
    case IM_float:
        copyImageDataToPatch<float>((float*) input->getDataPtr(),totalsize, patch);
        break;
    case IM_int:
        copyImageDataToPatch<int>((int*) input->getDataPtr(),totalsize, patch);
        break;
    case IM_uchar:
        copyImageDataToPatch<unsigned char>((unsigned char*) input->getDataPtr(),totalsize, patch);
        break;
    default:
        throw std::runtime_error("Unsuppoted image type");
        break;
    }

}
template <class S>
void ImagePatchDetector::copyImageDataToPatch(S *inData, int size, ImagePatch &patch)
{
#pragma omp parallel for
    for(int i = 0; i < size; i++)
        patch.setData(i,inData[i]);
}

}}}
