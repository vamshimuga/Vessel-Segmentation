#include "miamodule2in1out.h"

MiaModule2In1Out::MiaModule2In1Out(MiaNetwork *parent) :
    MiaModule(parent)
{
    secondInputImage = NULL;
    secondInputModule = NULL;
    needImageTypeMatch = false;
    secondImageRequiredType = IM_undefined;
}

MiaModule2In1Out::~MiaModule2In1Out()
{
    if(secondInputImage)
        secondInputImage->release();
}

void MiaModule2In1Out::setSecondInputModule(MiaModule *input)
{
    if(secondInputModule)
    {
        secondInputModule->disconnect(this);
        disconnect(secondInputModule);

    }

    connect(this, SIGNAL(requestUpdateSecondInput(MiaImageND**)), input, SLOT(getOutputImage(MiaImageND**)));
    connect(input, SIGNAL(outputUpdated()), this, SLOT(inputUpdated()));

    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }

}

bool MiaModule2In1Out::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        MiaImageND* tempimg = NULL;
        emit requestUpdateSecondInput(&tempimg);
        if(!tempimg)
            return false;
        if( needImageTypeMatch && inputImage->getType() != tempimg->getType() )
        {
            tempimg = castImageToAnotherType(tempimg, inputImage->getType());
            if(!tempimg)
                return false;
        }
        else if(secondImageRequiredType != IM_undefined)
        {
            tempimg = castImageToAnotherType(tempimg, secondImageRequiredType);
            if(!tempimg)
                return false;
        }
        if(secondInputImage)
        {
            secondInputImage->release();
            secondInputImage = NULL;
        }
        secondInputImage = tempimg;
        secondInputImage->retain();
    }
    else
        return false;
    return true;
}

