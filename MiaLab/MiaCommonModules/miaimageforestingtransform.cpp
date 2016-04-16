#include "miaimageforestingtransform.h"
#include <algorithm>
#include "miaconstants.h"

#define COSTFUNC(ind1, ind2) ( (inData[ind1] - inData[ind2]) * (inData[ind1] - inData[ind2]) ) + ((seedMeanValue - inData[ind2])*(seedMeanValue - inData[ind2]))

bool NodeCompare::ascending = true;

MiaImageForestingTransform::MiaImageForestingTransform(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Image Foresting Transform";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    stopAfterReachTarget = false;
    tagMap = NULL;
    imDimension = 2;
    selectedAlgorith = MIA_SquareDifference;

    upperThreshold = MAX_FLOAT;
    lowerThreshold = -MAX_FLOAT;
    stopAfterReachValue = MAX_FLOAT;
    seedListIsSet = false;

    logging_level = 5;

    NodeCompare::ascending = true;
}
MiaImageForestingTransform::~MiaImageForestingTransform()
{
    if(tagMap)
        tagMap->release();
}

void MiaImageForestingTransform::createValidatedSeedPoints(const QList<MiaMarker *> markerList, std::list<PathNode> &outlist)
{
    outlist.clear();
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    sliceSize = width*height;
    QList<QColor> colorList;
    for( int i = 0; i < markerList.size(); i++ )
    {
        if(markerList.at(i)->getKeyPointNumber()>1)
            continue;
        MiaPoint4D pt = markerList.at(i)->getKeyPointAt(0);
        pt = inputImage->convertPatientCoordinateToVoxel(pt);
        int x,y,z;
        x = pt.pos[0]; y = pt.pos[1]; z = pt.pos[2];
        if(x<0 || x>=width || y<0 || y>=height || z<0 || z>=depth)
            continue;
        int ind = z*sliceSize + y*width + x;

        QColor color = markerList.at(i)->color;
        if(!colorList.contains(color))
        {
            colorList.append(color);
        }
        int colortag = colorList.indexOf(color) + 1;
        PathNode node;
        node.index = ind;
        node.cost = colortag;
        outlist.push_back(node);
    }
}

bool MiaImageForestingTransform::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        imDimension = inputImage->getDimension();
        if(imDimension>3 || imDimension<2)
        {
            qDebug()<<"IFT: only support 2 or 3 dimension";
            return false;
        }

        QList <MiaMarker*> markerList;
        emit getInputMarkers(markerList);
        if(!seedListIsSet)
            createValidatedSeedPoints(markerList, seedPointsList);
        if(seedPointsList.size()<1 && !seedListIsSet)
        {
            qDebug()<<"no seed points";
            return false;
        }
    }
    else
        return false;
    return true;
}

void MiaImageForestingTransform::setSeedPointsList(const QList <MiaMarker*>& list)
{
    seedListIsSet = true;
    if(!reloadInputImage())
        throw std::runtime_error("Input image is empty");
    createValidatedSeedPoints(list, seedPointsList);

}
void MiaImageForestingTransform::setTargetPointsList(const QList<MiaMarker *> &list)
{
    if(!inputImage)
        throw std::runtime_error("Input image is empty");
    createValidatedSeedPoints(list, targetPointsList);
    if(targetPointsList.size()>0)
        stopAfterReachTarget = true;
}

void MiaImageForestingTransform::setAlgorithm(AlgorithmType algorithm)
{
     selectedAlgorith = algorithm;
     needUpdate = true;
     if(selectedAlgorith == MIA_FuzzyConnectedness)
     {
         NodeCompare::ascending = false;
         stopAfterReachValue = -MAX_FLOAT;
     }
     else
     {
         NodeCompare::ascending = true;
         stopAfterReachValue = MAX_FLOAT;
     }

}
void MiaImageForestingTransform::getOutputTagImage(MiaImageND** img)
{
    if(needUpdate && autoUpdateWhenOutputRequested)
        update();
    *img = tagMap;
    return;

}

