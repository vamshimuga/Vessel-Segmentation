#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T21:55:16
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MiaApp
TEMPLATE = app

include(../SharedProjectSettings.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    listviewwithkeyboardsupport.cpp \
    MiaNetworks/miabasicmprandvrtnetwork.cpp \
    MiaNetworks/miathresholdlevelsetnetwork.cpp \
    MiaNetworks/miaregistrationnetwork.cpp \
    MiaNetworks/miavesselanalysisnetwork.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    listviewwithkeyboardsupport.h \
    MiaNetworks/miabasicmprandvrtnetwork.h \
    MiaNetworks/miathresholdlevelsetnetwork.h \
    MiaNetworks/miaregistrationnetwork.h \
    MiaNetworks/miavesselanalysisnetwork.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/release/ -lMiaCoreModules
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/debug/ -lMiaCoreModules
else:unix: LIBS += -L$$OUT_PWD/../MiaCoreModules/ -lMiaCoreModules

INCLUDEPATH += $$PWD/../MiaCoreModules
INCLUDEPATH += $$PWD/../MiaApp
DEPENDPATH += $$PWD/../MiaCoreModules

RESOURCES += \
    logos.qrc


win32{
    INCLUDEPATH += "C:\raymed\vtk\include\vtk-6.2"
CONFIG(debug, debug|release){
        LIBS += -L"C:\raymed\vtk\lib\vtk-6.2debug" \
            -lvtkalglib-6.2 \
            -lvtkChartsCore-6.2 \
            -lvtkCommonColor-6.2 \
            -lvtkCommonComputationalGeometry-6.2 \
            -lvtkCommonCore-6.2 \
            -lvtkCommonDataModel-6.2 \
            -lvtkCommonExecutionModel-6.2 \
            -lvtkCommonMath-6.2 \
            -lvtkCommonMisc-6.2 \
            -lvtkCommonSystem-6.2 \
            -lvtkCommonTransforms-6.2 \
            -lvtkDICOMParser-6.2 \
            -lvtkDomainsChemistry-6.2 \
            -lvtkexoIIc-6.2 \
            -lvtkexpat-6.2 \
            -lvtkFiltersAMR-6.2 \
            -lvtkFiltersCore-6.2 \
            -lvtkFiltersExtraction-6.2 \
            -lvtkFiltersFlowPaths-6.2 \
            -lvtkFiltersGeneral-6.2 \
            -lvtkFiltersGeneric-6.2 \
            -lvtkFiltersGeometry-6.2 \
            -lvtkFiltersHybrid-6.2 \
            -lvtkFiltersHyperTree-6.2 \
            -lvtkFiltersImaging-6.2 \
            -lvtkFiltersModeling-6.2 \
            -lvtkFiltersParallel-6.2 \
            -lvtkFiltersParallelImaging-6.2 \
            -lvtkFiltersProgrammable-6.2 \
            -lvtkFiltersSelection-6.2 \
            -lvtkFiltersSources-6.2 \
            -lvtkFiltersStatistics-6.2 \
            -lvtkFiltersTexture-6.2 \
            -lvtkFiltersVerdict-6.2 \
            -lvtkfreetype-6.2 \
            -lvtkftgl-6.2 \
            -lvtkGeovisCore-6.2 \
            -lvtkgl2ps-6.2 \
            -lvtkGUISupportQt-6.2 \
            -lvtkGUISupportQtOpenGL-6.2 \
            -lvtkGUISupportQtSQL-6.2 \
            -lvtkGUISupportQtWebkit-6.2 \
            -lvtkhdf5-6.2 \
            -lvtkhdf5_hl-6.2 \
            -lvtkImagingColor-6.2 \
            -lvtkImagingCore-6.2 \
            -lvtkImagingFourier-6.2 \
            -lvtkImagingGeneral-6.2 \
            -lvtkImagingHybrid-6.2 \
            -lvtkImagingMath-6.2 \
            -lvtkImagingMorphological-6.2 \
            -lvtkImagingSources-6.2 \
            -lvtkImagingStatistics-6.2 \
            -lvtkImagingStencil-6.2 \
            -lvtkInfovisCore-6.2 \
            -lvtkInfovisLayout-6.2 \
            -lvtkInteractionImage-6.2 \
            -lvtkInteractionStyle-6.2 \
            -lvtkInteractionWidgets-6.2 \
            -lvtkIOAMR-6.2 \
            -lvtkIOCore-6.2 \
            -lvtkIOEnSight-6.2 \
            -lvtkIOExodus-6.2 \
            -lvtkIOExport-6.2 \
            -lvtkIOGeometry-6.2 \
            -lvtkIOImage-6.2 \
            -lvtkIOImport-6.2 \
            -lvtkIOInfovis-6.2 \
            -lvtkIOLegacy-6.2 \
            -lvtkIOLSDyna-6.2 \
            -lvtkIOMINC-6.2 \
            -lvtkIOMovie-6.2 \
            -lvtkIONetCDF-6.2 \
            -lvtkIOParallel-6.2 \
            -lvtkIOPLY-6.2 \
            -lvtkIOSQL-6.2 \
            -lvtkIOVideo-6.2 \
            -lvtkIOXML-6.2 \
            -lvtkIOXMLParser-6.2 \
            -lvtkjpeg-6.2 \
            -lvtkjsoncpp-6.2 \
            -lvtklibxml2-6.2 \
            -lvtkmetaio-6.2 \
            -lvtkNetCDF-6.2 \
            -lvtkNetCDF_cxx-6.2 \
            -lvtkoggtheora-6.2 \
            -lvtkParallelCore-6.2 \
            -lvtkpng-6.2 \
            -lvtkproj4-6.2 \
            -lvtkRenderingAnnotation-6.2 \
            -lvtkRenderingContext2D-6.2 \
            -lvtkRenderingCore-6.2 \
            -lvtkRenderingFreeType-6.2 \
            -lvtkRenderingFreeTypeOpenGL-6.2 \
            -lvtkRenderingGL2PS-6.2 \
            -lvtkRenderingImage-6.2 \
            -lvtkRenderingLabel-6.2 \
            -lvtkRenderingLIC-6.2 \
            -lvtkRenderingLOD-6.2 \
            -lvtkRenderingOpenGL-6.2 \
            -lvtkRenderingQt-6.2 \
            -lvtkRenderingVolume-6.2 \
            -lvtkRenderingVolumeOpenGL-6.2 \
            -lvtkRenderingContextOpenGL-6.2 \
            -lvtksqlite-6.2 \
            -lvtksys-6.2 \
            -lvtktiff-6.2 \
            -lvtkverdict-6.2 \
            -lvtkViewsContext2D-6.2 \
            -lvtkViewsCore-6.2 \
            -lvtkViewsInfovis-6.2 \
            -lvtkViewsQt-6.2 \
            -lvtkzlib-6.2 \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\AdvAPI32.Lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\OpenGL32.Lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\user32.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\gdi32.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\WbemUuid.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\Shell32.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\d3d9.lib"
}
CONFIG(release, debug|release) {
        QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.02
            LIBS += -L"C:\raymed\vtk\lib\vtk-6.2" \
            -lvtkalglib-6.2 \
            -lvtkChartsCore-6.2 \
            -lvtkCommonColor-6.2 \
            -lvtkCommonComputationalGeometry-6.2 \
            -lvtkCommonCore-6.2 \
            -lvtkCommonDataModel-6.2 \
            -lvtkCommonExecutionModel-6.2 \
            -lvtkCommonMath-6.2 \
            -lvtkCommonMisc-6.2 \
            -lvtkCommonSystem-6.2 \
            -lvtkCommonTransforms-6.2 \
            -lvtkDICOMParser-6.2 \
            -lvtkDomainsChemistry-6.2 \
            -lvtkexoIIc-6.2 \
            -lvtkexpat-6.2 \
            -lvtkFiltersAMR-6.2 \
            -lvtkFiltersCore-6.2 \
            -lvtkFiltersExtraction-6.2 \
            -lvtkFiltersFlowPaths-6.2 \
            -lvtkFiltersGeneral-6.2 \
            -lvtkFiltersGeneric-6.2 \
            -lvtkFiltersGeometry-6.2 \
            -lvtkFiltersHybrid-6.2 \
            -lvtkFiltersHyperTree-6.2 \
            -lvtkFiltersImaging-6.2 \
            -lvtkFiltersModeling-6.2 \
            -lvtkFiltersParallel-6.2 \
            -lvtkFiltersParallelImaging-6.2 \
            -lvtkFiltersProgrammable-6.2 \
            -lvtkFiltersSelection-6.2 \
            -lvtkFiltersSources-6.2 \
            -lvtkFiltersStatistics-6.2 \
            -lvtkFiltersTexture-6.2 \
            -lvtkFiltersVerdict-6.2 \
            -lvtkfreetype-6.2 \
            -lvtkftgl-6.2 \
            -lvtkGeovisCore-6.2 \
            -lvtkgl2ps-6.2 \
            -lvtkGUISupportQt-6.2 \
            -lvtkGUISupportQtOpenGL-6.2 \
            -lvtkGUISupportQtSQL-6.2 \
            -lvtkGUISupportQtWebkit-6.2 \
            -lvtkhdf5-6.2 \
            -lvtkhdf5_hl-6.2 \
            -lvtkImagingColor-6.2 \
            -lvtkImagingCore-6.2 \
            -lvtkImagingFourier-6.2 \
            -lvtkImagingGeneral-6.2 \
            -lvtkImagingHybrid-6.2 \
            -lvtkImagingMath-6.2 \
            -lvtkImagingMorphological-6.2 \
            -lvtkImagingSources-6.2 \
            -lvtkImagingStatistics-6.2 \
            -lvtkImagingStencil-6.2 \
            -lvtkInfovisCore-6.2 \
            -lvtkInfovisLayout-6.2 \
            -lvtkInteractionImage-6.2 \
            -lvtkInteractionStyle-6.2 \
            -lvtkInteractionWidgets-6.2 \
            -lvtkIOAMR-6.2 \
            -lvtkIOCore-6.2 \
            -lvtkIOEnSight-6.2 \
            -lvtkIOExodus-6.2 \
            -lvtkIOExport-6.2 \
            -lvtkIOGeometry-6.2 \
            -lvtkIOImage-6.2 \
            -lvtkIOImport-6.2 \
            -lvtkIOInfovis-6.2 \
            -lvtkIOLegacy-6.2 \
            -lvtkIOLSDyna-6.2 \
            -lvtkIOMINC-6.2 \
            -lvtkIOMovie-6.2 \
            -lvtkIONetCDF-6.2 \
            -lvtkIOParallel-6.2 \
            -lvtkIOPLY-6.2 \
            -lvtkIOSQL-6.2 \
            -lvtkIOVideo-6.2 \
            -lvtkIOXML-6.2 \
            -lvtkIOXMLParser-6.2 \
            -lvtkjpeg-6.2 \
            -lvtkjsoncpp-6.2 \
            -lvtklibxml2-6.2 \
            -lvtkmetaio-6.2 \
            -lvtkNetCDF-6.2 \
            -lvtkNetCDF_cxx-6.2 \
            -lvtkoggtheora-6.2 \
            -lvtkParallelCore-6.2 \
            -lvtkpng-6.2 \
            -lvtkproj4-6.2 \
            -lvtkRenderingAnnotation-6.2 \
            -lvtkRenderingContext2D-6.2 \
            -lvtkRenderingCore-6.2 \
            -lvtkRenderingFreeType-6.2 \
            -lvtkRenderingFreeTypeOpenGL-6.2 \
            -lvtkRenderingGL2PS-6.2 \
            -lvtkRenderingImage-6.2 \
            -lvtkRenderingLabel-6.2 \
            -lvtkRenderingLIC-6.2 \
            -lvtkRenderingLOD-6.2 \
            -lvtkRenderingOpenGL-6.2 \
            -lvtkRenderingQt-6.2 \
            -lvtkRenderingVolume-6.2 \
            -lvtkRenderingVolumeOpenGL-6.2 \
            -lvtkRenderingContextOpenGL-6.2 \
            -lvtksqlite-6.2 \
            -lvtksys-6.2 \
            -lvtktiff-6.2 \
            -lvtkverdict-6.2 \
            -lvtkViewsContext2D-6.2 \
            -lvtkViewsCore-6.2 \
            -lvtkViewsInfovis-6.2 \
            -lvtkViewsQt-6.2 \
            -lvtkzlib-6.2 \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\AdvAPI32.Lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\OpenGL32.Lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\user32.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\gdi32.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\WbemUuid.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\Shell32.lib" \
            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\d3d9.lib"
        }
}



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaMarkers/release/ -lMiaMarkers
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaMarkers/debug/ -lMiaMarkers
else:unix: LIBS += -L$$OUT_PWD/../MiaMarkers/ -lMiaMarkers

INCLUDEPATH += $$PWD/../MiaMarkers
DEPENDPATH += $$PWD/../MiaMarkers

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaViewModules/release/ -lMiaViewModules
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaViewModules/debug/ -lMiaViewModules
else:unix: LIBS += -L$$OUT_PWD/../MiaViewModules/ -lMiaViewModules

INCLUDEPATH += $$PWD/../MiaViewModules
DEPENDPATH += $$PWD/../MiaViewModules

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaCommonModules/release/ -lMiaCommonModules
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaCommonModules/debug/ -lMiaCommonModules
else:unix: LIBS += -L$$OUT_PWD/../MiaCommonModules/ -lMiaCommonModules

INCLUDEPATH += $$PWD/../MiaCommonModules
DEPENDPATH += $$PWD/../MiaCommonModules

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaLevelSetModules/release/ -lMiaLevelSetModules
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaLevelSetModules/debug/ -lMiaLevelSetModules
else:unix: LIBS += -L$$OUT_PWD/../MiaLevelSetModules/ -lMiaLevelSetModules

INCLUDEPATH += $$PWD/../MiaLevelSetModules
DEPENDPATH += $$PWD/../MiaLevelSetModules

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaMachineLearningModules/release/ -lMiaMachineLearningModules
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaMachineLearningModules/debug/ -lMiaMachineLearningModules
#else:unix: LIBS += -L$$OUT_PWD/../MiaMachineLearningModules/ -lMiaMachineLearningModules

#INCLUDEPATH += $$PWD/../MiaMachineLearningModules
#DEPENDPATH += $$PWD/../MiaMachineLearningModules
