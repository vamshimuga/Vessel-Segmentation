#include "miamodule.h"
#include <QtGui>
#include <iostream>
#include "mianetwork.h"

MiaModule::MiaModule(MiaNetwork *parent) :
    QObject(parent)
{
    needUpdate = true;
    autoUpdateWhenInputChanged = true;
    autoUpdateWhenParameterChanged = true;
    autoUpdateWhenOutputRequested = true;
    autoUpdateWhenInputMarkerUpdated = true;
    inputImage = NULL;
    outputImage = NULL;
    inputModule = NULL;
    inputImageIsSet = false;
    requiredInputImageType = IM_undefined;
    operateInSitu = false;
    if(parent != NULL)
    {
        parentNetwork = parent;
        logging_level = parentNetwork->enviromentSetting->logging_level;
    }
    else
    {
        parentNetwork = NULL;
        logging_level = 0;
    }

}
MiaModule::~MiaModule()
{
    if(inputImage)
        inputImage->release();
    if(outputImage)
        outputImage->release();
}

void MiaModule::getOutputImage(MiaImageND** img)
{
    if(needUpdate && autoUpdateWhenOutputRequested)
        update();
    *img = outputImage;
    return;

}
void MiaModule::reLatedModuleModified()
{
    if(!needUpdate)
    {
        needUpdate = true;
        emit modified();
    }

}

//void MiaModule::setInputModule(MiaModule* input,int port)
//{
//    if(inputModule)
//    {
//        disconnect(inputModule, SIGNAL(moduleDeleted()), this, SIGNAL(inputModuleDeleted()));
//        disconnect(inputModule, SIGNAL(outputUpdated()), this, SIGNAL(inputUpdated()));
//    }
//    inputModule = input;
//    inputModulePortNO = port;
//    needUpdate = true;
//    if(inputModule)
//    {
//        connect(inputModule, SIGNAL(moduleDeleted()), this, SIGNAL(inputModuleDeleted()));
//        connect(inputModule, SIGNAL(outputUpdated()), this, SIGNAL(inputUpdated()));
//    }
//    if(autoUpdateWhenInputChanged)
//    {
//        needUpdate = true;
//        update();
//    }
//}

void MiaModule::setInputModule( MiaModule* input )
{
    if(inputModule)
    {
        inputModule->disconnect(this);
        disconnect(inputModule);

    }
    inputModule = input;
    if(input)
    {
        connect(this, SIGNAL(requestUpdateInput(MiaImageND**)), input, SLOT(getOutputImage(MiaImageND**)));
        connect(input, SIGNAL(outputUpdated()), this, SLOT(inputUpdated()));
    }
    needUpdate = true;
    inputImageIsSet = false;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }

}

void MiaModule::SetInputImage(MiaImageND* img)
{
    if(inputModule)
    {
        inputModule->disconnect(this);
        disconnect(inputModule);

    }
    if( img && requiredInputImageType != IM_undefined &&  requiredInputImageType != img->getType() )
    {
        img = castImageToAnotherType(img, requiredInputImageType);
        if(!img)
            return;
    }
    if(inputImage != img)
    {
        if(inputImage)
            inputImage->release();
        inputImage = img;
        inputImage->retain();
    }

    inputImageIsSet = true;
    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }
}

bool MiaModule::update()
{

    QTime t;
     t.start();
    if(reloadInputImage() == false)
    {
#ifdef QT_DEBUG
        qDebug() << "Failed to load input Image in " << moduleName;
#endif
        if(logging_level>=3)
        {
            logMessage(moduleName + ":Failed to load input Image");
        }
        return false;
    }
    if(checkIfNeedReAllocOutputImage() && reAllocOutputImage() == false)
    {
#ifdef QT_DEBUG
        qDebug() << "Failed to alloc output Image in " << moduleName;
#endif
        if(logging_level>=3)
        {
            logMessage(moduleName + ":Failed to alloc output Image");
        }
        return false;
    }

//#ifdef QT_DEBUG
//    qDebug() << moduleName << "loading image: time elapsed: " << t.elapsed();
//#endif
    if(logging_level>=5)
    {
        logMessage(moduleName + ": loading image: time elapsed: " + QVariant(t.elapsed()).toString());
    }
    try
    {
        if(runOperation()==false)
        {
#ifdef QT_DEBUG
            qDebug() << "Failed to run the operation in " << moduleName;
#endif
            if(logging_level>=3)
            {
                logMessage(moduleName + ":Failed to run the operation");
            }
            return false;
        }
    }
    catch(const std::exception& e)
    {
#ifdef QT_DEBUG
        qDebug() << "something wrong with " << moduleName;
#endif
        if(logging_level>=3)
        {
            logMessage(moduleName + ":Exception raised, something went wrong");
        }
        std::cerr << "exception caught: " << e.what() << endl;
    }
//#ifdef QT_DEBUG
//     qDebug() << moduleName << "processing: time elapsed: " << t.elapsed();
//#endif
     if(logging_level>=5)
     {
         logMessage(moduleName + ":processing: time elapsed: " + QVariant(t.elapsed()).toString());
     }
    needUpdate = false;
    emit outputUpdated();
    return true;
}

