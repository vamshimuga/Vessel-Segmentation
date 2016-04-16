#include "miazerocrossingfilter.h"
#define FARAWAY      10.0

MiaZeroCrossingFilter::MiaZeroCrossingFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Zero Crossing Filter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    requiredInputImageType = IM_float;
}
bool MiaZeroCrossingFilter::runOperation()
{
    int depth = inputImage->getDepth();
    int height = inputImage->getHeight();
    int width = inputImage->getWidth();
    int sliceSize = width*height;
    std::vector<int> zerocrossingpts;
    zerocrossingpts.reserve(sliceSize*2);
    int curindex;
    int x,y,z;
    float* inData = (float*)inputImage->getDataPtr();
    float* outData = (float*)outputImage->getDataPtr();

    int dimension = inputImage->getDimension();

    for( z = 0 ; z < depth ; z++)
        for(y = 0; y < height ; y ++ )
            for( x = 0 ; x < width;  x++ )
            {
                curindex = z*sliceSize + y*width + x;
                float curvalue = *(inData + curindex);
                if( curvalue > 0)
                {
                    *(outData + curindex) =  FARAWAY;
                }
                else
                {
                     *(outData + curindex) =  -FARAWAY;
                }
                float neighborValue;
                if ( curvalue > 0)
                {
                    if(x>1)
                    {
                        neighborValue = *(inData + curindex - 1);
                        if(neighborValue <= 0)
                        {
                            if ( curvalue < -neighborValue)
                                zerocrossingpts.push_back(curindex);
                            else
                                zerocrossingpts.push_back(curindex - 1);
                        }
                    }
                    if(x < width - 1)
                    {
                        neighborValue = *(inData + curindex + 1);
                        if(neighborValue <= 0)
                        {
                            if ( curvalue <= -neighborValue)
                                zerocrossingpts.push_back(curindex);
                            else
                                zerocrossingpts.push_back(curindex + 1);
                        }
                    }
                    if(y>1)
                    {
                        neighborValue = *(inData + curindex - width);
                        if(neighborValue <= 0)
                        {
                            if ( curvalue < -neighborValue)
                                zerocrossingpts.push_back(curindex);
                            else
                                zerocrossingpts.push_back(curindex - width);
                        }
                    }
                    if(y < height - 1)
                    {
                        neighborValue = *(inData + curindex + width);
                        if(neighborValue <= 0)
                        {
                            if ( curvalue <= -neighborValue)
                                zerocrossingpts.push_back(curindex);
                            else
                                zerocrossingpts.push_back(curindex + width);
                        }
                    }
                    if(z>1)
                    {
                        neighborValue = *(inData + curindex - sliceSize);
                        if(neighborValue <= 0)
                        {
                            if ( curvalue < -neighborValue)
                                zerocrossingpts.push_back(curindex);
                            else
                                zerocrossingpts.push_back(curindex - sliceSize);
                        }
                    }
                    if(z < depth - 1)
                    {
                        neighborValue = *(inData + curindex + sliceSize);
                        if(neighborValue <= 0)
                        {
                            if ( curvalue <= -neighborValue)
                                zerocrossingpts.push_back(curindex);
                            else
                                zerocrossingpts.push_back(curindex + sliceSize);
                        }
                    }


                }
                else
                {
                    if(dimension == 3)
                    {
                        if(x == 0 || x == width-1 || y == 0 || y == height-1 || z == 0 || z == depth -1)
                            zerocrossingpts.push_back(curindex);
                    }
                    else
                    {
                        if(x == 0 || x == width-1 || y == 0 || y == height-1)
                            zerocrossingpts.push_back(curindex);
                    }
                }
            }

    std::vector<int>::const_iterator iter;
    for(iter=zerocrossingpts.begin();iter!=zerocrossingpts.end();iter++)
    {
        int ind=*iter;
        *(outData + ind) = 0;
    }
    return true;
}

bool MiaZeroCrossingFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    if(operateInSitu && inputImage->getType()==IM_float)
        outputImage = inputImage;
    else
        outputImage = creatEmptyImageFrom( inputImage, IM_float);
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;

    return true;
}
