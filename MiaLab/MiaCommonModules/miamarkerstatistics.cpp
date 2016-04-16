#include "miamarkerstatistics.h"
#include "MiaMarkers/miamarker3dpolygon.h"
#include "regression/nr3.h"
#include "regression/sort.h"
#include "regression/fitmed.h"

#define max(x,y)    ((x>y) ? x : y )
#define min(x,y)    ((x<y) ? x : y )

MiaMarkerStatistics::MiaMarkerStatistics(MiaNetwork *parent) :
    MiaModule(parent)
{
     moduleName = "Marker Statistics";
     currentStatisticsMethod = MIA_NormalStatistics;
     mean = 0;
     standard_deviation = 0;
     max = 0; min = 0;
     number_voxel = 0;
     volume = 0;
     area = 0;
     wiredParameters.insert(QString("meanvalue"), mean);
     wiredParameters.insert(QString("lowerthrehold"), mean - standard_deviation );
     wiredParameters.insert(QString("upperthrehold"), mean + standard_deviation );
     wiredParameters.insert(QString("out_mean"), 0);
     wiredParameters.insert(QString("out_std"),  0 );
     wiredParameters.insert(QString("in_mean"), 0 );
     wiredParameters.insert(QString("in_std"), 0 );
     wiredParameters.insert(QString("endPointIndex"),0);
}


