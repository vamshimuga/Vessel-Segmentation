#-------------------------------------------------
#
# Project created by QtCreator 2016-03-25T21:40:03
#
#-------------------------------------------------

QT       += widgets

TARGET = MiaLevelSetModules
TEMPLATE = lib

DEFINES += MIALEVELSETMODULES_LIBRARY

include(../SharedProjectSettings.pri)

SOURCES += \
    miadistancetransformfrom3dmarkers.cpp \
    miaimageregistrationfilter.cpp \
    miaimageregistrationfilterwithstatisticmodel.cpp \
    miaimageregistrationfilterwithtemplate.cpp \
    miasfgradientlevelsetsegmenter.cpp \
    miasfinteractivesegmenter.cpp \
    miasfthresholdlevelsetsegmenter.cpp \
    miasparsefieldlevelsetsegmenter.cpp \
    LevelsetKernels/abstractlevelsetkernel.cpp \
    LevelsetKernels/abstractlevelsetspeedfucntion.cpp \
    LevelsetKernels/denselevelset.cpp \
    LevelsetKernels/levelsetfunction_gradientbased.cpp \
    LevelsetKernels/levelsetfunction_thresholdbased.cpp \
    LevelsetKernels/sparsefieldchanveselevelset.cpp \
    LevelsetKernels/sparsefieldlevelsetkernel.cpp \
    LevelsetKernels/sparsefieldlevelsetwithprinciplecurvature.cpp \
    LevelsetKernels/sparsefieldvessellevelsetkernel.cpp \
    registrationkernel/costfunctionbase.cpp \
    registrationkernel/costfunctionsquareddifference.cpp \
    registrationkernel/costfunctionsquareddifferencewithstatisticmodel.cpp \
    registrationkernel/costfunctionsquareddifferencewithtemplatefitting.cpp \
    registrationkernel/gaussnewtonoptimizer.cpp \
    registrationkernel/gaussnewtontemplateoptimizer.cpp \
    registrationkernel/intensitymappingfuctionbase.cpp \
    registrationkernel/intensitymappingfunctionlinear.cpp \
    registrationkernel/transformfunctionaffine.cpp \
    registrationkernel/transformfunctionbase.cpp \
    registrationkernel/transformfunctionconstrainedrigid3d.cpp \
    registrationkernel/transformfunctionfixedcenterrigid3d.cpp \
    registrationkernel/transformfunctionrigid3d.cpp \
    registrationkernel/transformfunctiontraslation.cpp

HEADERS +=\
    miadistancetransformfrom3dmarkers.h \
    miaimageregistrationfilter.h \
    miaimageregistrationfilterwithstatisticmodel.h \
    miaimageregistrationfilterwithtemplate.h \
    miasfgradientlevelsetsegmenter.h \
    miasfinteractivesegmenter.h \
    miasfthresholdlevelsetsegmenter.h \
    miasparsefieldlevelsetsegmenter.h \
    LevelsetKernels/abstractlevelsetkernel.h \
    LevelsetKernels/abstractlevelsetspeedfucntion.h \
    LevelsetKernels/denselevelset.h \
    LevelsetKernels/levelsetfunction_gradientbased.h \
    LevelsetKernels/levelsetfunction_thresholdbased.h \
    LevelsetKernels/LevelSetRoutines.h \
    LevelsetKernels/sparsefieldchanveselevelset.h \
    LevelsetKernels/sparsefieldlevelsetkernel.h \
    LevelsetKernels/sparsefieldlevelsetwithprinciplecurvature.h \
    LevelsetKernels/sparsefieldvessellevelsetkernel.h \
    registrationkernel/costfunctionbase.h \
    registrationkernel/costfunctionsquareddifference.h \
    registrationkernel/costfunctionsquareddifferencewithstatisticmodel.h \
    registrationkernel/costfunctionsquareddifferencewithtemplatefitting.h \
    registrationkernel/gaussnewtonoptimizer.h \
    registrationkernel/gaussnewtontemplateoptimizer.h \
    registrationkernel/intensitymappingfuctionbase.h \
    registrationkernel/intensitymappingfunctionlinear.h \
    registrationkernel/transformfunctionaffine.h \
    registrationkernel/transformfunctionbase.h \
    registrationkernel/transformfunctionconstrainedrigid3d.h \
    registrationkernel/transformfunctionfixedcenterrigid3d.h \
    registrationkernel/transformfunctionrigid3d.h \
    registrationkernel/transformfunctiontraslation.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/release/ -lMiaCoreModules
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MiaCoreModules/debug/ -lMiaCoreModules
else:unix: LIBS += -L$$OUT_PWD/../MiaCoreModules/ -lMiaCoreModules

INCLUDEPATH += $$PWD/../MiaCoreModules
DEPENDPATH += $$PWD/../MiaCoreModules
