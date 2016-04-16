#include "miameanfilter.h"

MiaMeanFilter::MiaMeanFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Curvature 3D Filter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
}

bool MiaMeanFilter::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();
    float* out = (float*)outputImage->getDataPtr();
    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            result = runOperation<uchar>(in,out);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            result = runOperation<short>(in,out);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            result = runOperation<unsigned short>(in,out);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            result = runOperation<int>(ini,out);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            result = runOperation<float>(inf,out);
        }
            break;
        case IM_RGBA:
        {
//            QRgb* inr = (QRgb*) inputImage->getDataPtr();
//            runOperation<QRgb>(inr,out);
//            qDebug() << "RGB image is not supported yet";
            return false;
        }
            break;
    }
    outputImage->copyGeometryInfoFrom(inputImage);


    return result;
}

template <class S>
bool MiaMeanFilter::runOperation(S* inData, float* outdata)
{


    int ind;

    int size = inputImage->getImageSize();
    int imageW = inputImage->getWidth();
    int imageH = inputImage->getHeight();
    int imageD = inputImage->getDepth();
    int sliceSize = imageW*imageH;


 #pragma omp parallel for
    for( ind = 0; ind < size; ind++ )
    {
        int i,j,k;
        k=ind/sliceSize;
        j=(ind-k*sliceSize)/imageW;
        i=ind-k*sliceSize-j*imageW;

        float sum = inData[ind];
        int count = 1;
        if(i>1)
        {
            sum+=inData[ind-1];
            count ++;
        }
        if(i<imageW-1)
        {
            sum+=inData[ind+1];
            count ++;
        }
        if(j>1)
        {
            sum+=inData[ind-imageW];
            count ++;
        }
        if(j<imageH-1)
        {
            sum+=inData[ind+imageW];
            count ++;
        }
        if(k>1)
        {
            sum+=inData[ind-sliceSize];
            count ++;
        }
        if(k<imageD-1)
        {
            sum+=inData[ind+sliceSize];
            count ++;
        }
        outdata [ind] = sum/(float)count;

    }

    return true;
}

bool MiaMeanFilter::reAllocOutputImage()
{
    if(outputImage)
    {
        outputImage->release();
    }
    outputImage = creatEmptyImageFrom( inputImage, IM_float);
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;

    return true;
}
