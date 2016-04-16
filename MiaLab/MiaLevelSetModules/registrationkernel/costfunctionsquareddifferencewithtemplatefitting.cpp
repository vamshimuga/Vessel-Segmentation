#include "costfunctionsquareddifferencewithtemplatefitting.h"
#include "transformfunctionbase.h"
#include "intensitymappingfuctionbase.h"
#include "miaimagend.h"
#include <omp.h>
#include <QtGui>
#include <iostream>

CostFunctionSquaredDifferenceWithTemplateFitting::CostFunctionSquaredDifferenceWithTemplateFitting()
{
}

double CostFunctionSquaredDifferenceWithTemplateFitting::getValueAndDerivativeForMappingFunction(Eigen::VectorXd &value, Eigen::MatrixXd &jacobian)
{
    int num_para = getNumberOfMappingParameters();
    if(intensityMappingFucntion == NULL)
    {
        throw std::runtime_error("mappint function is empty");
    }

    value.resize(num_para);
    value.setZero();
    jacobian.resize(num_para,num_para);
    jacobian.setZero();
    double sum = 0;
    int* movimg_size = movingImage->getDimSize();
    int* fixed_size = fixedImage->getDimSize();
    int count_samples = 0;
    Eigen::Vector3f fixedpt;
    Eigen::Vector3f movingpt;
    Eigen::VectorXd jacob_v(num_para);

    jacob_v.setZero();

    reinitialSamples();
    while(getNextSamplePairs(fixedpt,movingpt,0))
    {
        if(fixedpt[0] < 0 ||fixedpt[0] > fixed_size[0] - 1 || fixedpt[1] < 0 ||fixedpt[1] > fixed_size[1] - 1 ||fixedpt[2] < 0 ||fixedpt[2] > fixed_size[2] - 1 )
            continue;
        if(movingpt[0] < 0 ||movingpt[0] > movimg_size[0] - 1 || movingpt[1] < 0 ||movingpt[1] > movimg_size[1] - 1 ||movingpt[2] < 0 ||movingpt[2] > movimg_size[2] - 1 )
            continue;
        float sqr_cost = getValueAndMappingDerivativeAtPoint(fixedpt,movingpt,
                                                      value,jacobian,
                                                      jacob_v);
        if(sqr_cost >= 0)
        {
            count_samples++;
            sum += sqr_cost;
        }

    }
    return sum/count_samples;
}
double CostFunctionSquaredDifferenceWithTemplateFitting::getValueAndTransformDerivativeAtPoint(Eigen::Vector3f &fixedpt, Eigen::Vector3f &movingpt,
                                                                   Eigen::VectorXd &value, Eigen::MatrixXd &jacobian,
                                                                   Eigen::VectorXd &dD, Eigen::MatrixXd &dT,
                                                                   Eigen::VectorXd &jacob_v)
{
    float fixedvalue = fixedImage->getValueAt(fixedpt.data());
    float movingvalue = movingImage->getValueAt(movingpt.data());
    if(fixedvalue == backgroundValue || movingvalue == backgroundValue)
    {
        return -1.0;
    }
    if(intensityMappingFucntion)
    {
        movingvalue = intensityMappingFucntion->getMappedIntensityValue(movingvalue);
    }
    Eigen::Vector3f imageGradient;
    movingImage->getGradientAt(movingpt.data(),imageGradient.data());
    if(intensityMappingFucntion)
    {
        imageGradient[0] = intensityMappingFucntion->getMappedIntensityValue(imageGradient[0]);
        imageGradient[1] = intensityMappingFucntion->getMappedIntensityValue(imageGradient[1]);
        imageGradient[2] = intensityMappingFucntion->getMappedIntensityValue(imageGradient[2]);
    }
    if(fixedImage->getDimension() == 2)
    {
        dD[0] = imageGradient[0];
        dD[1]  = imageGradient[1];
    }
    else
    {
        dD = imageGradient.cast<double>();
    }
    transformFunction->getDerivative(dT,fixedpt);
    jacob_v = dT*dD;
    value += (fixedvalue - movingvalue)*jacob_v;
    jacobian += jacob_v*jacob_v.transpose() ;
//    std::cout << "fixed: " << fixedpt << "value: " << fixedvalue;
//    std::cout << "moving: " << movingpt << "value: " << movingvalue;
//    std::cout << "Jacobian: " << jacob_v;
    return (fixedvalue - movingvalue)*(fixedvalue - movingvalue);

}
double CostFunctionSquaredDifferenceWithTemplateFitting::getValueAndMappingDerivativeAtPoint(Eigen::Vector3f &fixedpt, Eigen::Vector3f &movingpt, Eigen::VectorXd &value, Eigen::MatrixXd &jacobian, Eigen::VectorXd &jacob_v)
{
    float fixedvalue = fixedImage->getValueAt(fixedpt.data());
    float movingvalue = movingImage->getValueAt(movingpt.data());
    if(fixedvalue == backgroundValue || movingvalue == backgroundValue)
    {
        return -1.0;
    }
    float mappedvalue = intensityMappingFucntion->getMappedIntensityValue(movingvalue);

    intensityMappingFucntion->getDerivative(jacob_v,movingvalue);
    value += (fixedvalue - mappedvalue)*jacob_v;
    jacobian += jacob_v*jacob_v.transpose() ;
//    std::cout << "fixed: " << fixedpt << "value: " << fixedvalue;
//    std::cout << "moving: " << movingpt << "value: " << movingvalue;
//    std::cout << "Jacobian: " << jacob_v;
    return (fixedvalue - mappedvalue)*(fixedvalue - mappedvalue);
}
