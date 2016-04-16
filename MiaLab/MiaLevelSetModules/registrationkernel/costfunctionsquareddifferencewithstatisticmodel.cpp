#include "costfunctionsquareddifferencewithstatisticmodel.h"
#include "miaimagend.h"
#include "transformfunctionbase.h"
CostFunctionSquaredDifferenceWithStatisticModel::CostFunctionSquaredDifferenceWithStatisticModel()
{
    numberOfPCA = 0;
    pcaModelsWeightingFactor = 1.0;
    scaleCompensitionFactor = 1.0;
}

void CostFunctionSquaredDifferenceWithStatisticModel::setMovingImage(MiaImageND *input)
{
    throw std::runtime_error("Model based registration doesn't take moving image");
}


int CostFunctionSquaredDifferenceWithStatisticModel::getNumberOfTransformationParameters() const
{
    return transformFunction->getNumberofParameters() + numberOfPCA;
}

bool CostFunctionSquaredDifferenceWithStatisticModel::setCurrentTransformationParameters(const Eigen::VectorXf &parameters)
{
//    for(int i = 0; i < parameters.rows(); i++)
//        printf("%f\n",parameters(i));
    Eigen::VectorXf transformParameters = parameters.topRows(transformFunction->getNumberofParameters());
    if(transformFunction->setCurrentParameters(transformParameters))
    {
        if(parameters.rows()==getNumberOfTransformationParameters())
        {
            pCAWeightingFactors = parameters.bottomRows(numberOfPCA);
        }
        for(int i =0 ; i < numberOfPCA; i ++)
        {
            if(pcaModelsWeightingFactor == 0.0)
            {
                pCAWeightingFactors(i) = 0;
                continue;
            }
            if(pCAWeightingFactors(i) > 50)
            {
                pCAWeightingFactors(i) = 50;
            }
            else if(pCAWeightingFactors(i) < -50)
            {
                pCAWeightingFactors(i) = -50;
            }

        }
        //update the scaling factor as well, this factor
        //is used to make the level set distance matches
        //the model distance, otherwise the model fitting
        //maynot converge.
        scaleCompensitionFactor = 1.0f/transformFunction->getScalingFactor();
        printf("scaleCompensitionFactor is %f\n", scaleCompensitionFactor);
        updateMovingImage();
    }
    else
    {
        return false;
    }
    return true;
}

bool CostFunctionSquaredDifferenceWithStatisticModel::getCurrentTransformationParameters(Eigen::VectorXf &parameters)
{
    parameters.resize(getNumberOfTransformationParameters());
    Eigen::VectorXf transformparameters;
    transformFunction->getCurrentParameters(transformparameters);
    parameters.topRows(transformFunction->getNumberofParameters()) = transformparameters;
    parameters.bottomRows(numberOfPCA) = pCAWeightingFactors;
    return true;
}

void CostFunctionSquaredDifferenceWithStatisticModel::initializeParamerter()
{
    pCAWeightingFactors.resize(numberOfPCA);
    for(int i =0 ; i < numberOfPCA; i ++)
    {
        pCAWeightingFactors(i) = 0;

    }
    CostFunctionSquaredDifference::initializeParamerter();

}

bool CostFunctionSquaredDifferenceWithStatisticModel::isValid() const
{
    if(statisticModel.size()>0)
    {
        for(int i = 0 ; i < statisticModel.size();i++)
        {
            if(statisticModel[i]->getType() != IM_float)
                return false;
        }
        return CostFunctionSquaredDifference::isValid();
    }
    else
        return false;
}

