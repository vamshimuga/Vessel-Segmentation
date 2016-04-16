#include "miahistogramanalysisfilter.h"
#include "miaconstants.h"
#include "MiaCoreModules/mianetwork.h"

#define max(x,y)    ((x>y) ? x : y )
#define min(x,y)    ((x<y) ? x : y )
#define MAX_ITERATION 20000
#define BRAINTEMPLATESTD 77

bool compare_peaks (const GaussianMixtureElement& first, const GaussianMixtureElement& second)
{
  double peak1 = first.alpha/first.sigma;
  double peak2 = second.alpha/second.sigma;
  return ( peak1 > peak2 );
}
bool compare_mu (const GaussianMixtureElement& first, const GaussianMixtureElement& second)
{
  return ( first.mu < second.mu );
}

MiaHistogramAnalysisFilter::MiaHistogramAnalysisFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Histogram Analysis Filter";
    autoUpdateWhenInputChanged = true;
    autoUpdateWhenParameterChanged = true;

    currentOperation = Mia_Histo_T1GMWM_Peaks;
    histSettingMinValue = 0;
    histSettingMaxValue = 0;
    histSettingBinSize = 1;
    histAutoMinMaxValue = true;
    histUsingFullVolume = true;
    gmmNumberOfCluster = 5;

    trunkBottomSlice = 0;
    trunkTopSlice = 10000000;
}

void MiaHistogramAnalysisFilter::setHistogramMinMaxValue(bool automatic, float min, float max)
{
    histAutoMinMaxValue = automatic;
    histSettingMinValue = min;
    histSettingMaxValue = max;
}

void MiaHistogramAnalysisFilter::setHistogramBinSize(float size)
{
    histSettingBinSize = size;
    reAllocOutputImage();
}

void MiaHistogramAnalysisFilter::setHistogramROIBos(bool fullvolume, int *box)
{
    histUsingFullVolume = fullvolume;
    if(!histUsingFullVolume)
    {
        for(int i = 0 ; i < 6; i++)
            histROIBox[i] = box[i];
    }
}

void MiaHistogramAnalysisFilter::getGMM(float *gmAlpha, float *gmmMu, float *gmmSigmaSquare, int n)
{

}

bool MiaHistogramAnalysisFilter::runOperation()
{
    bool success = false;
    success = createHistgramForCurrentRegion();
    if(success)
    {
        switch (currentOperation) {
        case Mia_Histo_T1GMWM_Peaks:
            success = runOperationForT1GMWMPeaks();
            break;
        case Mia_Histo_T2Brain_Peaks:
            success = runOperationForT2BrainPeak();
            break;
        case Mia_Histo_GMM:
            success = runExpectationMaximization(gmmNumberOfCluster);
            break;
        case Mia_Histo_BodyTrunk_Crop:
            success = runOperationForBodyTrunckCrop();
            break;
        case Mia_Histo_Visceral_Liver:
            success = runOperationForLiverPeak();
            break;
        case Mia_Histo_Visceral_Spleen:
            success = runOperationForSpleenPeak();
            break;
        case Mia_Histo_Visceral_Kidney:
            success = runOperationForKidneyPeak();
            break;
        case Mia_Histo_MRILiverGMM:
            success = runOperationForMRILIverGMMAnalysis();
            break;
        case Mia_Histo_MRILiverSTD:
            success = runOperationForMRILIverSTDAnalysis();
            break;
        case Mia_Histo_BackgroundSTD:
            success = runOperationForBackgroundPeak();
            break;
        default:
            break;
        }
    }

    return success;
}

