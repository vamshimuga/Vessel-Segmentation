#include "miadistancetransformfrom3dmarkers.h"
#include "MiaCoreModules/miamarker.h"
#include "LevelsetKernels/abstractlevelsetspeedfucntion.h"
#define max(x,y)    ((x>y) ? x : y )
#define min(x,y)    ((x<y) ? x : y )

MiaDistanceTransformFrom3DMarkers::MiaDistanceTransformFrom3DMarkers(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Distance Transform based on 3D balls";
    autoUpdateWhenInputChanged = false;
    autoUpdateWhenParameterChanged = false;

}

bool MiaDistanceTransformFrom3DMarkers::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        emit getInputMarkers(markerList);
        int i;
        if(markerList.size()<1)
            return false;

    }
    else
        return false;
    return true;
}

bool MiaDistanceTransformFrom3DMarkers::runOperation()
{
#ifdef USESHORTTYPELEVELSETFUNCTION
    short* distmap = (short*) outputImage->getDataPtr();
#else
    float* distmap = (float*) outputImage->getDataPtr();
#endif

    QList <MiaMarker*> ballList;
    QList <MiaMarker*> boxList;
    int i;
    for( i = 0; i < markerList.size(); i++ )
    {
        if (markerList.at(i)->type == MIA_Line || markerList.at(i)->type == MIA_3DBall || markerList.at(i)->type == MIA_Point)
        {
            ballList.push_back(markerList.at(i));
        }
        else if (markerList.at(i)->type == MIA_3DBox)
        {
            boxList.push_back(markerList.at(i));
        }
    }

    int num = ballList.size() + boxList.size();
    if( num < 1)
    {
        qDebug()<<"no enough seeds";
        return false;
    }

    QProgressDialog progress("Generating Distance Map...", 0, 0, (num+1)*3,0,Qt::WindowStaysOnTopHint );
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    int sliceSize = width*height;
    int totalSize = inputImage->getImageSize();
    int dimension = inputImage->getDimension();
#pragma omp parallel for
    for( i = 0 ; i < totalSize;  i++ )
    {
        distmap [i] = FARAWAY;
    }
    progress.setValue(3);

    if(ballList.count() > 0)
        distanceTransformFor3DBalls(ballList, &progress);
    if(boxList.count() > 0)
        distanceTransformFor3DBoxs(boxList, &progress);

    std::vector<int> zerocrossingpts;
    zerocrossingpts.reserve(sliceSize*2);
    int curindex;
    for( i = 0 ; i < num ; i++)
    {
        int box[6];
        getActiveBoundingRegionForMarker( markerList.at(i), box);
        int x,y,z;
        for( z = box[4] ; z <= box[5] ; z++)
            for(y = box[2]; y <= box[3] ; y ++ )
                for( x = box[0] ; x <= box[1];  x++ )
                {
                    curindex = z*sliceSize + y*width + x;
                    float curvalue = *(distmap + curindex);
                    float neighborValue;
                    if ( curvalue > 0)
                    {
                        if(x>1)
                        {
                            neighborValue = *(distmap + curindex - 1);
                            if(neighborValue <= 0)
                            {
                                if ( curvalue < -neighborValue)
                                    zerocrossingpts.push_back(curindex);
                                else
                                    zerocrossingpts.push_back(curindex - 1);
                            }
                        }
                        if(x < width - 1)
                        {
                            neighborValue = *(distmap + curindex + 1);
                            if(neighborValue <= 0)
                            {
                                if ( curvalue <= -neighborValue)
                                    zerocrossingpts.push_back(curindex);
                                else
                                    zerocrossingpts.push_back(curindex + 1);
                            }
                        }
                        if(y>1)
                        {
                            neighborValue = *(distmap + curindex - width);
                            if(neighborValue <= 0)
                            {
                                if ( curvalue < -neighborValue)
                                    zerocrossingpts.push_back(curindex);
                                else
                                    zerocrossingpts.push_back(curindex - width);
                            }
                        }
                        if(y < height - 1)
                        {
                            neighborValue = *(distmap + curindex + width);
                            if(neighborValue <= 0)
                            {
                                if ( curvalue <= -neighborValue)
                                    zerocrossingpts.push_back(curindex);
                                else
                                    zerocrossingpts.push_back(curindex + width);
                            }
                        }
                        if(z>1)
                        {
                            neighborValue = *(distmap + curindex - sliceSize);
                            if(neighborValue <= 0)
                            {
                                if ( curvalue < -neighborValue)
                                    zerocrossingpts.push_back(curindex);
                                else
                                    zerocrossingpts.push_back(curindex - sliceSize);
                            }
                        }
                        if(z < depth - 1)
                        {
                            neighborValue = *(distmap + curindex + sliceSize);
                            if(neighborValue <= 0)
                            {
                                if ( curvalue <= -neighborValue)
                                    zerocrossingpts.push_back(curindex);
                                else
                                    zerocrossingpts.push_back(curindex + sliceSize);
                            }
                        }


                    }
                    else
                    {
                        if(dimension == 3)
                        {
                            if(x == 0 || x == width-1 || y == 0 || y == height-1 || z == 0 || z == depth -1)
                                zerocrossingpts.push_back(curindex);
                        }
                        else
                        {
                            if(x == 0 || x == width-1 || y == 0 || y == height-1)
                                zerocrossingpts.push_back(curindex);
                        }
                    }
                }
        progress.setValue(progress.value()+1);

    }
    for( i = 0 ; i < num ; i++)
    {
        int box[6];
        getActiveBoundingRegionForMarker( markerList.at(i), box);
        int x,y,z;
        for( z = box[4] ; z <= box[5] ; z++)
            for(y = box[2]; y <= box[3] ; y ++ )
                for( x = box[0] ; x <= box[1];  x++ )
                {
                    if( *(distmap + z*sliceSize + y*width + x) > 0)
                    {
                        *(distmap + z*sliceSize + y*width + x) =  FARAWAY;
                    }
                    else
                    {
                         *(distmap + z*sliceSize + y*width + x) =  -FARAWAY;
                    }
                    //else dot is out of circle, do nothing

                }
        progress.setValue(progress.value()+1);

    }

    std::vector<int>::const_iterator iter;
    for(iter=zerocrossingpts.begin();iter!=zerocrossingpts.end();iter++)
    {
        int ind=*iter;
        *(distmap + ind) = 0;
    }

    return true;



}


