#include "miaimagepropertyspreadingfilter.h"

#define VISITED 0x80
#define DIRECTIONBITSMASK 0x1f
#define DIRECTIONTOSELF 13 //0-26 represent the 26 neighbor's direction. 13 is the one in the middle

MiaImagePropertySpreadingFilter::MiaImagePropertySpreadingFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "image property spreading";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    inputModule = NULL;
    directionMapInputModule = NULL;
    directionMap = NULL;
}

MiaImagePropertySpreadingFilter::~MiaImagePropertySpreadingFilter()
{
    if(directionMap)
        directionMap->release();
}
void MiaImagePropertySpreadingFilter::setDirectionMapInputModule( MiaModule* input, const char * method)
{
    if(directionMapInputModule)
    {
        directionMapInputModule->disconnect(this);
        disconnect(directionMapInputModule);

    }

    connect(this, SIGNAL(requestUpdateDirectionMap(MiaImageND**)), input, method);
    connect(input, SIGNAL(outputUpdated()), this, SLOT(inputUpdated()));
    needUpdate = true;
    if(autoUpdateWhenInputChanged)
    {
        update();
    }
}

bool MiaImagePropertySpreadingFilter::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();
    dmap = (uchar*)directionMap->getDataPtr();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            uchar* out = (uchar*)outputImage->getDataPtr();
            result = runOperation<uchar>(in,out);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            short* out = (short*)outputImage->getDataPtr();
            result = runOperation<short>(in,out);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            unsigned short* out = (unsigned short*)outputImage->getDataPtr();
            result = runOperation<unsigned short>(in,out);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            int* out = (int*)outputImage->getDataPtr();
            result = runOperation<int>(ini,out);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            float* out = (float*)outputImage->getDataPtr();
            result = runOperation<float>(inf,out);
        }
            break;
        case IM_RGBA:
        {
            QRgb* inr = (QRgb*) inputImage->getDataPtr();
            QRgb* out = (QRgb*)outputImage->getDataPtr();
            result = runOperation<QRgb>(inr,out);
        }
            break;
    }
    outputImage->copyGeometryInfoFrom(inputImage);


    return result;
}

template <class A>
bool MiaImagePropertySpreadingFilter::runOperation(A* inData,A* outData)
{
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    sliceSize = width*height;
    int totalSize = inputImage->getImageSize();
    for(int i = 0 ; i < totalSize ; i ++)
    {
        dmap[i] &= DIRECTIONBITSMASK;
    }

    int currentindex;
    int currentdirection;
    int x,y,z;
    int dx,dy,dz;
    for(currentindex = 0; currentindex <totalSize; currentindex++)
    {
        z = currentindex/sliceSize;
        y = (currentindex - z*sliceSize)/width;
        x = currentindex - z*sliceSize - y*width;

        if(dmap[currentindex] & VISITED)
            continue;
        if ( (dmap[currentindex] & DIRECTIONBITSMASK)!=DIRECTIONTOSELF)
        {
            currentdirection = dmap[currentindex] & DIRECTIONBITSMASK;
            dz = currentdirection/9;
            dy = (currentdirection - dz*9)/3;
            dx = currentdirection - dz*9 - dy*3;
            dx--; dy--; dz--;
            x += dx;
            y += dy;
            z += dz;
#ifdef QT_DEBUG
            if(x<0 || x>=width || y < 0 || y >=height || z<0 || z>=depth)
            {
                qDebug()<<"direction map pointing out side image";
                throw std::runtime_error("direction map pointing out side image");
            }
#endif
            int neighborindex = z*sliceSize + y*width + x;
            outData[currentindex] = getPropertyAt<A>(neighborindex,inData,outData);
        }
        else
        {
            outData[currentindex] = inData[currentindex];
        }
        dmap[currentindex] |= VISITED;
    }



    return true;
}

template <class A>
A MiaImagePropertySpreadingFilter::getPropertyAt(int currentindex, A* inData,A* outData)
{
    int currentdirection;
    if(dmap[currentindex] & VISITED)
    {
        return outData[currentindex];
    }
    int x,y,z;
    int dx,dy,dz;
    z = currentindex/sliceSize;
    y = (currentindex - z*sliceSize)/width;
    x = currentindex - z*sliceSize - y*width;
    if ( (dmap[currentindex] & DIRECTIONBITSMASK)!=DIRECTIONTOSELF)
    {
        currentdirection = dmap[currentindex] & DIRECTIONBITSMASK;
        dz = currentdirection/9;
        dy = (currentdirection - dz*9)/3;
        dx = currentdirection - dz*9 - dy*3;
        dx--; dy--; dz--;
        x += dx;
        y += dy;
        z += dz;
#ifdef QT_DEBUG
        if(x<0 || x>=width || y < 0 || y >=height || z<0 || z>=depth)
        {
            qDebug()<<"direction map pointing out side image";
            throw std::runtime_error("direction map pointing out side image");
        }
#endif
        int neighborindex = z*sliceSize + y*width + x;
        outData[currentindex] = getPropertyAt(neighborindex,inData,outData);
    }
    else
    {
        outData[currentindex] = inData[currentindex];
    }
    dmap[currentindex] |= VISITED;
    return outData[currentindex];
}

bool MiaImagePropertySpreadingFilter::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        MiaImageND* tempimg = NULL;
        emit requestUpdateDirectionMap(&tempimg);
        if(!tempimg)
            return false;
        if(tempimg->getType() != IM_uchar)
        {
            qDebug()<< "the direction map must be a unsigned char type";
            return false;
        }
        if(directionMap)
            directionMap->release();
        directionMap = tempimg;
        directionMap->retain();
        if(directionMap && directionMap->getWidth() == inputImage->getWidth() && directionMap->getHeight() == inputImage->getHeight() && directionMap->getDepth() == inputImage->getDepth() && directionMap->getNumberOfPhases() == inputImage->getNumberOfPhases())
            return true;
        else
        {
            qDebug()<<"direction map must be the same size as the input image";
            return false;
        }
    }
    else
        return false;
    return true;
}

bool MiaImagePropertySpreadingFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    if(operateInSitu)
        outputImage = inputImage;
    else
        outputImage = creatEmptyImageFrom( inputImage, inputImage->getType());
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;

    return true;
}