bool MiaHistogramAnalysisFilter::runOperationForT1GMWMPeaks()
{
    float mean = 0;
    float std = 0;
    float count = 0;
    caculateMeanAndSTDWithIntensityRange(0,100000,mean,std, count);
    float normalizedBinSize = std/BRAINTEMPLATESTD;
    if(normalizedBinSize < 1.0 && inputImage->getType() != IM_float)
        normalizedBinSize = 1.0;

    setHistogramBinSize(normalizedBinSize);
    createHistgramForCurrentRegion();
    gmmElements.clear();
    GaussianMixtureElement element1;
    element1.alpha = 0.5;
    element1.mu = 80*normalizedBinSize;
    element1.sigma = sqrt(histSettingNumberOfBins)*histSettingBinSize;
    GaussianMixtureElement element2;
    element2.alpha = 0.5;
    element2.mu = 250*normalizedBinSize;
    element2.sigma = sqrt(histSettingNumberOfBins)*histSettingBinSize;
    gmmElements.push_back(element1);
    gmmElements.push_back(element2);

// test with 2 elements
    bool success = runExpectationMaximization(2,&gmmElements);
    if(!success)
        return false;
    gmmElements.sort(compare_mu);
    element1 = gmmElements.front();
    element2 = gmmElements.back();
    element2.alpha *=0.5;
    GaussianMixtureElement element3 = element2;
    element2.mu -= element2.sigma;
    element3.mu += element3.sigma;
    qDebug() << "estimated peaks are" << element1.mu << element2.mu << element3.mu;
    gmmElements.clear();
    gmmElements.push_back(element1);
    gmmElements.push_back(element2);
    gmmElements.push_back(element3);
// run EM again
    success = runExpectationMaximization(3,&gmmElements);
    if(!success)
        return false;
//    gmmElements.sort(compare_peaks);
    std::list<GaussianMixtureElement> brainelements;

    std::list<GaussianMixtureElement>::iterator it;
    it = gmmElements.begin();
    for(int i = 0; i < 3; i++)
    {
        brainelements.push_back(*it);
        ++it;
    }
    brainelements.sort(compare_mu);
    float sortedpeaks[3];
    float sortedstd[3];
    it =brainelements.begin();
    for (int i = 0; i < 3; i++)
    {
        qDebug() << "sorted peaks"<< (*it).mu;
        sortedpeaks[i] = (*it).mu;
        sortedstd[i] = (*it).sigma;
        ++it;
    }

    float csfPeak = sortedpeaks[0];
    float csfStd = sortedstd[0];
    float grayMatterPeak = sortedpeaks[1];
    float graymatterStd = sortedstd[1];
    float whiteMatterPeak = sortedpeaks[2];
    float whiteMatterStd  = sortedstd[2];

    if(csfPeak > 70*normalizedBinSize)
        csfPeak -= (grayMatterPeak-csfPeak) - graymatterStd;

    QMap<QString,QVariant> t1para;

    float nuclei_lower = grayMatterPeak;
    float nuclei_upper = whiteMatterPeak - whiteMatterStd*0.5f;
    QList<QVariant> lut1;
    lut1.append(QPointF(csfPeak,-1.0));
    lut1.append(QPointF(nuclei_lower,1.0));
    lut1.append(QPointF(nuclei_upper,0.0));
    lut1.append(QPointF(whiteMatterPeak,-1.0));

    QList<QVariant> lut3;
    lut3.append(QPointF(whiteMatterPeak,0));
    lut3.append(QPointF(grayMatterPeak,0));
    lut3.append(QPointF(csfPeak,0));
    lut3.append(QPointF(graymatterStd,0));

    QList<QVariant> lut4;
    lut4.append(QPointF(csfPeak,-1.0));
    lut4.append(QPointF(grayMatterPeak-graymatterStd*0.5,-1.0));
    lut4.append(QPointF(grayMatterPeak-graymatterStd*0.25,0));
    lut4.append(QPointF(whiteMatterPeak, 1.0));

    QList<QVariant> lut4a;
    lut4a.append(QPointF(csfPeak,-1.0));
    lut4a.append(QPointF(grayMatterPeak-graymatterStd*0.5,-1.0));
    lut4a.append(QPointF(whiteMatterPeak - whiteMatterStd,0));
    lut4a.append(QPointF(whiteMatterPeak, 0.1));

    QList<QVariant> lut5;

    lut5.append(QPointF(0,1.0));
    lut5.append(QPointF(csfPeak ,1.0));
    lut5.append(QPointF(grayMatterPeak - 2.0f*graymatterStd, -1.0));

    QList<QVariant> lut6;
    lut6.append(QPointF(csfPeak - 20,-1.0));
    lut6.append(QPointF(grayMatterPeak,1.0));
    lut6.append(QPointF(grayMatterPeak+4.0f*graymatterStd,-1.0));


    t1para.insert("CSFThreshold",csfPeak);
    t1para.insert("WhiteMatterPeakValue",whiteMatterPeak);
    t1para.insert("NucleiLUT",lut1);
    t1para.insert("WhiteMatterGrayMatterLUT",lut3);
    t1para.insert("CerebellumLUTScout",lut4a);
    t1para.insert("CerebellumLUT",lut4);
    t1para.insert("VentricleLUT", lut5);
    t1para.insert("SkullStrippingLUT", lut6);
    parentNetwork->enviromentSetting->saveParametersToXMLDoc(t1para);
    return true;
}

bool MiaHistogramAnalysisFilter::runOperationForT2BrainPeak()
{
    float mean = 0;
    float std = 0;
    float count = 0;
    caculateMeanAndSTDWithIntensityRange(100,100000,mean,std, count);
    QMap<QString,QVariant> t2para;
    t2para.insert("MSCutThreshold",mean+3.0*std);
    parentNetwork->enviromentSetting->saveParametersToXMLDoc(t2para);

    return true;
}
bool MiaHistogramAnalysisFilter::runOperationForBackgroundPeak()
{
    float mean = 0;
    float std = 0;
    float count = 0;
    caculateMeanAndSTDWithIntensityRange(-10000,10000,mean,std,count);
    qDebug() << mean << std << count;

    return true;
}

