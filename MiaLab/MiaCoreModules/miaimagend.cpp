#include "miaimagend.h"
#include "transform/miairregulartransformAlgorithms.h"
#include <QtGui>


MiaImageND::MiaImageND(int x, int y, int z, int t,ImageType type, QByteArray* data) :
    MiaRetainableObject()
{

    dimSize[0] = x;
    dimSize[1] = y;
    dimSize[2] = z;
    dimSize[3] = t;
    int i;
    dimension = 0;
    for (i = 0; i < 4; i++)
    {
        if(dimSize[i] <= 1)
            dimSize[i] = 1;
        else
            dimension++;

        offset[i] = 0;
        spacing[i] = 1.0;
    }
    numberOfNeighbors = 1<<dimension;
    sliceThickness = 0.0f;

    for(i = 0; i < 9; i++)
    {
        if(i % 4 == 0)
            orientationMatrix[i] = 1.0;
        else
            orientationMatrix[ i] = 0.0;

    }

    dataType = type;
    bytePerElement=0;
    switch(type)
    {
        case IM_uchar:
            bytePerElement=sizeof(uchar);
            break;
        case IM_short:
        case IM_unsignedshort:
            bytePerElement=sizeof(short);
            break;
        case IM_int:
            bytePerElement=sizeof(int);
            break;
        case IM_float:
            bytePerElement=sizeof(float);
        case IM_RGBA:
            bytePerElement=sizeof(char)*4;
            break;
         default:
            break;
    }
    if(data)
    {
        dataArray = data;
    }
    else
    {
        dataArray = new QByteArray();

        dataArray->resize(dimSize[0]*dimSize[1]*dimSize[2]*dimSize[3]*bytePerElement);
    }

    dataPtr = dataArray->data();


//    dataStoreInRightHandSystem = true;
    needFlipUpDown = false;
    needFlipLeftRight = false;
    isUsingSharedMemory = false;
    isCastingToUnsignedValue = false;
    defaultWindowCenter = 127;
    defaultWindowWidth = 255;
    minValue = -1200;
    maxValue = 2000;
    outBoundaryValue = 0;
}

MiaImageND::MiaImageND(int x, int y, int z, int t,ImageType type, void* data) :
    MiaRetainableObject()
{

    dimSize[0] = x;
    dimSize[1] = y;
    dimSize[2] = z;
    dimSize[3] = t;
    int i;
    dimension = 0;
    for (i = 0; i < 4; i++)
    {
        if(dimSize[i] <= 1)
            dimSize[i] = 1;
        else
            dimension++;

        offset[i] = 0;
        spacing[i] = 1.0;
    }
    numberOfNeighbors = 1<<dimension;

    for(i = 0; i < 9; i++)
    {
        if(i % 4 == 0)
            orientationMatrix[i] = 1.0;
        else
            orientationMatrix[ i] = 0.0;

    }

    dataType = type;
    bytePerElement=0;
    switch(type)
    {
        case IM_uchar:
            bytePerElement=sizeof(uchar);
            break;
        case IM_short:
        case IM_unsignedshort:
            bytePerElement=sizeof(short);
            break;
        case IM_int:
            bytePerElement=sizeof(int);
            break;
        case IM_float:
            bytePerElement=sizeof(float);
        case IM_RGBA:
            bytePerElement=sizeof(char)*4;
            break;
         default:
            break;
    }
    if(data)
    {
        dataPtr = (char*)data;
    }
    else
    {
        qDebug()<< "data is null";
        throw std::runtime_error("data is null");
    }


    usedByModules = 0;

//    dataStoreInRightHandSystem = true;
    needFlipUpDown = false;
    needFlipLeftRight = false;
    isUsingSharedMemory = true;
    isCastingToUnsignedValue = false;
    defaultWindowCenter = 127;
    defaultWindowWidth = 255;
    minValue = -1200;
    maxValue = 2000;
}

MiaImageND:: ~MiaImageND()
{
    if(!isUsingSharedMemory)
        delete dataArray;
}
void MiaImageND::setOffset(float* var)
{
  int i;
  for( i = 0; i < 4 ; i++)
  {
      offset[i] = var[i];
  }
}
void MiaImageND::setSpacing(float* var)
{
    int i;
    for( i = 0; i < 4 ; i++)
    {
        spacing[i] = var[i];
    }
    for( i = dimension; i < 4 ; i++)
    {
        spacing[i] = 1.0f;
    }
}

