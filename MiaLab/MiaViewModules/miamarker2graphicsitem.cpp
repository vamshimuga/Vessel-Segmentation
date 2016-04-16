#include "miamarker2graphicsitem.h"
#include "miaimageview2d.h"
#include "miadrawingmanager.h"

MiaMarker2GraphicsItem::MiaMarker2GraphicsItem(QObject *parent) :
    QObject(parent)
{
    imageViewer = NULL;
    markerManager = NULL;
    topMostMarkerZValue = MARKERMINZVALUE;
    markersAreEditable = true;
    markersAlpha = 0.5f;
    visibleDistance = 0.1f;
}
MiaMarker2GraphicsItem::~MiaMarker2GraphicsItem()
{
    while(!markerItemList.isEmpty())
    {
            if(imageViewer)
                imageViewer->scene()->removeItem( markerItemList.first() );
            delete markerItemList.takeFirst();
    }

    emit moduleWillBeRemoved(this);
}
void MiaMarker2GraphicsItem::setImageView(MiaImageView2D *v)
{
    if(imageViewer!=v && imageViewer!=0)
    {
        for(int i = 0; i < markerItemList.size() ; i++)
        {
            imageViewer->scene()->removeItem(markerItemList.at(i));
        }
    }
    imageViewer = v;
    if(imageViewer)
    {
        visibleDistance = imageViewer->getSliceThickness();
        for(int i = 0; i < markerItemList.size() ; i++)
        {
            markerItemList.at(i)->resetParentView(imageViewer);
            markerItemList.at(i)->setVisibleDistance(visibleDistance);
            imageViewer->scene()->addItem(markerItemList.at(i));
        }
    }
}
void MiaMarker2GraphicsItem::setNewMarkerManger(MiaDrawingManager* manager)
{
    if( manager != markerManager)
    {
        markerManager->disconnect(this);
        this->disconnect(markerManager);
        for(int i = markerItemList.size() - 1; i >=0  ; i--)
        {
            if(imageViewer)
                imageViewer->scene()->removeItem( markerItemList.at(i) );
            delete markerItemList.takeAt(i);
        }
        markerItemList.clear();
        markerManager = manager;

        QList <MiaMarker*> newMarkerList;
        markerManager->getAllMarkers(newMarkerList);
        for(int i =0; i < newMarkerList.size(); i++)
        {
            MiaMarker* marker = newMarkerList.at(i);
            markerAdded( marker );
        }
        update();

    }
}

void MiaMarker2GraphicsItem::setMarkersToEditable(bool editable)
{
    markersAreEditable = editable;
    for(int i = 0; i < markerItemList.size() ; i++)
    {
        markerItemList.at(i)->setEditable(editable);
    }
}

void MiaMarker2GraphicsItem::setMarkersAlpha(float alpha)
{
    markersAlpha = alpha;
    for(int i = 0; i < markerItemList.size() ; i++)
    {
        markerItemList.at(i)->setMarkerAlpha(markersAlpha);
    }
}

void MiaMarker2GraphicsItem::setVisibleDistance(float dist)
{
    visibleDistance = dist;
    for(int i = 0; i < markerItemList.size() ; i++)
    {
        markerItemList.at(i)->setVisibleDistance(visibleDistance);
    }
}

void MiaMarker2GraphicsItem::setTransformMatrix(MiaTransform &t)
{
    transform = t;
    for(int i = 0; i < markerItemList.size() ; i++)
    {
        markerItemList.at(i)->transformFromSceneToPatient = t;
    }
}

void MiaMarker2GraphicsItem::update()
{
    for(int i = 0; i < markerItemList.size() ; i++)
    {
        markerItemList.at(i)->updateShapePath();
    }
}

void MiaMarker2GraphicsItem::markerAdded(MiaMarker* marker)
{
    if(marker)
    {
        markerRemoved(marker);
        MiaGraphicsMarkerItem* item = new MiaGraphicsMarkerItem(imageViewer, marker);
        markerItemList.append(item);
        if(imageViewer)
            imageViewer->scene()->addItem(item);
        item->transformFromSceneToPatient = transform;
        item->updateShapePath();
        item->setEditable(markersAreEditable);
        item->setZValue(topMostMarkerZValue);
        item->setMarkerAlpha(markersAlpha);
        item->setVisibleDistance(visibleDistance);
        topMostMarkerZValue++;

    }
}

void MiaMarker2GraphicsItem::markerUpdated(MiaMarker* marker)
{
    for(int i = 0; i < markerItemList.size() ; i++)
    {
        if( markerItemList.at(i)->parentMarker == marker)
        {
            markerItemList.at(i)->updateShapePath();
//            markerItemList.at(i)->update();
            break;
        }

    }
}

void MiaMarker2GraphicsItem::markerModeChanged(MiaMarker* marker)
{
    markerUpdated(marker);
}

void MiaMarker2GraphicsItem::markerRemoved(MiaMarker* marker)
{
    for(int i = 0; i < markerItemList.size() ; i++)
    {
        if( markerItemList.at(i)->parentMarker == marker)
        {
            if(imageViewer)
                imageViewer->scene()->removeItem( markerItemList.at(i) );
            delete markerItemList.takeAt(i);
            break;
        }
    }
}