bool MiaHistogramAnalysisFilter::runOperationForLiverPeak()
{
    float mean = 0;
    float std = 0;
    float count = 0;
    caculateMeanAndSTDWithIntensityRange(0,400,mean,std,count);

    if(std <= 0 || mean+std < 50 || mean > 300 || std > 100 || count < 1000)
    {
        qDebug() << "liver intensity analysis failed" << mean << std << count;
        return false;
    }
    QList<QVariant> lut_liver;
    lut_liver.append(QPointF(-100,-2.0));


    if(mean-2*std > -10)
    {
        lut_liver.append(QPointF(mean-2*std,-0.5));
        lut_liver.append(QPointF(mean-1*std,0.5));
    }
    else
    {
        lut_liver.append(QPointF(-10,-0.5));
        lut_liver.append(QPointF(50,0.5));
    }

    lut_liver.append(QPointF(mean+2*std,0.5));
    lut_liver.append(QPointF(mean+3*std,-2.0));


    QMap<QString,QVariant> liverpara;
    liverpara.insert("LiverLUT",lut_liver);
    if(mean>70)
    {
        liverpara.insert("LiverExportThreshold",0.5);
    }
    else
    {
        liverpara.insert("LiverExportThreshold",0.5);
    }
    parentNetwork->enviromentSetting->saveParametersToXMLDoc(liverpara);

    return true;
}

bool MiaHistogramAnalysisFilter::runOperationForMRILIverGMMAnalysis()
{

    setHistogramBinSize(10*MRILIVERSCALESLOPE);
    createHistgramForCurrentRegion();
    int lastValidBin = 0;
    int imagetotalsize = inputImage->getImageSize();
    for(int i = 0; i < histSettingNumberOfBins; i++)
    {
        if(histogram[i]/(float)imagetotalsize > 0.001)
            lastValidBin = i;
    }
    int lastpeak = lastValidBin;
    for(int i = lastValidBin-1; i > 0; i--)
    {
        if(histogram[i] < histogram[i+1])
        {
            lastpeak = i;
            break;
        }
    }
    float livermax = lastpeak*histSettingBinSize;


    setHistogramBinSize(MRILIVERSCALESLOPE);
    createHistgramForCurrentRegion();
    gmmElements.clear();
    GaussianMixtureElement element1;
    element1.alpha = 0.33;
    element1.mu = 50*MRILIVERSCALESLOPE;
    element1.sigma = sqrt(histSettingNumberOfBins)*histSettingBinSize;
;
    GaussianMixtureElement element2;
    element2.alpha = 0.33;
    element2.mu = (50*MRILIVERSCALESLOPE+livermax)*0.5;
    element2.sigma = sqrt(histSettingNumberOfBins)*histSettingBinSize;

    GaussianMixtureElement element3;
    element3.alpha = 0.33;
    element3.mu = livermax;
    element3.sigma = sqrt(histSettingNumberOfBins)*histSettingBinSize;

    gmmElements.push_back(element1);
    gmmElements.push_back(element2);
    gmmElements.push_back(element3);
    qDebug() << "estimated peaks are" << element1.mu << element2.mu << element3.mu;
// run EM again
    bool success = runExpectationMaximization(3,&gmmElements);
    if(!success)
        return false;
//    gmmElements.sort(compare_peaks);
    std::list<GaussianMixtureElement> mriElements;

    std::list<GaussianMixtureElement>::iterator it;
    it = gmmElements.begin();
    for(int i = 0; i < 3; i++)
    {
        mriElements.push_back(*it);
        ++it;
    }
    mriElements.sort(compare_mu);
    float sortedpeaks[3];
    float sortedstd[3];
    it =mriElements.begin();
    for (int i = 0; i < 3; i++)
    {
        qDebug() << "sorted peaks"<< (*it).mu;
        sortedpeaks[i] = (*it).mu;
        sortedstd[i] = (*it).sigma;
        ++it;
    }

    float airPeak = sortedpeaks[0];
    float airStd = sortedstd[0];
    float bodymassPeak = sortedpeaks[1];
    float bodymassSTD = sortedstd[1];
    float liverPeak = sortedpeaks[2];
    float liverSTD  = sortedstd[2];
    qDebug() << "Liver Histog analysis:" <<
           airPeak << airStd << bodymassPeak << bodymassSTD << liverPeak << liverSTD;

    QMap<QString,QVariant> mriLiverPara;
    QList<QVariant> lut1;
    lut1.append(QPointF(airPeak,-1.0));
    lut1.append(QPointF(bodymassPeak,-1.0));
    lut1.append(QPointF(liverPeak,1.0));
    lut1.append(QPointF(liverPeak + liverSTD*6,-1.0));

    mriLiverPara.insert("AirPeakValue",airPeak);
    mriLiverPara.insert("BodymassPeakValue",bodymassPeak);
    mriLiverPara.insert("LiverPeakValue",liverPeak);
    mriLiverPara.insert("MRILiverLUT",lut1);

    parentNetwork->enviromentSetting->saveParametersToXMLDoc(mriLiverPara);
    return true;
}

