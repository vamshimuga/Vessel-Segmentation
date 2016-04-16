#include "miaregistrationnetwork.h"
#include "ui_mainwindow.h"
#include "miaimageregistrationfilter.h"
#include "miaimageregistrationfilterwithtemplate.h"
#include "miaimagereader.h"
#include "miaImage2qimage.h"
#include "miaorthogonalslicer.h"
#include "mainwindow.h"
#include "miaparameterconnector.h"
#include "MiaViewModules/miaimageexporter.h"
#include <QtGui>
#include "miaconstants.h"

MiaRegistrationNetwork::MiaRegistrationNetwork(NetWorkEnviromentVariable *var, QObject *parent) :
    MiaBasicMPRandVRTNetwork(var,parent)
{
    registrationFilter = NULL;
    movingImageReader = NULL;
    axialRegisteredVolumeSlicer = NULL;
    saggitalRegisteredVolumeSlicer = NULL;
    coronalRegisteredVolumeSlicer = NULL;
    axialViewRegisteredMask = NULL;
    saggitalViewRegisteredMask = NULL;
    coronalViewRegisteredMask = NULL;
    exporter = NULL;

}

void MiaRegistrationNetwork::setupNetwork(MiaModule *input)
{
    MiaBasicMPRandVRTNetwork::setupNetwork(input);
    registrationFilter = new MiaImageRegistrationFilter(this);
//    registrationFilter = (MiaImageRegistrationFilter*) new MiaImageRegistrationFilterWithTemplate(this);
    movingImageReader = new MiaImageReader(this);
    axialRegisteredVolumeSlicer = new MiaOrthogonalSlicer(this);
    axialRegisteredVolumeSlicer->setOrientation(Axial);
    saggitalRegisteredVolumeSlicer = new MiaOrthogonalSlicer(this);
    saggitalRegisteredVolumeSlicer->setOrientation(Saggital);
    coronalRegisteredVolumeSlicer = new MiaOrthogonalSlicer(this);
    coronalRegisteredVolumeSlicer->setOrientation(Coronal);
    axialViewRegisteredMask = new MiaImage2QImage(this);
    saggitalViewRegisteredMask = new MiaImage2QImage(this);
    coronalViewRegisteredMask = new MiaImage2QImage(this);
    exporter = new MiaImageExporter(this);

    registrationFilter->setInputModule(input);
    registrationFilter->setMovingImageInputModule(movingImageReader);
    registrationFilter->setMaximumIteration(70);
//    registrationFilter->setSampleMethodToRandomPoints(500000);
    registrationFilter->setSampleMethodToDownSample(3);
//    registrationFilter->setSampleMethodToUsingMovingImage();
    connect(registrationFilter,SIGNAL(registrationComplete()),this,SLOT(registrationFinished()));

    exporter->setInputModule(registrationFilter);

    axialRegisteredVolumeSlicer->setInputModule(registrationFilter);
    axialViewRegisteredMask->setInputModule(axialRegisteredVolumeSlicer);

    saggitalRegisteredVolumeSlicer->setInputModule(registrationFilter);
    saggitalViewRegisteredMask->setInputModule(saggitalRegisteredVolumeSlicer);

    coronalRegisteredVolumeSlicer->setInputModule(registrationFilter);
    coronalViewRegisteredMask->setInputModule(coronalRegisteredVolumeSlicer);

    miaModuleList.append(registrationFilter);
    miaModuleList.append(movingImageReader);
    miaModuleList.append(axialRegisteredVolumeSlicer);
    miaModuleList.append(saggitalRegisteredVolumeSlicer);
    miaModuleList.append(coronalRegisteredVolumeSlicer);
    miaModuleList.append(axialViewRegisteredMask);
    miaModuleList.append(saggitalViewRegisteredMask);
    miaModuleList.append(coronalViewRegisteredMask);
    miaModuleList.append(exporter);
}

