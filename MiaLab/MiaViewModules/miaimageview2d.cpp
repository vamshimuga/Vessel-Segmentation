#include "miaimageview2d.h"
#include <math.h>
#include <QtGui>
//#include <QGLWidget>
#include <QGraphicsScene>

MiaImageView2D::MiaImageView2D(QWidget *parent) :
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    imageScene = new QGraphicsScene(this) ;
    imageScene->setItemIndexMethod (QGraphicsScene::NoIndex); //There is a bug when using Bsptree index. There will be ghost item after delete markers.
    setScene(imageScene);
//    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    //when use QGLWidget it causes crashes on destruction. related to Marker Agents
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setBackgroundBrush(QColor(0, 0, 0));
    crossHair = new MiaGraphicsCrossHairItem(this);
    crossHair->setZValue(CROSSHAIRZVALUE);
    imageScene->addItem(crossHair);

    mouseMoveInterpretor = NULL;
    mouseLeftDragInterpretor = NULL;
    mouseRightDragInterpretor = NULL;
    wheelInterpretor = NULL;
    keyboardInterpretor = NULL;
    leftButtonDown = false;
    rightButtonDown = false;
    blockLeftButtonEvents = false;
    referenceImage = NULL;
    settingCrossHairBySelf = false;
    expectedSliceThickness = 0.0f;

    wiredParameters.insert("cross_view_x", 0);
    wiredParameters.insert("cross_view_y", 0);
    wiredParameters.insert("cross_pat_x", 0);
    wiredParameters.insert("cross_pat_y", 0);
    wiredParameters.insert("cross_pat_z", 0);
    wiredParameters.insert("cross_pat_t", 0);
    wiredParameters.insert("translate_x", 0);
    wiredParameters.insert("translate_y", 0);
    wiredParameters.insert("scaling_delta", 0);

    topMostImageZValue = IMAGEMINZVALUE;
    setFocusPolicy ( Qt::StrongFocus );

}

//MiaImageView2D::~MiaImageView2D()
//{
//    imageItemList.clear();
//}

void MiaImageView2D::addInputModule(MiaImage2QImage* imagemodule)
{
    MiaGraphics2DImageItem* item = imagemodule->getQImageItem();
//    qDebug() << item->boundingRect();
    addQImage(item);
    QObject::connect(imagemodule, SIGNAL(qimageWillRemoved(MiaGraphics2DImageItem*)), this, SLOT(removeQImage(MiaGraphics2DImageItem*)));
    QObject::connect(imagemodule, SIGNAL(qimageAdded(MiaGraphics2DImageItem*)), this, SLOT(addQImage(MiaGraphics2DImageItem*)));
    QObject::connect(imagemodule, SIGNAL(qimageGeometryChanged(MiaGraphics2DImageItem*)), this, SLOT(imageGeometryChanged(MiaGraphics2DImageItem*)));


}
void MiaImageView2D::removInputModule( MiaImage2QImage* imagemodule )
{
    removeQImage(imagemodule->getQImageItem());
}

void MiaImageView2D::repositionImages()
{
    if(! referenceImage )
    {
        return;
    }
    int i;
    for( i = 0 ; i < imageItemList.count() ; i++)
    {
        MiaGraphics2DImageItem* item = imageItemList.at(i);
        if(item == referenceImage)
            continue;

        MiaPoint4D cornerpos = item->getOrigin();
        QPointF pos = convertPatientCoordinateToScenePoint( cornerpos );
        item->setPos(pos);
    }
}

void MiaImageView2D::resetViewSize()
{
   int i;
   QRectF boundrect;
   if(referenceImage)
   {
       boundrect = referenceImage->boundingRect();
       previousSceneRect = referenceImage->mapRectToScene(boundrect);
   }
   else
       return;
   for( i = 0 ; i < imageItemList.size() ; i++ )
   {
        boundrect = imageItemList.at(i)->boundingRect();
        previousSceneRect.united( imageItemList.at(i)->mapRectToScene(boundrect) );

    }
   fitInView(previousSceneRect,Qt::KeepAspectRatio);

}


