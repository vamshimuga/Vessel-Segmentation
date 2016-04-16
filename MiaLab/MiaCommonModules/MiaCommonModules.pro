#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T00:04:09
#
#-------------------------------------------------

QT       += widgets

TARGET = MiaCommonModules
TEMPLATE = lib

DEFINES += MIACOMMONMODULES_LIBRARY

include(../SharedProjectSettings.pri)

SOURCES += \
    miacentertrackingonarrivalmap.cpp \
    miaconnectedcomponentsfilter.cpp \
    miaconvolutionfilter.cpp \
    miacurvaturefilter.cpp \
    miacurvedmprslicer.cpp \
    miadistancetransformfromcenterline.cpp \
    miafastmarchingfilter.cpp \
    miafocusvaluereadout.cpp \
    miagradient3dfilter.cpp \
    miaimageforestingtransform.cpp \
    miaimagepropertyspreadingfilter.cpp \
    miaintensitymappingfilter.cpp \
    mialevelsetfunctionmanualmodifier.cpp \
    miamanualliversegmentgenerator.cpp \
    miamarkerstatistics.cpp \
    miameanfilter.cpp \
    miamodelguidedconvolutionfilter.cpp \
    miamorphologyfilter.cpp \
    miamultiinputintensitymappingfilter.cpp \
    miaobliquemprslicer.cpp \
    miaorthogonalslicer.cpp \
    miaorthopermutefilter.cpp \
    miaparameterconnector.cpp \
    miapersistantlabel.cpp \
    miaregionstatistics.cpp \
    miaresample3d.cpp \
    miasubimagefilter.cpp \
    miathresholding.cpp \
    miazerocrossingfilter.cpp

HEADERS +=\
        miacommonmodules_global.h \
    miacentertrackingonarrivalmap.h \
    miaconnectedcomponentsfilter.h \
    miaconvolutionfilter.h \
    miacurvaturefilter.h \
    miacurvedmprslicer.h \
    miadistancetransformfromcenterline.h \
    miafastmarchingfilter.h \
    miafocusvaluereadout.h \
    miagradient3dfilter.h \
    miaimageforestingtransform.h \
    miaimagepropertyspreadingfilter.h \
    miaintensitymappingfilter.h \
    mialevelsetfunctionmanualmodifier.h \
    miamanualliversegmentgenerator.h \
    miamarkerstatistics.h \
    miameanfilter.h \
    miamodelguidedconvolutionfilter.h \
    miamorphologyfilter.h \
    miamultiinputintensitymappingfilter.h \
    miaobliquemprslicer.h \
    miaorthogonalslicer.h \
    miaorthopermutefilter.h \
    miaparameterconnector.h \
    miapersistantlabel.h \
    miaregionstatistics.h \
    miaresample3d.h \
    miasubimagefilter.h \
    miathresholding.h \
    miazerocrossingfilter.h \
    regression/fitmed.h \
    regression/nr3.h \
    regression/sort.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/release/ -lMiaCoreModules
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/debug/ -lMiaCoreModules
else:unix: LIBS += -L$$OUT_PWD/../MiaCoreModules/ -lMiaCoreModules

INCLUDEPATH += $$PWD/../MiaCoreModules
DEPENDPATH += $$PWD/../MiaCoreModules

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaMarkers/release/ -lMiaMarkers
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaMarkers/debug/ -lMiaMarkers
else:unix: LIBS += -L$$OUT_PWD/../MiaMarkers/ -lMiaMarkers

INCLUDEPATH += $$PWD/../MiaMarkers
DEPENDPATH += $$PWD/../MiaMarkers
