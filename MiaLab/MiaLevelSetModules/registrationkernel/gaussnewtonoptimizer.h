#ifndef GAUSSNEWTONOPTIMIZER_H
#define GAUSSNEWTONOPTIMIZER_H
#include <QtGui>
#include <Eigen/Dense>
class CostFunctionBase;
class GaussNewtonOptimizer : public QThread
{
    Q_OBJECT
public:
    explicit GaussNewtonOptimizer(QObject *parent = 0);
    void setCostFuction(CostFunctionBase* func);
    virtual void run();
    virtual void abort();
    virtual void resume();
    virtual void restart();
    virtual bool isValid() const;
    void setMaximumIteration(int limit){maxIteration = limit;}
signals:
    void parameterUpdated();
    void optimizationCompleted();
    void optimizationRunIntoError();
public slots:

protected:
    Eigen::MatrixXd mJacobin;
    Eigen::VectorXd deltaY;
    Eigen::VectorXd deltaTransformParameters;
    Eigen::VectorXf currentTransformParameters;
    CostFunctionBase* costFuction;
    int updateInterval;
    int maxIteration;
    float stopCriteriaSSE;

};

#endif // GAUSSNEWTONOPTIMIZER_H
