#include "miaimageregistrationfilterwithtemplate.h"
#include "registrationkernel/costfunctionbase.h"
#include "registrationkernel/intensitymappingfunctionlinear.h"
#include "registrationkernel/gaussnewtontemplateoptimizer.h"
#include "registrationkernel/transformfunctionconstrainedrigid3d.h"
#include "registrationkernel/transformfunctionrigid3d.h"
#include "registrationkernel/costfunctionsquareddifferencewithtemplatefitting.h"

MiaImageRegistrationFilterWithTemplate::MiaImageRegistrationFilterWithTemplate(MiaNetwork *parent, int dim) :
    MiaImageRegistrationFilter(parent, dim)
{
    if(optimizer) delete optimizer;
    optimizer = (GaussNewtonOptimizer*) new GaussNewtonTemplateOptimizer(this);
    if(transformFunction) delete transformFunction;
    transformFunction = (TransformFunctionBase*) new TransformFunctionRigid3d(transformFunctionDimension);

    if(costFuction) delete costFuction;
    costFuction = new CostFunctionSquaredDifferenceWithTemplateFitting();
    costFuction->setTransformFunction(transformFunction);
    optimizer->setCostFuction(costFuction);

    intensityMappingFunction = (IntensityMappingFuctionBase*) new IntensityMappingFunctionLinear();
    costFuction->setIntensityMappingFunction(intensityMappingFunction);
}

void MiaImageRegistrationFilterWithTemplate::initializeTemplateParameter(Eigen::Quaternionf &r, Eigen::Vector3f &t,
                                                                         float scale, float A, float b)
{
    Eigen::VectorXf transform_parameters;
    Eigen::VectorXf mapping_parameters;
    transform_parameters.resize(7);
    mapping_parameters.resize(2);
//    transform_parameters[0] = 0;
//    transform_parameters[1] = 0;
//    transform_parameters[2] = 0;
//    transform_parameters[3] = -46;
//    transform_parameters[4] = -80;
//    transform_parameters[5] = -125;
//    transform_parameters[6] = 2.0;
//    mapping_parameters[0] = 2.0;
//    mapping_parameters[1] = -1.0;
    transform_parameters[0] = r.x();
    transform_parameters[1] = r.y();
    transform_parameters[2] = r.z();
    transform_parameters[3] = t[0];
    transform_parameters[4] = t[1];
    transform_parameters[5] = t[2];
    transform_parameters[6] = scale;
    mapping_parameters[0] = A;
    mapping_parameters[1] = b;
    costFuction->setCurrentTransformationParameters(transform_parameters);
    costFuction->setCurrentIntensityMappingParameters(mapping_parameters);
    ((TransformFunctionRigid3d*)transformFunction)->setRotationCenter(-t[0],-t[1],-t[2]);
    update();
}


