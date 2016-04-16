#include "miaresample3d.h"
#include <QtGui>

MiaResample3D::MiaResample3D(MiaNetwork *parent) :
    MiaModule2In1Out(parent)
{
    moduleName = "3D resmaple";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
    currentResampleStrategy = Mia_Resample_IsotropicMaxSpacing;
    int dim;
    for(dim = 0; dim < 3; dim++)
    {
        newdimension[dim] = 0;
        scale[dim] = 1.0;
    }
    secondInputImage = NULL;
}

bool MiaResample3D::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();
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
//            QRgb* inr = (QRgb*) inputImage->getDataPtr();
//            runOperation<QRgb>(inr,out);
            qDebug() << "RGB image is not supported yet";
            return false;
        }
            break;
    }


    return result;
}

bool MiaResample3D::reloadInputImage()
{
    if(currentResampleStrategy == Mia_Resample_UseAnotherImage)
    {
        return MiaModule2In1Out::reloadInputImage();
    }
    return MiaModule::reloadInputImage();
}

template <class S>
bool MiaResample3D::runOperation(S* inData, S* outdata)
{



    int size = outputImage->getImageSize();
    int imageW = outputImage->getWidth();
    int imageH = outputImage->getHeight();
    int imageD = outputImage->getDepth();
    int sliceSize = imageW*imageH;
    int inputW = inputImage->getWidth();
    int inputH = inputImage->getHeight();
    int inputD = inputImage->getDepth();


    QTime t;
    t.start();
    int ind;
     #pragma omp parallel for
    for( ind = 0; ind < size; ind++ )
    {
                int i,j,k;
                k=ind/sliceSize;
                j=(ind-k*sliceSize)/imageW;
                i=ind-k*sliceSize-j*imageW;
                int inputindex_x, inputindex_y,inputindex_z,inputindex1,inputindex2,inputindex;
                float x,y,z;
                inputindex_x = (int)((float)i*delta[0]);
                inputindex_y = (int)((float)j*delta[1]);
                inputindex_z = (int)((float)k*delta[2]);
                if(inputindex_x < 0 || inputindex_x >= inputW || inputindex_y < 0 || inputindex_y >= inputH || inputindex_z < 0 || inputindex_z >= inputD )
                {
                    outdata[ind] = 0;
                    continue;
                }
                x = (float)i*delta[0] - (float)inputindex_x;
                y = (float)j*delta[1] - (float)inputindex_y;
                z = (float)k*delta[2] - (float)inputindex_z;
                //first pair
                inputindex = inputindex_z*inputW*inputH + inputindex_y*inputW + inputindex_x;
                outdata[ind] = (float)inData[inputindex]* (1 - x) * (1 - y) * (1 - z);
                if(inputindex_x < inputW-1)
                {
                    inputindex2 =  inputindex +1;
                }
                else
                {
                    inputindex2 =  inputindex;
                }
                outdata[ind] += (float)inData[inputindex2] * x * (1 - y) * (1 - z) ;

                //second pair
                if(inputindex_y < inputH - 1)
                    inputindex1 = inputindex + inputW;
                else
                    inputindex1 = inputindex;
                outdata[ind] += (float)inData[inputindex1]*  (1 - x) * y * (1 - z);

                if(inputindex_x < inputW-1)
                {
                    inputindex2 =  inputindex1 +1;
                }
                else
                {
                    inputindex2 =  inputindex1;
                }
                outdata[ind] += (float)inData[inputindex2] * x * y * (1 - z);

               //third pair
               if(inputindex_z < inputD - 1)
                   inputindex1 = inputindex + inputW*inputH;
               else
                   inputindex1 = inputindex;
                outdata[ind] += (float)inData[inputindex1]*  (1 - x) * (1 - y) * z ;
                if(inputindex_x < inputW-1)
                {
                    inputindex2 =  inputindex1 +1;
                }
                else
                {
                     inputindex2 =  inputindex1;
                }
                outdata[ind] += (float)inData[inputindex2] * x * (1 - y) * z ;

                //fourth pair
                inputindex1 = inputindex;
                if(inputindex_y < inputH - 1)
                    inputindex1 += inputW;

                if(inputindex_z < inputD - 1)
                    inputindex1 += inputW*inputH;

                outdata[ind] += (float)inData[inputindex1]* (1 - x) * y * z ;
                if(inputindex_x < inputW-1)
                {
                    inputindex2 =  inputindex1 +1;
                }
                else
                {
                    inputindex2 =  inputindex1;
                }
                outdata[ind] += (float)inData[inputindex2] * x * y * z ;
            }
        qDebug() << moduleName << " test: time elapsed: " << t.elapsed();
//    t.restart();
//    int totalsize = 512*128;
//    float* testdata = (float*)malloc(totalsize*sizeof(float));
//    for(int i =0 ; i< 1000; i++)
//        for (int j = 0; j < totalsize; ++j)
//        {
//            int x = j/128;
//            int y =  j - 128*x;
//            testdata[y*512 + x]+= j;
//        }
//    qDebug() << moduleName << " single thread test: time elapsed: " << t.elapsed();
//    t.restart();

//for(int i =0 ; i< 1000; i++)
//    #pragma omp parallel for
//    for (int j = 0; j < totalsize; ++j)
//    {
//        int x = j/128;
//        int y =  j - 128*x;
//        testdata[y*512 + x]+= j;
//    }
//qDebug() << moduleName << " openmp n thread test: time elapsed: " << t.elapsed();
//t.restart();

//for(int i =0 ; i< 1000; i++)
//    #pragma omp parallel for num_threads(2)
//    for (int j = 0; j < totalsize; ++j)
//    {
//        int x = j/128;
//        int y =  j - 128*x;
//        testdata[y*512 + x]+= j;
//    }
//qDebug() << moduleName << " openmp 2 thread test: time elapsed: " << t.elapsed();
//t.restart();
    return true;
}