void MiaImageView2D::mouseMoveEvent ( QMouseEvent * event )
{
    QGraphicsView::mouseMoveEvent( event);

    if(leftButtonDown && !blockLeftButtonEvents)
    {
        if(mouseLeftDragInterpretor)
            mouseLeftDragInterpretor->mouseMovedAt( event->pos() );
    }
    else if (rightButtonDown)
    {
        if(mouseRightDragInterpretor)
            mouseRightDragInterpretor->mouseMovedAt( event->pos() );
    }
}
void MiaImageView2D::mousePressEvent ( QMouseEvent * event )
{
    QGraphicsView::mousePressEvent( event);
    if( event->button() == Qt::LeftButton && !blockLeftButtonEvents)
    {
        leftButtonDown = true;
        if(mouseLeftDragInterpretor)
            mouseLeftDragInterpretor->buttonPressedAt( event->pos() );
    }
    else if( event->button() == Qt::RightButton )
    {
        rightButtonDown = true;
        if(mouseRightDragInterpretor)
            mouseRightDragInterpretor->buttonPressedAt( event->pos() );
    }

}

void MiaImageView2D::mouseReleaseEvent ( QMouseEvent * event )
{
    QGraphicsView::mouseReleaseEvent( event);
    if(leftButtonDown && !blockLeftButtonEvents)
    {
        if(mouseLeftDragInterpretor)
            mouseLeftDragInterpretor->buttonReleaseAt( event->pos() );
    }
    else if (rightButtonDown)
    {
        if(mouseRightDragInterpretor)
            mouseRightDragInterpretor->buttonReleaseAt( event->pos() );
    }
    if( event->button() == Qt::LeftButton )
        leftButtonDown = false;
    else if( event->button() == Qt::RightButton )
        rightButtonDown = false;
}
 void MiaImageView2D::wheelEvent(QWheelEvent *event)
 {
     float step=event->delta();
     if(step>0)
         step = 1.0;
     else if(step < 0)
         step = -1.0;

     if(wheelInterpretor)
         wheelInterpretor->updateYValue(step);
 }

 void MiaImageView2D::mouseDoubleClickEvent ( QMouseEvent * event )
 {
     QGraphicsView::mouseDoubleClickEvent( event);
     if( event->button() == Qt::LeftButton && !blockLeftButtonEvents)
     {
         if(mouseLeftDragInterpretor)
             mouseLeftDragInterpretor->mouseDoubleClickedAt( event->pos() );
     }
     else if( event->button() == Qt::RightButton )
     {
         if(mouseRightDragInterpretor)
             mouseRightDragInterpretor->mouseDoubleClickedAt( event->pos() );
     }
 }

 void	MiaImageView2D::keyPressEvent ( QKeyEvent * event )
 {
     if (keyboardInterpretor)
        keyboardInterpretor->keyPressed( event->key() );

 }

 void	MiaImageView2D::keyReleaseEvent ( QKeyEvent * event )
 {
     if (keyboardInterpretor)
        keyboardInterpretor->keyReleased( event->key() );
 }

 void MiaImageView2D::resizeEvent ( QResizeEvent * event )
 {
     fitInView(previousSceneRect,Qt::KeepAspectRatio);
 }

 void MiaImageView2D::setParameters(const QMap <QString, QVariant> &parameters)
 {
     if(!referenceImage)
         return;
     QMapIterator<QString, QVariant> i(parameters);
     QPoint viewpt = mapFromScene( crossHair->pos() );

     ViewParameterGroup currentParameterGroup = MIA_Parameter_None;
     QPoint translate_step(0,0);
     float scale_delta;
     while (i.hasNext()) {
         i.next();

         if(wiredParameters.contains(i.key()) && wiredParameters.value(i.key()) != i.value())
         {
             if(i.key().left(5) == "cross")
             {
                 // here we update the wire parameters of the cross hair position
                 wiredParameters.insert(i.key(),i.value());
                 if(i.key() == "cross_view_x")
                 {
                     viewpt.setX( i.value().toInt() );
                     currentParameterGroup = MIA_Parameter_SettingCrossViewCoordinate;
                 }
                 if(i.key() == "cross_view_y")
                 {
                     viewpt.setY( i.value().toInt() );
                     currentParameterGroup = MIA_Parameter_SettingCrossViewCoordinate;
                 }

                 if(i.key() == "cross_pat_x")
                 {
                     crossHair->centerPoint.pos[0] = i.value().toFloat();
                     currentParameterGroup = MIA_Parameter_SettingCrossPatientCoordinate;
                 }
                 if(i.key() == "cross_pat_y")
                 {
                     crossHair->centerPoint.pos[1] = i.value().toFloat();
                     currentParameterGroup = MIA_Parameter_SettingCrossPatientCoordinate;
                 }
                 if(i.key() == "cross_pat_z")
                 {
                     crossHair->centerPoint.pos[2] = i.value().toFloat();
                     currentParameterGroup = MIA_Parameter_SettingCrossPatientCoordinate;
                 }
                 if(i.key() == "cross_pat_t")
                 {
                     crossHair->centerPoint.pos[3] = i.value().toFloat();
                     currentParameterGroup = MIA_Parameter_SettingCrossPatientCoordinate;
                 }
             }
             else
             {
                 //NOTE: we do not update wiredparameter of the translate and zooming factors
                 if(i.key() == "translate_x")
                 {
                     translate_step.setX( i.value().toInt() );
                     currentParameterGroup = MIA_Parameter_Panning;
                 }
                 if(i.key() == "translate_y")
                 {
                     translate_step.setY( i.value().toInt() );
                     currentParameterGroup = MIA_Parameter_Panning;
                 }
                 if(i.key() == "scaling_delta")
                 {
                     scale_delta = i.value().toFloat();
                     currentParameterGroup = MIA_Parameter_Zooming;
                 }
             }

         }
     }
     switch(currentParameterGroup)
     {
     case MIA_Parameter_SettingCrossPatientCoordinate:
     {
         moveCrossHairTo(crossHair->centerPoint);
         break;
     }
    case MIA_Parameter_SettingCrossViewCoordinate:
     {
         QPointF scene_pt =  mapToScene( viewpt );
         crossHair->setPos( scene_pt );
         break;
     }
    case MIA_Parameter_Panning:
     {
         QPointF viewAreaCenter =  previousSceneRect.center();
         QPointF delta = mapToScene( translate_step )-mapToScene(QPoint(0,0));
         previousSceneRect.moveCenter(viewAreaCenter - delta);
         fitInView(previousSceneRect,Qt::KeepAspectRatio);
         break;
     }
    case MIA_Parameter_Zooming:
     {
         QPointF viewAreaCenter =  previousSceneRect.center();
         QPointF unitvector = (mapToScene( QPoint(0,1) )-mapToScene(QPoint(0,0)));
         float scale = 1.0f/sqrt(unitvector.x()*unitvector.x() + unitvector.y()*unitvector.y());
         float newscale = scale - 0.01f*scale_delta;
         if(newscale < 0.01f)
             newscale = 0.01f;
         QSizeF oldsize = previousSceneRect.size();
         previousSceneRect.setSize(oldsize*(scale/newscale));
         previousSceneRect.moveCenter(viewAreaCenter);
         fitInView(previousSceneRect,Qt::KeepAspectRatio);
         break;
     }
     default:
         //nothing changed
         break;

     }
 }

 QPointF MiaImageView2D::crosshairMoved(QPointF scene_pt)
 {
     if (! referenceImage)
         return QPointF(0,0);
     wiredParameters.insert("cross_view_x", scene_pt.x());
     wiredParameters.insert("cross_view_y", scene_pt.y());

     if(settingCrossHairBySelf)
         return scene_pt;

     MiaPoint4D pat_pos = convertScenePointToPatientCoordinate(scene_pt); //referenceImage->convertSceneCoordinateToPatientCoordinate( scene_pt );

     crossHair->centerPoint = pat_pos;
     notifyCrossHairChanged();
     return scene_pt;

 }
