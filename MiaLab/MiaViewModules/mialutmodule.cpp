#include "mialutmodule.h"
#include <stdexcept>

MiaLUTModule::MiaLUTModule(LUTType type, QObject *parent) :
    QObject(parent)
{
    lutResolution = 1024;
    lutMinValue = 0;
    lutBinSize = 255.0f/lutResolution;
    lookupTable = (QRgb*) malloc ( lutResolution * sizeof( QRgb ) );
    lookupTableType = type;
    switch (type)
    {
    case LUT_BinaryColor:
        insertKeyPoint( 0.0, QColor(Qt::transparent));
        insertKeyPoint( 1, QColor(255,0,0, 120));
        break;
    case LUT_SingleColor:
        insertKeyPoint( 0.0, QColor(Qt::black));
        insertKeyPoint( 255, QColor(Qt::white));
        break;
    case LUT_RainBow:
        insertKeyPoint( 0.0, QColor(Qt::black));
        insertKeyPoint( 63.0, QColor(Qt::red));
        insertKeyPoint( 127.0, QColor(Qt::green));
        insertKeyPoint( 191.0, QColor(Qt::blue));
        insertKeyPoint( 255.0, QColor(Qt::magenta));
        break;

    }


}

void MiaLUTModule::insertKeyPoint( float intensity, QColor color )
{
    int index = searchIndexOfValue( intensity );
    LUTNode node;
    node.intensity = intensity;
    node.color = color;
    keyPointsVector.insert(index, node);
    updateLUTVector();
}

QRgb MiaLUTModule::getColorForValue( float intensity )
{
    float x1, x2;
    QColor c1, c2;
    float y1, y2;
    int r,g,b,a;
    int index = searchIndexOfValue( intensity );
    if( index < 1 )
        return keyPointsVector.first().color.rgba();
    if( index >= keyPointsVector.size() )
        return keyPointsVector.last().color.rgba();

    x1 = keyPointsVector.at( index - 1 ).intensity;
    x2 = keyPointsVector.at( index ).intensity;
    c1 = keyPointsVector.at( index - 1 ).color;
    c2 = keyPointsVector.at( index ).color;

    y1 = c1.red();
    y2 = c2.red();

    r = roundf( y1 + (y2 - y1)*(intensity - x1)/(x2 - x1) );

    y1 = c1.green();
    y2 = c2.green();

    g = roundf( y1 + (y2 - y1)*(intensity - x1)/(x2 - x1) );

    y1 = c1.blue();
    y2 = c2.blue();

    b = roundf( y1 + (y2 - y1)*(intensity - x1)/(x2 - x1) );

    y1 = c1.alpha();
    y2 = c2.alpha();

    a = roundf( y1 + (y2 - y1)*(intensity - x1)/(x2 - x1) );

    return qRgba(r,g,b,a);

}

void MiaLUTModule::resetKeyPoint( int index, float intensity, QColor color )
{
    LUTNode node;
    node.intensity = intensity;
    node.color = color;
    keyPointsVector.replace( index, node);
    updateLUTVector();

}

void MiaLUTModule::removeKeyPoint( int index )
{

    keyPointsVector.remove( index );
    updateLUTVector();
}

void MiaLUTModule::removeAllKeyPoints()
{
    keyPointsVector.clear();
}

int MiaLUTModule::searchIndexOfValue( float intensity )
{
    int i;
    if(keyPointsVector.size() == 0)
        return 0;
    for ( i = 0; i < keyPointsVector.size(); i++ )
    {
        if( intensity < keyPointsVector.at(i).intensity )
            break;
    }

    return i;
}


void MiaLUTModule::updateLUTVector()
{
    float min = keyPointsVector.first().intensity;
    float max = keyPointsVector.last().intensity;
    lutMinValue = min;
    lutBinSize = (max-min)/lutResolution;
    int i;
    for ( i = 0 ; i < lutResolution ; i++ )
    {
        float value = (float)i * lutBinSize + lutMinValue;
        lookupTable[i] = getColorForValue( value );

    }
}

void MiaLUTModule::processingImage( QImage* img, float* indata, float ww, float wl )
{
    if( lookupTableType == LUT_BinaryColor )
    {
        processingBinaryImage(img, indata);
        return;
    }

    int i;
//    #pragma omp parallel for
    for( i = 0 ; i < img->height() ; i++)
    {
        QRgb* out = (QRgb*)img->scanLine(i);
        float* in = indata + i*img->width();

        int j;
        for(j = 0 ; j < img->width() ; j++)
        {
            float  value = (*(in +j) - wl)/ww *lutResolution + lutResolution/2 ;
            int index = roundf(value);
            if(index<0)
                index = 0;
            else if(index >= lutResolution)
                index = lutResolution - 1;
//            assert(index >= 0 && index < lutResolution);
            *out++ = lookupTable[index];

        }
    }
    return ;
}

void MiaLUTModule::processingBinaryImage( QImage* img, float* indata)
{
    QRgb incolor;
    QRgb outcolor;
    incolor = keyPointsVector.first().color.rgba();
    outcolor = keyPointsVector.last().color.rgba();

    int i;
//    #pragma omp parallel for
    for( i = 0 ; i < img->height() ; i++)
    {
        QRgb* out = (QRgb*) img->scanLine(i);
        float* in = indata + i*img->width();

        int j;
        for(j = 0 ; j < img->width() ; j++)
        {
            float  value = *(in +j) ;
            if(value == 0.0)
            {
                *out++ = incolor;
            }
            else

            {
                *out++ = outcolor;
            }

        }
    }
    return ;
}

void MiaLUTModule::getDefaultWindowSetting(float& ww, float& wl)
{
    if(keyPointsVector.size() < 2)
        throw std::runtime_error("Invalid LUT");
    float min = keyPointsVector.first().intensity;
    float max = keyPointsVector.last().intensity;
    ww = max-min;
    wl = (max+min)*0.5;
}

