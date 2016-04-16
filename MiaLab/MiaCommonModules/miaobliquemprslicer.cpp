#include "miaobliquemprslicer.h"
#include "MiaMarkers/miamarker3dbspline.h"
MiaObliqueMPRSlicer::MiaObliqueMPRSlicer(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "MiaObliquMPRSlicer";
    mprMode = MIA_MPR_UserDefine;
    wiredParameters.insert(QString("pat_x"), currentCrossHairPostionInPatientSpace.pos[0]);
    wiredParameters.insert(QString("pat_y"), currentCrossHairPostionInPatientSpace.pos[1]);
    wiredParameters.insert(QString("pat_z"), currentCrossHairPostionInPatientSpace.pos[2]);
    wiredParameters.insert(QString("pat_t"), currentCrossHairPostionInPatientSpace.pos[3]);
    wiredParameters.insert(QString("scroll_y"), 0);
    fovSize = 40; //defautl crossetion images size is 40*40mm
    isSelfScrolling = false;
    selectedSpline = NULL;
    needReAllocOutputImage = true;
    scrollStepLength = 0.5f;
}

void MiaObliqueMPRSlicer::setPositionAndOrientation(const MiaPoint4D &pos, const MiaPoint4D &ori)
{
    currentCrossHairPostionInPatientSpace = pos;
    currentPlaneNormalDirectionInPatientSpace = ori;
    needUpdate = true;
    needReAllocOutputImage = true;
    update();
}

bool MiaObliqueMPRSlicer::reloadInputImage()
{
    if( MiaModule::reloadInputImage() )
    {
        if(mprMode != MIA_MPR_CrossSectionOfCurve)
        {
            return true;
        }
        emit getInputMarkers(markerList);
        int i;
        if(markerList.size()<1)
            return false;
        selectedSpline = NULL;
        for( i = 0; i < markerList.size(); i++ )
        {
            if(markerList.at(i)->type == MIA_Spline && ( markerList.at(i)->currentMode == MIA_Selected || markerList.at(i)->currentMode == MIA_Initializing))
            {
                selectedSpline = (MiaMarker3DBSpline*) markerList.at(i);
            }
        }
        if(selectedSpline)
        {
            bool pathChanged = false;
            QList<MiaPoint4D> renderpts;
            selectedSpline->getRenderPoints(renderpts);
            if(renderpts.size() == 0)
                return false;
            if(!(currentCrossHairPostionInPatientSpace == selectedSpline->getFocus()))
            {
                pathChanged = true;
            }
            else if(pathIn3D.size() != renderpts.size() )
            {
                pathChanged = true;
            }
            else
            {
                for( int i = 0; i < pathIn3D.size(); i++)
                {
                    if(pathIn3D[i].pos[0]!=renderpts[i].pos[0] || pathIn3D[i].pos[1]!=renderpts[i].pos[1] || pathIn3D[i].pos[2]!=renderpts[i].pos[2] || pathIn3D[i].pos[3]!=renderpts[i].pos[3])
                    {
                        pathChanged = true;
                        break;
                    }
                }
            }
            if(pathChanged)
            {
                pathIn3D.clear();
                pathIn3D.append(renderpts);
                needReAllocOutputImage = true;
            }
            // else needReAllocOutputImage is not changed (true if setAngle is called

        }
        else
        {
            pathIn3D.clear();
            needReAllocOutputImage = true;
            return false;
        }
    }
    else
        return false;
    return true;
}
bool MiaObliqueMPRSlicer::runOperation()
{
    bool result = false;
    ImageType sourcetype = inputImage->getType();
    float* out = (float*)outputImage->getDataPtr();
    if(sourcetype == IM_RGBA)
    {
        qDebug()<<"obliqur MPR doesn't support RGB images";
        throw std::runtime_error("obliqur MPR doesn't support RGB images");
    }
    int width, height;
    width = outputImage->getWidth();
    height = outputImage->getHeight();
    MiaAffineTransform input_bt = inputImage->getTransformMatrix().getBulkTransform();
    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++)
        {
            MiaPoint4D pt(x,y,0,0);
            pt = currentTransform.toPatientCoordinate( pt );
            out[y*width+x] = inputImage->getValueAt( input_bt.inversedTransform(pt).pos );
        }

    return true;
}

void MiaObliqueMPRSlicer::scroll(float delta, bool notify_crosshair_change)
{
    if(mprMode == MIA_MPR_CrossSectionOfCurve)
    {
        if(!selectedSpline)
            return;

        scrollTo(selectedSpline->getFocusDistanceToStart()+delta*scrollStepLength,notify_crosshair_change);

    }
    else
    {
        throw std::runtime_error("Scrolling is not supported in other mode than MPR crossection curver");
    }
}

void MiaObliqueMPRSlicer::scrollTo(float dist, bool notify_crosshair_change)
{
    if(mprMode == MIA_MPR_CrossSectionOfCurve)
    {
        if(!selectedSpline)
            return;
        isSelfScrolling = true;
        selectedSpline->moveFocusPointAlongCurveToLocation( dist );
        isSelfScrolling = false;
        if(notify_crosshair_change)
        {
            currentCrossHairPostionInPatientSpace = selectedSpline->getFocus();
            wiredParameters.insert(QString("pat_x"), currentCrossHairPostionInPatientSpace.pos[0]);
            wiredParameters.insert(QString("pat_y"), currentCrossHairPostionInPatientSpace.pos[1]);
            wiredParameters.insert(QString("pat_z"), currentCrossHairPostionInPatientSpace.pos[2]);
            wiredParameters.insert(QString("pat_t"), currentCrossHairPostionInPatientSpace.pos[3]);
            emit parameterChanged(wiredParameters);
        }

    }
    else
    {
        throw std::runtime_error("Scrolling is not supported in other mode than MPR crossection curver");
    }
}


