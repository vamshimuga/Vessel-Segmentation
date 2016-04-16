#include "miadrawingmanager.h"
#include "miaimageview2d.h"
#include "MiaMarkers/miamarker3dball.h"
#include "MiaMarkers/miamarker3dpoint.h"
#include "MiaMarkers/miamaker3dline.h"
#include "MiaMarkers/miamarker3dpolygon.h"
#include "MiaMarkers/miamarker3dbspline.h"
#include "MiaMarkers/miamarker3dbox.h"
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyDataWriter.h>


MiaDrawingManager::MiaDrawingManager(MiaNetwork *parent) :
    QAbstractListModel(parent)
{
    currentActiveMaker = NULL;
    currentType = MIA_3DBall;
    currentColor = QColor(Qt::green);
    selectionModel = NULL;
    itemNumber = 0;
    selfChangingSelection = false;
    wiredParameters.insert("cross_pat_x", 0);
    wiredParameters.insert("cross_pat_y", 0);
    wiredParameters.insert("cross_pat_z", 0);
    wiredParameters.insert("cross_pat_t", 0);
    ownerNetwork = parent;


}

MiaDrawingManager::~MiaDrawingManager()
{
    while(!markerList.isEmpty())
        delete markerList.takeFirst();
}

int MiaDrawingManager::rowCount(const QModelIndex &parent ) const
{
//    qDebug() << markerList.count();
    return markerList.count();


}
QVariant MiaDrawingManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
//    qDebug() << "request" <<index.row();


    if (index.row() >= markerList.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return markerList.at(index.row())->name;
    else
        return QVariant();

}
//QVariant DrawingManager::headerData(int section, Qt::Orientation orientation,
//                         int role ) const
//{
//    if (role != Qt::DisplayRole)
//        return QVariant();

//    if (orientation == Qt::Horizontal)
//        return QString("Markers");
//    else
//        return QString("Row");
//}


void MiaDrawingManager::buttonPressedAt( MiaPoint4D pt, MiaTransform& sceneTransform)
{
    logMessage("Mouse Clicked at " + QString::number(pt.pos[0]) + ", " + QString::number(pt.pos[1])+", "+ QString::number(pt.pos[2]));
    if(currentActiveMaker)
    {
        currentActiveMaker->mouseButtonPressedAT(pt);
    }
    else
    {
        switch(currentType)
        {
        case MIA_Point:
            currentActiveMaker = new MiaMarker3DPoint(pt, this);
            break;
        case MIA_Line:
            currentActiveMaker = new MiaMaker3DLine(pt, this);
            break;
        case MIA_Circle:
        case MIA_Rectangle:
        case MIA_Polygon:
            currentActiveMaker = new MiaMarker3DPolygon(pt, this);
            break;
        case MIA_Spline:
            currentActiveMaker = new MiaMarker3DBSpline(pt,this);
            break;
        case MIA_ClosedSpline:
            currentActiveMaker = new MiaMarker3DClosedBSpline(pt,this);
            break;
        case MIA_Text:
            break;
        case MIA_3DBall:
            currentActiveMaker = new MiaMarker3DBall(pt, this);
            break;
        case MIA_3DBox:
            currentActiveMaker = new MiaMarker3DBox(pt, this);
            break;
        }
        if(currentActiveMaker)
        {
            currentActiveMaker->setTransformMatrixOfInitial2DSlice(sceneTransform);
            addMarker( currentActiveMaker );
            logMessage("Created marker: " + currentActiveMaker->name);
        }
        else
        {
            logMessage("Fail to create marker");
        }
    }
    notifyCrossHairPosition( pt );
}
void MiaDrawingManager::notifyCrossHairPosition(MiaPoint4D pt)
{
    if(currentActiveMaker && currentActiveMaker->needCrossHairFolowTheFocus)
    {
        wiredParameters.insert("cross_pat_x", pt.pos[0]);
        wiredParameters.insert("cross_pat_y", pt.pos[1]);
        wiredParameters.insert("cross_pat_z", pt.pos[2]);
        wiredParameters.insert("cross_pat_t", pt.pos[3]);

        emit parameterChanged(wiredParameters);

    }
}

void MiaDrawingManager::markerRemoved(MiaMarker* marker)
{
    int i;
    for(i  = 0; i < markerList.size(); i++)
    {
        if(markerList.at(i) == marker)
            markerList.removeAt(i);
    }
    if(currentActiveMaker == marker)
    {
        currentActiveMaker = NULL;
    }
    emit markerHasBeenRemoved(marker);
    QModelIndex index1 = createIndex(0,0);
    QModelIndex index2 = createIndex(markerList.size(),0);
    emit dataChanged(index1, index2);
}