float MiaImageND::getSliceThickness()
{
    if(sliceThickness == 0.0f)
    {
        return spacing[2];
    }
    else
    {
        return sliceThickness;
    }
}
void MiaImageND::set3x2OrientationMatrix(float* var)
{
    int i;
    for( i = 0; i < 6 ; i++)
    {
        orientationMatrix[i] = var[i];
    }

    float* a, *b, *c;
    a = orientationMatrix;
    b = orientationMatrix+3;
    c = orientationMatrix+6;
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2] ;
    c[2] = a[0]*b[1] - a[1]*b[0];

    for( i = 0; i < 3; i++)
    {
        normalizeVector( orientationMatrix+i*3 );
    }

}

void MiaImageND::set3x3OrientationMatrix(float* var)
{
    int i;
    for( i = 0; i < 9 ; i++)
    {
        orientationMatrix[i] = var[i];
    }


    for( i = 0; i < 3; i++)
    {
        normalizeVector( orientationMatrix+i*3 );
    }
// check whether the system is right hand for sliers
//    if(dimension > 2)
//    {
//        float* a, *b, c[3];
//        a = orientationMatrix;
//        b = orientationMatrix+3;
//        c[0] = a[1]*b[2] -  a[2]*b[1];
//        c[1] = a[2]*b[0] -  a[0]*b[2];
//        c[2] = a[0]*b[1] -  a[1]*b[0];
//        float err = 0;
//        for( i = 0; i < 3; i++)
//        {
//            err += (c[i] - orientationMatrix[6+i])*(c[0] - orientationMatrix[6+i]);
//        }
//        if (err > 1.0)
//            dataStoreInRightHandSystem = false;
//    }

}
void MiaImageND::computeTransformMatrix()
{
    MiaAffineTransform affine(orientationMatrix,spacing,offset);
    transform.setBulkTranform(affine);

}
MiaTransform MiaImageND::computeTransformMatrixWithoutSpacing()
{
    float s[4] = {1.0,1.0,1.0,1.0};
    MiaAffineTransform affine(orientationMatrix,s,offset);


    MiaIrregularTransformAlgorithm* algorithm = transform.getTransformAlgorithm();
    if(algorithm)
    {
        algorithm = algorithm->clone();
        algorithm->setOutputSpacing(s);
    }
    MiaTransform t;
    t.setBulkTranform(affine);
    t.setTransformAlgorithm(algorithm);
    return t;

}

void MiaImageND::normalizeVector(float* var)
{
    float norm = sqrt( var[0]*var[0] + var[1]*var[1] + var[2]*var[2] );
    assert (norm != 0);
    var[0] = var[0]/norm;
    var[1] = var[1]/norm;
    var[2] = var[2]/norm;
}

void MiaImageND::printSummary()
{
    qDebug() << dimension << dimSize[0] << dimSize[1] << dimSize[2] << dimSize[3];
    qDebug() << "spacing" << spacing[0] << spacing[1] << spacing[2] << spacing[3];
    if(!isUsingSharedMemory)
        qDebug() << "bytes" << dataArray->count();
}

MiaPoint4D MiaImageND::convertVoxelToPatientCoordinate(MiaPoint4D inpt)
{
    return transform.toPatientCoordinate(inpt);
}

MiaPoint4D MiaImageND::convertPatientCoordinateToVoxel(MiaPoint4D inpt)
{
    return transform.fromPatientCoordinate(inpt);
}

void MiaImageND::getMaxMin(float &min, float &max)
{
    scanMaxMinValue();
    min = minValue;
    max = maxValue;
}
void MiaImageND::castToUChar(uchar* outport, float delta, float scale)
{
    isCastingToUnsignedValue = true;
    switch(dataType)
    {
        case IM_uchar:
            castTypeAtoB<uchar,uchar>((uchar*)dataPtr,outport, delta, scale);
            break;
        case IM_short:
            castTypeAtoB<short,uchar>((short*)dataPtr,outport, delta, scale);
            break;
        case IM_unsignedshort:
            castTypeAtoB<unsigned short,uchar>((unsigned short*)dataPtr,outport, delta, scale);
            break;
        case IM_int:
            castTypeAtoB<int,uchar>((int*)dataPtr,outport, delta, scale);
            break;
        case IM_float:
            castTypeAtoB<float,uchar>((float*)dataPtr,outport, delta, scale);
        case IM_RGBA:
            //undefined
            break;
         default:
            break;
    }
}

