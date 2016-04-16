#ifndef MIAHISTOGRAMANALYSISFILTER_H
#define MIAHISTOGRAMANALYSISFILTER_H

#include "MiaCoreModules/miamodule.h"
#include <vector>
#define MRIBRAINSCALESLOPE 5.0f
#define MRILIVERSCALESLOPE 10.0f
struct GaussianMixtureElement
{
    double mu;
    double sigma;
    double alpha;
};

enum MIAMACHINELEARNINGMODULESSHARED_EXPORT Mia_Histogram_Operation_Type {Mia_Histo_T1GMWM_Peaks,Mia_Histo_T2Brain_Peaks, Mia_Histo_GMM, Mia_Histo_BodyTrunk_Crop,
                                   Mia_Histo_Undefine, Mia_Histo_BodyTrunk_Match, Mia_Histo_BodyTrunk_FixOrigin,
                                   Mia_Histo_Visceral_Liver, Mia_Histo_Visceral_Spleen, Mia_Histo_Visceral_Kidney,
                                   Mia_Histo_MRILiverGMM,Mia_Histo_MRILiverSTD,Mia_Histo_BackgroundSTD};

class MIAMACHINELEARNINGMODULESSHARED_EXPORT MiaHistogramAnalysisFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaHistogramAnalysisFilter(MiaNetwork *parent = 0);
    void setHistogramMinMaxValue(bool automatic, float min = 0, float max = 0);
    void setHistogramBinSize(float size);
    void setHistogramROIBos(bool fullvolume, int* box = NULL);
    void setCurrentOperation(Mia_Histogram_Operation_Type type) { currentOperation = type; }
    void setBackgroundValue(float value) { backgroundValue = value;}
    
signals:
    
public slots:
    void getGMM(float* gmmAlpha, float* gmmMu, float* gmmSigma, int n);
protected:
    bool runOperation();
    bool createHistgramForCurrentRegion();
    template <class S>
    bool createHistgram(const S* inData);
    bool reAllocOutputImage();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);
    bool runExpectationMaximization(int numberofcluster, std::list<GaussianMixtureElement>* initvalues = NULL);
    void scanPeaksAndValleys(std::vector<float>& peaks, std::vector<float>& valleys, float sigma);
    void smoothHistogramWithGuassionKernel(std::vector<double>& input, std::vector<double>& output, float sigma);
    bool runOperationForT1GMWMPeaks();
    bool runOperationForT2BrainPeak();
    bool runOperationForBodyTrunckCrop();
    bool runOperationForKidneyPeak();
    bool runOperationForSpleenPeak();
    bool runOperationForLiverPeak();
    bool runOperationForBackgroundPeak();
    bool runOperationForMRILIverGMMAnalysis();
    bool runOperationForMRILIverSTDAnalysis();
    bool caculateMeanAndSTDWithIntensityRange(float lower, float upper, float& mean, float& std, float& count);
    template <class T>
    QList<QRectF> createConnnectedComponentsListForSlice(int slicenum, const T* inData);
    Mia_Histogram_Operation_Type currentOperation;
    float histSettingMinValue,histSettingMaxValue;
    float histSettingBinSize;
    int histSettingNumberOfBins;
    int histROIBox[6];
    bool histAutoMinMaxValue;
    bool histUsingFullVolume;

    //parameters for GMM
    std::vector<double> histogram;
    std::list<GaussianMixtureElement> gmmElements;
    int gmmNumberOfCluster;
    float backgroundValue;
    int trunkBottomSlice;
    int trunkTopSlice;

};

#endif // MIAHISTOGRAMANALYSISFILTER_H
