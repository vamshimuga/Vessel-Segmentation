#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include "MiaCoreModules/mianetwork.h"
#include "MiaApp/MiaNetworks/miabasicmprandvrtnetwork.h"
#include "MiaApp/MiaNetworks/miathresholdlevelsetnetwork.h"
#include "MiaApp/MiaNetworks/miavesselanalysisnetwork.h"
#include "MiaApp/MiaNetworks/miaregistrationnetwork.h"

#include "MiaViewModules/miaimageexporter.h"
#include "MiaCommonModules/miaresample3d.h"
#include "MiaCommonModules/miaorthopermutefilter.h"
#include "aboutdialog.h"
#include <QNetworkInterface>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    savedResultToSharedMemory = false;
    currentNetwork = NULL;
    ui->setupUi(this);
    setWindowTitle(tr("MIALite"));
    reader= new MiaImageReader();

    ui->methodSelectionWidget->hide();

    resetToolBoxPages();

    QList<int> defaultSpliterSizeList;
    defaultSpliterSizeList.append(1);
    defaultSpliterSizeList.append(0);
    ui->mPRSplitter->setSizes(defaultSpliterSizeList);
    ui->curvedMPRPage->setVisible(false);
//    ui->methodButton2->setEnabled(false);
//    ui->methodButton_VesselAnlysis->setEnabled(false);

    ui->openButton->setMenu(ui->menuOpen);
    ui->chooseOperationButton->setMenu(ui->menuOperations);
    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setContentsMargins(0,0,0,0);
    ui->gobackOsiriX->setVisible(false);
    ui->gobackOsiriX2->setVisible(false);

    connect(ui->actionThresholdBasedSegmentation,SIGNAL(triggered()), this, SLOT(startThresholdBasedLevelsetSegmentation()));
    connect(ui->actionClickDragInteractiveSegmentation,SIGNAL(triggered()), this, SLOT(startInteractiveThresholdBasedLevelsetSegmentation()));
    connect(ui->actionModelBasedSegmentation,SIGNAL(triggered()), this ,SLOT(startModelBasedSegmentation()));
//    connect(ui->methodButton3,SIGNAL(triggered()), this ,SLOT(startTemplateGuidedPhaseBasedSegmentation()));
    connect(ui->actionVesselAnalysis,SIGNAL(triggered()), this, SLOT(startVesselAnalysis()));
    connect(ui->actionRegistration,SIGNAL(triggered()), this, SLOT(startRegistration()));
    connect(ui->actionBrainSegmentation,SIGNAL(triggered()),this, SLOT(startBrainSegmentationChain()));
    connect(ui->actionLiverSegmentMarking,SIGNAL(triggered()),this, SLOT(startManualLiverSegmentMarking()));
    connect(ui->actionLeftVentricleSegmentation, SIGNAL(triggered()), this, SLOT(startLeftVentricleSegmentationChain()));
    connect(ui->actionBodyPartDetectorTraining,SIGNAL(triggered()), this, SLOT(startRandomForestTraining()));
    connect(ui->actionBodyPartDetection, SIGNAL(triggered()), this, SLOT(startBodyPartDetection()));
    connect(ui->actionOpen_mhd_file, SIGNAL(triggered()), this, SLOT(openMHDImage()));
    connect(ui->actionOpen_Nifti_file, SIGNAL(triggered()), this, SLOT(openNiftiImage()));
    connect(ui->actionOpen_VTK_file, SIGNAL(triggered()), this, SLOT(openVTKImage()));
    connect(ui->actionOpen_DICOM_folder,SIGNAL(triggered()), this, SLOT(openDICOMImage()));
    connect(ui->actionOpen_2D_image, SIGNAL(triggered()), this, SLOT(open2DFile()));
    connect(ui->actionNextStep, SIGNAL(triggered()), this, SLOT(startVesselAnalysis()));
    connect(ui->actionFuzzyConnectednessSegmentation, SIGNAL(triggered()), this, SLOT(startFuzzyConnectednessSegmentation()));
    connect(ui->actionBoneSegmentationPrinting, SIGNAL(triggered()), this, SLOT(startBoneSegmentationAndPrintChain()));
    //---Code by vamshi ---//
    connect(ui->actionMulti_seed_vessel_segmentation,SIGNAL(triggered()), this , SLOT(startMultiSeedVesselSegmentation()));

    ui->actionWindow_Setting->setEnabled(false);
    ui->actionPan->setEnabled(false);
    ui->actionZoom->setEnabled(false);
    ui->actionCurve_ROI->setEnabled(false);

    //-------------RMS
