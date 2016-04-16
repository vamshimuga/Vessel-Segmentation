#include "miaimageregistrationfilter.h"
#include "registrationkernel/costfunctionbase.h"
#include "registrationkernel/costfunctionsquareddifference.h"
#include "registrationkernel/transformfunctionbase.h"
#include "registrationkernel/transformfunctiontraslation.h"
#include "registrationkernel/transformfunctionrigid3d.h"
#include "registrationkernel/transformfunctionconstrainedrigid3d.h"
#include "registrationkernel/gaussnewtonoptimizer.h"
#include "registrationkernel/transformfunctionaffine.h"
#include "registrationkernel/transformfunctionfixedcenterrigid3d.h"
#include <QMessageBox>
MiaImageRegistrationFilter::MiaImageRegistrationFilter(MiaNetwork *parent, int dim) :
    MiaModule(parent)
{
    moduleName = "image registration filter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenOutputRequested = false;
    autoUpdateWhenParameterChanged = false;
    movingImage = NULL;
    weightingImage = NULL;
    movingImageInputModule = NULL;
    weightingImagingInputModule = NULL;
    operateInSitu = false;
    optimizer = new GaussNewtonOptimizer(this);
    currentTransformationMethod = MIA_Transform_Affine;
    transformFunctionDimension = dim;
    transformFunction = (TransformFunctionBase*) new TransformFunctionAffine(transformFunctionDimension);
    costFuction = new CostFunctionSquaredDifference();
    optimizer->setCostFuction(costFuction);
    costFuction->setTransformFunction(transformFunction);
    backgroundValue = 0;
}

MiaImageRegistrationFilter::~MiaImageRegistrationFilter()
{
    if(optimizer)
        delete optimizer;
    if(transformFunction)
        delete transformFunction;
    if(costFuction)
        delete costFuction;
}

void MiaImageRegistrationFilter::setSampleMethodToKeyPoints()
{
    std::vector<int> pts;
    costFuction->setSampleStrategy(MIA_Sample_Given,pts.size(),&pts);
}

void MiaImageRegistrationFilter::setSampleMethodToRandomPoints(int size)
{
    costFuction->setSampleStrategy(MIA_Sample_Random,size);
}

void MiaImageRegistrationFilter::setSampleMethodToAllPoints()
{
    costFuction->setSampleStrategy(MIA_Sample_All);
}

void MiaImageRegistrationFilter::setSampleMethodToDownSample(int interval)
{
    costFuction->setSampleStrategy(MIA_Sample_DownSampled,interval);
}

void MiaImageRegistrationFilter::setSampleMethodToUsingMovingImage()
{
    costFuction->setSampleStrategy(MIA_Sample_From_MovingImage);
}

void MiaImageRegistrationFilter::setBackGroundValue(float value)
{
    backgroundValue = value;
    costFuction->setBackgroundValue(value);
}

void MiaImageRegistrationFilter::saveCurrentTransformToFile(QDataStream& out)
{
    Eigen::VectorXf currentParameters;
    costFuction->getCurrentTransformationParameters(currentParameters);
    int num = currentParameters.rows();
    qDebug() << "Saving registration parameters" << num;
    out << num;
    for(int i = 0 ;i < num ; i++)
    {
        out << currentParameters(i);
        qDebug() << currentParameters(i);
    }
}

void MiaImageRegistrationFilter::parseTransformFile(QDataStream &in, Eigen::VectorXf &parameters)
{
    int num;
    in >> num;
    parameters.resize(num);
    qDebug() << "Loading registration parameters" << num;
    for(int i = 0 ;i < num ; i++)
    {
        in >> parameters(i);
        qDebug() << parameters(i);
    }

}

void MiaImageRegistrationFilter::loadTransformFromFile(QDataStream &in)
{
    parseTransformFile(in,specifiedInitialParameters);
    if(costFuction->setCurrentTransformationParameters(specifiedInitialParameters))
    {
        update();
    }
    else
    {
        qDebug() << "Fail to load transform matrix";
    }
}

Eigen::VectorXf MiaImageRegistrationFilter::getCurrentTransform()
{
    Eigen::VectorXf currentParameters;
    costFuction->getCurrentTransformationParameters(currentParameters);
    return currentParameters;
}

void MiaImageRegistrationFilter::setCurrentTransformParameter(const Eigen::VectorXf& transform)
{
    specifiedInitialParameters = transform.topRows(transformFunction->getNumberofParameters());
    if(costFuction->setCurrentTransformationParameters(specifiedInitialParameters))
    {
        update();
    }
    else
    {
        qDebug() << "Fail to set transform matrix";
    }
}

void MiaImageRegistrationFilter::setCurrentTransformParameter(const QVector<float> &parameters)
{
    Eigen::VectorXf transform;
    transform.resize(parameters.size());
    for(int i = 0 ; i < parameters.size(); i++)
        transform[i] = parameters[i];
    setCurrentTransformParameter(transform);

}
void MiaImageRegistrationFilter::setRotationCenter(MiaPoint4D rotationcenter)
{
    transformFunction->setRotationCenter(rotationcenter.pos[0],rotationcenter.pos[1],rotationcenter.pos[2]);
}

void MiaImageRegistrationFilter::setFixedTransformation(MiaPoint4D translation)
{
    transformFunction->setFixedTranslation(translation.pos[0], translation.pos[1], translation.pos[2]);
}

