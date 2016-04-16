#include "miacurvedmprslicer.h"
#include "MiaMarkers/miamarker3dbspline.h"
#include "transform/miacurvedmprtransformalgorithm.h"
#include <algorithm>
#include "miaconstants.h"

MiaCurvedMPRSlicer::MiaCurvedMPRSlicer(MiaNetwork *parent) :
    MiaModule(parent)
{
    moduleName = "Curved MPR module";
    currentRotationAngle = 0.0;
    MiaCurvedMPRTransformAlgorithm* algorithm = new MiaCurvedMPRTransformAlgorithm();
    curvedMPRtransform.setTransformAlgorithm(algorithm);
    wiredParameters.insert(QString("scroll_y"), 0);
    rotationMode = MIA_RotateAroundStardAndEndPoints;
    needReAllocOutputImage = true;
}

void MiaCurvedMPRSlicer::setRotateAxis(MiaPoint4D& axis)
{
    rotationAxis = axis;
}

void MiaCurvedMPRSlicer::setRotationAngle(float angle)
{
    currentRotationAngle = angle;
    currentRotationAngle = fmod(currentRotationAngle+2.0*E_PI,2.0*E_PI);
    needReAllocOutputImage = true;
    update();
}

void MiaCurvedMPRSlicer::setBulkTransform(MiaAffineTransform &transfomr)
{
}