//    connect(ui->actionOpen_Challenge_folder,SIGNAL(triggered()), this, SLOT(openChallengeFolder()));
    //-------------RMS
    connect(ui->actionAbout_MiaLite, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(ui->actionOnline_Tutorial, SIGNAL(triggered()), this, SLOT(openOnlineTutorial()));
    connect(ui->gobackOsiriX, SIGNAL(clicked()),this,SLOT(saveResultToSharedMemory()));
    connect(ui->gobackOsiriX2, SIGNAL(clicked()),this,SLOT(saveResultToSharedMemory()));
    connect(ui->multi_Resolution_CheckBox,SIGNAL(clicked(bool)),this, SLOT(setLevelSetToUsingMultiResolution(bool)) );
    ui->toolBox->setCurrentIndex(0);


    //for release
//    ui->saveSeedButton->setVisible(false);
//    ui->loadSeedButton->setVisible(false);
    environmentSettings.setLogFileName(QApplication::applicationDirPath() + "/log.txt");
    environmentSettings.logging_level = 3;
//    QMap<QString,QVariant> testmap;
//    testmap.insert("test",3.1415926);
//    QList<QVariant> lut2;
//    lut2.append(QPointF(1700,1.0));
//    lut2.append(QPointF(2000,0.0));
//    lut2.append(QPointF(2300,-1.0));
//    testmap.insert("testlut",lut2);
//    environmentSettings.setNetworkSettingFile(QApplication::applicationDirPath() + "/network.xml");
//    environmentSettings.saveParametersToXMLDoc(testmap);
//    QMap<QString,QVariant> testoutmap;
//    testoutmap.insert("test",0);
//    testoutmap.insert("testlut",0);
//    environmentSettings.readParametersFromXMLDoc(testoutmap);
//    double a = testoutmap["test"].toDouble();
//    qDebug() << testoutmap["test"].toDouble();
//    qDebug() << testoutmap["testlut"].toList().at(0).toPointF();
//    qDebug() << testoutmap["testlut"].toList().at(1).toPointF();
//    qDebug() << testoutmap["testlut"].toList().at(2).toPointF();
    currentWorkListStep = -1;
    workListStepTimer = new QTimer(this);
    connect(workListStepTimer, SIGNAL(timeout()), this, SLOT(stepThroughWorkList()));


}


MainWindow::~MainWindow()
{
//    if(window2d)
//        delete window2d;
    if(currentNetwork)
        currentNetwork->destroyNetwork(); //destroyNetwork is called before the window is destroyed, to avoid qgraphimageitem to be destroyed by the scene
    if(reader)
        delete reader;
    delete ui;
}

void MainWindow::openMHDImage()
{
    qDebug()<<"open file";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), 0, tr("Images (*.mhd)"));
    if (fileName != "")
    {
       if(loadImageWithNameAndFormat(fileName, MIA_MHD))
       {
           startBasic3DVisualizationNetwork();
           setWindowTitle(fileName);
       }

    }
    else
    {
        qDebug() << "User canceled open file";
    }

}

void MainWindow::openNiftiImage()
{
    qDebug()<<"open file";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), 0, tr("Images (*.nii *.nii.gz)"));
    if (fileName != "")
    {
        if(loadImageWithNameAndFormat(fileName, MIA_NII))
        {
            startBasic3DVisualizationNetwork();
            setWindowTitle(fileName);
        }
     }
     else
     {
         qDebug() << "User canceled open file";
     }
}

//Code by Vamshi
void MainWindow::startMultiSeedVesselSegmentation()
{
    MiaThresholdLevelsetNetwork* network = new MiaThresholdLevelsetNetwork(&environmentSettings,this);
    if(switchActiveNetwork(network))
    {
        disableMethodButtons();
        ui->thresholdWidget->show();
    }
}


void MainWindow::openDICOMImage()
{

    qDebug()<<"open DICOM directory";
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose director"));
    if (fileName != "")
    {
        if(loadImageWithNameAndFormat(fileName, MIA_DICOM))
        {
            startBasic3DVisualizationNetwork();
            setWindowTitle(fileName);
        }
     }
     else
     {
         qDebug() << "User canceled open file";
     }
}

