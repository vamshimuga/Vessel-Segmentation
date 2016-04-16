#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "MiaCoreModules/networkenviromentvariable.h"

class MiaNetwork;
class MiaImageReader;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadImageFromSharedMemory(QString memid,int size);
    bool savedResultToSharedMemory;
    float getSavedValue(QString savedname);

public Q_SLOTS: //slots to receive action of menu trigger.
    void openMHDImage();
    void openNiftiImage();
    void openDICOMImage();
    void openVTKImage();
    void open2DFile();
    void startBasic3DVisualizationNetwork();
    void startThresholdBasedLevelsetSegmentation();
    void startVesselAnalysis();
    void startRegistration();
//Code by Vamshi
    void startMultiSeedVesselSegmentation();

    bool switchActiveNetwork(MiaNetwork* anetwork);
    void saveResultToSharedMemory();
    void showAboutDialog();
    void openOnlineTutorial();

private:
    Ui::MainWindow *ui;
    MiaNetwork* currentNetwork;
    void disableMethodButtons();
    void resetToolBoxPages();

    bool loadImageWithNameAndFormat(QString input, int type);
    void initializeUserInterfaceWithImage();

    MiaImageReader* reader;
    NetWorkEnviromentVariable environmentSettings;
    QList< QMap<QString, QVariant> > workList;
    int currentWorkListStep;
    QTimer *workListStepTimer;

};

#endif // MAINWINDOW_H