void MiaDrawingManager::buttonReleasedAt( MiaPoint4D pt, MiaTransform& sceneTransform)
{
    Q_UNUSED(sceneTransform);
    logMessage("Mouse released at " + QString::number(pt.pos[0]) + ", " + QString::number(pt.pos[1])+", "+ QString::number(pt.pos[2]));
    if(currentActiveMaker)
    {
        currentActiveMaker->mouseButtonReleasedAT(pt);
        emit mouseReleased();
    }
    notifyCrossHairPosition( pt );
}

void MiaDrawingManager::mouseMovedAt( MiaPoint4D pt, MiaTransform& sceneTransform)
{
    Q_UNUSED(sceneTransform);
    logMessage("Mouse Moved to " + QString::number(pt.pos[0]) + ", " + QString::number(pt.pos[1])+", "+ QString::number(pt.pos[2]));
    if(currentActiveMaker)
    {
        currentActiveMaker->mouseMovedAT(pt);
    }
    notifyCrossHairPosition( pt );
}

void MiaDrawingManager::mouseDoubleClickedAt( MiaPoint4D pt, MiaTransform& sceneTransform)
{
    Q_UNUSED(sceneTransform);
    if(currentActiveMaker)
    {
        currentActiveMaker->mouseDoubleClickedAt(pt);
    }
    notifyCrossHairPosition( pt );
}

void MiaDrawingManager::keyPressed (int key, MiaTransform& sceneTransform)
{
    Q_UNUSED(sceneTransform);
    logMessage("Key pressed: " + QString::number(key));
}

void MiaDrawingManager::keyReleased( int key, MiaTransform& sceneTransform)
{
    Q_UNUSED(sceneTransform);
    logMessage("Key released: " + QString::number(key));
    if(key == Qt::Key_Delete || key == Qt::Key_Backspace)
    {
        deleteCurrentMarker();
    }
    else if(key == Qt::Key_Enter || key == Qt::Key_Space)
    {
        if(currentActiveMaker != NULL)
        {
            MiaMarker* themarker = currentActiveMaker;
            if(currentActiveMaker->markerCreatingFinished())
            {
                selectAnotherMarker(themarker);
            }
        }
        else
        {
            selectAnotherMarker(NULL);
        }
    }
}

void MiaDrawingManager::selectAnotherMarker( MiaMarker* marker )
{
    int selectedIndex = -1 ;
    if( currentActiveMaker && currentActiveMaker != marker )
    {
        MiaMarker* themarker = currentActiveMaker;
        if(currentActiveMaker->markerCreatingFinished())
        {
            themarker->setSelected(false);
            emit markerModeChanged(themarker);
        }

    }
    if(marker && marker->currentMode != MIA_Selected && marker->currentMode != MIA_Initializing)
    {
        marker->currentMode = MIA_Selected;
        emit markerModeChanged(marker);
    }
    int i;
    for( i = 0 ; i < markerList.size() ; i++ )
    {
        if( markerList.at(i) != marker )
            markerList.at(i)->setSelected(false);
        else
            selectedIndex = i;
    }

    QModelIndex index = createIndex( selectedIndex, 0 );
    selfChangingSelection = true;

    if(selectionModel)
    {
        if ( index.isValid() )
        {
            QItemSelection selection(index, index);
            selectionModel->select( selection, QItemSelectionModel::ClearAndSelect );
        }
        else
        {
            index = createIndex( 0, 0 );
            QItemSelection selection(index, index);
            selectionModel->select( selection, QItemSelectionModel::Clear);
        }
    }
    selfChangingSelection = false;


}
void MiaDrawingManager::currentMarkerCompleted( MiaMarker* marker )
{
    if(currentActiveMaker == marker )
    {
//        marker->setSelected(true);
        currentActiveMaker = NULL;
    }
}