void MainWindow::openVTKImage()
{
    qDebug()<<"open file";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), 0, tr("Images (*.vtk)"));
    if (fileName != "")
    {
        if(loadImageWithNameAndFormat(fileName, MIA_VTK))
        {
            startBasic3DVisualizationNetwork();
            setWindowTitle(fileName);
        }
     }
     else
     {
         qDebug() << "User canceled open file";
     }
}


void MainWindow::open2DFile()
{
    qDebug()<<"open file";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), 0, tr("Images (*.bmp *.png *.tif *.jpg)"));
    if (fileName != "")
    {
        if(loadImageWithNameAndFormat(fileName, MIA_2D))
        {
            startBasic3DVisualizationNetwork();
            setWindowTitle(fileName);
        }
     }
     else
     {
         qDebug() << "User canceled open file";
     }
}

void MainWindow::startBasic3DVisualizationNetwork()
{
    MiaBasicMPRandVRTNetwork* network = new MiaBasicMPRandVRTNetwork(&environmentSettings,this);
    switchActiveNetwork(network);
}

void MainWindow::initializeUserInterfaceWithImage()
{
    MiaImageND* im;
    reader->getOutputImage(&im);
    if(im)
    {
        char str[64];
        sprintf(str,"Image Size: %d * %d * %d\n", im->getWidth(), im->getHeight(), im->getDepth());
//        MiaTransform t = im->getTransformMatrix();
//        MiaAffineTransform aft = t.getBulkTransform();
//        qDebug() << aft.fromPatientSystemMatrix[0] << aft.fromPatientSystemMatrix[1] << aft.fromPatientSystemMatrix[2] << aft.fromPatientSystemMatrix[3];
//        qDebug() << aft.fromPatientSystemMatrix[4] << aft.fromPatientSystemMatrix[5] << aft.fromPatientSystemMatrix[6] << aft.fromPatientSystemMatrix[7];
//        qDebug() << aft.fromPatientSystemMatrix[8] << aft.fromPatientSystemMatrix[9] << aft.fromPatientSystemMatrix[10] << aft.fromPatientSystemMatrix[11];
//        qDebug() << aft.fromPatientSystemMatrix[12] << aft.fromPatientSystemMatrix[13] << aft.fromPatientSystemMatrix[14] << aft.fromPatientSystemMatrix[15];
        ui->imageSummary->setText(QString(str));
        ui->methodSelectionWidget->show();
        ui->openButton->setEnabled(false);
//        startInteractiveThresholdBasedLevelsetSegmentation();
    }
    else
    {
        ui->imageSummary->setText(QString("fail to load image"));
    }

}

void MainWindow::startThresholdBasedLevelsetSegmentation()
{
    MiaThresholdLevelsetNetwork* network = new MiaThresholdLevelsetNetwork(&environmentSettings,this);
    if(switchActiveNetwork(network))
    {
        disableMethodButtons();
        ui->thresholdWidget->show();
    }

}

void MainWindow::startVesselAnalysis()
{
    MiaVesselAnalysisNetwork* network = new MiaVesselAnalysisNetwork(&environmentSettings,this);
    if(switchActiveNetwork(network))
    {
        disableMethodButtons();
        ui->toolBox->setCurrentWidget(ui->VesselAnalysisPage);
    }
}


void MainWindow::startRegistration()
{
    MiaRegistrationNetwork* network = new MiaRegistrationNetwork(&environmentSettings,this );
    if(switchActiveNetwork(network))
    {
        disableMethodButtons();
        ui->toolBox->setCurrentWidget(ui->RegistrationPage);
    }
}

bool MainWindow::switchActiveNetwork(MiaNetwork *anetwork)
{
    Qt::WindowFlags flags = this->windowFlags();
    if(currentNetwork && (flags& Qt::WindowStaysOnTopHint))
    {
        this->setWindowFlags(flags & (~Qt::WindowStaysOnTopHint));
        this->show();
    }

    if(currentNetwork)
    {
        currentNetwork->destroyNetwork();
        delete currentNetwork;
        resetToolBoxPages();
    }
    currentNetwork = anetwork;
    currentNetwork->setupNetwork(reader);
    currentNetwork->setupUserInterface(ui);
    currentNetwork->activeUserInteraction();
    return true;
}

