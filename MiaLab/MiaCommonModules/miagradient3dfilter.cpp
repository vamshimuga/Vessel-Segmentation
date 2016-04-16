#include "miagradient3dfilter.h"

MiaGradient3DFilter::MiaGradient3DFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Gradient 3D Filter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
}

bool MiaGradient3DFilter::runOperation()
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
bool MiaGradient3DFilter::runOperation(S* inData, float* outdata)
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



        float gradient[3];

        if(i == 0)
            gradient[0]= inData[ind]- inData[ind+1];
        else if(i == imageW-1)
            gradient[0]= inData[ind-1]- inData[ind];
        else
            gradient[0]= inData[ind-1]- inData[ind+1];

        if(j == 0)
            gradient[1]= inData[ind]- inData[ind+imageW];
        else if(j == imageH-1)
            gradient[1]= inData[ind-imageW]- inData[ind];
        else
            gradient[1]= inData[ind-imageW]- inData[ind+imageW];

        if(k == 0)
            gradient[2]= inData[ind]- inData[ind+sliceSize];
        else if(k == imageD-1)
            gradient[2]= inData[ind-sliceSize]- inData[ind];
        else
            gradient[2]= inData[ind-sliceSize]- inData[ind+sliceSize];

        float input_gradient = gradient[0]*gradient[0] + gradient[1]*gradient[1] + gradient[2]*gradient[2];

        if(input_gradient < 1.0)
            input_gradient = 1.0;

        outdata [ind] = 1000.0/sqrt(input_gradient);

    }

    return true;
}

bool MiaGradient3DFilter::reAllocOutputImage()
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

void MiaGradient3DFilter::dispatchParameters(const QMap<QString, QVariant> &parameters)
{

}