void MiaDrawingManager::addMarker(MiaMarker* marker)
{
    markerList.append( marker );
    emit markerAdded( marker );
    itemNumber++;
    marker->name += QVariant(itemNumber).toString();
    marker->color = currentColor;
    QColor brcol = QColor(currentColor);
    brcol.setAlpha(marker->brushColor.alpha());
    marker->brushColor = brcol;
    brcol.setAlpha(marker->preActiveColor.alpha());
    marker->preActiveColor  = brcol;
    QModelIndex index1 = createIndex(0,0);
    QModelIndex index2 = createIndex(markerList.size(),0);
    emit dataChanged(index1, index2);
    selectAnotherMarker( marker );
}
void MiaDrawingManager::setSelectionModel(QItemSelectionModel* model)
{
    selectionModel = model;
    QObject::connect(model, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
}
void MiaDrawingManager::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
    Q_UNUSED(deselected);
    if(selfChangingSelection)
        return;
    QModelIndexList indexes = selected.indexes();
    if(indexes.isEmpty())
        return;
    QModelIndex index = indexes.first();

    int selectedIndex = index.row();
    if(selectedIndex >= 0 && selectedIndex<markerList.size())
    {
        if(markerList.at(selectedIndex)->currentMode != MIA_Selected)
        {
             markerList.at(selectedIndex)->justSelectedAndNeedResetFocus = true;
             markerList.at(selectedIndex)->setSelected( true );
             markerList.at(selectedIndex)->justSelectedAndNeedResetFocus = false;
        }
    }
}
Qt::ItemFlags MiaDrawingManager::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MiaDrawingManager::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        delete markerList.takeAt(position);
    }

    endRemoveRows();
    return true;
}
bool MiaDrawingManager::setData(const QModelIndex &index,
                               const QVariant &value, int role)
 {
     if (index.isValid() && role == Qt::EditRole) {

         markerList.at(index.row())->name = value.toString();
         emit dataChanged(index, index);
         return true;
     }
     return false;
 }
void MiaDrawingManager::getAllMarkers(QList <MiaMarker*>& list)
{
    list.clear();
    for(int i = 0; i < markerList.size(); i++)
        list.append( markerList.at(i) );
}

void MiaDrawingManager::connectToDownStreamAgent(QObject* agent)
{
    QObject::connect(this, SIGNAL(markerAdded(MiaMarker*)), agent, SLOT( markerAdded(MiaMarker*)));
    QObject::connect(this, SIGNAL(markerHasBeenRemoved( MiaMarker*)), agent, SLOT(markerRemoved(MiaMarker*)));
    QObject::connect(this, SIGNAL(markerUpdated(MiaMarker*)), agent, SLOT(markerUpdated( MiaMarker*)));
    QObject::connect(this, SIGNAL(markerModeChanged(MiaMarker*)), agent, SLOT(markerModeChanged( MiaMarker*)));

}
void MiaDrawingManager::freezeAllMarkers( bool freeze )
{
    if( currentActiveMaker )
        return;
//    selectAnotherMarker(NULL);
    for(int i = 0; i < markerList.size(); i++)
    {
         markerList.at(i)->isFrozen = freeze;
         emit markerUpdated(markerList.at(i));
    }
}

void MiaDrawingManager::hideAllMarkers(bool hide)
{
    if( currentActiveMaker )
        return;

    for(int i = 0; i < markerList.size(); i++)
    {
         markerList.at(i)->isVisible = !hide;
         emit markerUpdated(markerList.at(i));
    }
}

void MiaDrawingManager::saveMarkers(QDataStream& out)
{
    out << (qint32)markerList.size();
    for(int i = 0 ; i < markerList.size(); i++)
    {
        out << markerList.at(i)->type;
        out << *(markerList.at(i));
    }
}
void MiaDrawingManager::loadMarkers(QDataStream& in)
{
    qint32 listsize;
    in >> listsize;
    int type;
    MiaMarker* tempmarker = NULL;
    MiaPoint4D pt;
    for(int i = 0 ; i < listsize; i++)
    {
        in >> type;
        switch((MarkerType)type)
        {
        case MIA_Point:
            tempmarker = new MiaMarker3DPoint(pt, this);
            break;
        case MIA_Line:
            tempmarker = new MiaMaker3DLine(pt, this);
            break;
        case MIA_Circle:
        case MIA_Rectangle:
        case MIA_Polygon:
        case MIA_ClosedPolygon:
            tempmarker = new MiaMarker3DPolygon(pt,this);
            break;
        case MIA_Spline:
            tempmarker = new MiaMarker3DBSpline(pt,this);
            break;
        case MIA_ClosedSpline:
            tempmarker = new MiaMarker3DClosedBSpline(pt,this);
            break;
        case MIA_Text:
            break;
        case MIA_3DBall:
            tempmarker = new MiaMarker3DBall(pt, this);
            break;
        case MIA_3DBox:
            tempmarker = new MiaMarker3DBox(pt, this);
        default:
            throw std::runtime_error("Unsupported Maker Type");
        }

        in >> *tempmarker;
        addMarker(tempmarker);
        tempmarker->currentMode = MIA_Selected;
    }
}

