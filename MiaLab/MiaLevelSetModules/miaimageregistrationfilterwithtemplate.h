#ifndef MIAIMAGEREGISTRATIONFILTERWITHTEMPLATE_H
#define MIAIMAGEREGISTRATIONFILTERWITHTEMPLATE_H
#include "miaimageregistrationfilter.h"

class IntensityMappingFuctionBase;
class MIALEVELSETMODULESSHARED_EXPORT MiaImageRegistrationFilterWithTemplate : public MiaImageRegistrationFilter
{
    Q_OBJECT
public:
    explicit MiaImageRegistrationFilterWithTemplate(MiaNetwork *parent = 0, int dim = 3);
    void initializeTemplateParameter(Eigen::Quaternionf& r, Eigen::Vector3f& t, float scale, float A, float b);
    
signals:
    
public slots:
protected:
    IntensityMappingFuctionBase* intensityMappingFunction;

    
};

#endif // MIAIMAGEREGISTRATIONFILTERWITHTEMPLATE_H