void MiaImageView2D::moveCrossHairTo( MiaPoint4D pt )
 {

    QPointF scene_pt = convertPatientCoordinateToScenePoint(pt);
    settingCrossHairBySelf = true;
    crossHair->setPos( scene_pt );
    settingCrossHairBySelf = false;
//    crosshairMoved();
    crossHair->centerPoint = pt;
    notifyCrossHairChanged();

 }

 void MiaImageView2D::notifyCrossHairChanged()
 {
     MiaPoint4D pt = crossHair->centerPoint;
     wiredParameters.insert("cross_pat_x", pt.pos[0]);
     wiredParameters.insert("cross_pat_y", pt.pos[1]);
     wiredParameters.insert("cross_pat_z", pt.pos[2]);
     wiredParameters.insert("cross_pat_t", pt.pos[3]);

     emit parameterChanged(wiredParameters);
#ifdef Q_OS_WIN
     viewport()->repaint();
#endif

 }

MiaPoint4D MiaImageView2D::convertViewPointToPatientCoordinate( QPoint viewpt)
{
    QPointF scene_pt =  mapToScene( viewpt );
//    if (referenceImage)
//       return referenceImage->convertSceneCoordinateToPatientCoordinate( scene_pt );
//    Point4D pt;
//    qDebug() << "no reference image, return origin ";
    return convertScenePointToPatientCoordinate(scene_pt);
}

