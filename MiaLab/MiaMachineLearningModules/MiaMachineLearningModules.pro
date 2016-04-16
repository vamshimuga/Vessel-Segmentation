#-------------------------------------------------
#
# Project created by QtCreator 2016-03-25T23:05:30
#
#-------------------------------------------------

QT       += widgets

TARGET = MiaMachineLearningModules
TEMPLATE = lib

DEFINES += MIAMACHINELEARNINGMODULES_LIBRARY

include(../SharedProjectSettings.pri)

SOURCES += \
    miahistogramanalysisfilter.cpp \
    miarandomforestdetectionmodule.cpp \
    miarandomforesttrainingmodule.cpp \
    graph/miaanatomygraph.cpp \
    graph/miaanatomypartfilter.cpp \
    radomforest/ClassificationTrainingContext.cpp \
    radomforest/DataPointCollection.cpp \
    radomforest/FeatureResponseFunctions.cpp \
    radomforest/imagepatchclassificationtrainer.cpp \
    radomforest/imagepatchdetector.cpp \
    radomforest/imagepatchgenerator.cpp \
    radomforest/StatisticsAggregators.cpp

HEADERS +=\
    miahistogramanalysisfilter.h \
    miarandomforestdetectionmodule.h \
    miarandomforesttrainingmodule.h \
    graph/miaanatomygraph.h \
    graph/miaanatomypartfilter.h \
    radomforest/ClassificationTrainingContext.h \
    radomforest/DataPointCollection.h \
    radomforest/FeatureResponseFunctions.h \
    radomforest/Forest.h \
    radomforest/ForestTrainer.h \
    radomforest/imagepatchclassificationtrainer.h \
    radomforest/imagepatchdetector.h \
    radomforest/imagepatchgenerator.h \
    radomforest/Interfaces.h \
    radomforest/Node.h \
    radomforest/ParallelForestTrainer.h \
    radomforest/ProgressStream.h \
    radomforest/Random.h \
    radomforest/Sherwood.h \
    radomforest/StatisticsAggregators.h \
    radomforest/TrainingParameters.h \
    radomforest/Tree.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/release/ -lMiaCoreModules
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/debug/ -lMiaCoreModules
else:unix: LIBS += -L$$OUT_PWD/../MiaCoreModules/ -lMiaCoreModules

INCLUDEPATH += $$PWD/../MiaCoreModules
DEPENDPATH += $$PWD/../MiaCoreModules