void MiaRegistrationNetwork::setupUserInterface(Ui::MainWindow *ui)
{
    MiaBasicMPRandVRTNetwork::setupUserInterface(ui);
    connect(userInterface->openMovingImageButton,SIGNAL(clicked()),this,SLOT(openMovingImage()));
    connect(userInterface->startRegistrationButton, SIGNAL(clicked()),this, SLOT(startRegistration()));
    connect(userInterface->resetRegistrationButton, SIGNAL(clicked()),this, SLOT(resetRegistrationFilter()));
    connect(userInterface->saveRegistrationButton,SIGNAL(clicked()),this,SLOT(saveTransformParameters()));
    connect(userInterface->loadRegistrationButton,SIGNAL(clicked()),this,SLOT(loadTransformParameters()));
    connect(userInterface->saveRegistrationResultButton,SIGNAL(clicked()),this,SLOT(saveRegistrationResults()));
    userInterface->RegistrationPage->setVisible(true);
    userInterface->toolBox->addItem(userInterface->RegistrationPage, QString("Registration"));

    //reslicing position connected to crosshair postion
    MiaParameterConnector* connector4 = new MiaParameterConnector(this);
    connector4->addParameterPairs(QString("cross_pat_x"),QString("pat_x"));
    connector4->addParameterPairs(QString("cross_pat_y"),QString("pat_y"));
    connector4->addParameterPairs(QString("cross_pat_z"),QString("pat_z"));
    connector4->addParameterPairs(QString("cross_pat_t"),QString("pat_t"));
    connector4->linkModules(userInterface->leftUpperView, axialRegisteredVolumeSlicer);
    connector4->linkModules(userInterface->rightUpperView, coronalRegisteredVolumeSlicer);
    connector4->linkModules(userInterface->leftLowerView, saggitalRegisteredVolumeSlicer);
    connector4->initializeUpperModuleParameter();

    userInterface->leftUpperView->addInputModule(axialViewRegisteredMask);
    userInterface->leftLowerView->addInputModule(saggitalViewRegisteredMask);
    userInterface->rightUpperView->addInputModule(coronalViewRegisteredMask);
}

void MiaRegistrationNetwork::openMovingImageWithName(QString filename)
{
    QProgressDialog progress("loading data...", 0, 0, 100,0,Qt::WindowStaysOnTopHint );
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(10);
    progress.show();
    movingImageReader->setFileName(filename,MIA_MHD);
    bool result = movingImageReader->update();
    progress.setValue(50);


    if(result)
    {
        progress.setValue(100);
        MiaLUTModule* registrationLabelLUT = new MiaLUTModule(LUT_SingleColor,this);
        MiaImageND* referenceImage;
        movingImageReader->getOutputImage(&referenceImage);
        float threshold = 0.0f;

        if(referenceImage)
        {
            float min = 0.0f;
            float max = 1.0f;
            referenceImage->getMaxMin(min,max);
            threshold = min + (max - min)/10;
        }
        registrationLabelLUT->resetKeyPoint(0,threshold,QColor(Qt::transparent));
        registrationLabelLUT->resetKeyPoint(1,threshold+0.5f,QColor(255,0,0, 120));
        axialViewRegisteredMask->setCurrentLUT(registrationLabelLUT);
        saggitalViewRegisteredMask->setCurrentLUT(registrationLabelLUT);
        coronalViewRegisteredMask->setCurrentLUT(registrationLabelLUT);
        registrationFilter->update();
        axialslicer->resetPositionToCenter();
    }
    else
    {
        qDebug() <<QString("Open file failed!");
    }

}

void MiaRegistrationNetwork::loadTransformParametersFromFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    registrationFilter->loadTransformFromFile(in);
    file.close();
//            registrationFilter->invertTransform();
}

void MiaRegistrationNetwork::saveTransformParametersToFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    registrationFilter->saveCurrentTransformToFile(out);
    file.close();
}

void MiaRegistrationNetwork::setBackgroundValue(float value)
{
    registrationFilter->setBackGroundValue(value);
}