bool MiaImageForestingTransform::runOperation()
{
    bool result = false;
    initializeOutputs();
    ImageType sourcetype = inputImage->getType();
    float* out = (float*)outputImage->getDataPtr();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            result = runOperation<uchar>(in,out);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            result = runOperation<short>(in,out);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            result = runOperation<unsigned short>(in,out);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            result = runOperation<int>(ini,out);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            result = runOperation<float>(inf,out);
        }
            break;
        case IM_RGBA:
        {
//            QRgb* inr = (QRgb*) inputImage->getDataPtr();
//            runOperation<QRgb>(inr,out);
//            qDebug() << "RGB image is not supported yet";
            return false;
        }
            break;
    }
    outputImage->copyGeometryInfoFrom(inputImage);
    if(selectedAlgorith == MIA_FuzzyConnectedness)
        caculateColorMapFromTagMap();

    return result;

}

float MiaImageForestingTransform::getSpeedAt(int x, int y, int z, float inValue, float *outdata, unsigned char *tagOutput)
{
    throw std::runtime_error("This method shoauld only be used in subclass of image forest transform");
    return 0.0f;
}

template <class S>
bool MiaImageForestingTransform::runOperation(S* inData, float* outdata)
{
    int x,y,z,dx,dy,dz;
    int currentindex,neighborindex;
    float currentcost,newcost;
    unsigned char currentdirection;
    unsigned char* tagOutput = (unsigned char*)tagMap->getDataPtr();
    int totalVoxel = sliceSize*imageDepth;
    #pragma omp parallel for
    for (int i=0; i<totalVoxel; i++)
    {
        if(inData[i]<lowerThreshold || inData[i] > upperThreshold)
            tagOutput[i] = (OPTIMIZED|DIRECTIONTOSELF);
    }
    if(priorityQ.empty())
        return false;
    currentindex = priorityQ.top().index;
    seedMeanValue = inData[currentindex];
    while (!priorityQ.empty())
    {
        currentindex = priorityQ.top().index;
        priorityQ.pop();

        if(tagOutput[currentindex]&OPTIMIZED)
                continue;

        z = currentindex/(sliceSize);
        y = (currentindex - z*(sliceSize))/imageWidth;
        x = currentindex - z*(sliceSize) - y*imageWidth;
        //set this point has been optimized.
        tagOutput[currentindex] = tagOutput[currentindex]|OPTIMIZED;
        currentcost = outdata[currentindex];

        if(stopAfterReachTarget)
        {
            if(tagOutput[currentindex] & STOPPOINT)
                numberOfTargetPoints--;
            if(numberOfTargetPoints <= 0)
                break;
        }
        if(NodeCompare::compareCost(currentcost, stopAfterReachValue))
            break;

        for (dz=-1; dz<=1; dz++)
        {
            if(z+dz < 0 || z+dz >= imageDepth)
                continue;
            for (dy=-1; dy<=1; dy++)
            {
                if(y+dy < 0 || y+dy >= imageHeight)
                    continue;
                for (dx=-1; dx<=1; dx++)
                {
                    if(x+dx < 0 || x+dx >= imageWidth)
                        continue;
                    neighborindex = (z+dz)*(sliceSize) + (y+dy)*imageWidth + (x+dx);

                    //if the neighbor is not optimized
                    if (!(tagOutput[neighborindex]&FIXEDVALUE))
                    {
                        switch(selectedAlgorith)
                        {
                        case MIA_ChamferDistance:
                            newcost = currentcost + sqrt((float)(dx*dx + dy*dy + dz*dz));
                            break;
                        case MIA_SquareDifference:
                            newcost = currentcost + COSTFUNC(currentindex, neighborindex);
                            break;
                        case MIA_SubclassDefined:
                            if(abs(dx)+abs(dy)+abs(dz)==1)
                                newcost = getSpeedAt(x+dx,y+dy,z+dz,inData[neighborindex],outdata,tagOutput);
                            else
                                newcost = MAX_FLOAT;
                            break;
                        case MIA_FuzzyConnectedness:
                            newcost = fmin(currentcost, inData[neighborindex]);
                            break;
                        default:
                            throw std::runtime_error("Unknown algorithm");
                        }

                        if (NodeCompare::compareCost(outdata[neighborindex], newcost))
                        {
                                outdata[neighborindex] = newcost;
                                currentdirection=(-dz+1)*9+(-dy+1)*3-dx+1;
                                tagOutput[neighborindex] = currentdirection;
                                PathNode aneighbor;
                                aneighbor.index = neighborindex;
                                aneighbor.cost = newcost;
                                priorityQ.push(aneighbor);
                        }
                    }


                }
             }

          }

      }
    while(!priorityQ.empty()){
        priorityQ.pop();
    }
    return true;
}