void MiaImageND::castToShort(short* outport, float delta, float scale)
{
    isCastingToUnsignedValue = false;
    switch(dataType)
    {
        case IM_uchar:
            castTypeAtoB<uchar,short>((uchar*)dataPtr,outport, delta, scale);
            break;
        case IM_short:
            castTypeAtoB<short,short>((short*)dataPtr,outport, delta, scale);
            break;
        case IM_unsignedshort:
            castTypeAtoB<unsigned short,short>((unsigned short*)dataPtr,outport, delta, scale);
            break;
        case IM_int:
            castTypeAtoB<int,short>((int*)dataPtr,outport, delta, scale);
            break;
        case IM_float:
            castTypeAtoB<float,short>((float*)dataPtr,outport, delta, scale);
        case IM_RGBA:
            //undefined
            break;
         default:
            break;
    }
}

void MiaImageND::castToUsignedShort(unsigned short* outport, float delta, float scale)
{
    isCastingToUnsignedValue = true;
    switch(dataType)
    {
        case IM_uchar:
            castTypeAtoB<uchar,unsigned short>((uchar*)dataPtr,outport, delta, scale);
            break;
        case IM_short:
            castTypeAtoB<short,unsigned short>((short*)dataPtr,outport, delta, scale);
            break;
        case IM_unsignedshort:
            castTypeAtoB<unsigned short,unsigned short>((unsigned short*)dataPtr,outport, delta, scale);
            break;
        case IM_int:
            castTypeAtoB<int,unsigned short>((int*)dataPtr,outport, delta, scale);
            break;
        case IM_float:
            castTypeAtoB<float,unsigned short>((float*)dataPtr,outport, delta, scale);
        case IM_RGBA:
            //undefined
            break;
         default:
            break;
    }
}

void MiaImageND::castToFloat(float* outport, float delta, float scale)
{
    isCastingToUnsignedValue = false;
    switch(dataType)
    {
        case IM_uchar:
            castTypeAtoB<uchar,float>((uchar*)dataPtr,outport, delta, scale);
            break;
        case IM_short:
            castTypeAtoB<short,float>((short*)dataPtr,outport, delta, scale);
            break;
        case IM_unsignedshort:
            castTypeAtoB<unsigned short,float>((unsigned short*)dataPtr,outport, delta, scale);
            break;
        case IM_int:
            castTypeAtoB<int,float>((int*)dataPtr,outport, delta, scale);
            break;
        case IM_float:
            castTypeAtoB<float,float>((float*)dataPtr,outport, delta, scale);
        case IM_RGBA:
            //undefined
            break;
         default:
            break;
    }
}

void MiaImageND::castToInt(int* outport, float delta, float scale)
{
    isCastingToUnsignedValue = false;
    switch(dataType)
    {
        case IM_uchar:
            castTypeAtoB<uchar,int>((uchar*)dataPtr,outport, delta, scale);
            break;
        case IM_short:
            castTypeAtoB<short,int>((short*)dataPtr,outport, delta, scale);
            break;
        case IM_unsignedshort:
            castTypeAtoB<unsigned short,int>((unsigned short*)dataPtr,outport, delta, scale);
            break;
        case IM_int:
            castTypeAtoB<int,int>((int*)dataPtr,outport, delta, scale);
            break;
        case IM_float:
            castTypeAtoB<float,int>((float*)dataPtr,outport, delta, scale);
        case IM_RGBA:
            //undefined
            break;
         default:
            break;
    }
}

