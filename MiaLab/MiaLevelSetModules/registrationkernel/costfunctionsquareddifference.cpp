#include "costfunctionsquareddifference.h"
#include "transformfunctionbase.h"
#include "intensitymappingfuctionbase.h"
#include "miaimagend.h"
#include <omp.h>
#include <QtGui>
#include <iostream>

CostFunctionSquaredDifference::CostFunctionSquaredDifference()
{
}


double CostFunctionSquaredDifference::getValueAndDerivativeForTransformFunction(Eigen::VectorXd &value, Eigen::MatrixXd &jacobian)
{
    int num_para = getNumberOfTransformationParameters();

    value.resize(num_para);
    value.setZero();
    jacobian.resize(num_para,num_para);
    jacobian.setZero();
    double sum = 0;
    int* movimg_size = movingImage->getDimSize();
    int* fixed_size = fixedImage->getDimSize();
    int count_samples = 0;

#ifdef USE_OPENMP
    Eigen::VectorXd threaded_value[MAXNUMTHREAD];
    Eigen::MatrixXd threaded_jacobin[MAXNUMTHREAD];
    Eigen::VectorXd dD[MAXNUMTHREAD];
    Eigen::MatrixXd dT[MAXNUMTHREAD];
    Eigen::VectorXd jacob_v[MAXNUMTHREAD];
    double threaded_sum[MAXNUMTHREAD];
    int threaded_sample_count[MAXNUMTHREAD];
    const int nthreads = omp_get_max_threads();

    for(int t = 0 ; t < nthreads; t++)
    {
        threaded_value[t].resize(num_para);
        threaded_value[t].setZero();
        threaded_jacobin[t].resize(num_para,num_para);
        threaded_jacobin[t].setZero();
        dD[t].resize(fixedImage->getDimension());
        dT[t].resize(num_para,fixedImage->getDimension());
        jacob_v[t].resize(num_para);
        jacob_v[t].setZero();
        threaded_sum[t] = 0;
        threaded_sample_count[t] = 0;
    }
    reinitialSamples();
    #pragma omp parallel for
    for(int i = 0; i < sampleSize; i++)
    {
        int threadindex = omp_get_thread_num();
        Eigen::Vector3f fixedpt;
        Eigen::Vector3f movingpt;
        if(getSampleWithIndex(fixedpt,i))
        {

            movingpt = transformFunction->fixedImageVoxelToMovingImageVoxel(fixedpt);
            if(movingpt[0] < 0 ||movingpt[0] > movimg_size[0] - 1 || movingpt[1] < 0 ||movingpt[1] > movimg_size[1] - 1 ||movingpt[2] < 0 ||movingpt[2] > movimg_size[2] - 1 )
                continue;
            float sqr_cost = getValueAndTransformDerivativeAtPoint(fixedpt,movingpt,
                                                          threaded_value[threadindex],threaded_jacobin[threadindex],
                                                          dD[threadindex],dT[threadindex],
                                                          jacob_v[threadindex]);

            if(sqr_cost >= 0)
            {
                threaded_sample_count[threadindex] ++;
                threaded_sum[threadindex] += sqr_cost;
            }

        }

    }
    for(int t = 0 ; t < nthreads; t++)
    {
        value += threaded_value[t];
        jacobian += threaded_jacobin[t];
        sum += threaded_sum[t];
        count_samples += threaded_sample_count[t];
    }
#else
    Eigen::Vector3f fixedpt;
    Eigen::Vector3f movingpt;
    Eigen::VectorXd jacob_v(num_para);
    Eigen::VectorXd dD;
    Eigen::MatrixXd dT;

    jacob_v.setZero();
    dD.resize(fixedImage->getDimension());
    dT.resize(num_para,fixedImage->getDimension());

    reinitialSamples();
    while(getNextSamplePairs(fixedpt,movingpt,0))
    {
        if(fixedpt[0] < 0 ||fixedpt[0] > fixed_size[0] - 1 || fixedpt[1] < 0 ||fixedpt[1] > fixed_size[1] - 1 ||fixedpt[2] < 0 ||fixedpt[2] > fixed_size[2] - 1 )
            continue;
        if(movingpt[0] < 0 ||movingpt[0] > movimg_size[0] - 1 || movingpt[1] < 0 ||movingpt[1] > movimg_size[1] - 1 ||movingpt[2] < 0 ||movingpt[2] > movimg_size[2] - 1 )
            continue;
        float sqr_cost = getValueAndTransformDerivativeAtPoint(fixedpt,movingpt,
                                                      value,jacobian,
                                                      dD,dT,
                                                      jacob_v);
        if(sqr_cost >= 0)
        {
            count_samples++;
            sum += sqr_cost;
        }
//        std::cout << movingpt[0] << "," << movingpt[1] << "," << movingpt[2] << ":"<< sqr_cost <<std::endl;

    }
#endif
//    std::cout <<"sum:"<< sum << " count:" << count_samples << "\n" <<std::endl;
    return sum/count_samples;

}


double CostFunctionSquaredDifference::getValueAndTransformDerivativeAtPoint(Eigen::Vector3f &fixedpt, Eigen::Vector3f &movingpt,
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
    Eigen::Vector3f imageGradient;
    movingImage->getGradientAt(movingpt.data(),imageGradient.data());
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