void MiaImageForestingTransform::plantSeeds(unsigned char* tagOutput, float* outdata)
{
    for (std::list<PathNode>::iterator iter = seedPointsList.begin(); iter != seedPointsList.end(); iter++)
    {
        PathNode anode = *iter;
        unsigned char tag = (unsigned char)(iter->cost);
        if(selectedAlgorith == MIA_FuzzyConnectedness)
        {
            outdata[anode.index]=MAX_FLOAT;
            anode.cost = MAX_FLOAT;
        }
        else
        {
            outdata[anode.index]=0;
            anode.cost = 0;
        }

        priorityQ.push(anode);
        tagOutput[anode.index] = SEEDPOINT|tag;
    }

    if( !targetPointsList.empty() ){
        numberOfTargetPoints = 0;
        for (std::list<PathNode>::iterator iter = targetPointsList.begin(); iter != targetPointsList.end(); iter++) {
            tagOutput[iter->index] |= STOPPOINT;
            numberOfTargetPoints++;
        }
    }
    else{
        numberOfTargetPoints = 1;
    }
}

void MiaImageForestingTransform::initializeOutputs()
{
    // initialize cost map and tag images
    unsigned char* tagOutput = (unsigned char*)tagMap->getDataPtr();
    float* outdata = (float*)outputImage->getDataPtr();

    int totalVoxel = sliceSize*imageDepth;
    while(!priorityQ.empty()){
        priorityQ.pop();
    }
    if(selectedAlgorith == MIA_FuzzyConnectedness)
    {
        #pragma omp parallel for
        for (int i=0; i<totalVoxel; i++)
        {
            outdata[i]=-MAX_FLOAT;
        }
    }
    else
    {
        #pragma omp parallel for
        for (int i=0; i<totalVoxel; i++)
        {
            outdata[i]=MAX_FLOAT;
        }
    }

    #pragma omp parallel for
    for (int i=0; i<totalVoxel; i++)
    {
            tagOutput[i] = DIRECTIONTOSELF;
    }
    qDebug() << "total voxel count" << totalVoxel;

    //plant seed
    plantSeeds(tagOutput, outdata);
}

int MiaImageForestingTransform::convertDirectionToOffset(int direction)
{
    int itemp=0;
    if(direction > 26)
        throw std::runtime_error("unknown driection");
    switch(direction)
    {
      case 0: itemp =  (-sliceSize-imageWidth-1);
        break;
      case 1: itemp =  (-sliceSize-imageWidth);
        break;
      case 2: itemp = (-sliceSize-imageWidth+1);
        break;
      case 3: itemp = (-sliceSize-1);
        break;
      case 4: itemp = (-sliceSize);
        break;
      case 5: itemp = (-sliceSize+1);
        break;
      case 6: itemp = (-sliceSize+imageWidth-1);
        break;
      case 7: itemp = (-sliceSize+imageWidth);
        break;
      case 8: itemp = (-sliceSize+imageWidth+1);
        break;
      case 9: itemp = (-imageWidth-1);
        break;
      case 10: itemp = (-imageWidth);
        break;
      case 11: itemp = (-imageWidth+1);
        break;
      case 12: itemp = (-1);
        break;
      case 13: itemp = 0;
        throw std::runtime_error("pointer to self");
        break;
      case 14: itemp = 1;
        break;
      case 15: itemp = imageWidth-1;
        break;
      case 16: itemp = imageWidth;
        break;
      case 17: itemp = imageWidth+1;
        break;
      case 18: itemp = sliceSize-imageWidth-1;
        break;
      case 19: itemp = sliceSize-imageWidth;
        break;
      case 20: itemp = sliceSize-imageWidth+1;
        break;
      case 21: itemp = sliceSize-1;
        break;
      case 22: itemp = sliceSize;
        break;
      case 23: itemp = sliceSize+1;
        break;
      case 24: itemp = sliceSize+imageWidth-1;
        break;
      case 25: itemp = sliceSize+imageWidth;
        break;
      case 26: itemp = sliceSize+imageWidth+1;
        break;
    }

    return itemp;
}