bool MiaDistanceTransformFrom3DMarkers::reAllocOutputImage()
{
    if(outputImage)
        outputImage->release();
#ifdef USESHORTTYPELEVELSETFUNCTION
    outputImage = creatEmptyImageFrom(inputImage, IM_short);
#else
    outputImage = creatEmptyImageFrom(inputImage, IM_float);
#endif

    if(outputImage)
    {
        outputImage->retain();
        return true;
    }
    else
        return false;
}

void MiaDistanceTransformFrom3DMarkers::distanceTransformFor3DBalls(QList<MiaMarker *> &mlist, QProgressDialog *dlg)

{
#ifdef USESHORTTYPELEVELSETFUNCTION
    short* distmap = (short*) outputImage->getDataPtr();
#else
    float* distmap = (float*) outputImage->getDataPtr();
#endif
    int num = mlist.size();
    if( num < 1)
    {
        qDebug()<<"no enough balls";
        return;
    }
    MiaMarker* ball;
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    int sliceSize = width*height;
    int i;
    int dimension = inputImage->getDimension();

    int progressstep = dlg->value();

    float* spacing = inputImage->getSpacing();

    for( i = 0 ; i < num ; i++)
    {
        ball = mlist.at(i);
        MiaPoint4D cpt = ball->getKeyPointAt( 0 );
        cpt = inputImage->convertPatientCoordinateToVoxel(cpt);
        float radius = ball->getSeedRegionRadius();
        int box[6];
        getActiveBoundingRegionForMarker( markerList.at(i), box);
        int x,y,z;
        for( z = box[4] ; z <= box[5] ; z++)
            for(y = box[2]; y <= box[3] ; y ++ )
                for( x = box[0] ; x <= box[1];  x++ )
                {

                    float curdist = *(distmap + z*sliceSize + y*width + x);
#ifdef USESHORTTYPELEVELSETFUNCTION
                    curdist *= 0.0001f;
#endif
                    float dist = ( (float)x - cpt.pos[0] )*( (float)x - cpt.pos[0] )*spacing[0]*spacing[0]
                                    + ( (float)y - cpt.pos[1] )*( (float)y - cpt.pos[1] )*spacing[1]*spacing[1]
                                    + ( (float)z - cpt.pos[2] )*( (float)z - cpt.pos[2] )*spacing[2]*spacing[2];
                    dist = sqrt(dist) - radius;
                    if(curdist > dist)
                    {
#ifdef USESHORTTYPELEVELSETFUNCTION
                        int scaled_dist = dist*10000.0f;
                        if(scaled_dist > FARAWAY)
                            scaled_dist = FARAWAY;
                        else if(scaled_dist < -FARAWAY)
                            scaled_dist = -FARAWAY;
                        *(distmap + z*sliceSize + y*width + x) =  (short)scaled_dist;
#else
                        *(distmap + z*sliceSize + y*width + x) =  dist;
#endif
                    }
                    //else dot is out of circle, do nothing

                }

        dlg->setValue(progressstep+i);

    }

}

