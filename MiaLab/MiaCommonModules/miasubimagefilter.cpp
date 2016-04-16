#include "miasubimagefilter.h"

MiaSubImageFilter::MiaSubImageFilter(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "MiaSubImageFilter";
    for(int i = 0; i < 8; i++)
        boundingBox[i] = 0;
    backgroundValue = 0;
}

void MiaSubImageFilter::setOutputImageBoundingBox(int *box)
{
    for(int i = 0; i < 8; i++)
        boundingBox[i] = box[i];
}

void MiaSubImageFilter::cropInZAxis(int bottom, int top)
{
    if(!inputImage)
    {
        throw std::runtime_error("No input image!");
    }
    int* dimsize = inputImage->getDimSize();
    for(int dim = 0 ; dim < 4; dim++)
    {
        boundingBox[dim*2] = 0;
        boundingBox[dim*2+1] = dimsize[dim] - 1;
    }
    boundingBox[4] = bottom < top ? bottom : top;
    boundingBox[5] = bottom > top ? bottom : top;

}

bool MiaSubImageFilter::checkIfNeedReAllocOutputImage()
{

    if(outputImage)
    {
        bool outputIsTheSame = true;
        int* dim = outputImage->getDimSize();
        for(int i = 0 ; i < 4; i++)
        {
            if(dim[i] != boundingBox[i*2+1] - boundingBox[i*2] + 1)
                outputIsTheSame = false;
        }
        if(outputIsTheSame)
            return false;
    }

    return true;
}

bool MiaSubImageFilter::runOperation()
{
    bool result;
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
    MiaPoint4D origin(boundingBox[0], boundingBox[2], boundingBox[4], boundingBox[6]);
    MiaPoint4D newoffset = inputImage->convertVoxelToPatientCoordinate(origin);
    outputImage->setOffset( newoffset.pos );
    outputImage->setSpacing(inputImage->getSpacing());

    outputImage->set3x3OrientationMatrix(inputImage->getOrientationMatrix());
    outputImage->computeTransformMatrix();
    return result;

}

bool MiaSubImageFilter::reAllocOutputImage()
{
    if(outputImage != NULL)
    {
        outputImage->release();
        outputImage = NULL;
    }
    ImageType sourcetype = inputImage->getType();
    int outWidth = boundingBox[1] - boundingBox[0] + 1;
    int outHeight = boundingBox[3] - boundingBox[2] + 1;
    int outDepth = boundingBox[5] - boundingBox[4] + 1;
    int outPhase = boundingBox[7] - boundingBox[6] + 1;
    outputImage = new MiaImageND( outWidth, outHeight, outDepth, outPhase, sourcetype);
    if(outputImage != NULL)
    {
        outputImage->retain();
        return true;
    }
    return false;
}

template <class S>
bool MiaSubImageFilter::runOperation(S *inData, S *outdata)
{
    int inWidth = inputImage->getWidth();
    int inHeight = inputImage->getHeight();
    int inDepth = inputImage->getDepth();
    int inPhase = inputImage->getNumberOfPhases();
    int outWidth = outputImage->getWidth();
    int outHeight = outputImage->getHeight();
    int outDepth = outputImage->getDepth();
    int outPhase = outputImage->getNumberOfPhases();

    for(int t = boundingBox[6]; t <= boundingBox[7]; t++ )
        for(int z = boundingBox[4]; z <= boundingBox[5]; z ++)
            for(int y = boundingBox[2]; y <= boundingBox[3]; y++)
                for(int x = boundingBox[0]; x <= boundingBox[1]; x++)
                {
                    int outIndex = (t - boundingBox[6])*outWidth*outHeight*outDepth
                            +(z - boundingBox[4])*outWidth*outHeight
                            +(y - boundingBox[2])*outWidth
                            + x - boundingBox[0];
                    if( x < 0 || x >= inWidth || y < 0 || y >= inHeight || z < 0 || z >= inDepth
                            || t < 0 || t >= inPhase)
                    {
                        outdata[outIndex] = backgroundValue;
                    }
                    else
                    {
                        int inIndex = t*inWidth*inHeight*inDepth
                                + z*inWidth*inHeight
                                + y*inWidth
                                + x;
                        outdata[outIndex] = inData[inIndex];
                    }
                }
    return true;
}