void MainWindow::disableMethodButtons()
{
    ui->chooseOperationButton->setEnabled(false);
}

void MainWindow::resetToolBoxPages()
{
    ui->thresholdWidget->hide();
    for(int i = ui->toolBox->count()-1;i>0;i--)
        ui->toolBox->removeItem(i);
    ui->step2->setVisible(false);
    ui->step3->setVisible(false);
    ui->step4->setVisible(false);
    ui->VesselAnalysisPage->setVisible(false);
    ui->InteracitveSegmentationPage->setVisible(false);
    ui->RegistrationPage->setVisible(false);
    ui->brainSegmentationPage->setVisible(false);
    ui->randomForestTrainingPage->setVisible(false);
}


//#ifdef Q_WS_MAC
// void MainWindow::loadImageFromSharedMemory(QString memid,int size)
// {
//     QProgressDialog progress("loading data...", 0, 0, 100,0,Qt::WindowStaysOnTopHint );
//     progress.setWindowModality(Qt::WindowModal);
//     progress.setValue(10);
//     progress.show();
//     ui->methodSelectionWidget->show();
//     ui->openButton->setEnabled(false);
//     ui->gobackOsiriX->setVisible(true);
//     ui->gobackOsiriX2->setVisible(true);
//     ui->saveButton->setVisible(false);
////     ui->saveSTLButton->setVisible(false);
//     Qt::WindowFlags flags = this->windowFlags();
//     this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
//     this->show();

//     if(reader->LoadImageInSharedMemory(memid, size))
//     {
//         progress.setValue(40);
//         initializeUserInterfaceWithImage();
//         startBasic3DVisualizationNetwork();

//     }
//     else
//     {
//          ui->imageSummary->setText(QString("Open file failed!"));
//     }
// }

//#endif
void MainWindow::saveResultToSharedMemory()
{
    MiaImageND* im;
    MiaImageND* result;
    reader->getOutputImage(&im);
    MiaModule* out = currentNetwork->getOutputModule();
    out->getOutputImage(&result);
    if(im->getType()==IM_short && result->getType()==IM_uchar && im->getImageSize() == result->getImageSize())
    {
        int totalsize = im->getImageSize();
        int i;
        unsigned char* result_ptr = (unsigned char*) result->getDataPtr();
        short* sharedmem = (short*) im->getDataPtr();
         #pragma omp parallel for
        for(i=0; i < totalsize; i++)
        {
            if( result_ptr[i] == 0 )
                sharedmem[i] = 0;
            else
                sharedmem[i] = 1;
        }
        savedResultToSharedMemory = true;
        close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("The result type or size doesn't match the input image.");
        msgBox.exec();
    }

}

#ifndef Q_WS_MAC
 void MainWindow::loadImageFromSharedMemory(QString memid,int size)
 {
     QMessageBox msgBox;
     msgBox.setText("SharedMemory is only supported on Mac OS");
     msgBox.exec();
 }

#endif


 bool MainWindow::loadImageWithNameAndFormat(QString input, int type)
 {
     QProgressDialog progress("loading data...", 0, 0, 100,0,Qt::WindowStaysOnTopHint );
     progress.setWindowModality(Qt::WindowModal);
     progress.setValue(10);
     progress.show();
     QString filename = input;
     reader->setFileName(filename,(FileFormat) type);
     bool result = reader->update();
     progress.setValue(50);
     if(result)
     {
          initializeUserInterfaceWithImage();
     }
     else
     {
         ui->imageSummary->setText(QString("Open file failed!"));
         qDebug() << "Fail to open file" << filename;
         return false;
     }
     progress.setValue(100);
     return true;
 }

 void MainWindow::showAboutDialog()
 {
     AboutDialog dialog(this);
     dialog.exec();
 }


 void MainWindow::openOnlineTutorial()
 {
     QUrl url("http://www.youtube.com/playlist?list=PLFAD92BECEF9E603F");
     QDesktopServices::openUrl(url);
 }


float MainWindow::getSavedValue(QString savedname)
{
    QMap<QString,QVariant> savedparameter;
    savedparameter.insert(savedname,0);
    environmentSettings.readParametersFromXMLDoc(savedparameter);
    qDebug() << savedparameter;
    return savedparameter[savedname].toFloat();
}