bool MiaObliqueMPRSlicer::reAllocOutputImage()
{
    float min_spacing;
    float* in_s = inputImage->getSpacing();
    min_spacing = std::min(std::min(in_s[0],in_s[1]),in_s[2]);
    float newspacing[4];
    for(int i = 0 ;i < 3; i ++)
        newspacing[i] = min_spacing;
    newspacing[3] = 1.0;
    float neworientation[9];
    MiaPoint4D origin;

    int outWidth = 0, outHeight = 0;


    MiaAffineTransform input_bt = inputImage->getTransformMatrix().getBulkTransform();
    MiaAffineTransform temp_bt;
    switch(mprMode)
    {
    case MIA_MPR_UserDefine:
        throw std::runtime_error("Not implemented");
        break;
    case MIA_MPR_CrossSectionOfCurve:
        if(!selectedSpline->isMarkerValid())
            return false;
        outWidth = outHeight = fovSize/min_spacing;
        currentCrossHairPostionInPatientSpace = selectedSpline->getFocus();
        currentPlaneNormalDirectionInPatientSpace = selectedSpline->getFocusTangentDirection();
        MiaAffineTransform scaleMatrix;
        scaleMatrix.scale(newspacing);
        temp_bt = getTransformMatrixFromOrientation(currentPlaneNormalDirectionInPatientSpace);
        for(int i = 0 ; i < 3 ; i++)
            for(int j = 0; j < 3; j++)
            {
                neworientation[j*3+i] = temp_bt.toPatientSystemMatrix[i*4+j];
            }
        temp_bt = temp_bt*scaleMatrix;
        temp_bt.translate(currentCrossHairPostionInPatientSpace.pos);
        MiaPoint4D leftUpperCorner(-outWidth/2.0, - outHeight/2.0, 0, 0);
        origin = temp_bt.transform(leftUpperCorner);
        MiaPoint4D offset = origin - currentCrossHairPostionInPatientSpace;
        temp_bt.translate(offset.pos);
        currentTransform.setBulkTranform(temp_bt);
        break;
    }
    if(outputImage)
    {
        outputImage->release();
    }
    if(inputImage->getType() == IM_RGBA)
        outputImage = new MiaImageND( outWidth, outHeight, 1, 1, IM_RGBA);
    else
        outputImage = new MiaImageND( outWidth, outHeight, 1, 1, IM_float);
    if(outputImage)
    {
        outputImage->retain();

    }
    else
        return false;
    outputImage->setOffset( origin.pos );
    outputImage->setSpacing(newspacing);
    outputImage->set3x3OrientationMatrix( neworientation );
    outputImage->computeTransformMatrix();

 return true;
}

void MiaObliqueMPRSlicer::dispatchParameters(const QMap<QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
    while (i.hasNext()) {
        i.next();
        qDebug() <<moduleName << "set parameter" << i.key() << i.value();
        if(i.key() == "scroll_y")
        {
            float scroll_delta = i.value().toFloat();
            if(scroll_delta!=0)
            {
               wiredParameters.insert(QString("scroll_y"), 0);
               scroll((int)scroll_delta);
            }
        }
    }

}

MiaAffineTransform MiaObliqueMPRSlicer::getTransformMatrixFromOrientation(const MiaPoint4D &ori)
{

    MiaPoint4D normal_axis = ori;
    normal_axis.normalize();

    MiaPoint4D rowOrientation;

    if(normal_axis.pos[0] == 0 && normal_axis.pos[1] == 0)
    {
        //if the colomn direction is along the z axis
        rowOrientation.pos[0] = 1.0f;

    }
    else
    {
        MiaPoint4D z_axis(0,0,1.0f,0);
        rowOrientation = z_axis.cross(normal_axis);
        rowOrientation.normalize();
    }
    MiaPoint4D columnOrientation = normal_axis.cross(rowOrientation);
    columnOrientation.normalize();

    float spacing[4] = { 1.0,1.0,1.0,1.0};
    float orientation[9];
    for(int i = 0; i < 3; i++)
    {
        orientation[i] = rowOrientation.pos[i];
        orientation[i+3] = columnOrientation.pos[i];
        orientation[i+6] = normal_axis.pos[i];
    }
    float offset[3] = {0.0,0.0,0.0};


    MiaAffineTransform t(orientation,spacing,offset);

    return t;

//    float anglex,angley;//anglez;
//    if(normal_axis.pos[2]==0)
//    {
//        if(normal_axis.pos[1]>0)
//            anglex=pi()/2.0f;
//        if(normal_axis.pos[1]<0)
//            anglex=-pi()/2.0f;
//        if(normal_axis.pos[1]==0)
//            anglex=0;
//    }
//    else
//    {
//        anglex = atan(normal_axis.pos[1]/normal_axis.pos[2]);
//        if(normal_axis.pos[2]<0)
//            anglex+=pi();
//    }

//    angley = asin(normal_axis.pos[0]);
//    Point4D x_axis(1.0f,0.0f,0.0f,0.0f);
//    MiaAffineTransform rotateAroundXMatrix(x_axis,anglex);
//    Point4D y_axis(0.0f,1.0f,0.0f,0.0f);
//    MiaAffineTransform rotateAroundYMatrix(y_axis, -angley);

//    MiaAffineTransform result = rotateAroundYMatrix * rotateAroundXMatrix;
//    return result;
}


void MiaObliqueMPRSlicer::markerRemoved(MiaMarker *marker)
{
    needUpdate = true;
    if(selectedSpline == marker)
        selectedSpline = NULL;
    update();
}
