#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T15:45:23
#
#-------------------------------------------------

QT       += widgets network xml

TARGET = MiaCoreModules
TEMPLATE = lib

DEFINES += MIACOREMODULES_LIBRARY

include(../SharedProjectSettings.pri)

SOURCES += \
    miaimagend.cpp \
    miamodule.cpp \
    transform/miacurvedmprtransformalgorithm.cpp \
    transform/miairregulartransformAlgorithms.cpp \
    transform/miapoint4d.cpp \
    transform/MiaTransform.cpp \
    miamarker.cpp \
    mianetwork.cpp \
    networkenviromentvariable.cpp \
    miamodule2in1out.cpp

HEADERS +=\
    miaimagend.h \
    miamodule.h \
    miaretainableobject.h \
    transform/MiaAffineTransform.h \
    transform/miacurvedmprtransformalgorithm.h \
    transform/miairregulartransformAlgorithms.h \
    transform/miapoint4d.h \
    transform/MiaTransform.h \
    miamarker.h \
    mianetwork.h \
    networkenviromentvariable.h \
    miamodule2in1out.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