void MiaImageForestingTransform::caculateColorMapFromTagMap()
{

  unsigned char* pMap = (unsigned char*)tagMap->getDataPtr();
  int totalvoxel=sliceSize*imageDepth;
  //clean the unreached areas before setting colors
#pragma omp parallel for
  for(int i=0;i<totalvoxel;i++)
  {
      if(pMap[i] == DIRECTIONTOSELF || pMap[i] == (OPTIMIZED|DIRECTIONTOSELF))
      {
          pMap[i] = 0;
      }
  }
  //recursively checking the parent's color
  for(int i=0;i<totalvoxel;i++)
  {

      if(pMap[i]&SEEDPOINT)
      {
          unsigned char temp = pMap[i];
          pMap[i] = pMap[i]&0x1f;
          qDebug() << "seed" << i << temp << pMap[i];
      }
      else if(pMap[i]&OPTIMIZED)
      {
        int direction=pMap[i]&0x1f;
        int itemp = convertDirectionToOffset(direction);
        itemp+=i;
        qDebug() << "Checking" << i << direction << itemp;
        pMap[i] = colorOfParent(pMap, itemp);


      }
   }

}

unsigned char MiaImageForestingTransform::colorOfParent(unsigned char* pMap, int ind)
{
  qDebug() << "read" << ind;
  if(pMap[ind]&OPTIMIZED)
  {
    if(!(pMap[ind]&SEEDPOINT))
    {

        int direction=pMap[ind]&0x1f;
        int itemp = convertDirectionToOffset(direction);
        itemp += ind;
        qDebug() << "Checking" << ind << direction << itemp;
        pMap[ind] = colorOfParent(pMap, itemp);
    }
    else
    {
        pMap[ind]=pMap[ind]&0x1f;
        qDebug() << "seed" << ind << pMap[ind];
    }
  }

  return pMap[ind];

}

bool MiaImageForestingTransform::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    if(operateInSitu)
    {
        outputImage = inputImage;
    }
    else
    {
        outputImage = creatEmptyImageFrom( inputImage, IM_float);
    }

    if(outputImage)
    {
        outputImage->retain();
        if(tagMap)
            tagMap->release();
        tagMap = creatEmptyImageFrom(inputImage,IM_uchar);

        if(tagMap)
        {
            tagMap->retain();
        }
        else
            return false;

    }
    else
        return false;
    int* size = outputImage->getDimSize();
    float* sp = outputImage->getSpacing();
    imageWidth = size[0];
    imageHeight = size[1];
    imageDepth = size[2];
    float minSpacing = MAX_FLOAT;
    for(int i = 0; i<3; i++)
    {
        imageSize[i] = size[i];

        if(sp[i] < minSpacing)
            minSpacing = sp[i];
    }
    for(int i = 0; i<3; i++)
    {
        spacing[i] = sp[i]/minSpacing;
    }
    sliceSize = imageWidth*imageHeight;
    dimOffset[0] = 1;
    dimOffset[1] = imageWidth;
    dimOffset[2] = sliceSize;
    return true;
//    needReinitialize = false;
}