void MiaModule::inputUpdated()
{
    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }
    emit modified();
}
void MiaModule::inputMarkerUpdated(MarkerUpdateType msg)
{
    needUpdate = true;
    if(msg==MIA_MarkerUpdated && autoUpdateWhenInputMarkerUpdated)
    {
        update();
    }
    emit modified();
}

void MiaModule::setParameters(const QMap <QString, QVariant> &parameters)
{
    //if the upper module send an empty parameter, it means the upper module is asking for initial values
    if(parameters.isEmpty())
    {
        emit parameterChanged(wiredParameters);
        return;
    }

    QMapIterator<QString, QVariant> i(parameters);
    bool ifanychanges = false;
     while (i.hasNext()) {
         i.next();
         if(wiredParameters.contains(i.key()) && wiredParameters.value(i.key()) != i.value())
         {
             wiredParameters.insert(i.key(),i.value());
             ifanychanges = true;
         }
//         qDebug() <<moduleName << "set parameter" << i.key() << i.value();
     }
    if(ifanychanges)
    {
        needUpdate = true;
        dispatchParameters(wiredParameters);
        emit modified();

    }
    if(autoUpdateWhenParameterChanged && needUpdate)
    {
        update();
    }
    if(ifanychanges)
        emit parameterChanged(wiredParameters);
}
void MiaModule::dispatchParameters(const QMap <QString, QVariant> &parameters)
{
    qDebug() << parameters;

}
void MiaModule::changeWiredParameter(QString &name, float value)
{

    if(wiredParameters.contains(name) && wiredParameters.value(name) != value)
    {
        wiredParameters.insert(name,value);
        emit parameterChanged(wiredParameters);
    }
}

MiaImageND* MiaModule::castImageToAnotherType(MiaImageND* img, ImageType targettype , float delta, float scale)
{
    MiaImageND* newImage = creatEmptyImageFrom(img, targettype);
    if(newImage)
    {
        switch(targettype)
        {
            case IM_uchar:
                img->castToUChar((uchar*)newImage->getDataPtr(),delta,scale);
                break;
            case IM_short:
                img->castToShort((short*)newImage->getDataPtr(),delta,scale);
                break;
            case IM_unsignedshort:
                img->castToUsignedShort((unsigned short*)newImage->getDataPtr(),delta,scale);
                break;
            case IM_int:
                img->castToInt((int*)newImage->getDataPtr(),delta,scale);
                break;
            case IM_float:
                img->castToFloat((float*)newImage->getDataPtr(),delta,scale);
            case IM_RGBA:
                //undefined
                break;
             default:
                break;
        }
    }
    return newImage;
}

MiaImageND* MiaModule::creatEmptyImageFrom(MiaImageND* img, ImageType targettype)
{
    MiaImageND* newImage = creatEmptyImage(img->getWidth(), img->getHeight(), img->getDepth(), img->getNumberOfPhases(),targettype);
    if(newImage)
    {
        newImage->copyGeometryInfoFrom(img);
    }
    return newImage;
}

MiaImageND* MiaModule::creatEmptyImage(int x, int y, int z, int t, ImageType type)
{
    MiaImageND* img = new MiaImageND(x, y, z, t, type);
    return img;
}
bool MiaModule::reloadInputImage()
{
    if(!inputImageIsSet)
    {
        MiaImageND* tempimg = NULL;
        emit requestUpdateInput(&tempimg);
        if(!tempimg)
            return false;
        if( requiredInputImageType != IM_undefined &&  requiredInputImageType != tempimg->getType() )
        {
            tempimg = castImageToAnotherType(tempimg, requiredInputImageType);
            if(!tempimg)
                return false;
        }
        if(inputImage!=tempimg)
        {
            if(inputImage)
                inputImage->release();
            inputImage = tempimg;
            inputImage->retain();

        }

    }
    return true;
}
bool MiaModule::runOperation()
{
    return true;
}

bool  MiaModule::reAllocOutputImage()
{
    return true;
}
void MiaModule::logMessage(QString msg)
{
    if(parentNetwork->enviromentSetting->logging_level>=3)
        parentNetwork->enviromentSetting->logMessage(msg);
}

bool MiaModule::checkIfNeedReAllocOutputImage()
{
    if(outputImage && outputImage->getWidth() == inputImage->getWidth() && outputImage->getHeight() == inputImage->getHeight() && outputImage->getDepth() == inputImage->getDepth() && outputImage->getNumberOfPhases() == inputImage->getNumberOfPhases())
        return false;
    return true;
}
