#ifndef MIAIMAGEREGISTRATIONFILTERWITHSTATISTICMODEL_H
#define MIAIMAGEREGISTRATIONFILTERWITHSTATISTICMODEL_H

#include "miaimageregistrationfilter.h"

class MIALEVELSETMODULESSHARED_EXPORT MiaImageRegistrationFilterWithStatisticModel : public MiaImageRegistrationFilter
{
    Q_OBJECT
public:
    explicit MiaImageRegistrationFilterWithStatisticModel(MiaNetwork *parent = 0);
    void setModelImagesAndInitialTransform(std::vector<MiaImageND*>& model, const Eigen::VectorXf &parameters);
    virtual ~MiaImageRegistrationFilterWithStatisticModel();
    void setPCAModelsWeightingFactor(float factor);
signals:
    
public slots:
protected:
    virtual bool reloadInputImage();
    std::vector<MiaImageND*> statisticModel;
    
};

#endif // MIAIMAGEREGISTRATIONFILTERWITHSTATISTICMODEL_H