MiaPoint4D MiaImageView2D::convertScenePointToPatientCoordinate(QPointF scenept)
{
    MiaPoint4D spt(scenept.rx(),scenept.ry(),0,0);
    return transform.toPatientCoordinate(spt);
}

QPointF MiaImageView2D::convertPatientCoordinateToScenePoint( MiaPoint4D pt)
{
    MiaPoint4D spt = transform.fromPatientCoordinate(pt);
    QPointF scenept(spt.pos[0],spt.pos[1]);
    return scenept;
}

QPoint MiaImageView2D::convertPatientCoordinateToViewPoint( MiaPoint4D pt)
{
    QPointF scenept = convertPatientCoordinateToScenePoint(pt);
    return mapFromScene(scenept);
}

float MiaImageView2D::getSliceThickness()
{
    if(referenceImage)
    {
        return referenceImage->getSliceThickness();
    }
    else
    {
        return 0.0f;
    }
}

QCursor MiaImageView2D::getZoomCursor()
{
    QPixmap map(QString::fromUtf8(":/image/zoomcur"));
    QCursor cur(map,10,10);
    return cur;
}

QCursor MiaImageView2D::getPanCursor()
{
    QPixmap map(QString::fromUtf8(":/image/pancur"));
    QCursor cur(map,16,16);
    return cur;
}

QCursor MiaImageView2D::getWinCursor()
{
    QPixmap map(QString::fromUtf8(":/image/wincur"));
    QCursor cur(map);
    return cur;
}
void MiaImageView2D::removeQImage(MiaGraphics2DImageItem* img)
{
    bool needfindnewreference = false;
    int i;
    for( i = 0 ; i < imageItemList.count() ; i++)
    {
        ;
        if( imageItemList.at(i) == img)
        {
            if( referenceImage == img )
                needfindnewreference = true;
            imageScene->removeItem(img);
            imageItemList.removeAt(i);
            break;
        }
    }
    if(needfindnewreference)
    {
        if( imageItemList.count() )
        {
            referenceImage = imageItemList.at(0);
        }
        else
            referenceImage = NULL;
    }
}

void MiaImageView2D::addQImage(MiaGraphics2DImageItem* img)
{
    removeQImage(img);
    imageScene->addItem(img); // item is set at position 0,0
    imageItemList.append(img);
    topMostImageZValue ++;
    img->setZValue( topMostImageZValue );
    if( img->isValid() )
    {
        if(!referenceImage)
        {
            referenceImage = img;
            transform = img->getImageTransformMatrix();
            moveCrossHairTo(crossHair->centerPoint);
        }
        else
            repositionImages();
        resetViewSize();
    }
}

void MiaImageView2D::imageGeometryChanged(MiaGraphics2DImageItem* img)
{
    if(!img->isValid())
        return;
    if (! referenceImage)
    {
        referenceImage = img;
        transform = img->getImageTransformMatrix();
        resetViewSize();
    }

    if( referenceImage == img )
    {

        transform = referenceImage->getImageTransformMatrix();
        for(int i = 0; i < markerModuleList.size() ; i++)
        {
            if(expectedSliceThickness <= 0.0f)
            {
                markerModuleList.at(i)->setVisibleDistance(referenceImage->getSliceThickness());
            }
            markerModuleList.at(i)->setTransformMatrix(transform);
            markerModuleList.at(i)->update();
        }

    }
    repositionImages();
}
void MiaImageView2D::addMarkerModule(MiaMarker2GraphicsItem *module)
{
    if(module)
    {
        removeMarkerModule(module);
        module->setImageView(this);
        markerModuleList.append(module);
        QObject::connect(module, SIGNAL(moduleWillBeRemoved( MiaMarker2GraphicsItem*)), this, SLOT(removeMarkerModule(MiaMarker2GraphicsItem*)));
        module->setTransformMatrix(transform);
        module->update();
    }
}

void MiaImageView2D::removeMarkerModule(MiaMarker2GraphicsItem* module)
{
    int i;
    for(i = 0; i < markerModuleList.size() ; i++)
    {
        if ( markerModuleList.at(i) == module )
        {
            markerModuleList.removeAt(i);
            module->setImageView(NULL);
            module->disconnect(this);
            break;
        }
    }
}

void MiaImageView2D::showCrossHair(bool show)
{
    crossHair->setVisible(show);
}
