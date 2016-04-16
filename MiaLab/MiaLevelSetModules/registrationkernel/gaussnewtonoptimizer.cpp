#include "gaussnewtonoptimizer.h"
#include "costfunctionbase.h"
#include <iostream>
GaussNewtonOptimizer::GaussNewtonOptimizer(QObject *parent) :
    QThread(parent)
{
    costFuction = NULL;
    updateInterval = 10;
    maxIteration = 300;
    stopCriteriaSSE = 0.01f;
    connect(this,SIGNAL(optimizationCompleted()),parent,SLOT(forwardRegistrationComplete()));
    connect(this, SIGNAL(parameterUpdated()),parent,SLOT(forwardReigstrationUpdate()));
    connect(this, SIGNAL(optimizationRunIntoError()),parent,SLOT(forwardRegistrationComplete()));
    setPriority(QThread::HighestPriority);
}

void GaussNewtonOptimizer::setCostFuction(CostFunctionBase *func)
{
    costFuction = func;
}


void GaussNewtonOptimizer::run()
{
    if(!isValid())
    {
        emit optimizationRunIntoError();
        return;
    }
    int its;
    double previous_cost = -1.0;
    Eigen::MatrixXd previous_jacobin;
    Eigen::VectorXd previous_deltaY;
    Eigen::VectorXf previous_parameters;
    double marquardt_factor = 0;
    const double damping_factor = 50.0;

    for( its = 0; its < maxIteration; its++)
    {
        costFuction->getCurrentTransformationParameters( currentTransformParameters );
        double cost = costFuction->getValueAndDerivativeForTransformFunction( deltaY, mJacobin );
        if(previous_cost < 0)
        {
            previous_cost = cost;
            previous_jacobin = mJacobin;
            previous_deltaY = deltaY;
            previous_parameters = currentTransformParameters;
        }
        else if(cost >= previous_cost)
        {
            if(marquardt_factor == 0)
            {
                marquardt_factor = 1.0;
            }
            else
            {
                marquardt_factor *= damping_factor;
            }
            deltaY = previous_deltaY;
            mJacobin = previous_jacobin;
            currentTransformParameters = previous_parameters;
        }
        else
        {
            previous_cost = cost;
            previous_jacobin = mJacobin;
            previous_deltaY = deltaY;
            previous_parameters = currentTransformParameters;
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
//        if(its!=0 && its%updateInterval == 0)
//            emit parameterUpdated();
    }
//    qDebug() << "Total iterations: " << its;
    double cost = costFuction->getValueAndDerivativeForTransformFunction( deltaY, mJacobin );
    if(cost > previous_cost)
        costFuction->setCurrentTransformationParameters(previous_parameters);
    emit optimizationCompleted();

}

void GaussNewtonOptimizer::abort()
{
}

void GaussNewtonOptimizer::resume()
{
}

void GaussNewtonOptimizer::restart()
{
}

bool GaussNewtonOptimizer::isValid() const
{
    if(costFuction && costFuction->isValid())
    {
        return true;
    }

    return false;
}
