#include "miapersistantlabel.h"
#include <QMessageBox>
#define RESULTBIT 0x80

MiaPersistantLabel::MiaPersistantLabel(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Persistant Label";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    requiredInputImageType = IM_float;
    isRemovingRegion = false;
    autoUpdateWhenOutputRequested = false;
    startHistoryBit = 0;
    currentHistoryBit = 0;
}

bool MiaPersistantLabel::runOperation()
{
    float* indata = (float*) inputImage->getDataPtr();
    uchar* outdata = (uchar*)outputImage->getDataPtr();
    int totalSize = inputImage->getImageSize();
    int i;
    int lastStepBit = currentHistoryBit;
    currentHistoryBit++;
    currentHistoryBit = currentHistoryBit%7;
    if(currentHistoryBit == startHistoryBit)
        startHistoryBit = (currentHistoryBit+1)%7;
    #pragma omp parallel for
    for(i = 0; i < totalSize; i++)
    {
        if(isRemovingRegion)
        {
            if(indata[i]<0 )
            {
                outdata[i] &= ~(0x01<<currentHistoryBit); // clean the current bit
            }
            else
            {
                //copy result from last step
                if(outdata[i] & (0x01<<lastStepBit)) //check previous bit
                {
                    outdata[i] |= (0x01<<currentHistoryBit); //set curent bit
                }
                else
                {
                    outdata[i] &= ~(0x01<<currentHistoryBit);// clean the current bit
                }
            }
        }
        else
        {
            if(indata[i]<0 )
            {
                outdata[i] |= (0x01<<currentHistoryBit);
            }
            else
            {
                //copy result from last step
                if(outdata[i] & (0x01<<lastStepBit)) //check previous bit
                {
                    outdata[i] |= (0x01<<currentHistoryBit); //set curent bit
                }
                else
                {
                    outdata[i] &= ~(0x01<<currentHistoryBit);// clean the current bit
                }
            }
        }
        //set result bit use current step
        if(outdata[i] & (0x01<<currentHistoryBit)) //check previous bit
        {
            outdata[i] |= (RESULTBIT); //set curent bit
        }
        else
        {
            outdata[i] &= ~(RESULTBIT);// clean the current bit
        }
    }
    return true;

}

void MiaPersistantLabel::reset()
{
    if(inputImage == NULL || outputImage==NULL)
        return;
    int totalSize = inputImage->getImageSize();
    uchar* outdata = (uchar*)outputImage->getDataPtr();
    memset(outdata,0,totalSize*sizeof(char));
    startHistoryBit = 0;
    currentHistoryBit = 0;
}
void MiaPersistantLabel::undo()
{
    if(currentHistoryBit == startHistoryBit)
    {
        QMessageBox msgBox;
        msgBox.setText("You cannot undo this operation, no cache available.Try to reset and start over.");
        msgBox.exec();
        return;
    }
    currentHistoryBit = (currentHistoryBit + 6)%7;
    uchar* outdata = (uchar*)outputImage->getDataPtr();
    int totalSize = inputImage->getImageSize();
    #pragma omp parallel for
    for(int i = 0; i < totalSize; i++)
    {
        //set result bit use current step
        if(outdata[i] & (0x01<<currentHistoryBit)) //check previous bit
        {
            outdata[i] |= (RESULTBIT); //set curent bit
        }
        else
        {
            outdata[i] &= ~(RESULTBIT);// clean the current bit
        }
    }
    emit outputUpdated();
}

bool MiaPersistantLabel::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    outputImage = creatEmptyImageFrom( inputImage, IM_uchar);
    if(outputImage)
    {
        outputImage->retain();
        reset();

    }
    else
        return false;

    return true;
}