void MiaDistanceTransformFrom3DMarkers::distanceTransformFor3DBoxs(QList<MiaMarker *> &mlist, QProgressDialog *dlg)
{
#ifdef USESHORTTYPELEVELSETFUNCTION
    short* distmap = (short*) outputImage->getDataPtr();
#else
    float* distmap = (float*) outputImage->getDataPtr();
#endif
    int num = mlist.size();
    if( num < 1)
    {
        qDebug()<<"no enough balls";
        return;
    }
    MiaMarker* marker;
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    int sliceSize = width*height;
    int i;
    int dimension = inputImage->getDimension();

    int progressstep = dlg->value();

    float* spacing = inputImage->getSpacing();


    for( i = 0 ; i < num ; i++)
    {
        marker = mlist.at(i);

        int box[6];
        getActiveBoundingRegionForMarker( markerList.at(i), box);

        MiaPoint4D pt1,pt2,pt3;
        pt1= inputImage->convertPatientCoordinateToVoxel(marker->keyPointList[0]);
        pt2= inputImage->convertPatientCoordinateToVoxel(marker->keyPointList[1]);
        pt3= inputImage->convertPatientCoordinateToVoxel(marker->keyPointList[2]);
        bool aligned = false;
        for(int dim = 0; dim < 3; dim++)
        {
            if(pt1.pos[dim] == pt2.pos[dim] && pt2.pos[dim] == pt3.pos[dim])
                aligned = true;
        }


        int x,y,z;
        if(aligned)
        {

            pt2= inputImage->convertPatientCoordinateToVoxel(marker->keyPointList[6]);
            float box_width = fabs(pt2.pos[0] - pt1.pos[0])*spacing[0];
            float box_height = fabs(pt2.pos[1] - pt1.pos[1])*spacing[1];
            float box_depth = fabs(pt2.pos[2] - pt1.pos[2])*spacing[2];
            MiaPoint4D cpt = (pt1 + pt2)*0.5;
            for( z = box[4] ; z <= box[5] ; z++)
                for(y = box[2]; y <= box[3] ; y ++ )
                    for( x = box[0] ; x <= box[1];  x++ )
                    {
                        float dist_x = fabs((float)x-cpt.pos[0])*spacing[0] - box_width*0.5f;
                        float dist_y = fabs((float)y-cpt.pos[1])*spacing[1] - box_height*0.5f;
                        float dist_z = fabs((float)z-cpt.pos[2])*spacing[2] - box_depth*0.5f;
                        float dist = 0;
                        if(dist_x <= 0 && dist_y <= 0 && dist_z <=0)
                        {
                            dist = max(dist_x,dist_y);
                            dist = max(dist,dist_z);
                        }
                        else
                        {
                            if(dist_x > 0)
                                dist += dist_x*dist_x;
                            if(dist_y > 0)
                                dist += dist_y*dist_y;
                            if(dist_z > 0)
                                dist += dist_z*dist_z;
                            dist = sqrt(dist);
                        }

                        float curdist = *(distmap + z*sliceSize + y*width + x);
#ifdef USESHORTTYPELEVELSETFUNCTION
                        curdist *= 0.0001f;
#endif
                        if(curdist > dist)
                        {
#ifdef USESHORTTYPELEVELSETFUNCTION
                            int scaled_dist = dist*10000.0f;
                            if(scaled_dist > FARAWAY)
                                scaled_dist = FARAWAY;
                            else if(scaled_dist < -FARAWAY)
                                scaled_dist = -FARAWAY;
                            *(distmap + z*sliceSize + y*width + x) =  (short)scaled_dist;
#else
                            *(distmap + z*sliceSize + y*width + x) =  dist;
#endif
                        }

                    }
        }
        else
        {
            for( z = box[4] ; z <= box[5] ; z++)
                for(y = box[2]; y <= box[3] ; y ++ )
                    for( x = box[0] ; x <= box[1];  x++ )
                    {
                        MiaPoint4D pt(x,y,z,0);
                        pt = inputImage->convertVoxelToPatientCoordinate(pt);
                        float dist = marker->getDistanceToSurface(pt);
                        float curdist = *(distmap + z*sliceSize + y*width + x);
#ifdef USESHORTTYPELEVELSETFUNCTION
                        curdist *= 0.0001f;
#endif
                        if(curdist > dist)
                        {
#ifdef USESHORTTYPELEVELSETFUNCTION
                            int scaled_dist = dist*10000.0f;
                            if(scaled_dist > FARAWAY)
                                scaled_dist = FARAWAY;
                            else if(scaled_dist < -FARAWAY)
                                scaled_dist = -FARAWAY;
                            *(distmap + z*sliceSize + y*width + x) =  (short)scaled_dist;
#else
                            *(distmap + z*sliceSize + y*width + x) =  dist;
#endif
                        }

                    }
        }

        dlg->setValue(progressstep+i);

    }

}

