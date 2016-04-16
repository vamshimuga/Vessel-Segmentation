#include "miafastmarchingfilter.h"
#include "miaconstants.h"
#define INVERSE_SPEED -1.0f

MiaFastMarchingFilter::MiaFastMarchingFilter(MiaNetwork *parent)
    :MiaImageForestingTransform(parent)
{
    moduleName = "Fast matching";
    inputImageCarrysInitialValue = false;
    selectedAlgorith = MIA_SubclassDefined;
    lowerSeedThreshold = -0.5;
    upperSeedThreshold = 0.5;
    useInputImageAsSpeed = false;
}

MiaFastMarchingFilter::~MiaFastMarchingFilter()
{

}

void MiaFastMarchingFilter::setInputImageCarrysInitialValue(bool value)
{
    inputImageCarrysInitialValue = value;
    seedListIsSet = inputImageCarrysInitialValue;
}

void MiaFastMarchingFilter::initializeOutputs()
{
    if(inputImageCarrysInitialValue)
    {
        unsigned char* tagOutput = (unsigned char*)tagMap->getDataPtr();
        float* outdata = (float*)outputImage->getDataPtr();
        int totalVoxel = sliceSize*imageDepth;
        while(!priorityQ.empty()){
            priorityQ.pop();
        }
        if(inputImage->getType() != IM_float)
        {
            qDebug()<<"inputImage carrys InitialValue, but input is not a float type!";
            throw std::runtime_error("inputImage carrys InitialValue, but input is not a float type!");
        }
        float* inData = (float*)inputImage->getDataPtr();

        for (int i=0; i<totalVoxel; i++)
        {
            if(inData[i] >= lowerSeedThreshold && inData[i] <= upperSeedThreshold)
            {
                PathNode anode;
                anode.index = i;
                anode.cost = inData[i];
                priorityQ.push(anode);
                tagOutput[i] = SEEDPOINT|DIRECTIONTOSELF;
                if(inData != outdata)
                    outdata[i]=inData[i];
            }
            else if(inData[i] < lowerThreshold || inData[i] > upperThreshold)
            {
                tagOutput[i] = OPTIMIZED|DIRECTIONTOSELF;
                if(inData != outdata)
                    outdata[i]=inData[i];
            }
            else
            {
                outdata[i]=MAX_FLOAT;
                tagOutput[i] = DIRECTIONTOSELF;
            }

        }
    }
    else
    {
        MiaImageForestingTransform::initializeOutputs();
    }
}

float MiaFastMarchingFilter::getSpeedAt(int x, int y, int z, float inValue, float* outdata, unsigned char* tagOutput)
{
    int index = z * sliceSize + y*imageWidth + x;
    int neighIndex;
    int pos[3];
    pos[0] = x; pos[1] = y; pos[2] = z;
    float neighValue;

    for ( int j = 0; j < imDimension; j++ )
      {
      neighborNodes[j].cost = MAX_FLOAT;

      // find smallest valued neighbor in this dimension
      for( int s = -1; s < 2; s = s + 2 )
        {
            if( pos[j] + s < 0 || pos[j] + s > imageSize[j]-1 )
            {
                continue;
            }

            neighIndex = index + s*dimOffset[j];

            if(tagOutput[neighIndex]&FIXEDVALUE)
            {
                neighValue = outdata[neighIndex];
                if( neighborNodes[j].cost > neighValue)
                {
                    neighborNodes[j].cost = neighValue;
                    neighborNodes[j].index = neighIndex;
                }
            }

            }

          // put the minimum neighbor onto the heap
          neighborNodes[j].axis = j;

      }

    // sort the local list
    std::sort( neighborNodes, neighborNodes + imDimension );

    // solve quadratic equation
    double aa, bb, cc;
    double solution = MAX_FLOAT;

    aa = 0.0;
    bb = 0.0;
    if (useInputImageAsSpeed)
      {
          cc = inValue;
          if(cc <= 0)
          {
              qDebug()<<"Fast matching: input speed can not be negative!";
              throw std::runtime_error("Fast matching: input speed can not be negative!");
          }
          cc = -1.0 / (cc*cc);
      }
    else
      {
      cc = INVERSE_SPEED;
      }


    double discrim;

    for ( int j = 0; j < imDimension; j++ )
      {

      if ( solution >= neighborNodes[j].cost )
        {
        const int axis = neighborNodes[j].axis;
        double spaceFactor = 1.0 / spacing[axis];
        spaceFactor = spaceFactor*spaceFactor;
        const double value = neighborNodes[j].cost;
        aa += spaceFactor;
        bb += value * spaceFactor;
        cc += value* value * spaceFactor;

        discrim = bb*bb - aa * cc;
        if ( discrim < 0.0 )
        {
            qDebug()<<"discrim < 0.0"<<x<<y<<z<<inValue<<j<<value<<aa<<bb<<cc<<discrim;
            throw std::runtime_error("discrim < 0.0");
        }

        solution = ( sqrt( discrim ) + bb ) / aa;
        }
      else
        {
        break;
        }
      }


    return solution;

}

void MiaFastMarchingFilter::setUseInputImageAsSpeed(bool value)
{
    useInputImageAsSpeed = value;
}

