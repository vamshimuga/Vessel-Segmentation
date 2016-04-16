#include "gaussnewtontemplateoptimizer.h"
#include "costfunctionbase.h"
#include <iostream>
GaussNewtonTemplateOptimizer::GaussNewtonTemplateOptimizer(QObject *parent)
    : GaussNewtonOptimizer(parent)
{
}

void GaussNewtonTemplateOptimizer::run()
{
    if(!isValid())
    {
        emit optimizationRunIntoError();
        return;
    }
    int its;
    double previous_transform_cost = -1.0;
    Eigen::MatrixXd previous_transform_jacobin;
    Eigen::VectorXd previous_transform_deltaY;
    Eigen::VectorXf previous_transform_parameters;

    double marquardt_factor = 0;
    const double damping_factor = 50.0;

    for( its = 0; its < maxIteration; its++)
    {
        costFuction->getCurrentTransformationParameters( currentTransformParameters );
        double cost = costFuction->getValueAndDerivativeForTransformFunction( deltaY, mJacobin );
        if(previous_transform_cost < 0)
        {
            previous_transform_cost = cost;
            previous_transform_jacobin = mJacobin;
            previous_transform_deltaY = deltaY;
            previous_transform_parameters = currentTransformParameters;
        }
        else if(cost >= previous_transform_cost)
        {
            if(marquardt_factor == 0)
            {
                marquardt_factor = 1.0;
            }
            else
            {
                marquardt_factor *= damping_factor;
            }
            deltaY = previous_transform_deltaY;
            mJacobin = previous_transform_jacobin;
            currentTransformParameters = previous_transform_parameters;
        }
        else
        {
            previous_transform_cost = cost;
            previous_transform_jacobin = mJacobin;
            previous_transform_deltaY = deltaY;
            previous_transform_parameters = currentTransformParameters;
            marquardt_factor /= damping_factor;
        }
        std::cout << "cost:" << cost << "marquardt factor:" << marquardt_factor<< "\n" <<std::endl;
        Eigen::ColPivHouseholderQR<Eigen::MatrixXd> dec(mJacobin + marquardt_factor*Eigen::MatrixXd::Identity(mJacobin.rows(),mJacobin.cols()));
//        Eigen::ColPivHouseholderQR<Eigen::MatrixXf> dec(mJacobin);
        deltaTransformParameters = dec.solve(deltaY);

        if(deltaTransformParameters.norm() < stopCriteriaSSE)
        {
            qDebug() << "delta parameter is too small";
            break;
        }
#ifdef QT_DEBUG
        std::cout << "current parameters" << currentTransformParameters << "\n"<< std::endl;
#endif
        currentTransformParameters += deltaTransformParameters.cast<float>();
#ifdef QT_DEBUG
        std::cout <<"jacob:"<< mJacobin << "\n" <<std::endl;
        std::cout <<"del_y:"<< deltaY << "\n" << std::endl;
        std::cout <<"solve:"<< deltaTransformParameters << "\n"<< std::endl;
        std::cout << "new parameters" << currentTransformParameters << "\n"<< std::endl;
#endif

        costFuction->setCurrentTransformationParameters(currentTransformParameters);

//        Eigen::VectorXf currentMappingParameters;
//        Eigen::MatrixXd mappingJacobin;
//        Eigen::VectorXd mappingDeltaY;
//        Eigen::VectorXd deltaMappingParameters;
//        costFuction->getCurrentIntensityMappingParameters(currentMappingParameters);
//        costFuction->getValueAndDerivativeForMappingFunction(mappingDeltaY,mappingJacobin);
//        Eigen::ColPivHouseholderQR<Eigen::MatrixXd> dec2(mappingJacobin);
//        deltaMappingParameters = dec2.solve(mappingDeltaY);
//        currentMappingParameters += deltaMappingParameters.cast<float>();
////        costFuction->setCurrentIntensityMappingParameters(currentMappingParameters);

//        std::cout <<"mapping jacob:"<< mappingJacobin << "\n" <<std::endl;
//        std::cout <<"mapping del_y:"<< mappingDeltaY << "\n" << std::endl;
//        std::cout <<"mapping solve:"<< deltaMappingParameters << "\n"<< std::endl;
//        std::cout << "mapping new parameters" << currentMappingParameters << "\n"<< std::endl;

//        if(its!=0 && its%updateInterval == 0)
//            emit parameterUpdated();
    }
//    qDebug() << "Total iterations: " << its;
    double cost = costFuction->getValueAndDerivativeForTransformFunction( deltaY, mJacobin );
    if(cost > previous_transform_cost)
        costFuction->setCurrentTransformationParameters(previous_transform_parameters);
    emit optimizationCompleted();
}
