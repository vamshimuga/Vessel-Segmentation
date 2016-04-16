#include "FeatureResponseFunctions.h"

#include <cmath>

#include <sstream>

#include "DataPointCollection.h"
#include "Random.h"

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
    HaarLikeFeature3DResponse HaarLikeFeature3DResponse::CreateRandom(Random &random)
    {
        Eigen::Vector3i size;
        size = random.patchSize;

        int b[2][6];
        for(int p = 0; p < 2; p++)
            for(int dim = 0; dim < 3; dim++){
                b[p][dim*2] = random.Next(-1, size[dim]); // [-1,size-1]
                b[p][dim*2+1] = random.Next(-1, size[dim]); // [-1,size-1]
                while(b[p][dim*2+1] == b[p][dim*2])
                {
                    b[p][dim*2+1] = random.Next(-1, size[dim]); // [-1,size-1]
                }
                if(b[p][dim*2] > b[p][dim*2+1])
                {
                    int temp = b[p][dim*2];
                    b[p][dim*2] = b[p][dim*2+1];
                    b[p][dim*2+1] = temp;
                }
            }
        return HaarLikeFeature3DResponse(random.patchSize, b[0], b[1]);

    }


    float HaarLikeFeature3DResponse::GetResponse(const IDataPointCollection &data, unsigned int sampleIndex) const
    {
        const ImagePatchCollection& patchs = (const ImagePatchCollection&) data;
        const ImagePatch& apatch = patchs.GetImagePatch(sampleIndex);
        float mean1 = GetAreaMean(apatch, box1);
        float mean2 = GetAreaMean(apatch, box2);
        return mean1-mean2;
    }

    //the sum in the box can be caculated using (C2+A2-B2-D2) - (C1+A1-B1-D1)
    PATCHDATATYPE HaarLikeFeature3DResponse::GetAreaMean(const ImagePatch& patch, const int box[]) const
    {
        const int minx = box[0];
        const int maxx = box[1];
        const int miny = box[2];
        const int maxy = box[3];
        const int minz = box[4];
        const int maxz = box[5];

        PATCHDATATYPE a1 = 0, b1 = 0, c1 = 0, d1 = 0, a2 = 0, b2 = 0, c2 = 0, d2 = 0;
        if(minx>=0 && miny>=0 && minz>=0)
            a1 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(minx, miny, minz));
        if(miny>=0 && minz>=0)
            a2 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(maxx, miny, minz));
        if(minx>=0 && minz>=0)
            b1 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(minx, maxy, minz));
        if(minz>=0)
            b2 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(maxx, maxy, minz));
        if(minx>=0)
            c1 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(minx, maxy, maxz));

        c2 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(maxx, maxy, maxz));

        if(minx>=0 && miny>=0)
            d1 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(minx, miny, maxz));
        if(miny>=0)
            d2 = patch.readOutData(patch.convertPatchCoordinateTo1DimensionIndexOfImage(maxx, miny, maxz));
        PATCHDATATYPE volume = (maxx-minx)*(maxy-miny)*(maxz-minz);
        PATCHDATATYPE sum = (c2 + a2 - b2 - d2) - (c1 + a1 - b1 - d1);
        //test the sum is correct
//        PATCHDATATYPE testsum = 0;
//        int testbuffer = patch.imagedata.size()/2;
//        for(int z = minz+1; z <= maxz; z++)
//            for(int y = miny+1; y <= maxy; y++)
//                for(int x = minx+1; x <= maxx; x++)
//                {
//                    int ind = patch.convertPatchCoordinateTo1DimensionIndexOfImage(x,y,z);
//                    testsum += patch.imagedata[testbuffer+ind];
//                }
//        std::cout << "(" << minx << "," << maxx << ","
//                         << miny << "," << maxy << ","
//                         << minz << "," << maxz << ","
//                         << sum << " vs " << testsum <<std::endl;
//        assert(fabs(testsum-sum)<0.1);
        return sum/volume;
    }


    std::string HaarLikeFeature3DResponse::ToString() const
    {
        std::stringstream s;
        s << "[" << box1[0] << ", "
                 << box1[1] << ", "
                 << box1[2] << " - "
                 << box1[3] << ", "
                 << box1[4] << ", "
                 << box1[5]
              << "], ["
                 << box2[0] << ", "
                 << box2[1] << ", "
                 << box2[2] << " - "
                 << box2[3] << ", "
                 << box2[4] << ", "
                 << box2[5] << "]";

        return s.str();
    }

  AxisAlignedFeatureResponse AxisAlignedFeatureResponse ::CreateRandom(Random& random)
  {
    return AxisAlignedFeatureResponse(random.Next(0, 2));
  }

  float AxisAlignedFeatureResponse::GetResponse(const IDataPointCollection& data, unsigned int sampleIndex) const
  {
    const DataPointCollection& concreteData = (DataPointCollection&)(data);
    return concreteData.GetDataPoint((int)sampleIndex)[axis_];
  }

  std::string AxisAlignedFeatureResponse::ToString() const
  {
    std::stringstream s;
    s << "AxisAlignedFeatureResponse(" << axis_ << ")";

    return s.str();
  }

  /// <returns>A new LinearFeatureResponse2d instance.</returns>
  LinearFeatureResponse2d LinearFeatureResponse2d::CreateRandom(Random& random)
  {
    double dx = 2.0 * random.NextDouble() - 1.0;
    double dy = 2.0 * random.NextDouble() - 1.0;

    double magnitude = sqrt(dx * dx + dy * dy);

    return LinearFeatureResponse2d((float)(dx / magnitude), (float)(dy / magnitude));
  }

  float LinearFeatureResponse2d::GetResponse(const IDataPointCollection& data, unsigned int index) const
  {
    const DataPointCollection& concreteData = (const DataPointCollection&)(data);
    return dx_ * concreteData.GetDataPoint((int)index)[0] + dy_ * concreteData.GetDataPoint((int)index)[1];
  }

  std::string LinearFeatureResponse2d::ToString() const
  {
    std::stringstream s;
    s << "LinearFeatureResponse(" << dx_ << "," << dy_ << ")";

    return s.str();
  }

} } }
