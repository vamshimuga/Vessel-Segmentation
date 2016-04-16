#ifndef MIAIMAGEFORESTINGTRANSFORM_H
#define MIAIMAGEFORESTINGTRANSFORM_H

#include "MiaCoreModules/miamodule.h"
#include <list>
#include <queue>
#include "MiaCoreModules/miamarker.h"
#define	OPTIMIZED 0x80
#define STOPPOINT 0x40
#define SEEDPOINT 0x20
#define FIXEDVALUE 0xA0 //OPTIMIZED|SEEDPOINT
#define DIRECTIONTOSELF 13 //0-26 represent the 26 neighbor's direction. 13 is the one in the middle

enum MIACOMMONMODULESSHARED_EXPORT AlgorithmType { MIA_ChamferDistance, MIA_SquareDifference,MIA_FuzzyConnectedness, MIA_SubclassDefined};

struct PathNode
{
        int index;
        float cost; // this is also used as label tag for seedlist
};
typedef std::vector<PathNode> PathContainer;
class NodeCompare
{
public:
        static bool ascending;
        bool operator()(const PathNode& node1, const PathNode& node2)
        {
                return compareCost(node1.cost, node2.cost);
        }

        static bool compareCost(float cost1, float cost2)
        {
            return ascending ? cost1 > cost2 : cost1 < cost2;
        }

};


class MIACOMMONMODULESSHARED_EXPORT MiaImageForestingTransform : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaImageForestingTransform(MiaNetwork *parent = 0);
    virtual ~MiaImageForestingTransform();
    bool stopAfterReachTarget;
    float stopAfterReachValue;
    void setSeedPointsList(const QList <MiaMarker*>& list);
    void setTargetPointsList(const QList <MiaMarker*>& list);
    void setThresholds(float lower, float upper){ lowerThreshold = lower; upperThreshold = upper;}//voxels out of this range are ignored
    void setAlgorithm(AlgorithmType algorithm);
    void createValidatedSeedPoints(const QList <MiaMarker*> markerList,std::list<PathNode>& outlist);
    void plantSeeds(unsigned char* tagOutput, float* outdata);
    int convertDirectionToOffset(int direction);
signals:
    void getInputMarkers(QList <MiaMarker*>& list);
public slots:
    void markerAdded(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;}
    void markerUpdated(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;}
    void markerRemoved(MiaMarker* marker) {Q_UNUSED(marker); needUpdate = true;}
    void getOutputTagImage(MiaImageND** img);
protected:
    virtual bool reloadInputImage();
    virtual bool runOperation();
    template <class S>
    bool runOperation(S* inData, float* outdata);
    virtual float getSpeedAt(int x, int y, int z, float inValue, float* outdata, unsigned char* tagOutput);
    virtual bool reAllocOutputImage();
    virtual void initializeOutputs();
    virtual bool checkIfNeedReAllocOutputImage(){ return true;}
    void caculateColorMapFromTagMap();
    unsigned char colorOfParent(unsigned char* pMap, int pointer);
    int numberOfTargetPoints;
    std::list<PathNode> seedPointsList;
    std::list<PathNode> targetPointsList;
    std::priority_queue<PathNode, PathContainer, NodeCompare> priorityQ;
    MiaImageND* tagMap;
    float seedMeanValue;
    int imageWidth;
    int imageHeight;
    int imageDepth;
    int sliceSize;
    int dimOffset[3];
    int imageSize[3];
    float spacing[3];
    int imDimension;
    AlgorithmType selectedAlgorith;
    float upperThreshold,lowerThreshold; //range to be included in caculation
    bool seedListIsSet;
};

#endif // MIAIMAGEFORESTINGTRANSFORM_H


