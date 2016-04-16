#include "miamodelguidedconvolutionfilter.h"

MiaModelGuidedConvolutionFilter::MiaModelGuidedConvolutionFilter(MiaNetwork *parent) :
     MiaConvolutionFilter(parent)
{
    moduleName = "Model Guided Convolution 3D Filter";
    modelImage = NULL;
    secondInputModule = NULL;
}

MiaModelGuidedConvolutionFilter::~MiaModelGuidedConvolutionFilter()
{
    if(modelImage)
        modelImage->release();
}
void MiaModelGuidedConvolutionFilter::setSecondInputModule(MiaModule *input)
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
bool MiaModelGuidedConvolutionFilter::runOperation()
{
    if(modelImage->getType()!=IM_float)
    {
        qDebug() << "Model image should be a float type";
        throw std::runtime_error("Model image should be a float type");
    }
    int ind;
    float* outdata = (float*) outputImage->getDataPtr();
    float* modeldata = (float*) modelImage->getDataPtr();
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

    #pragma omp parallel
    {
        MiaPoint4D centerPosition;
        MiaPoint4D gradientDirection;
        MiaPoint4D rotationAxis;
        MiaPoint4D samplePosition;

        #pragma omp for nowait
        for( ind = 0; ind < totalSize; ind++ )
        {
            if(fabs(modeldata[ind]) > 4)
            {
                outdata[ind] = 0;
                continue;
            }
            int im_x,im_y,im_z;
            im_z=ind/sliceSize;
            im_y=(ind-im_z*sliceSize)/imageW;
            im_x=ind-im_z*sliceSize-im_y*imageW;
            float coor[4];
            coor[0] = im_x; coor[1] = im_y; coor[2] = im_z; coor[3] = 0;
            float gradient[3];
            modelImage->getGradientAt(coor, gradient);
            if(gradient[0] == 0 && gradient[1] == 0 && gradient[2] == 0)
            {
                outdata[ind] = 0;
                continue;
            }
            centerPosition.pos[0] = im_x;
            centerPosition.pos[1] = im_y;
            centerPosition.pos[2] = im_z;
            gradientDirection.pos[0] = gradient[0];
            gradientDirection.pos[1] = gradient[1];
            gradientDirection.pos[2] = gradient[2];
            gradientDirection.normalize();
            rotationAxis = filterDirection.cross(gradientDirection);
            float rotationAngle = acos(gradientDirection*filterDirection);
            MiaAffineTransform transform(rotationAxis, rotationAngle);
            float value = 0.0;
            for(int k_z = 0; k_z < kernelD; k_z++)
                for(int k_y = 0; k_y < kernelH; k_y++)
                    for(int k_x = 0; k_x <kernelW; k_x++)
                    {
                        int k_i =  k_z*kernelW*kernelH + k_y*kernelW + k_x;
                        float o_z = k_z + kernel_im_offsetz;
                        float o_y = k_y + kernel_im_offsety;
                        float o_x = k_x + kernel_im_offsetx;
                        samplePosition.pos[0] = o_x;
                        samplePosition.pos[1] = o_y;
                        samplePosition.pos[2] = o_z;
                        samplePosition = transform.transform(samplePosition);
                        samplePosition = centerPosition + samplePosition;
                        float sampleValue = inputImage->getValueAt(samplePosition.pos);
                        value += convolutionKernel[k_i]*sampleValue;
                    }
            outdata[ind] = value;
        }
    }
    outputImage->copyGeometryInfoFrom(inputImage);
    return true;
}

bool MiaModelGuidedConvolutionFilter::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        MiaImageND* tempimg = NULL;
        emit requestUpdateSecondInput(&tempimg);
        if(!tempimg)
            return false;
        if(modelImage)
            modelImage->release();
        modelImage = tempimg;
        modelImage->retain();
        int inputsize = inputImage->getImageSize();
        int modelsize = modelImage->getImageSize();
        if(inputsize != modelsize)
        {
            qDebug() << "the size of the input image and the model doesn't match";
            throw std::runtime_error("the size of the input image and the model doesn't match");
        }

    }
    else
        return false;
    return true;

}