void MiaDistanceTransformFrom3DMarkers::getActiveBoundingRegionForMarker(MiaMarker *marker, int *box)
{
    float* spacing = inputImage->getSpacing();
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    if (marker->type == MIA_Line || marker->type == MIA_3DBall || marker->type == MIA_Point)
    {
        float radius = marker->getSeedRegionRadius();
        if(radius <= 0)
        {
            box[0] = box[1] = box[2] = box[3] = box[4] = box[5] = 0;
            return;
        }
        MiaPoint4D cpt = marker->getKeyPointAt( 0 );
        cpt = inputImage->convertPatientCoordinateToVoxel(cpt);
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
        box[0] = x_start; box[1] = x_end;
        box[2] = y_start; box[3] = y_end;
        box[4] = z_start; box[5] = z_end;
    }
    else if(marker->type == MIA_3DBox)
    {
        MiaPoint4D pt1 = marker->getKeyPointAt( 0 );
        pt1 = inputImage->convertPatientCoordinateToVoxel(pt1);
        box[0] = box[1] = pt1.pos[0];
        box[2] = box[3] = pt1.pos[1];
        box[4] = box[5] = pt1.pos[2];
        for(int i = 1; i < marker->getKeyPointNumber(); i++)
        {
            MiaPoint4D pt = marker->getKeyPointAt( i );
            pt = inputImage->convertPatientCoordinateToVoxel(pt);
            if(box[0] > pt.pos[0])
                box[0] = pt.pos[0];
            if(box[1] < pt.pos[0])
                box[1] = pt.pos[0];
            if(box[2] > pt.pos[1])
                box[2] = pt.pos[1];
            if(box[3] < pt.pos[1])
                box[3] = pt.pos[1];
            if(box[4] > pt.pos[2])
                box[4] = pt.pos[2];
            if(box[5] < pt.pos[2])
                box[5] = pt.pos[2];
        }
        box[0] -= 3; if(box[0] < 0) box[0] = 0;
        box[1] += 3; if(box[1] >= width) box[1] = width - 1;
        box[2] -= 3; if(box[2] < 0) box[2] = 0;
        box[3] += 3; if(box[3] >= height) box[3] = height - 1;
        box[4] -= 3; if(box[4] < 0) box[4] = 0;
        box[5] += 3; if(box[5] >= depth) box[5] = depth - 1;
    }
    else
    {
        throw std::runtime_error("Unsupported maker type!");
    }
}

void MiaDistanceTransformFrom3DMarkers::reset()
{
    if(outputImage == NULL)
        return;
#ifdef USESHORTTYPELEVELSETFUNCTION
    short* distmap = (short*) outputImage->getDataPtr();
#else
    float* distmap = (float*) outputImage->getDataPtr();
#endif
    int totalSize = outputImage->getImageSize();
    int i;
#pragma omp parallel for
    for( i = 0 ; i < totalSize;  i++ )
    {
        distmap [i] = FARAWAY;
    }
}
