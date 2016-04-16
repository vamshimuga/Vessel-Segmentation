#ifndef COSTFUCTIONBASE_H
#define COSTFUCTIONBASE_H
#include <Eigen/Dense>
#include <vector>
#define MAXNUMTHREAD 256
#define USE_OPENMP
class TransformFunctionBase;
class IntensityMappingFuctionBase;
class MiaImageND;
enum SampleStrategy {MIA_Sample_All, MIA_Sample_From_MovingImage, MIA_Sample_Random, MIA_Sample_Given, MIA_Sample_DownSampled};
class CostFunctionBase
{
public:
    explicit CostFunctionBase();
    virtual ~CostFunctionBase();
    virtual void setFixedImage(MiaImageND *input);
    virtual void setMovingImage(MiaImageND* input);
    virtual void setWeightingImage(MiaImageND* input);
    virtual MiaImageND* getMovingImage(); // the statistic model cost function will have it own movingimage
    virtual void setTransformFunction(TransformFunctionBase* func);
    virtual void setIntensityMappingFunction(IntensityMappingFuctionBase* func);
    virtual int getNumberOfTransformationParameters()const;
    virtual int getNumberOfMappingParameters()const;
    virtual bool setCurrentTransformationParameters(const Eigen::VectorXf& parameters);
    virtual bool setCurrentIntensityMappingParameters(const Eigen::VectorXf& parameters);
    virtual bool getCurrentTransformationParameters(Eigen::VectorXf& parameters);
    virtual bool getCurrentIntensityMappingParameters(Eigen::VectorXf& parameters);
    virtual double getValueAndDerivativeForTransformFunction(Eigen::VectorXd& value, Eigen::MatrixXd& jacobian) = 0;
    virtual double getValueAndDerivativeForMappingFunction(Eigen::VectorXd& value, Eigen::MatrixXd& jacobian) = 0;
    virtual void initializeParamerter();
    virtual bool isValid() const;
    virtual void setSampleStrategy(SampleStrategy strategy, int numpt = 0, std::vector<int> *pts=NULL);
    virtual SampleStrategy getSampleStrategy(){return currentSampleStrategy;}
    virtual void setBackgroundValue(float value) { backgroundValue = value; }
    virtual int getSampleSize() {return sampleSize;}
protected:
    virtual void reinitialSamples();
    virtual bool getNextSamplePairs(Eigen::Vector3f &fixedpt, Eigen::Vector3f& movingpt, int threadID);
    virtual bool getSampleWithIndex(Eigen::Vector3f& voxel, int index);
    int currentSampleCount[MAXNUMTHREAD];
    TransformFunctionBase* transformFunction;
    IntensityMappingFuctionBase* intensityMappingFucntion;
    MiaImageND* fixedImage;
    MiaImageND* weightingImage;
    MiaImageND* movingImage;
    SampleStrategy currentSampleStrategy;
    int sampleSize;
    int dataSize;
    int imageWidth,imageHeight,imageDepth;
    int sampleSizePerThread[MAXNUMTHREAD];
    std::vector<int>  samplePointsList;
    int numberOfThreadRunning;
    float backgroundValue;
    int sampleInterval;
    int downSampledDimSize[3];
};

#endif // COSTFUCTIONBASE_H
