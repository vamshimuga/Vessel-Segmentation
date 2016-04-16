#include "miaimageregistrationfilterwithstatisticmodel.h"
#include "registrationkernel/costfunctionsquareddifferencewithstatisticmodel.h"
#include "registrationkernel/gaussnewtonoptimizer.h"
#include "registrationkernel/transformfunctionrigid3d.h"

MiaImageRegistrationFilterWithStatisticModel::MiaImageRegistrationFilterWithStatisticModel(MiaNetwork *parent) :
    MiaImageRegistrationFilter(parent)
{
    moduleName = "model registration filter";
    costFuction = new CostFunctionSquaredDifferenceWithStatisticModel();
    optimizer->setCostFuction(costFuction);
    costFuction->setTransformFunction(transformFunction);
}

void MiaImageRegistrationFilterWithStatisticModel::setModelImagesAndInitialTransform(std::vector<MiaImageND *> &model, const Eigen::VectorXf& parameters)
{
    if(statisticModel.size()>0)
    {
        for(int i = 0; i < statisticModel.size();i++)
        {
            statisticModel[i]->release();
        }
    }
    statisticModel.clear();
    statisticModel = model;
    for(int i = 0; i < statisticModel.size();i++)
    {
        statisticModel[i]->retain();
    }
    CostFunctionSquaredDifferenceWithStatisticModel* modelCostFunction = (CostFunctionSquaredDifferenceWithStatisticModel*)costFuction;
    modelCostFunction->setStatisticModelWithInitialParameters(statisticModel, parameters);
    if(parameters.rows()>0)
        specifiedInitialParameters = parameters;
    needUpdate = true;
}


MiaImageRegistrationFilterWithStatisticModel::~MiaImageRegistrationFilterWithStatisticModel()
{
    for(int i = 0; i < statisticModel.size();i++)
    {
        statisticModel[i]->release();
    }
}

void MiaImageRegistrationFilterWithStatisticModel::setPCAModelsWeightingFactor(float factor)
{
    CostFunctionSquaredDifferenceWithStatisticModel* staticmodel = (CostFunctionSquaredDifferenceWithStatisticModel*)costFuction;
    if(staticmodel->getPCAModelWeightingFactor() != factor)
    {
        staticmodel->setPCAModelWeightingFactor(factor);
        if(factor == 0)
            update();
    }
}

bool MiaImageRegistrationFilterWithStatisticModel::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        if(statisticModel.size()<=0)
        {
            qDebug()<<"Statistic Model was not set";
            return false;
        }
        //get weighting image
        MiaImageND* tempimg = NULL;
        emit requestUpdateWeightingImage(&tempimg);
        if(tempimg != NULL)
        {
            if(weightingImage != tempimg)
            {
                if(weightingImage)
                    weightingImage->release();
                weightingImage = tempimg;
                weightingImage->retain();
            }
        }
        else
        {
            qDebug()<<"No Weighting image set";
        }
        costFuction->setFixedImage(inputImage);
        if(movingImage)
            costFuction->setMovingImage(movingImage);
        if(weightingImage)
            costFuction->setWeightingImage(weightingImage);

        if(costFuction->getSampleStrategy()==MIA_Sample_Given)
        {
            std::vector<int> pts;
            requestUpdateKeyPoints(pts);
            costFuction->setSampleStrategy(MIA_Sample_Given,pts.size(),&pts);

        }
    }
    else
        return false;
    return true;
}