void MiaCurvedMPRSlicer::markerRemoved(MiaMarker *marker)
{
    needUpdate = true;
    if(selectedSpline == marker)
        selectedSpline = NULL;
    update();
}
bool MiaCurvedMPRSlicer::reloadInputImage()
{
    MiaImageND* tempimg = NULL;
    emit requestUpdateInput(&tempimg);
    if(!tempimg)
        return false;
    if(inputImage!=tempimg)
    {
        if(inputImage)
            inputImage->release();
        inputImage = tempimg;
        inputImage->retain();

    }
    if( inputImage )
    {
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
            if(pathIn3D.size() != renderpts.size() )
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
            else if(!needReAllocOutputImage)
            {
                return false;
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
bool MiaCurvedMPRSlicer::runOperation()
{
    bool result = false;
    ImageType sourcetype = inputImage->getType();
    float* out = (float*)outputImage->getDataPtr();
    if(sourcetype == IM_RGBA)
    {
        qDebug()<<"CPR doesn't support RGB images";
        throw std::runtime_error("CPR doesn't support RGB images");
    }
    int width, height;
    width = outputImage->getWidth();
    height = outputImage->getHeight();
    MiaAffineTransform input_bt = inputImage->getTransformMatrix().getBulkTransform();
    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++)
        {
            MiaPoint4D pt(x,y,0,0);
            pt = curvedMPRtransform.toPatientCoordinate( pt );
            out[y*width+x] = inputImage->getValueAt( input_bt.inversedTransform(pt).pos );
        }

    return true;
}

void MiaCurvedMPRSlicer::scroll(int delta)
{
    setRotationAngle( currentRotationAngle + (float)delta*E_PI/36.0f );
}

bool MiaCurvedMPRSlicer::reAllocOutputImage()
{
    float min_spacing;
    float* in_s = inputImage->getSpacing();
    min_spacing = std::min(std::min(in_s[0],in_s[1]),in_s[2]);
    float newspacing[4];
    for(int i = 0 ;i < 3; i ++)
        newspacing[i] = min_spacing;
    newspacing[3] = 1.0f;

    int outWidth = 0, outHeight = 0;

    MiaAffineTransform temp_bt;
    switch(rotationMode)
    {
    case MIA_RotateAroundGivenAxis:
        temp_bt = getTransformMatrixFromAxis(rotationAxis);
        break;
    case MIA_RotateAroundOriginalViewYAxis:
        temp_bt = selectedSpline->transformMatrixOfInitial2DScene.getBulkTransform();
        break;
    case MIA_RotateAroundStardAndEndPoints:
        rotationAxis = pathIn3D.last() - pathIn3D.first();
        temp_bt = getTransformMatrixFromAxis(rotationAxis);
        break;
    }
    MiaAffineTransform rotationMatrix(rotationAxis,currentRotationAngle);
    temp_bt = rotationMatrix*temp_bt;

    curvedMPRtransform.setBulkTranform( temp_bt );
    MiaCurvedMPRTransformAlgorithm* algorithm = (MiaCurvedMPRTransformAlgorithm*) curvedMPRtransform.getTransformAlgorithm();
    algorithm->setOutputSpacing(newspacing);
    algorithm->setKeyPoints(pathIn3D);
    outHeight = algorithm->getProjectedLength();

    //to figure out the width
    int bounding_box[3][2];
    int* size = inputImage->getDimSize();
    for( int dim = 0; dim < 3; dim++)
    {
        bounding_box[dim][0] = 0;
        bounding_box[dim][1] = size[dim] -1;
    }
    float max_x, min_x;
    MiaPoint4D v = temp_bt.inversedTransform(pathIn3D[0]);
    max_x = min_x = v.pos[0];

//    MiaAffineTransform input_bt = inputImage->getTransformMatrix().getBulkTransform();
//    for(int i = 0; i < 2; i++)
//        for(int j = 0; j < 2; j++)
//            for(int k = 0; k < 2; k++)
//            {
//                v.pos[0] = bounding_box[0][k];
//                v.pos[1] = bounding_box[1][j];
//                v.pos[2] = bounding_box[2][i];
//                cpr_index = temp_bt.inversedTransform(input_bt.transform(v));
//                if(cpr_index.pos[0] < min_x)
//                    min_x = cpr_index.pos[0];
//                if(cpr_index.pos[0] > max_x)
//                    max_x = cpr_index.pos[0];
//            }

    for(int i = 0 ; i < pathIn3D.size(); i++)
    {
        v = temp_bt.inversedTransform(pathIn3D[i]);
        if(v.pos[0] < min_x)
            min_x = v.pos[0];
        if(v.pos[0] > max_x)
            max_x = v.pos[0];

    }
    min_x -= 20.0;
    max_x += 20.0;
    outWidth = (max_x - min_x)/min_spacing;

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

    // compute the transform
    MiaPoint4D origin = temp_bt.inversedTransform(pathIn3D[0]);
    origin.pos[0] = min_x;
    MiaPoint4D newoffset = temp_bt.transform(origin);
    MiaPoint4D firstrow = origin;
    firstrow.pos[0] += 1.0;
    MiaPoint4D firstcolomn = origin;
    firstcolomn.pos[1] += 1.0;

    MiaPoint4D newxorientation = temp_bt.transform(firstrow) - newoffset;
    MiaPoint4D newyorientation = temp_bt.transform(firstcolomn) - newoffset;
    outputImage->setOffset( newoffset.pos );
    outputImage->setSpacing(newspacing);
    float neworientation[9];
    for(int i = 0 ; i < 3 ; i++)
    {
        neworientation[i] = newxorientation.pos[i];
        neworientation[i+3] = newyorientation.pos[i];
        neworientation[i+6] = 0;

    }
    outputImage->set3x2OrientationMatrix( neworientation );

    outputImage->computeTransformMatrix();
    temp_bt = outputImage->computeTransformMatrixWithoutSpacing().getBulkTransform();
    curvedMPRtransform.setBulkTranform(temp_bt);
    outputImage->setTransformMatrix(curvedMPRtransform);
    needReAllocOutputImage = false;

 return true;
}

void MiaCurvedMPRSlicer::dispatchParameters(const QMap<QString, QVariant> &parameters)
{
    QMapIterator<QString, QVariant> i(parameters);
    while (i.hasNext()) {
        i.next();
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

MiaAffineTransform MiaCurvedMPRSlicer::getTransformMatrixFromAxis(const MiaPoint4D &axis)
{
    if(axis.pos[0] == 0 && axis.pos[1] == 0 && axis.pos[2] != 0 )
    {
        qDebug()<<"rotation axis is invalide";
        throw std::runtime_error("rotation axis is invalide");
    }

    MiaPoint4D columnOrientation = axis;
    columnOrientation.normalize();
    MiaPoint4D rowOrientation;
    if(columnOrientation.pos[0] == 0 && columnOrientation.pos[1] == 0)
    {
        //if the colomn direction is along the z axis
        rowOrientation.pos[0] = 1.0f;
    }
    else
    {
        MiaPoint4D z_axis(0,0,1.0f,0);
        rowOrientation = z_axis.cross(columnOrientation);
        rowOrientation.normalize();
    }
    MiaPoint4D rowCrossColumnOrientation = rowOrientation.cross(columnOrientation);
    rowCrossColumnOrientation.normalize();

    float spacing[4] = { 1.0,1.0,1.0,1.0};
    float orientation[9];
    for(int i = 0; i < 3; i++)
    {
        orientation[i] = rowOrientation.pos[i];
        orientation[i+3] = columnOrientation.pos[i];
        orientation[i+6] = rowCrossColumnOrientation.pos[i];
    }
    float offset[3] = {0.0,0.0,0.0};


    MiaAffineTransform t(orientation,spacing,offset);

    return t;

}