void CostFunctionSquaredDifferenceWithStatisticModel::setStatisticModelWithInitialParameters(std::vector<MiaImageND *>& model, const Eigen::VectorXf &parameters)
{
    statisticModel = model;
    numberOfPCA = statisticModel.size()-1;
    if(movingImage)
        movingImage->release();
    movingImage = NULL;
    if(statisticModel.size()<1)
        return;
    MiaImageND* img = statisticModel.at(0);
    MiaImageND* newImage = new MiaImageND(img->getWidth(), img->getHeight(), img->getDepth(), img->getNumberOfPhases(),IM_float);
    if(newImage)
    {
        newImage->copyGeometryInfoFrom(img);
        img->castToFloat((float*)newImage->getDataPtr(),0,1.0);
        movingImage =  newImage;
        movingImage->retain();
    }
    if(parameters.rows()>0)
        setCurrentTransformationParameters(parameters);
    else
        initializeParamerter();

}

void CostFunctionSquaredDifferenceWithStatisticModel::updateMovingImage()
{
    if(numberOfPCA<1)
        return;
    float** pcaMaps;
    float * meanimage = (float*)(statisticModel[0]->getDataPtr());
    float* target = (float*)movingImage->getDataPtr();
    pcaMaps = (float**) malloc(numberOfPCA*sizeof(float*));
    for(int i = 0 ; i < numberOfPCA; i++)
    {
        pcaMaps[i] = (float*)(statisticModel[i+1]->getDataPtr());
    }
    int totalsize = movingImage->getImageSize();
#ifdef USE_OPENMP
    #pragma omp parallel for
#endif
    for(int ind = 0; ind < totalsize; ind++)
    {
        target[ind] =  meanimage[ind];

        for(int j = 0 ; j < numberOfPCA; j ++ )
        {
            target[ind] += pcaMaps[j][ind]*pcaModelsWeightingFactor*pCAWeightingFactors[j];
        }
    }
    free(pcaMaps);
}


double CostFunctionSquaredDifferenceWithStatisticModel::getValueAndTransformDerivativeAtPoint(Eigen::Vector3f &fixedpt, Eigen::Vector3f &movingpt,
                                                                                     Eigen::VectorXd &value, Eigen::MatrixXd &jacobian,
                                                                                     Eigen::VectorXd &dD, Eigen::MatrixXd &dT,
                                                                                     Eigen::VectorXd &jacob_v)
{
    float fixedvalue = fixedImage->getValueAt(fixedpt.data());
    float movingvalue = movingImage->getValueAt(movingpt.data())*scaleCompensitionFactor;
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
    int numberOfTransformParameters = transformFunction->getNumberofParameters();
    transformFunction->getDerivative(dT,fixedpt);
    jacob_v = dT*dD;

//    if(pcaModelsWeightingFactor!= 0.0)
//    {
        for(int i = 0; i < numberOfPCA; i++ )
        {
            jacob_v[i + numberOfTransformParameters] = statisticModel[i+1]->getValueAt(movingpt.data());
    //        if(i == numberOfPCA-1)
    //            std::cout << jacob_v[i + numberOfTransformParameters] << "=" << fixedvalue - movingvalue << " ";
        }
//    }
//    cout << jacob_v << endl;
    double weightFactor = 1.0;
    if(weightingImage != NULL)
    {
        weightFactor = weightingImage->getValueAt(fixedpt.data());
        if(weightFactor>=-8 && weightFactor <8 )
        {
            weightFactor = weightFactor > 0 ? 10.0 - weightFactor : 10.0 + weightFactor;
            weightFactor *= 0.5;
        }
        else
        {
            weightFactor = 1.0;
        }
        if(movingpt[2] < 78)
        {
            weightFactor *= 0.5;
        }

    }
    if(fixedpt[0] == 0 || fixedpt[1] == 0 || fixedpt[2] == 0 ||
            fixedpt[0] == imageWidth -1 || fixedpt[1] == imageHeight - 1 || fixedpt[2] == imageDepth -1 )
        return -1.0f;
//    else
//    {
//        throw std::runtime_error("weighting image is missing.");
//    }
    value += (fixedvalue - movingvalue)*jacob_v*weightFactor;
    jacobian += jacob_v*jacob_v.transpose()*weightFactor;
    return (fixedvalue - movingvalue)*(fixedvalue - movingvalue);
}