bool MiaHistogramAnalysisFilter::runOperationForMRILIverSTDAnalysis()
{
    float mean = 0;
    float std = 0;
    float count = 0;
    caculateMeanAndSTDWithIntensityRange(0,100000,mean,std, count);
    float* spacing = inputImage->getSpacing();
    qDebug() << "Volume:" << count*spacing[0]*spacing[1]*spacing[2];

    gmmElements.clear();
    GaussianMixtureElement element1;
    element1.alpha = 0.5;
    element1.mu = mean - std;
    element1.sigma = sqrt(histSettingNumberOfBins)*histSettingBinSize;
    GaussianMixtureElement element2;
    element2.alpha = 0.5;
    element2.mu = mean + std;
    element2.sigma = sqrt(histSettingNumberOfBins)*histSettingBinSize;
    gmmElements.push_back(element1);
    gmmElements.push_back(element2);

    setHistogramBinSize(10);
    createHistgramForCurrentRegion();
// test with 2 elements
    bool success = runExpectationMaximization(2,&gmmElements);
    if(!success)
        return false;
    gmmElements.sort(compare_mu);
    element1 = gmmElements.front();
    element2 = gmmElements.back();
    mean = element2.mu;
    std = element2.sigma;

    QList<QVariant> lut_liver1;
    lut_liver1.append(QPointF(mean-3*std,-1.0));
    lut_liver1.append(QPointF(mean-1*std,1.0));
    lut_liver1.append(QPointF(mean+3*std,1.0));
    lut_liver1.append(QPointF(mean+4*std,-1.0));
    qDebug() << mean-2.0*std << mean+3.5*std;

    QList<QVariant> lut_liver2;
    lut_liver2.append(QPointF(mean-4*std,-1.0));
    lut_liver2.append(QPointF(mean-1*std,1.0));
    lut_liver2.append(QPointF(mean+2*std,1.0));
    lut_liver2.append(QPointF(mean+6*std,-1.0));
    qDebug() << mean-4*std << mean-1*std << mean+2*std << mean+6*std;


    QMap<QString,QVariant> liverpara;
    liverpara.insert("MRILiverSTDLUT1",lut_liver1);
    liverpara.insert("MRILiverSTDLUT2",lut_liver2);
    parentNetwork->enviromentSetting->saveParametersToXMLDoc(liverpara);

    return true;
}
bool MiaHistogramAnalysisFilter::runOperationForSpleenPeak()
{
    float mean = 0;
    float std = 0;
    float count = 0;
    caculateMeanAndSTDWithIntensityRange(0,400,mean,std, count);

    if(std <= 0 || mean+std < 50 || mean > 300 || std > 100 || count < 1000)
    {
        qDebug() << "liver intensity analysis failed" << mean << std << count;
        return false;
    }
    QList<QVariant> lut_spleen;
    lut_spleen.append(QPointF(-10,-2.0));

    if(mean-2.5*std > 20)
    {
        lut_spleen.append(QPointF(mean-3*std,-0.5));
        lut_spleen.append(QPointF(mean-2*std,0.5));
    }
    else
    {
        lut_spleen.append(QPointF(20,0.0));
        lut_spleen.append(QPointF(50,0.5));
    }

    lut_spleen.append(QPointF(mean+2*std,0.5));
    lut_spleen.append(QPointF(mean+3*std,-2.0));


    QMap<QString,QVariant> spleenpara;
    spleenpara.insert("SpleenLUT",lut_spleen);
    if(mean>70)
    {
        spleenpara.insert("SpleenExportThreshold",0.5);
    }
    else
    {
        spleenpara.insert("SpleenExportThreshold",0.5);
    }
    parentNetwork->enviromentSetting->saveParametersToXMLDoc(spleenpara);

    return true;
}
bool MiaHistogramAnalysisFilter::runOperationForKidneyPeak()
{
    float mean = 0;
    float std = 0;
    float count = 0;
    caculateMeanAndSTDWithIntensityRange(30,500,mean,std, count);
    if(std <= 0 || mean+std < 50 || mean > 400 || std > 100 || count < 800)
    {
        qDebug() << "kidney intensity analysis failed" << mean << std;
        return false;
    }
    QList<QVariant> lut1;
    lut1.append(QPointF(-50,-1.0));
    lut1.append(QPointF(50,1.0));
    lut1.append(QPointF(200,1.0));
    lut1.append(QPointF(500,-2.0));

    QList<QVariant> lut2;
    lut2.append(QPointF(-50,-1.0));
    lut2.append(QPointF(50,-0.5));
    lut2.append(QPointF(100,0.5));
    lut2.append(QPointF(150,0.5));
    lut2.append(QPointF(200,1.0));
    lut2.append(QPointF(500,-2.0));

    QMap<QString,QVariant> kidneypara;
    if(mean < 70)
        kidneypara.insert("KidneyLUT",lut1);
    else
        kidneypara.insert("KidneyLUT",lut2);
    if(mean>70)
    {
        kidneypara.insert("KidneyExportThreshold",0.5);
    }
    else
    {
        kidneypara.insert("KidneyExportThreshold",0.5);
    }
    parentNetwork->enviromentSetting->saveParametersToXMLDoc(kidneypara);

    return true;
}


