#ifndef COSTFUNCTIONSQUAREDDIFFERENCEWITHSTATISTICMODEL_H
#define COSTFUNCTIONSQUAREDDIFFERENCEWITHSTATISTICMODEL_H
#include "costfunctionsquareddifference.h"

class CostFunctionSquaredDifferenceWithStatisticModel:public CostFunctionSquaredDifference
{
public:
    CostFunctionSquaredDifferenceWithStatisticModel();
    virtual void setMovingImage(MiaImageND* input);// not allowed to set Moving Image, Use Set Model instead
    virtual int getNumberOfTransformationParameters()const;
    virtual bool setCurrentTransformationParameters(const Eigen::VectorXf& parameters);
    virtual bool getCurrentTransformationParameters(Eigen::VectorXf& parameters);
    virtual void initializeParamerter();
    virtual bool isValid() const;
    void setStatisticModelWithInitialParameters(std::vector<MiaImageND *>& model, const Eigen::VectorXf &parameters);
    void setPCAModelWeightingFactor(float factor)
    {
        if(pcaModelsWeightingFactor != factor)
        {
            pcaModelsWeightingFactor = factor;
            if(pcaModelsWeightingFactor == 0.0)
            {
                for(int i =0 ; i < numberOfPCA; i ++)
                    pCAWeightingFactors(i) = 0;
                updateMovingImage();
            }
        }
    }
    float getPCAModelWeightingFactor(){return pcaModelsWeightingFactor;}
protected:
    void updateMovingImage();
    std::vector<MiaImageND*> statisticModel;
    Eigen::VectorXf pCAWeightingFactors;
    int numberOfPCA;
    float pcaModelsWeightingFactor;
    float scaleCompensitionFactor;

    virtual double getValueAndTransformDerivativeAtPoint(Eigen::Vector3f& fixedpt, Eigen::Vector3f& movingpt,
                                                Eigen::VectorXd& value, Eigen::MatrixXd& jacobian,
                                                Eigen::VectorXd& dD, Eigen::MatrixXd& dT,
                                                Eigen::VectorXd& jacob_v);
};

#endif // COSTFUNCTIONSQUAREDDIFFERENCEWITHSTATISTICMODEL_H