void MiaRegistrationNetwork::setMovingImageThreshold(float value)
{
    MiaLUTModule* registrationLabelLUT = new MiaLUTModule(LUT_SingleColor,this);
    registrationLabelLUT->resetKeyPoint(0,value,QColor(Qt::transparent));
    registrationLabelLUT->resetKeyPoint(1,value+0.5f,QColor(255,0,0, 120));

    axialViewRegisteredMask->setCurrentLUT(registrationLabelLUT);
    saggitalViewRegisteredMask->setCurrentLUT(registrationLabelLUT);
    coronalViewRegisteredMask->setCurrentLUT(registrationLabelLUT);
}

void MiaRegistrationNetwork::setMaximumIteration(int ite)
{
     registrationFilter->setMaximumIteration(ite);
}

void MiaRegistrationNetwork::openMovingImage()
{
    qDebug()<<"open file";
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile );
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter(tr("Images (*.mhd)"));
    if (dialog.exec())
    {
        QStringList fileList;
        QString fileName;
        bool result = false;
        fileList = dialog.selectedFiles();
        fileName = fileList.first();
        openMovingImageWithName(fileName);

     }
     else
     {
         qDebug() << "User canceled open file";
     }
}

void MiaRegistrationNetwork::startRegistration()
{

    registrationFilter->startRegistration();
}

void MiaRegistrationNetwork::registrationFinished()
{
    if(!autoSaveTransformParameterPath.isEmpty())
    {
        saveTransformParametersToFile(autoSaveTransformParameterPath);
        emit procesingFinished(true);
    }
}

void MiaRegistrationNetwork::saveTransformParameters()
{
    QString filename = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (filename != "")
    {
        saveTransformParametersToFile(filename);
    }
    else
    {
        qDebug() << "User canceled save file";
    }
}

void MiaRegistrationNetwork::loadTransformParameters()
{
    QString filename = QFileDialog::getOpenFileName((QWidget*)parent(), tr("Open File"), 0, 0);
    if (filename != "")
    {
        loadTransformParametersFromFile(filename);
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}

void MiaRegistrationNetwork::saveRegistrationResults()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(), tr("Save File"), 0, 0);
    if (fileName != "")
    {
        exporter->setFileName(fileName, MIA_MHD);
        exporter->update();
    }
    else
    {
        qDebug() << "User canceled open file";
    }
}

void MiaRegistrationNetwork::resetRegistrationFilter()
{
    QString tranlationstr = userInterface->translationLineEdit->text();
    QString rotationstr = userInterface->rotationLineEdit->text();
    QString scalingstr = userInterface->scalingLineEdit->text();
    QString mappingstr = userInterface->mappingLineEdit->text();
    if(tranlationstr.length() == 0 && rotationstr.length() == 0 && scalingstr.length() == 0)
    {
        registrationFilter->resetRegistration();
    }
    else
    {
        Eigen::Quaternionf rotation;
        Eigen::Vector3f translate;
        float scale = 1.0;
        float A = 1.0f;
        float b = 0.0f;
        if(tranlationstr.length() > 0)
        {
            QStringList para = tranlationstr.split(",");
            if(para.count() == 3)
            {
                for(int i = 0 ; i < 3; i++)
                {
                    translate[i] = para[i].toFloat();
                }
            }
        }
        if(rotationstr.length() > 0)
        {
            QStringList para = rotationstr.split(",");
            if(para.count() == 3)
            {
                rotation = Eigen::AngleAxisf(para[0].toFloat()*E_PI/180.0, Eigen::Vector3f::UnitZ())
                         * Eigen::AngleAxisf(para[1].toFloat()*E_PI/180.0, Eigen::Vector3f::UnitY())
                         * Eigen::AngleAxisf(para[2].toFloat()*E_PI/180.0, Eigen::Vector3f::UnitZ());
            }
        }
        if(scalingstr.length())
        {
            scale = scalingstr.toFloat();
        }
        if(mappingstr.length())
        {
            QStringList para = mappingstr.split(",");
            A = para[0].toFloat();
            b = para[1].toFloat();
        }
        ((MiaImageRegistrationFilterWithTemplate*)registrationFilter)->initializeTemplateParameter(rotation,translate,scale,A,b);
    }
}