void MiaImageRegistrationFilter::invertTransform()
{
    transformFunction->invertTransform();
    update();
}

void MiaImageRegistrationFilter::setTranformationMethod(TransformationMethod method)
{
    if(currentTransformationMethod == method)
    {
        return;
    }
    else
    {    if(transformFunction) delete transformFunction;
        switch (method) {
        case MIA_Transform_Affine:
            transformFunction = (TransformFunctionBase*) new TransformFunctionAffine(transformFunctionDimension);
            break;
        case MIA_Transform_Rigid3D:
            transformFunction = (TransformFunctionBase*) new TransformFunctionRigid3d(transformFunctionDimension);
            break;
        case MIA_Transform_ConstrainedRigid3D:
             transformFunction = (TransformFunctionBase*) new TransformFunctionConstrainedRigid3D(transformFunctionDimension);
             break;
        case MIA_Transform_FixedCenteredRigid3D:
            transformFunction = (TransformFunctionBase*) new TransformFunctionFixedCenterRigid3D(transformFunctionDimension);
            break;
        default:
            throw std::runtime_error("unknown tranformation function");
            break;
        }
        costFuction->setTransformFunction(transformFunction);
    }
}

bool MiaImageRegistrationFilter::runOperation()
{
    if(!outputImage || !inputImage)
        return false;
    qDebug()<<"Update Moiving image";
    int totalSize = outputImage->getImageSize();
    //because the moving image maybe changed like in the model based registration case,
    //we ask the cost function for the updated one.
    MiaImageND* updatedMovingImage = costFuction->getMovingImage();
    if(!updatedMovingImage)
    {
        qDebug()<<"Fail to get Moiving image";
        return false;
    }
    updatedMovingImage->setOutBoundaryValue(backgroundValue);
#pragma omp parallel for
    for(int i = 0; i < totalSize; i ++)
    {
        Eigen::Vector3f fixed_coor;
        outputImage->convert1DIndexTo3DVoxel(i,fixed_coor.data());
        Eigen::Vector3f moving_coor = transformFunction->fixedImageVoxelToMovingImageVoxel(fixed_coor);
        float value = updatedMovingImage->getValueAt(moving_coor.data());
        outputImage->setValueAt(fixed_coor.data(),value);
    }
    return true;
}

bool MiaImageRegistrationFilter::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        MiaImageND* tempimg = NULL;
        emit requestUpdateMovingImage(&tempimg);
        if(!tempimg)
        {
            qDebug()<<"reloadInputImage fail to get Moiving image";
            return false;
        }
        if(movingImage && tempimg == movingImage)
        {
            //we do nothing so needReAllocOutputImage will not be changed
//            needReAllocOutputImage = false;
        }
        else
        {
            if(movingImage)
                movingImage->release();
            movingImage = tempimg;
            movingImage->retain();
        }
        tempimg = NULL;
        //get weighting image
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

bool MiaImageRegistrationFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    if(operateInSitu)
    {
        throw std::runtime_error("MiaImageRegistrationFilter doesn't support operationInSitu");
    }
    outputImage = creatEmptyImageFrom( inputImage, inputImage->getType());
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;

    // if the user has not specified a initial parameter, intialize it
    if(specifiedInitialParameters.size() < 1)
    {
        costFuction->initializeParamerter();
    }

    return true;
}
void MiaImageRegistrationFilter::setMovingImageInputModule( MiaModule* input)
{
    if(movingImageInputModule)
    {
        movingImageInputModule->disconnect(this);
        disconnect(movingImageInputModule);

    }

    connect(this, SIGNAL(requestUpdateMovingImage(MiaImageND**)), input, SLOT(getOutputImage(MiaImageND**)));
    connect(input, SIGNAL(outputUpdated()), this, SLOT(inputUpdated()));

    needUpdate = true;
    update();
}

void MiaImageRegistrationFilter::setWeightingImageInputModule( MiaModule* input)
{
    if(weightingImagingInputModule)
    {
        weightingImagingInputModule->disconnect(this);
        disconnect(weightingImagingInputModule);

    }

    connect(this, SIGNAL(requestUpdateWeightingImage(MiaImageND**)), input, SLOT(getOutputImage(MiaImageND**)));
    connect(input, SIGNAL(outputUpdated()), this, SLOT(inputUpdated()));

    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }
}


void MiaImageRegistrationFilter::forwardReigstrationUpdate()
{
    update();
    emit registrationUpdate();
}

void MiaImageRegistrationFilter::forwardRegistrationComplete()
{
    update();
    emit registrationComplete();
    qDebug() << "Registration took" << benchMarkerTimer.elapsed();
}

void MiaImageRegistrationFilter::startRegistration()
{
//    costFuction->setFixedImage(inputImage);
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
    benchMarkerTimer.restart();
    optimizer->start();
}

void MiaImageRegistrationFilter::registrationRunIntoError()
{
    QMessageBox msgBox;
    msgBox.setText("Registration has run into problems.");
    msgBox.exec();
}

void MiaImageRegistrationFilter::resetRegistration()
{
    transformFunction->resetParameters();
    costFuction->initializeParamerter();
    update();
}

void MiaImageRegistrationFilter::setMaximumIteration(int n)
{
    optimizer->setMaximumIteration(n);
}

