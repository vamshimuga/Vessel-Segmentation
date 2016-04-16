#ifndef MIAREGIONSTATISTICS_H
#define MIAREGIONSTATISTICS_H
#include "MiaCoreModules/miamodule.h"
#define MAXTYPES 64
class MIACOMMONMODULESSHARED_EXPORT MiaRegionStatistics : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaRegionStatistics(MiaNetwork *parent = 0);
    virtual ~MiaRegionStatistics();
    void setMaskImageInputModule( MiaModule* input);
    void exportSamplesToPlainText(QString filename);
    void addRegion(QString name, float lower, float upper);
    float getVolumeForRegion(int index){Q_ASSERT(index < numberOfRegions);return volume[index];}
    QString getSummaryString() {if(needUpdate) update(); return summaryStr;}

signals:
    void requestUpdateMaskImage(MiaImageND** img);

public slots:
    void maskImageReset();
protected:
    virtual bool runOperation();
    template <class A, class B>
    bool runOperation(A* inData, B* regiondata);
    template <class A, class B>
    void exportSamplesToPlainText(QString filename,A* inData, B* regiondata);
    virtual bool reloadInputImage();
    virtual bool checkIfNeedReAllocOutputImage() {return false;}
    void dispatchParameters(const QMap <QString, QVariant> &parameters);
    MiaImageND* maskImage;
    MiaModule* maskInputModule;
    float mean[MAXTYPES];
    float standard_deviation[MAXTYPES];
    float max[MAXTYPES],min[MAXTYPES];
    int number_voxel[MAXTYPES];
    float volume[MAXTYPES];
    float region_lower_threshold[MAXTYPES];
    float region_upper_threshold[MAXTYPES];
    QString summaryStr;
    QStringList regionNameList;
    int numberOfRegions;
};

#endif // MIAREGIONSTATISTICS_H