void MiaDrawingManager::getAllMarkersByCopy(QList<MiaMarker *> &list)
{
    for(int i = 0; i < markerList.size(); i++)
    {
        list.append( markerList.at(i)->getANewCopy(NULL));
    }
}

void MiaDrawingManager::setMarkersByCopy(QList<MiaMarker *> &list)
{
    deleteAllMarkers();
    for(int i = 0; i < list.size(); i++)
    {
        addMarker( list.at(i)->getANewCopy(this) ); //the parent of the marker must be set.
        markerList.last()->currentMode = MIA_Selected;
    }
}
void MiaDrawingManager::loadCenterLineFromMICCAIChallenge(QString filename, const MiaTransform& toPatientTransform)
{
    Q_UNUSED(toPatientTransform);
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    MiaPoint4D pt;
    currentActiveMaker = NULL;
    for( int i = markerList.size() - 1 ; i >=0  ; i-- )
    {
        if(markerList.at(i)->type != MIA_Spline)
            delete markerList.at(i);
    }
    QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();

            QStringList list = line.split(" ");
            if(list.size()<3)
                break;

            for (int i = 0; i < 3; i++)
            {

                QString para = list.at(i);
                pt.pos[i] = para.toFloat();

            }
//            qDebug()<<pt.pos[0]<<pt.pos[1]<<pt.pos[2];
            if(currentActiveMaker == NULL)
            {
                currentActiveMaker = new MiaMarker3DBSpline(pt, this);
                currentActiveMaker->name = filename;
                addMarker( currentActiveMaker );
            }
            else
            {
                currentActiveMaker->mouseButtonPressedAT(pt);
            }

        }
        if(currentActiveMaker != NULL)
        {
            MiaMarker3DBSpline* marker = (MiaMarker3DBSpline*)currentActiveMaker;
            marker->mouseDoubleClickedAt(currentActiveMaker->getKeyPointAt(currentActiveMaker->getKeyPointNumber()-1));
            marker->keyValueList.clear();
            for(int i = 0 ; i < marker->getKeyPointNumber(); i++)
            {
                MiaPoint4D v;
                v.pos[3] = -1.5;
                marker->keyValueList.append(v);
            }
//            marker->resampleKeyPointsWithStepLength(0.1);
//            selectAnotherMarker(NULL);
        }
}

void MiaDrawingManager::loadAllCenterLinesFromMICCAIChallenge(QString filename, const MiaTransform& toPatientTransform)
{
    Q_UNUSED(toPatientTransform);
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    MiaPoint4D pt;
    int current_vessel, vessel;
    MiaMarker * lastActiveMarker;

    currentActiveMaker = NULL;
    for( int i = markerList.size() - 1 ; i >=0  ; i-- )
    {
        if(markerList.at(i)->type != MIA_Polygon)
            delete markerList.at(i);
    }
    QTextStream in(&file);
    current_vessel = vessel = -1;
        while (!in.atEnd()) {
            QString line = in.readLine();

            QStringList list = line.split(" ");

            if(list.size()==3 || list.size()==4 )
            {
                for (int i = 0; i < 3; i++)
                {

                    QString para = list.at(i);
                    pt.pos[i] = para.toFloat();

                }
                if (list.size() == 4)
                {
                    vessel = list.at(3).toInt(); //number of vessel
                }
            }
            else if(list.size() == 5)
            {
                for (int i = 0; i < 3; i++)
                {

                    QString para = list.at(i+1);
                    pt.pos[i] = para.toFloat();

                }
                vessel = list.at(0).toInt(); //number of vessel
            }
            else
                break;
//            qDebug()<<pt.pos[0]<<pt.pos[1]<<pt.pos[2];
            if(currentActiveMaker == NULL || current_vessel != vessel )
            {
                lastActiveMarker = currentActiveMaker;
                currentActiveMaker = new MiaMarker3DBSpline(pt, this);
                addMarker( currentActiveMaker );
                current_vessel = vessel;
//                qDebug()<<vessel;
                if ( lastActiveMarker !=NULL )
                {
                    MiaMarker3DBSpline* marker = (MiaMarker3DBSpline*)lastActiveMarker;
                    marker->mouseDoubleClickedAt(lastActiveMarker->getKeyPointAt(lastActiveMarker->getKeyPointNumber()-1));
                    marker->keyValueList.clear();
                    for(int i = 0 ; i < marker->getKeyPointNumber(); i++)
                    {
                        MiaPoint4D v;
                        v.pos[3] = -1.5;
                        marker->keyValueList.append(v);
                    }
                    marker->resampleKeyPointsWithStepLength(0.1);
                    //selectAnotherMarker(NULL);
                }
            }
            else
            {
                currentActiveMaker->mouseButtonPressedAT(pt);
            }

        }
        if(currentActiveMaker != NULL)
        {
            MiaMarker3DBSpline* marker = (MiaMarker3DBSpline*)currentActiveMaker;
            marker->mouseDoubleClickedAt(currentActiveMaker->getKeyPointAt(currentActiveMaker->getKeyPointNumber()-1));
            marker->keyValueList.clear();
            for(int i = 0 ; i < marker->getKeyPointNumber(); i++)
            {
                MiaPoint4D v;
                v.pos[3] = -1.5;
                marker->keyValueList.append(v);
            }
            marker->resampleKeyPointsWithStepLength(0.1);
            selectAnotherMarker(NULL);
        }
}