template <class A, class B>
void MiaImageND::castTypeAtoB(A* indata, B* outdata, float delta, float scale)
{
    int size = getImageSize();
    int i;
    if(isCastingToUnsignedValue)
    {
        for(i = 0; i < size; i++)
        {
            A tempvalue = (*indata + delta) * scale;
            if(tempvalue < 0)
                tempvalue = 0;
            *outdata = (B)(tempvalue);
            indata++;
            outdata++;
        }
    }
    else
    {
        for(i = 0; i < size; i++)
        {
            *outdata = (B)((*indata + delta) * scale);
            indata++;
            outdata++;
        }
    }
}

void MiaImageND::copyGeometryInfoFrom( MiaImageND* other)
{
   setOffset( other->getOffset() );
   set3x3OrientationMatrix( other->getOrientationMatrix() );
   setSpacing( other->getSpacing() );
   transform = other->transform;
   needFlipUpDown = other->needFlipUpDown;
   needFlipLeftRight = other->needFlipLeftRight;
   return ;
}

bool MiaImageND::convert1DIndexTo3DVoxel(const int index, float *voxel) const
{
    int sliceSize = dimSize[0]*dimSize[1];

    if(index < 0 || index > sliceSize*dimSize[2])
    {
        return false;
    }
    else
    {
        int z = index/sliceSize;
        int y = floor((index - z*sliceSize)/dimSize[0]);
        int x = index - z*sliceSize - y*dimSize[0];
        voxel[0] = x; voxel[1] = y; voxel[2] = z;
    }
    return true;
}

bool MiaImageND::getGradientAt(float *voxelCoordinate, float *gradient)
{
    float coor[4];
    for( int dim = 0; dim < 4; dim++)
    {
        if(dim < dimension)
        {
            coor[dim]  = voxelCoordinate[dim];
        }
        else
            coor[dim]  = 0;
    }
    for( int dim = 0; dim < dimension; dim++)
    {
        if(coor[dim] <= 1 || coor[dim] >= dimSize[dim]-2)
        {
            gradient[dim] = 0.0f;
            continue;
        }
        coor[dim] += 1.0f;
        float rightv = getValueAt(coor);
        coor[dim] -= 2.0f;
        float leftv = getValueAt(coor);
        coor[dim] += 1.0f;
        gradient[dim] = (rightv - leftv)/2.0f;

    }
    return true;
}

void MiaImageND::scanMaxMinValue()
{
    switch(dataType)
    {
        case IM_uchar:
            scanMaxMinValueForTypeA<uchar>((uchar*)dataPtr,0,255);
            break;
        case IM_short:
            scanMaxMinValueForTypeA<short>((short*)dataPtr,-32768,32767);
            break;
        case IM_unsignedshort:
            scanMaxMinValueForTypeA<unsigned short>((unsigned short*)dataPtr,0,65535);
            break;
        case IM_int:
            scanMaxMinValueForTypeA<int>((int*)dataPtr,-2147483648, 2147483647);
            break;
        case IM_float:
            scanMaxMinValueForTypeA<float>((float*)dataPtr,-3.4e+38,3.4e+38);
        case IM_RGBA:
            //undefined
            break;
         default:
            break;
    }
}
template <class A>
void MiaImageND::scanMaxMinValueForTypeA(A* data,A typeMin, A typeMax)
{
    int size = getImageSize();
    int i;
    A max,min;
    min = typeMax;
    max = typeMin;
    for(i = 0; i < size; i++)
    {
        A tempvalue = *data ;
        if(tempvalue < min)
            min = tempvalue;
        if(tempvalue > max)
            max = tempvalue;
        data++;
    }
    minValue = min;
    maxValue = max;

}
float MiaImageND::getValueAt(float *voxelCoordinate)
{
    switch(dataType)
    {
        case IM_uchar:
            return getValueAt<uchar>(voxelCoordinate,(uchar*)dataPtr);
            break;
        case IM_short:
            return getValueAt<short>(voxelCoordinate,(short*)dataPtr);
            break;
        case IM_unsignedshort:
            return getValueAt<unsigned short>(voxelCoordinate,(unsigned short*)dataPtr);
            break;
        case IM_int:
            return getValueAt<int>(voxelCoordinate,(int*)dataPtr);
            break;
        case IM_float:
            return getValueAt<float>(voxelCoordinate,(float*)dataPtr);
        case IM_RGBA:
            //undefined
            break;
         default:
            break;
    }
    return 0;
}

