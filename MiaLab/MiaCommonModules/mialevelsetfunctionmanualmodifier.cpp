#include "mialevelsetfunctionmanualmodifier.h"
#include <QProgressDialog>


MiaLevelSetFunctionManualModifier::MiaLevelSetFunctionManualModifier(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Level Set Manual Modifier";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;
}


bool MiaLevelSetFunctionManualModifier::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        emit getInputMarkers(markerList);
        int i;
        for( i = 0; i < markerList.size(); i++ )
        {
            if (  markerList.at(i)->type != MIA_Line && markerList.at(i)->type != MIA_3DBall && markerList.at(i)->type != MIA_Point)
            {
                markerList.removeAt(i);
                i--;
            }
        }
    }
    else
        return false;
    return true;

}

bool MiaLevelSetFunctionManualModifier::runOperation()
{

    int num = markerList.size();
    if( num < 1)
    {
        return true;
    }
    float* externalMap = (float*) outputImage->getDataPtr();
    MiaMarker* ball;

    QProgressDialog progress("Modifying Level Set Fucntion...", 0, 0, num,0,Qt::WindowStaysOnTopHint );
    progress.setWindowModality(Qt::WindowModal);
    int progressstep = 0;

    progress.setValue(progressstep++);
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    int sliceSize = width*height;

    float* spacing = inputImage->getSpacing();
    int i;
    for( i = 0 ; i < num ; i++)
    {
        ball = markerList.at(i);
        MiaPoint4D cpt = ball->getKeyPointAt( 0 );
        cpt = inputImage->convertPatientCoordinateToVoxel(cpt);
        float radius = ball->getSeedRegionRadius();
        if(radius <= 0) continue;
        float radius_sqr = radius*radius;
        int x_start,x_end,y_start,y_end,z_start,z_end;
        x_start = cpt.pos[0] - radius/spacing[0] - 2.5;
        x_end = cpt.pos[0] + radius/spacing[0] + 2.5;
        if(x_start < 0) x_start = 0; if(x_start >= width) x_start = width-1;
        if(x_end < 0) x_end = 0; if(x_end >= width) x_end = width-1;
        y_start = cpt.pos[1] - radius/spacing[1] - 2.5;
        y_end = cpt.pos[1] + radius/spacing[1] + 2.5;
        if(y_start < 0) y_start = 0; if(y_start >= height) y_start = height-1;
        if(y_end < 0) y_end = 0; if(y_end >= height) y_end = height-1;
        z_start = cpt.pos[2] - radius/spacing[2] - 2.5;
        z_end = cpt.pos[2] + radius/spacing[2] + 2.5;
        if(z_start < 0) z_start = 0; if(z_start >= depth) z_start = depth-1;
        if(z_end < 0) z_end = 0; if(z_end >= depth) z_end = depth-1;
        int x,y,z;

        for( z = z_start ; z < z_end ; z++)
            for(y = y_start; y < y_end ; y ++ )
                for( x = x_start ; x < x_end;  x++ )
                {
                    float dist_sqr = ( (float)x - cpt.pos[0] )*( (float)x - cpt.pos[0] )*spacing[0]*spacing[0]
                                    + ( (float)y - cpt.pos[1] )*( (float)y - cpt.pos[1] )*spacing[1]*spacing[1]
                                    + ( (float)z - cpt.pos[2] )*( (float)z - cpt.pos[2] )*spacing[2]*spacing[2];
                    if(dist_sqr <= radius_sqr)
                    {
                        *(externalMap + z*sliceSize + y*width + x) =  -1.0;
                    }
                    //else dot is out of circle, do nothing

                }
        progress.setValue(progressstep++);

    }
    return true;
}
bool MiaLevelSetFunctionManualModifier::reAllocOutputImage()
{
    if(outputImage)
        outputImage->release();
    outputImage = inputImage;

    if(outputImage)
    {
        outputImage->retain();
        return true;
    }
    else
        return false;
}