bool MiaHistogramAnalysisFilter::runOperationForBodyTrunckCrop()
{
    histSettingMinValue = -1000;
    histSettingMaxValue = 0;
    int depth = inputImage->getDepth();
    QVector< QList <QRectF> > connectedAreaProfile(depth);
    ImageType sourcetype = inputImage->getType();
    for(int i = 0 ; i < depth; i++)
    {
        QList<QRectF> connectedAreas;
        switch(sourcetype)
        {
            case IM_uchar:
            {
                uchar* in = (uchar*) inputImage->getDataPtr();
                connectedAreas = createConnnectedComponentsListForSlice<uchar>(i,in);
            }
                break;
            case IM_short:
            {
                short* in = (short*) inputImage->getDataPtr();
                connectedAreas = createConnnectedComponentsListForSlice<short>(i,in);
            }
            break;
            case IM_unsignedshort:
            {
                unsigned short* in = (unsigned short*) inputImage->getDataPtr();
                connectedAreas = createConnnectedComponentsListForSlice<unsigned short>(i,in);
            }
            break;
            case IM_int:
            {
                int* ini = (int*) inputImage->getDataPtr();
                connectedAreas = createConnnectedComponentsListForSlice<int>(i,ini);
            }
                break;
            case IM_float:
            {
                float* inf = (float*) inputImage->getDataPtr();
                connectedAreas = createConnnectedComponentsListForSlice<float>(i,inf);
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
        connectedAreaProfile[i] = connectedAreas;
    }
    int max_width = 0;
    int max_index = -1;
    for(int i = depth/3; i < 2*depth/3; i++)
    {
        for(int j = 0; j < connectedAreaProfile[i].count(); j++)
        {
            if(connectedAreaProfile[i].at(j).width() > max_width)
            {
                max_width = connectedAreaProfile[i].at(j).width();
                max_index = i;
            }
        }
    }
    int foot_crop = 0, head_crop = depth -1;
    for(int i = max_index; i >= 0; i--)
    {
        bool hasRegionBelongToTrunk = false;
        for(int j = 0; j < connectedAreaProfile[i].count(); j++)
        {
            if(connectedAreaProfile[i].at(j).width() > max_width/2)
            {
                hasRegionBelongToTrunk = true;
                break;
            }
        }
        if(hasRegionBelongToTrunk == false)
        {
            foot_crop = i;
            break;
        }
    }

    for(int i = max_index; i < depth; i++)
    {
        bool hasRegionBelongToTrunk = false;
        for(int j = 0; j < connectedAreaProfile[i].count(); j++)
        {
            if(connectedAreaProfile[i].at(j).width() > max_width/2)
            {
                hasRegionBelongToTrunk = true;
                break;
            }
        }
        if(hasRegionBelongToTrunk == false)
        {
            head_crop = i;
            break;
        }
    }

    qDebug() << "Crop Poistion " << foot_crop << head_crop;
    trunkBottomSlice = foot_crop;
    trunkTopSlice = head_crop;
    QMap<QString,QVariant> clippara;
    if(trunkBottomSlice > 10 && trunkTopSlice < depth - 10)
    {

        clippara.insert("VisceralClipOn", true);
        clippara.insert("VisceralClipFoot",trunkBottomSlice);
        clippara.insert("VisceralClipTop", trunkTopSlice);
        clippara.insert("VisceralClipOriginalFoot",-trunkBottomSlice);
        clippara.insert("VisceralClipOriginalTop", depth-1-trunkTopSlice);
    }
    else
    {
        clippara.insert("VisceralClipOn", false);
    }

    parentNetwork->enviromentSetting->saveParametersToXMLDoc(clippara);

    return true;
}

bool MiaHistogramAnalysisFilter::createHistgramForCurrentRegion()
{
    bool result = false;
    ImageType sourcetype = inputImage->getType();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            result = createHistgram<uchar>(in);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            result = createHistgram<short>(in);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            result = createHistgram<unsigned short>(in);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            result = createHistgram<int>(ini);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            result = createHistgram<float>(inf);
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
    return result;
}

bool MiaHistogramAnalysisFilter::reAllocOutputImage()
{
    if(histUsingFullVolume)
    {
        histROIBox[0] = histROIBox[2] = histROIBox[4] = 0;
        int* dim = inputImage->getDimSize();
        histROIBox[1] = dim[0] - 1;
        histROIBox[3] = dim[1] - 1;
        histROIBox[5] = dim[2] - 1;
    }
    if(histAutoMinMaxValue)
    {
        float min, max;
        inputImage->getMaxMin(min,max);
        histSettingMinValue = min;
        histSettingMaxValue = max;
    }

    histSettingNumberOfBins = (histSettingMaxValue - histSettingMinValue)/histSettingBinSize + 1;
    histogram.resize(histSettingNumberOfBins);
    for(int i = 0; i< histSettingNumberOfBins; i++)
    {
        histogram[i] = 0;
    }

    return true;
}

void MiaHistogramAnalysisFilter::dispatchParameters(const QMap<QString, QVariant> &parameters)
{

}

bool MiaHistogramAnalysisFilter::runExpectationMaximization(int numberofcluster, std::list<GaussianMixtureElement>* initvalues)
{
    Eigen::VectorXd gmmMu;
    Eigen::VectorXd gmmAlpha;
    Eigen::VectorXd gmmSigmaSquare;

    //scan histogram
    int nonZeroBins = 0;
    double sum_histo = 0.0;
    histogram[0] = 0;
    histogram[1] = 0;
    histogram[2] = 0;
    histogram[3] = 0;
    histogram[4] = 0;
    for(int i = 0 ; i < histSettingNumberOfBins; i++)
    {
        if(histogram[i]>0)
        {
            nonZeroBins++;
            sum_histo += histogram[i];
        }

    }

    Eigen::VectorXd sample_values(nonZeroBins);
    Eigen::VectorXd sample_weights(nonZeroBins);
    int sample_index = 0;
    for(int i = 0; i < histSettingNumberOfBins; i++)
    {
        if(histogram[i] > 0)
        {
            sample_values[sample_index] = i+1;
            sample_weights[sample_index] = histogram[i]/sum_histo;
//            qDebug() << sample_weights[sample_index];
            sample_index++;
        }
        else {
//            qDebug() << 0;
        }
    }

    //initialize the GMM p = alpha*Gaussion(mu,sigma) + ...
    gmmMu.resize(numberofcluster);
    gmmSigmaSquare.resize(numberofcluster);
    gmmAlpha.resize(numberofcluster);
    double sml = 0;
    for(int i = 1; i < nonZeroBins; i++)
    {
        sml += sample_values[i] - sample_values[i - 1];
    }
    sml = sml/(double)(nonZeroBins - 1.0);
    sml = sml/1000.0;
    qDebug() <<"sml:" << sml << "num bins" << histSettingNumberOfBins;
    if(initvalues != NULL)
    {
        assert(initvalues->size() == numberofcluster);
        std::list<GaussianMixtureElement>::iterator it;
        it = initvalues->begin();
        for(int i = 0 ; i < numberofcluster; i++)
        {
            gmmMu[i] = (*it).mu/(double)histSettingBinSize;
            gmmSigmaSquare[i] = (*it).sigma * (*it).sigma/(double)(histSettingBinSize*histSettingBinSize);
            gmmAlpha[i] = (*it).alpha;
            ++it;
        }
    }
    else
    {
        for(int i = 0 ; i < numberofcluster; i++)
        {
            gmmMu[i] = (double)(histSettingNumberOfBins*(i+1)) / (double)(numberofcluster + 1.0);
            gmmSigmaSquare[i] = (double)histSettingNumberOfBins;
            gmmAlpha[i] = 1.0/(double)numberofcluster;

        }
    }
    Eigen::MatrixXd sample_probability(nonZeroBins,numberofcluster);
    Eigen::VectorXd sample_probability_sum(nonZeroBins);

    double loglik = 0.0;
    double previous_loglik = -MAX_FLOAT;
    Eigen::VectorXd mu_sum(numberofcluster);
    Eigen::VectorXd alpha_sum(numberofcluster);
    Eigen::VectorXd sigma_sum(numberofcluster);

    int iter;
    for(iter = 0; iter < MAX_ITERATION; iter++)
    {
        // clean buffer
        loglik = 0.0;

        mu_sum.setZero();
        alpha_sum.setZero();
        sigma_sum.setZero();

        for(int i = 0; i < numberofcluster ; i++)
        {
            qDebug() << gmmAlpha[i] << gmmMu[i] << gmmSigmaSquare[i];
        }


        // Expectation

        for(int ss = 0; ss < nonZeroBins; ss++)
        {
            sample_probability_sum[ss] = 0.0;
            for(int ii = 0; ii < numberofcluster; ii++)
            {
                double d = sample_values[ss] - gmmMu[ii];
                sample_probability(ss,ii) = gmmAlpha[ii]*exp(-0.5 * (d*d)/gmmSigmaSquare[ii])/sqrt(2.0*E_PI*gmmSigmaSquare[ii]);
                sample_probability_sum[ss] += sample_probability(ss,ii);
            }
            sample_probability_sum[ss] += EPSD;
            loglik += sample_weights[ss]*log(sample_probability_sum[ss]);
            for(int ii = 0; ii < numberofcluster; ii++)
            {
                double probability_weight = sample_weights[ss]*sample_probability(ss,ii)/sample_probability_sum[ss];
//                qDebug() << probability_weight;
                alpha_sum[ii] += probability_weight;
            }

//            qDebug() << sample_probability(ss,0) << sample_probability(ss,1) <<
//                        sample_probability(ss,2) << sample_probability_sum[ss];
        }

        // if converged then break
        if(loglik-previous_loglik<0.0001)
            break;
        else
            previous_loglik = loglik;
        // if not converged then updata alpha, mu and sigma
        for(int ii = 0; ii < numberofcluster; ii++)
        {
            for(int ss = 0; ss < nonZeroBins; ss++)
            {
                double probability_weight = sample_weights[ss]*sample_probability(ss,ii)/sample_probability_sum[ss];
                mu_sum[ii] += sample_values[ss]*probability_weight;
            }
            gmmMu[ii] = mu_sum[ii] / alpha_sum[ii];
        }

        for(int ii = 0; ii < numberofcluster; ii++)
        {
            for(int ss = 0; ss < nonZeroBins; ss++)
            {
                double probability_weight = sample_weights[ss]*sample_probability(ss,ii)/sample_probability_sum[ss];
                double vr = sample_values[ss] - gmmMu[ii];
                sigma_sum[ii] += vr*vr*probability_weight;
            }
            gmmSigmaSquare[ii] = sigma_sum[ii] / alpha_sum[ii] + sml;
        }

        for(int ii = 0; ii < numberofcluster; ii++)
        {
            alpha_sum[ii] += 1.0e-3;
        }
        gmmAlpha = alpha_sum/alpha_sum.sum();

    }
    if(iter >= MAX_ITERATION)
    {
        qDebug() << "EM didn't converge in 2000 iterations";
        return false;
    }
//    gmmMu +=  Eigen::VectorXd(gmmMu).setOnes()*(histSettingMinValue - 1.0);
    gmmElements.clear();
    for(int i = 0; i < numberofcluster ; i++)
    {

        //scale the value using histogram specification
        GaussianMixtureElement element;
        element.alpha = gmmAlpha[i];
        element.mu = gmmMu[i]*histSettingBinSize + histSettingMinValue;
        element.sigma = sqrt( gmmSigmaSquare[i] )*histSettingBinSize;
        qDebug() << element.alpha << element.mu << element.sigma;
        gmmElements.push_back(element);
    }
    return true;
}

void MiaHistogramAnalysisFilter::scanPeaksAndValleys(std::vector<float>& peaks, std::vector<float>& valleys, float sigma)
{
    std::vector<double> smoothed_hist;
    smoothed_hist.resize(histSettingNumberOfBins);
    smoothHistogramWithGuassionKernel(histogram, smoothed_hist, sigma);
    int len = smoothed_hist.size();
    for(int i = 1; i < len-1 ; i++)
    {
        float d_minus = smoothed_hist[i] - smoothed_hist[i-1];
        float d_plus = smoothed_hist[i+1] - smoothed_hist[i];
        if(d_minus > 0 && d_plus < 0)
        {
            peaks.push_back(i*histSettingBinSize);
        }
        else if(d_minus < 0 && d_plus > 0)
        {
            valleys.push_back(i*histSettingBinSize);
        }
    }

}

void MiaHistogramAnalysisFilter::smoothHistogramWithGuassionKernel(std::vector<double> &input, std::vector<double> &output, float sigma)
{
    float sigmaSquare = sigma*sigma;
    int kernelSize = (int)(sigmaSquare)*2 + 1;
    float* kernel = (float*)malloc(kernelSize*sizeof(float));
    int first_x = - floorf(kernelSize/2.0);

    for(int i = 0; i < kernelSize; i++)
    {
        float x = first_x + i;
        kernel[i] = exp(-0.5*x*x/sigmaSquare) / (sigma * sqrt(E_PI*2.0));
    }
    int len = input.size();
    for(int j = 0 ; j < len ; j++)
    {
        float sum = 0.0f;
        for(int i = 0 ;i < kernelSize; i++)
        {
            if(j+first_x+i >= 0 && j+first_x+i < len)
            {
                sum += input[j+first_x+i] * kernel[i];
            }
        }
        output[j] = sum;
//        qDebug() << sum;
    }
    free(kernel);
}



template <class S>
bool MiaHistogramAnalysisFilter::createHistgram(const S* inData)
{
    int* dim = inputImage->getDimSize();
    int sliceSize = dim[0]*dim[1];
    int width = dim[0];
    for(int i = 0; i< histSettingNumberOfBins; i++)
    {
        histogram[i] = 0;
    }

    for(int z = histROIBox[4]; z <= histROIBox[5]; z++)
        for(int y = histROIBox[2]; y <= histROIBox[3]; y++)
            for(int x = histROIBox[0]; x <= histROIBox[1]; x++)
            {
                int pindex = z*sliceSize + y*width + x;
                int binindex = ((float)(inData[pindex]) - histSettingMinValue)/histSettingBinSize;
                if(binindex < 0 || binindex >= histSettingNumberOfBins)
                    continue;
                histogram[binindex]++;
            }
    return true;
}

template <class T>
QList<QRectF> MiaHistogramAnalysisFilter::createConnnectedComponentsListForSlice(int slicenum, const T *inData)
{

    int* dim = inputImage->getDimSize();
    int sliceSize = dim[0]*dim[1];
    int width = dim[0], height = dim[1];

    int* tempimg= (int*)malloc(sliceSize*sizeof(int));
    memset(tempimg,0,sliceSize*sizeof(int));
    int* connectingmap=(int*)malloc(sliceSize*sizeof(int));
    memset(connectingmap,0,sliceSize*sizeof(int));
    const T* sliceimage = inData + sliceSize*slicenum;

    int labebindex = 1;
    for(int j=0; j<height; j++)
        for(int i=0; i<width; i++)
            if(*(sliceimage+j*width+i) >= histSettingMinValue &&  *(sliceimage+j*width+i) <= histSettingMaxValue)
            {
                int isconnected=0;
                for(int neighbor=0;neighbor<4;neighbor++)
                {
                    int x,y;
                    switch(neighbor)
                    {
                        case 0:
                            x=i-1;y=j-1;break;
                        case 1:
                            x=i;y=j-1;break;
                        case 2:
                            x=i+1;y=j-1;break;
                        case 3:
                            x=i-1;y=j;break;
                    }
                    if(x<0 || y<0 ||x>=width)
                        continue;
                    if(*(tempimg+y*width+x))
                    {
                        if(!isconnected)
                        {
                            *(tempimg+j*width+i)=*(tempimg+y*width+x);
                            isconnected=1;
                        }
                        else
                        {
                            if(*(tempimg+j*width+i)!=*(tempimg+y*width+x))
                            {
                                int index1=*(tempimg+j*width+i);
                                int index2=*(tempimg+y*width+x);
                                if(connectingmap[index1]==0&&connectingmap[index2]==0)
                                {

                                    connectingmap[index1]=index1;
                                    connectingmap[index2]=index1;

                                }
                                else if(connectingmap[index1]==0&&connectingmap[index2]!=0)
                                {
                                    connectingmap[index1]=connectingmap[index2];
                                }
                                else if(connectingmap[index1]!=0&&connectingmap[index2]==0)
                                {
                                    connectingmap[index2]=connectingmap[index1];
                                }
                                else
                                {

                                    index1=connectingmap[index1];
                                    index2=connectingmap[index2];
                                    //two neighbors have differen ancestors
                                    if(index1!=index2)
                                    {
                                        for(int k=1;k<labebindex;k++)
                                            if(connectingmap[k]==index2)
                                                connectingmap[k]=index1;
                                    }
                                }


                            }
                        }
                    }
                }
                if(!*(tempimg+j*width+i))
                {
                    *(tempimg+j*width+i)=labebindex;
                    connectingmap[labebindex]=labebindex;
                    labebindex++;
                }


            }

    // replace connected map with continuous number.
    int regionnum=0;
    for(int k=1;k<labebindex;k++)
    {
        if(connectingmap[k]>regionnum)
        {
            regionnum++;
            int index2=connectingmap[k];
            for(int i=k;i<labebindex;i++)
            {
                if(connectingmap[i]==index2)
                    connectingmap[i]=regionnum;

            }
        }
    }
    regionnum++;
    QVector<int> areaList;
    areaList.resize(4*regionnum);

    for(int i=0;i<regionnum;i++)
    {
        areaList[i*4]=width;
        areaList[i*4+1]=height;
        areaList[i*4+2]=0;
        areaList[i*4+3]=0;
    }

    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++)
        {
            int index1=connectingmap[tempimg[j*width+i]];
            tempimg[j*width+i]=index1;
            if(index1>0)
            {
                if(areaList[index1*4]>i)
                    areaList[index1*4]=i;
                if(areaList[index1*4+1]>j)
                    areaList[index1*4+1]=j;
                if(areaList[index1*4+2]<i)
                    areaList[index1*4+2]=i;
                if(areaList[index1*4+3]<j)
                    areaList[index1*4+3]=j;
            }
        }
    QList<QRectF> boxList;
//    qDebug() << "In slice" << slicenum;
    for(int i = 1; i < regionnum; i++)
    {
        int w = areaList[4*i+2] - areaList[4*i];
        int h = areaList[4*i+3] - areaList[4*i+1];
        QRectF rect(areaList[i*4], areaList[i*4+1], w, h);
        boxList.append(rect);
//        qDebug() << rect;
    }
    return boxList;
}

bool MiaHistogramAnalysisFilter::caculateMeanAndSTDWithIntensityRange(float lower, float upper, float& mean, float& std, float& count)
{
    mean = 0;
    std = 0;
    count  = 0;
    for(int i = 0; i < histogram.size(); i++)
    {
        float value = i*histSettingBinSize + histSettingMinValue;
        if(value > lower && value < upper)
        {
            mean += value*histogram[i];
            std += value*value*histogram[i];
            count += histogram[i];
        }
    }
    mean = mean/count;
    std = sqrt(std/count - mean*mean);
    qDebug() << "Mean and std" << mean << std << count;
    return true;
}
