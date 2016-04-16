#include "miaconvolutionfilter.h"
#include "miaconstants.h"

MiaConvolutionFilter::MiaConvolutionFilter(MiaNetwork *parent) :
     MiaModule(parent)
{
    moduleName = "Convolution 3D Filter";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    for(int i = 0 ;i < 3; i++)
    {
        kernelSize[i] = 0;
    }
    convolutionKernel = NULL;
    backgroundValue = 0;
}

void MiaConvolutionFilter::setConvolutionKernel(const int size[], const float *data)
{
    int data_size = 1;
    for(int i = 0 ;i < 3; i++)
    {
        kernelSize[i] = size[i];
        data_size *= size[i];
    }
    if(convolutionKernel)
        delete convolutionKernel;
    convolutionKernel = new float[data_size];
    for(int i = 0; i < data_size; i++)
    {
        convolutionKernel[i] = data[i];
    }
}

void MiaConvolutionFilter::setToUseGaussianKernel(int size, float sigma)
{
    int data_size = 1;
    double centerpos[3];
    for(int i = 0 ;i < 3; i++)
    {
        kernelSize[i] = size;
        data_size *= size;
        centerpos[i] = floor((double)size/2.0);
    }
    if(convolutionKernel)
        delete convolutionKernel;
    convolutionKernel = new float[data_size];
    double normalization_factor = 1.0/(sqrt(2.0*E_PI)*sigma);
    double invert_sigma_square = 1.0/(2.0*sigma*sigma);
    normalization_factor = normalization_factor*normalization_factor*normalization_factor;
    
    for(int z = 0; z < kernelSize[2]; z++)
    	for(int y = 0; y < kernelSize[1]; y++)
            for(int x = 0; x < kernelSize[0]; x++)
		    {
		    	int index = z*kernelSize[0]*kernelSize[1] + y*kernelSize[0] + x;
		    	double dist = (centerpos[0]-x)*(centerpos[0]-x) + (centerpos[1]-y)*(centerpos[1]-y) + (centerpos[2]-z)*(centerpos[2]-z);
		        convolutionKernel[index] = normalization_factor*exp(-invert_sigma_square*dist);
		    } 
}
void MiaConvolutionFilter::setToUseMeanKernel(int size)
{
    int data_size = 1;
    for(int i = 0 ;i < 3; i++)
    {
        kernelSize[i] = size;
        data_size *= size;
    }
    if(convolutionKernel)
        delete convolutionKernel;
    convolutionKernel = new float[data_size];
	float normalization_factor = 1.0/(float)data_size;
    
    for(int i = 0; i < data_size; i++)
    {
        convolutionKernel[i] = normalization_factor;
    }


}
bool MiaConvolutionFilter::runOperation()
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

bool MiaConvolutionFilter::reAllocOutputImage()
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

template <class S>
bool MiaConvolutionFilter::runOperation(S *inData, float *outdata)
{
    int ind;

    int totalSize = inputImage->getImageSize();
    int imageW = inputImage->getWidth();
    int imageH = inputImage->getHeight();
    int imageD = inputImage->getDepth();
    int sliceSize = imageW*imageH;
    int kernelW = kernelSize[0];
    int kernelH = kernelSize[1];
    int kernelD = kernelSize[2];
    int kernel_im_offsetx = -kernelW/2;
    int kernel_im_offsety = -kernelH/2;
    int kernel_im_offsetz = -kernelD/2;

// #pragma omp parallel for
    for( ind = 0; ind < totalSize; ind++ )
    {
        int im_x,im_y,im_z;
        im_z=ind/sliceSize;
        im_y=(ind-im_z*sliceSize)/imageW;
        im_x=ind-im_z*sliceSize-im_y*imageW;
        float value = 0.0;
        for(int k_z = 0; k_z < kernelD; k_z++)
            for(int k_y = 0; k_y < kernelH; k_y++)
                for(int k_x = 0; k_x <kernelW; k_x++)
                {
                    int k_i =  k_z*kernelW*kernelH + k_y*kernelW + k_x;
                    if(im_z + k_z + kernel_im_offsetz >= 0 && im_z + k_z + kernel_im_offsetz < imageD &&
                       im_y + k_y + kernel_im_offsety >= 0 && im_y + k_y + kernel_im_offsety < imageH &&
                       im_x + k_x + kernel_im_offsetx >= 0 && im_x + k_x + kernel_im_offsetx < imageW)
                    {
                        int im_i = (im_z + k_z + kernel_im_offsetz)*sliceSize
                                +  (im_y + k_y + kernel_im_offsety)*imageW
                                +  (im_x + k_x + kernel_im_offsetx);
                        value += convolutionKernel[k_i]*inData[im_i];
                    }
                    else
                    {
                        value += convolutionKernel[k_i]*backgroundValue;
                    }
                    outdata[ind] = value;
                }


    }
    return true;
}