bool MiaResample3D::reAllocOutputImage()
{

    if(outputImage)
    {
        outputImage->release();
    }
    float* inputspacing = inputImage->getSpacing();
    int* inputdim = inputImage->getDimSize();

    float minSpacing = inputspacing[0];
    float maxSpacing = inputspacing[0];
    if(minSpacing > inputspacing[1])
        minSpacing = inputspacing[1];
    if(minSpacing > inputspacing[2])
        minSpacing = inputspacing[2];
    if(maxSpacing < inputspacing[1])
        maxSpacing = inputspacing[1];
    if(maxSpacing < inputspacing[2])
        maxSpacing = inputspacing[2];

    int dim;
    for(dim = 0 ; dim < 3; dim ++)
    {
        switch(currentResampleStrategy)
        {
        case Mia_Resampe_SetSize:
            newspacing[dim] = (float)inputdim[dim]*inputspacing[dim]/(float)newdimension[dim];

            break;
        case Mia_Resampe_SetScale:
            newdimension[dim] = inputdim[dim]*scale[dim];
            newspacing[dim] = (float)inputdim[dim]*inputspacing[dim]/(float)newdimension[dim];
            break;
        case Mia_Resample_SetSpacing:
            newdimension[dim] = (float)inputdim[dim]*inputspacing[dim]/ newspacing[dim];
            break;
        case Mia_Resample_IsotropicMaxSpacing:
            newspacing[dim] = maxSpacing;
            newdimension[dim] = (float)inputdim[dim]*inputspacing[dim]/ newspacing[dim];
            break;
        case Mia_Resampe_IsotropicMinSpacing:
            newspacing[dim] = minSpacing;
            newdimension[dim] = (float)inputdim[dim]*inputspacing[dim]/ newspacing[dim];
            break;
        case Mia_Resample_UseAnotherImage:
            if(secondInputImage == NULL)
            {
                qDebug() << "Resample is set to use another image. But template image is not set";
            }
            else
            {
                float* templatespacing = secondInputImage->getSpacing();
                newspacing[dim] = templatespacing[dim];
                int* templateImageSize = secondInputImage->getDimSize();
                newdimension[dim] = templateImageSize[dim];
            }
            break;
        default:
            throw std::runtime_error("Unknown resampe method");
            break;
            
        }

        delta[dim] = (float)inputdim[dim]/(float)newdimension[dim];
           
    }
    newspacing[3] = 1.0;
    outputImage =  new MiaImageND(newdimension[0], newdimension[1], newdimension[2], 1, inputImage->getType());
    if(outputImage)
    {
        outputImage->setOffset(inputImage->getOffset());
        outputImage->setSpacing(newspacing);

        outputImage->set3x3OrientationMatrix(inputImage->getOrientationMatrix());
        outputImage->computeTransformMatrix();
        outputImage->retain();

    }
    else
        return false;

    return true;
}

void MiaResample3D::dispatchParameters(const QMap<QString, QVariant> &parameters)
{

}
void MiaResample3D::setOutputSize(int x, int y, int z)
{
    newdimension[0] = x;
    newdimension[1] = y;
    newdimension[2] = z;
    setResampleStrategy(Mia_Resampe_SetSize);
}
void MiaResample3D::setResampleScale(float x, float y, float z)
{

    scale[0] = x;
    scale[1] = y;
    scale[2] = z;
    setResampleStrategy(Mia_Resampe_SetScale);
}

void MiaResample3D::setResampleSpacing(float x, float y, float z)
{
    newspacing[0] = x;
    newspacing[1] = y;
    newspacing[2] = z;
    setResampleStrategy(Mia_Resample_SetSpacing);
}

void MiaResample3D::setResampleToIsotropic(bool max)
{
    if(max)
        setResampleStrategy(Mia_Resample_IsotropicMaxSpacing);
    else
        setResampleStrategy(Mia_Resampe_IsotropicMinSpacing);
}

void MiaResample3D::setResampleStrategy(ResampleStrategy strategy)
{
    currentResampleStrategy = strategy;
    needUpdate = true;
}