bool MiaMarkerStatistics::runOperation()
{
    bool result = false;

    ImageType sourcetype = inputImage->getType();

    switch(sourcetype)
    {
        case IM_uchar:
        {
            uchar* in = (uchar*) inputImage->getDataPtr();
            result = runOperation<uchar>(in);
        }
            break;
        case IM_short:
        {
            short* in = (short*) inputImage->getDataPtr();
            result = runOperation<short>(in);
        }
        break;
        case IM_unsignedshort:
        {
            unsigned short* in = (unsigned short*) inputImage->getDataPtr();
            result = runOperation<unsigned short>(in);
        }
        break;
        case IM_int:
        {
            int* ini = (int*) inputImage->getDataPtr();
            result = runOperation<int>(ini);
        }
            break;
        case IM_float:
        {
            float* inf = (float*) inputImage->getDataPtr();
            result = runOperation<float>(inf);
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

template <class A>
bool MiaMarkerStatistics::runOperation(A* inData)
{
    int i;
    if(markerList.size()<1)
        return false;
//    if(currentStatisticsMethod == MIA_ReadKeyValues)
//        scanBifurcations();
    for( i = 0; i < markerList.size(); i++ )
    {
        switch(currentStatisticsMethod)
        {
        case MIA_NormalStatistics:
            normalStatistics(markerList.at(i),inData);
            break;
        case MIA_LineProfile:
//            lineProfile(markerList.at(i),inData);
            crossHairProfile(markerList.at(i), inData);
            break;
        case MIA_ReadKeyValues:
            readKeyValues(markerList.at(i),inData);

        }

    }

    return true;
}


template <class A>
void MiaMarkerStatistics::readKeyValues(MiaMarker* marker, A* inData)
{
    MiaPoint4D lastpt;
    VecDoub x = VecDoub( marker->getKeyPointNumber() ); //this data structure is specific from numerical recipes
    VecDoub y = VecDoub( marker->getKeyPointNumber() );
    double curr_dist = 0;

    marker->keyValueList.clear();
    if ( marker->getKeyPointNumber() > 0 )
    {
        lastpt = inputImage->convertPatientCoordinateToVoxel(marker->getKeyPointAt(0));

    }
    else
    {
        return;
    }
//     qDebug()<<"Before fitting";
    for (int i = 0; i < marker->getKeyPointNumber(); i++)
    {
        MiaPoint4D pt = marker->getKeyPointAt(i);
        pt = inputImage->convertPatientCoordinateToVoxel(pt);

        float value = inputImage->getValueAt(pt.pos);
        value = -value-1.0;
        if(value>0)
            value = 0;
        curr_dist = curr_dist + lastpt.distance(pt);
        x[i] = curr_dist;
        y[i] = value;
        lastpt = pt;
//        qDebug()<<x[i]<<y[i];
    }

    //robust regression from numerical recipes
    float last_a = 10.0, last_b = 0.0;
//    if(soomthingWindow>100)
//    {
//        MiaMarker3DPolygon* centerline = (MiaMarker3DPolygon*)marker;
//        if(centerline->bifurcationNodeList.size()<2)
//        {
//            qDebug()<<"Single Segment Robust fitting";
//            Fitmed f(x, y);
//            qDebug()<<f.b<<f.a;
//            if(f.b<0)
//            {
//                f.a = f.a + f.b * curr_dist/2.0;
//                f.b = 0;
//                qDebug()<<f.b<<f.a;
//            }

//            //modify the kewValueList with the values obtained from the regression
//            for (int i = 0; i < marker->getKeyPointNumber(); i++)
//            {
//                Point4D v;
//                v.pos[3] = f.a + f.b * x[i];
//                if(v.pos[3]>-1.0)
//                    v.pos[3] = -1.0;
//                marker->keyValueList.append(v);
//        //        qDebug()<<x[i]<<v.pos[3];
//            }
//        }
//        else
//        {
//            qDebug()<<"Multipele Segment Robust fitting";
//            for(int seg = 0; seg < centerline->bifurcationNodeList.size() -1; seg++)
//            {
//                int start = centerline->bifurcationNodeList.at(seg);
//                int end = centerline->bifurcationNodeList.at(seg+1);
//                VecDoub sx = VecDoub( end - start );
//                VecDoub sy = VecDoub( end - start );
//                for(int i = start; i <end ; i ++)
//                {
//                    sx[i-start] = x[i];
//                    sy[i-start] = y[i];
//                }
//                Fitmed f(sx, sy);
//                qDebug()<<f.b<<f.a;
//                if(f.b<0)
//                {
//                    f.a = f.a + f.b * x[(start+end)/2]/2.0;
//                    f.b = 0;
//                    qDebug()<<f.b<<f.a;
//                }

//                //modify the kewValueList with the values obtained from the regression
//                for(int i = start; i <end ; i ++)
//                {
//                    Point4D v;
//                    v.pos[3] = f.a + f.b * x[i];
//                    if(v.pos[3]>-1.0)
//                        v.pos[3] = -1.0;
//                    marker->keyValueList.append(v);
////                    qDebug()<<x[i]<<v.pos[3];
//                }
//                if(end == marker->getKeyPointNumber()-1)
//                {
//                    Point4D v;
//                    v.pos[3] = f.a + f.b * x[end];
//                    if(v.pos[3]>-1.0)
//                        v.pos[3] = -1.0;
//                    marker->keyValueList.append(v);

//                }
//            }
//            qDebug()<<marker->keyPointList.size();
//            qDebug()<<marker->keyValueList.size();

//        }
//        return;
//    }

    int local_n = soomthingWindow;
    for(int i = 0; i < marker->getKeyPointNumber(); i++)
    {
        int pre_n,next_n;
        float localmean = 0.0;
        pre_n = max(0,i-local_n);
        next_n = min(marker->getKeyPointNumber(), i+local_n);
        for(int j = pre_n; j < next_n; j++)
        {
            localmean += y[j];
        }
        localmean = localmean/(float)(next_n - pre_n);
        MiaPoint4D v;
        v.pos[3] = localmean - 0.5; //0.5 is a bias field if too large the cylinder will grow forever, if too small the cynlinder will disappear
                                    //apperantly using cubic of the distance function will help to stablize this behavior
        if(v.pos[3]>-1.0)
            v.pos[3] = -1.0;
        marker->keyValueList.append(v);
//        qDebug()<<y[i]<<v.pos[3];
    }

}

template <class A>
void MiaMarkerStatistics::normalStatistics(MiaMarker* marker, A* inData)
{
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    int sliceSize = width*height;
    if ( marker->type == MIA_Point)
    {
        MiaPoint4D pt = marker->getKeyPointAt(0);
        pt = inputImage->convertPatientCoordinateToVoxel(pt);
        int x,y,z;
        x = pt.pos[0]; y = pt.pos[1]; z = pt.pos[2];
        int ind = z*sliceSize + y*width + x;
        mean = inData[ind];

        wiredParameters.insert(QString("meanvalue"), mean);
        qDebug()<<mean;
        emit parameterChanged(wiredParameters);
    }
}

template <class A>
void MiaMarkerStatistics::crossHairProfile(MiaMarker* marker, A* inData)
{
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    if(marker->type == MIA_Line)
    {
        MiaPoint4D pt1 = marker->getKeyPointAt(0);
        MiaPoint4D pt2 = marker->getKeyPointAt(1);
        float length = pt1.distance(pt2);
        MiaPoint4D scenept1 = marker->transformMatrixOfInitial2DScene.fromPatientCoordinate(pt1);
        MiaPoint4D scenept2 = marker->transformMatrixOfInitial2DScene.fromPatientCoordinate(pt2);
        pt1 = inputImage->convertPatientCoordinateToVoxel(pt1);
        pt2 = inputImage->convertPatientCoordinateToVoxel(pt2);
        if (pt1.distance(pt2) < 5) //different from length
            return;
        float inside_mean_v = 0;
        float inside_std_v = 0;
        int inside_count_v =0;
        float inside_mean_h = 0;
        float inside_std_h = 0;
        int inside_count_h = 0;
        inside_count_v = statisticsOnLine<A>(pt1,pt2,inside_mean_v,inside_std_v,inData);
        MiaPoint4D pt3;
        pt3 = pt2*2 - pt1;
        float outside_mean_v = 0;
        float outside_std_v = 0;
        int outside_count_v = 0;
        float outside_mean_h = 0;
        float outside_std_h = 0;
        int outside_count_h = 0;
        outside_count_v = statisticsOnLine<A>(pt2,pt3,outside_mean_v,outside_std_v,inData);

        float tempv; 
        scenept1 = scenept1 - scenept2;
        MiaPoint4D unitvector = scenept1;
        unitvector.normalize();
        tempv = scenept1.pos[0];
        scenept1.pos[0] = -scenept1.pos[1];
        scenept1.pos[1] = tempv;
        scenept1 = scenept1 + scenept2;
        scenept1 = (scenept1 + scenept2)*0.5f;
        scenept2 = scenept2*2 - scenept1;
        //shift 3 mm inside
        scenept1 = scenept1 + unitvector*3;
        scenept2 = scenept2 + unitvector*3;

        MiaPoint4D pt4 = marker->transformMatrixOfInitial2DScene.toPatientCoordinate(scenept1);
        MiaPoint4D pt5 = marker->transformMatrixOfInitial2DScene.toPatientCoordinate(scenept2);
        pt4 = inputImage->convertPatientCoordinateToVoxel(pt4);
        pt5 = inputImage->convertPatientCoordinateToVoxel(pt5);
        inside_count_h = statisticsOnLine<A>(pt4,pt5,inside_mean_h,inside_std_h,inData);
        //shift 3 mm outside
        scenept1 = scenept1 - unitvector*6;
        scenept2 = scenept2 - unitvector*6;
        MiaPoint4D pt6 = marker->transformMatrixOfInitial2DScene.toPatientCoordinate(scenept1);
        MiaPoint4D pt7 = marker->transformMatrixOfInitial2DScene.toPatientCoordinate(scenept2);
        pt6 = inputImage->convertPatientCoordinateToVoxel(pt6);
        pt7 = inputImage->convertPatientCoordinateToVoxel(pt7);
        outside_count_h = statisticsOnLine<A>(pt6,pt7,outside_mean_h,outside_std_h,inData);
        float inside_mean = inside_mean_v;
        float inside_std = inside_std_v;
        int inside_count = inside_count_v;

        float outside_mean;
        float outside_std;
        float outside_count;
        if(outside_std_v < outside_std_h)
        {
            outside_mean = outside_mean_v;
            outside_std = outside_std_v;
            outside_count = outside_count_v;
        }
        else
        {
            outside_mean = outside_mean_h;
            outside_std = outside_std_h;
            outside_count = outside_count_h;
        }

//        qDebug() << "marker statistics, in v:" << inside_mean_v << inside_std_v << inside_count_v<<
//                    "in h:" << inside_mean_h << inside_std_h << inside_count_h <<
//                    "out v:" << outside_mean_v << outside_std_v << outside_count_v <<
//                    "out h:" << outside_mean_h << outside_std_h << outside_count_h <<
//                    "distance" << length;

        int endPointIndex = (int)(pt2.pos[2])*width*height + (int)(pt2.pos[1])*width + (int)(pt2.pos[0]);

//        if(inside_std > outside_std*2.0) //inside heterogeneous
//        {
//            if(inside_mean_h > outside_mean)
//            {

//            }

//        }
        wiredParameters.insert(QString("out_mean"), outside_mean);
        wiredParameters.insert(QString("out_std"),  outside_std );
        wiredParameters.insert(QString("in_mean"), inside_mean );
        wiredParameters.insert(QString("in_std"), inside_std );
        wiredParameters.insert(QString("distance"),length);
        wiredParameters.insert(QString("endPointIndex"),endPointIndex);
        emit parameterChanged(wiredParameters);
        return;
//        else // inside and outside are both homogeneous
        {
            float t_value = fabs(outside_mean-inside_mean)/sqrt((float)(outside_std*outside_std + inside_std*inside_std));
            float approx = (1 + 0.278393*t_value + 0.230389*t_value*t_value + 0.000972*t_value*t_value*t_value + 0.078108*t_value*t_value*t_value*t_value);
            approx = approx*approx*approx*approx;
            float p_value = 1.0f - 1.0f/approx;
//            qDebug() << fabs(outside_mean-inside_mean) << sqrt((float)(outside_std*outside_std + inside_std*inside_std)) << min(inside_std,outside_std)<< t_value << p_value;

            if(fabs(outside_mean-inside_mean) > min(inside_std,outside_std)/2.0 && fabs(outside_mean-inside_mean) < inside_std*3.0f)
            {
                float lower_threshold = inside_mean - 3.0f*inside_std;
                float upper_threshold = inside_mean + 3.0f*inside_std;
                if(outside_mean>inside_mean)
                {
                    upper_threshold = inside_mean + (outside_mean - inside_mean)*outside_std/(inside_std+outside_std);
                }
                else
                {
                    lower_threshold = inside_mean + (outside_mean - inside_mean)*outside_std/(inside_std+outside_std);
                }

                wiredParameters.insert(QString("meanvalue"), inside_mean);
                wiredParameters.insert(QString("lowerthrehold"), lower_threshold );
                wiredParameters.insert(QString("upperthrehold"), upper_threshold );
                wiredParameters.insert(QString("endPointIndex"),endPointIndex);
                emit parameterChanged(wiredParameters);
            }
            else // inside and outside is equal
            {
                wiredParameters.insert(QString("meanvalue"), inside_mean);
                wiredParameters.insert(QString("lowerthrehold"), inside_mean - 1.5f*inside_std );
                wiredParameters.insert(QString("upperthrehold"), inside_mean + 1.5f*inside_std );
                wiredParameters.insert(QString("endPointIndex"),endPointIndex);
                emit parameterChanged(wiredParameters);
            }
        }

    }

    return;
}

template <class A>
void MiaMarkerStatistics::lineProfile(MiaMarker* marker, A* inData)
{
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    if(marker->type == MIA_Line)
    {
        MiaPoint4D pt1 = marker->getKeyPointAt(0);
        MiaPoint4D pt2 = marker->getKeyPointAt(1);
        pt1 = inputImage->convertPatientCoordinateToVoxel(pt1);
        pt2 = inputImage->convertPatientCoordinateToVoxel(pt2);
        if (pt1.distance(pt2) < 5)
            return;
        statisticsOnLine<A>(pt1,pt2,mean,standard_deviation,inData);
        MiaPoint4D pt3;
        pt3 = pt2*2 - pt1;

        float outside_mean = 0;
        float outside_std = 0;
        statisticsOnLine<A>(pt2,pt3,outside_mean,outside_std,inData);

        int endPointIndex = (int)(pt2.pos[2])*width*height + (int)(pt2.pos[1])*width + (int)(pt2.pos[0]);

        if(fabs(outside_mean-mean) > min(standard_deviation,outside_std)/2.0 && fabs(outside_mean-mean) < standard_deviation*3.0f)
        {
            float lower_threshold = mean - 3.0f*standard_deviation;
            float upper_threshold = mean + 3.0f*standard_deviation;
            if(outside_mean>mean)
            {
                upper_threshold = mean + (outside_mean - mean)*outside_std/(standard_deviation+outside_std);
            }
            else
            {
                lower_threshold = mean + (outside_mean - mean)*outside_std/(standard_deviation+outside_std);
            }

            wiredParameters.insert(QString("meanvalue"), mean);
            wiredParameters.insert(QString("lowerthrehold"), lower_threshold );
            wiredParameters.insert(QString("upperthrehold"), upper_threshold );
            wiredParameters.insert(QString("endPointIndex"),endPointIndex);
            emit parameterChanged(wiredParameters);
        }
        else
        {
            wiredParameters.insert(QString("meanvalue"), mean);
            wiredParameters.insert(QString("lowerthrehold"), mean - 1.5f*standard_deviation );
            wiredParameters.insert(QString("upperthrehold"), mean + 1.5f*standard_deviation );
            wiredParameters.insert(QString("endPointIndex"),endPointIndex);
            emit parameterChanged(wiredParameters);
        }

    }
    return;
}
template <class A>
int MiaMarkerStatistics::statisticsOnLine(MiaPoint4D& pt1, MiaPoint4D& pt2, float& m, float& std,A* inData)
{
    int width, height, depth;
    width = inputImage->getWidth();
    height = inputImage->getHeight();
    depth = inputImage->getDepth();
    int sliceSize = width*height;

    int x1,x2,x,y1,y2,y,z1,z2,z;
    x1 = min(pt1.pos[0], pt2.pos[0]);
    x2 = max(pt1.pos[0], pt2.pos[0]);

    y1 = min(pt1.pos[1], pt2.pos[1]);
    y2 = max(pt1.pos[1], pt2.pos[1]);

    z1 = min(pt1.pos[2], pt2.pos[2]);
    z2 = max(pt1.pos[2], pt2.pos[2]);

    float vx,vy,vz,len;
    vx = pt2.pos[0] - pt1.pos[0];
    vy = pt2.pos[1] - pt1.pos[1];
    vz = pt2.pos[2] - pt1.pos[2];
    len = sqrt(vx*vx + vy*vy + vz*vz);
    vx = vx/len; vy = vy/len; vz = vz/len;
    if(x1>0)x1--;if(x2<width-1)x2++;
    if(y1>0)y1--;if(y2<height-1)y2++;
    if(z1>0)z1--;if(z2<depth-1)z2++;

    m = 0;
    std = 0;
    int count = 0;
    int x0,y0,z0;
    x0 = pt1.pos[0]; y0 = pt1.pos[1]; z0 = pt1.pos[2];
    for(z = z1; z <= z2; z++)
        for(y = y1; y <= y2; y++)
            for(x = x1; x <= x2; x++)
            {
                float projected_len, distance;
                projected_len = (z-z0)*vz +(y-y0)*vy + (x-x0)*vx;
                distance = sqrt((z-z0)*(z-z0) + (y-y0)*(y-y0) + (x-x0)*(x-x0) - projected_len*projected_len);
                if(distance < 2.5)
                {
                    int ind = z*sliceSize + y*width + x;
                    float sample = (float)(inData[ind]);
                    m += sample;
                    std += sample*sample;
                    count++;
                }

            }



    m = m/count;
    std = sqrt(std/count-m*m);
    return count;
}

bool MiaMarkerStatistics::reloadInputImage()
{
    if( MiaModule::reloadInputImage())
    {
        emit getInputMarkers(markerList);
        if(markerList.size()==0)
            return false;
    }
    else
        return false;
    return true;
}

void MiaMarkerStatistics::scanBifurcations()
{
    for(int i = 0; i < markerList.size(); i++ )
    {
        MiaMarker3DPolygon* markerA = (MiaMarker3DPolygon*)markerList.at(i);
        markerA->bifurcationNodeList.clear();
        QList<int> bptlist;
        for(int j = 0; j < markerList.size();j++)
        {
            if(i == j)
                continue;
            MiaMarker3DPolygon* markerB = (MiaMarker3DPolygon*)markerList.at(j);

            QList<int> bpoints = findBifurcationPoints(markerA->keyPointList,markerB->keyPointList);
            bptlist.append(bpoints);
        }
        qSort(bptlist);
        qDebug()<<bptlist;
        for(int j = bptlist.size() - 1; j >= 0 ; j--)
        {
            //remove redundant points
            if(j>0 && bptlist[j] - bptlist[j-1] < 50)
                bptlist.removeAt(j);
        }
        if(bptlist.size()>0)
        {
            if(bptlist.first()!=0)
                markerA->bifurcationNodeList.append(0);
            markerA->bifurcationNodeList.append(bptlist);
            if(bptlist.last() != markerA->keyPointList.size()-1)
                markerA->bifurcationNodeList.append(markerA->keyPointList.size()-1);
        }
        qDebug()<<markerA->bifurcationNodeList;
    }
}
QList<int> MiaMarkerStatistics::findBifurcationPoints(QList<MiaPoint4D>& lineA, QList<MiaPoint4D>& lineB)
{
    QList<int> bpts;
    bpts.clear();
    bool lineMeet = false;
    for(int i = 0 ; i < lineA.size(); i ++)
    {
        MiaPoint4D pt = lineA.at(i);
        bool attachToLineB = false;

        for(int j = 0; j < lineB.size(); j++)
        {
            float dist = pt.distance(lineB.at(j));
            if(dist<0.5)
            {
                attachToLineB = true;
                break;
            }
        }

        if(attachToLineB && lineMeet == false)
        {
            lineMeet = true;
            bpts.append(i);
        }
        else if(!attachToLineB && lineMeet == true)
        {
            lineMeet = false;
            bpts.append(i);
        }

    }
    return bpts;
}