void MiaImageND::setValueAt(float *voxelCoordinate, float value)
{
    int index = 0;
    int dimIncrement = 1;
    for(int dim = 0 ; dim < dimension; dim++)
    {
        int coor_int = (int)(voxelCoordinate[dim]);
        if(coor_int<0 || coor_int>=dimSize[dim])
            return;
        index += dimIncrement*coor_int;
        dimIncrement *= dimSize[dim];
    }
    switch(dataType)
    {
        case IM_uchar:
    {
            uchar* dataCharPtr =  (uchar*)dataPtr;
            dataCharPtr[index] = (uchar)(value + 0.5f);
    }
            break;
        case IM_short:
    {
            short* dataShortPtr =  (short*)dataPtr;
            dataShortPtr[index] = (short)(value + 0.5f);
    }
            break;
        case IM_unsignedshort:
    {
            unsigned short* dataUShortPtr =  (unsigned short*)dataPtr;
            dataUShortPtr[index] = (unsigned short)(value + 0.5f);
    }
            break;
        case IM_int:
    {
            int* dataIntrPtr =  (int*)dataPtr;
            dataIntrPtr[index] = (int)(value + 0.5f);
    }
            break;
        case IM_float:
    {
            float* dataFloatPtr =  (float*)dataPtr;
            dataFloatPtr[index] = (float)value;
    }
            break;
        case IM_RGBA:
            throw std::runtime_error("setValueAt doesn't support RGBA images");
            break;
         default:
            break;
    }
}
template <class A>
float MiaImageND::getValueAt(float *voxelCoordinate, A* inData)
{
    unsigned int dim;  // index over dimension

    /**
     * Compute base index = closet index below point
     * Compute distance from point to base index
     */
    int baseIndex[3] = {0,0,0};
    float distance[3];
    int tIndex;

    for( dim = 0; dim < dimension; dim++ )
    {
        // The following "if" block is equivalent to the following line without
        // having to call floor.
        //    baseIndex[dim] = (long) vcl_floor(index[dim] );
        if (voxelCoordinate[dim] >= 0.0)
        {
            baseIndex[dim] = (int) voxelCoordinate[dim];
        }
        else
        {
            tIndex = (int) voxelCoordinate[dim];
            if (float(tIndex) != voxelCoordinate[dim])
            {
              tIndex--;
            }
            baseIndex[dim] = tIndex;
        }
        distance[dim] = voxelCoordinate[dim] - float( baseIndex[dim] );
    }

    /**
     * Interpolated value is the weighted sum of each of the surrounding
     * neighbors. The weight for each neighbor is the fraction overlap
     * of the neighbor pixel with respect to a pixel centered on point.
     */
    float value = 0.0f;
    float totalOverlap = 0.0f;


    for( unsigned int counter = 0; counter < numberOfNeighbors; counter++ )
    {

      float overlap = 1.0;          // fraction overlap
      unsigned int upper = counter;  // each bit indicates upper/lower neighbour
      int neighIndex[3] = {0,0,0};

      // get neighbor index and overlap fraction
      for( dim = 0; dim < dimension; dim++ )
      {

        if ( upper & 1 )
        {
          neighIndex[dim] = baseIndex[dim] + 1;
          overlap *= distance[dim];
        }
        else
        {
          neighIndex[dim] = baseIndex[dim];
          overlap *= 1.0 - distance[dim];
        }

        upper >>= 1;

      }

      // get neighbor value only if overlap is not zero
      if( overlap )
      {
          bool isNeighIndexValid = true;
          for(int j = 0; j < dimension; j++)
              if(neighIndex[j]<0 || neighIndex[j]>= dimSize[j])
                  isNeighIndexValid = false;
          if(isNeighIndexValid)
            value += overlap * inData[ neighIndex[2] * dimSize[0] * dimSize[1] + neighIndex[1] * dimSize[0] + neighIndex[0]];
          else
           value += overlap * outBoundaryValue;
          totalOverlap += overlap;
      }

      if( totalOverlap == 1.0 )
      {
          // finished
           break;
      }

    }

    return value;
}

void MiaImageND::resetValueToZero()
{
    memset(dataPtr,0,dimSize[0]*dimSize[1]*dimSize[2]*dimSize[3]*bytePerElement);
}