void MiaDrawingManager::deleteCurrentMarker()
{
    if(currentActiveMaker)
        delete currentActiveMaker;
    else
    {
        for( int i = 0 ; i < markerList.size() ; i++ )
        {
            if( markerList.at(i)->currentMode == MIA_Selected )
            {
                delete markerList.at(i);
                break;
            }
        }
    }
}
void MiaDrawingManager::deleteAllMarkers()
{
    while(!markerList.isEmpty())
    {
       int listsize = markerList.size();
       delete markerList.last(); // signal is sent from marker deconstructive function
       if(markerList.size() == listsize)
       {
           qDebug() << "Marker is deleted, but not removed from the list. The parent of the marker may not be set correctly";
           throw std::runtime_error("MarkerList is not updated");
       }
    }
}

void MiaDrawingManager::setAllMarkersToColor(const QColor &color)
{
    for( int i = 0 ; i < markerList.size() ; i++ )
    {
        if( markerList.at(i)->color != color )
        {
            markerList.at(i)->setMarkerColor( &color);
        }
    }
}

void MiaDrawingManager::setAllMarkersToShowHighlights(bool show)
{
    for( int i = 0 ; i < markerList.size() ; i++ )
    {
        markerList.at(i)->showHighlights(show);
    }
}

void MiaDrawingManager::loadMarkersFromAnotherManager(MiaDrawingManager* manager)
{
    if(manager)
    {
        deleteAllMarkers();
        QList <MiaMarker*> newMarkerList;
        manager->getAllMarkers(newMarkerList);
        for(int i =0; i < newMarkerList.size(); i++)
        {
            MiaMarker* marker = newMarkerList.at(i);
            addMarker( marker->getANewCopy(this) );
        }
        selectAnotherMarker(NULL);
    }
}

void MiaDrawingManager::saveClosedSplineToVTKPolyData(QString filename)
{
    if(!filename.endsWith(".vtk", Qt::CaseInsensitive))
        filename.append(".vtk");

    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();

    int pt_count = 0;
    for( int i = 0 ; i < markerList.size() ; i++ )
    {
        if( markerList.at(i)->type == MIA_ClosedSpline )
        {
            MiaMarker3DClosedBSpline* contour = (MiaMarker3DClosedBSpline*)markerList[i];
            QList<MiaPoint4D> points;
            contour->getRenderPoints(points);
            int cell_start_ptId = pt_count;
            polygons->InsertNextCell(points.size()+1);

            foreach(const MiaPoint4D pt, points)
            {
                pts->InsertPoint(pt_count,pt.pos);
                polygons->InsertCellPoint(pt_count);
                pt_count++;
            }
            polygons->InsertCellPoint(cell_start_ptId);


        }
    }
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints( pts );
    polydata->SetLines( polygons );
    vtkSmartPointer<vtkPolyDataWriter>  exporter =  vtkSmartPointer<vtkPolyDataWriter>::New();
    exporter->SetInputData(polydata);
    QByteArray byteArray = filename.toUtf8();
    const char* cString = byteArray.constData();
    exporter->SetFileName( cString );
    exporter->Write();

}
