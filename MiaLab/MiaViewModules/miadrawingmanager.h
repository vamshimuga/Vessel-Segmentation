#ifndef DRAWINGMANAGER_H
#define DRAWINGMANAGER_H

#include <QAbstractListModel>
#include <QPoint>
#include "MiaCoreModules/miamarker.h"
#include "MiaCoreModules/mianetwork.h"

class MIAVIEWMODULESSHARED_EXPORT MiaDrawingManager : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MiaDrawingManager(MiaNetwork *parent = 0);
    virtual ~MiaDrawingManager();

    MarkerType currentType;
    QColor currentColor;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
//    QVariant headerData(int section, Qt::Orientation orientation,
//                             int role = Qt::DisplayRole) const;
    void setSelectionModel(QItemSelectionModel* model);
    bool setData(const QModelIndex &index, const QVariant &value,
                    int role = Qt::EditRole);
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void connectToDownStreamAgent(QObject* agent);
    void freezeAllMarkers( bool freeze );
    void hideAllMarkers(bool hide);
    void saveMarkers(QDataStream& out);
    void loadMarkers(QDataStream& in);
    void getAllMarkersByCopy(QList <MiaMarker*>& list);
    void setMarkersByCopy(QList <MiaMarker*>& list);
    void deleteCurrentMarker();
    void deleteAllMarkers();
    void setAllMarkersToColor(const QColor& color);
    void setAllMarkersToShowHighlights(bool show);
    void loadCenterLineFromMICCAIChallenge(QString filename, const MiaTransform& toPatientTransform);
    void loadAllCenterLinesFromMICCAIChallenge(QString filename, const MiaTransform& toPatientTransform);
    void loadMarkersFromAnotherManager(MiaDrawingManager* manager);
    void saveClosedSplineToVTKPolyData(QString filename);
    void addMarker(MiaMarker* marker);

signals:
    void markerUpdated(MiaMarker* marker);
    void markerHasBeenRemoved(MiaMarker* marker);
    void markerAdded(MiaMarker* marker);
    void markerModeChanged(MiaMarker* marker);
    void parameterChanged(const QMap <QString, float> &parameters);
    void mouseReleased();
public slots:
    void buttonPressedAt( MiaPoint4D pt, MiaTransform& sceneTransform);
    void buttonReleasedAt( MiaPoint4D pt, MiaTransform& sceneTransform);
    void mouseMovedAt ( MiaPoint4D pt, MiaTransform& sceneTransform);
    void mouseDoubleClickedAt( MiaPoint4D pt, MiaTransform& sceneTransform);
    void keyPressed (int key, MiaTransform& sceneTransform);
    void keyReleased( int key, MiaTransform& sceneTransform);
    void selectAnotherMarker( MiaMarker* marker );
    void markerRemoved(MiaMarker* marker);
    void currentMarkerCompleted( MiaMarker* marker );
    void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
    void getAllMarkers(QList <MiaMarker*>& list);
    void forwardMarkerUpdatedSignal( MiaMarker* marker ) { emit markerUpdated(marker); }
    void forwardMarkerModeChanged(  MiaMarker* marker ) { emit markerModeChanged(marker); }

protected:
    MiaNetwork* ownerNetwork;
    MiaMarker* currentActiveMaker;

    QList <MiaMarker*> markerList;
    QMap <QString, float> wiredParameters;
    void notifyCrossHairPosition(MiaPoint4D pt);
    void logMessage(QString msg) { if(ownerNetwork->enviromentSetting->logging_level>=4)ownerNetwork->enviromentSetting->logMessage(msg);}
    QItemSelectionModel* selectionModel;
    bool selfChangingSelection;
    int itemNumber;


};

#endif // DRAWINGMANAGER_H
