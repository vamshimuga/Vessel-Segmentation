INCLUDEPATH += $$PWD

HEADERS  += ../miamodules_global.h \
    $$PWD/miaconstants.h

mac*{
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_CXXFLAGS_RELEASE += -O3
}

mac*{

    INCLUDEPATH += ../../vtk63release/include \
             /usr/include
    LIBS +=  -lgomp -framework AppKit -framework IOKit -framework OpenGL

    LIBS += -lpthread -mmacosx-version-min=10.7

    LIBS += -L../../vtk63release/lib/

    LIBS += -lvtkGUISupportQt-6.3 -lvtkCommonExecutionModel-6.3 -lvtkCommonDataModel-6.3 -lvtkCommonMath-6.3 \
            -lvtkCommonCore-6.3 -lvtksys-6.3 -lvtkCommonMisc-6.3 -lvtkCommonSystem-6.3 -lvtkCommonTransforms-6.3 \
            -lvtkFiltersExtraction-6.3 -lvtkFiltersCore-6.3 -lvtkFiltersGeneral-6.3 -lvtkCommonComputationalGeometry-6.3 \
            -lvtkFiltersStatistics-6.3 -lvtkImagingFourier-6.3 -lvtkImagingCore-6.3 -lvtkalglib-6.3 -lvtkInteractionStyle-6.3 \
            -lvtkFiltersSources-6.3 -lvtkRenderingCore-6.3 -lvtkFiltersGeometry-6.3 -lvtkRenderingOpenGL-6.3 \
            -lvtkImagingHybrid-6.3 -lvtkIOImage-6.3 -lvtkDICOMParser-6.3 -lvtkIOCore-6.3 -lvtkzlib-6.3 -lvtkmetaio-6.3 \
            -lvtkjpeg-6.3 -lvtkpng-6.3 -lvtktiff-6.3 -lvtkRenderingFreeType-6.3 -lvtkfreetype-6.3 -lvtkftgl-6.3 \
            -lvtkInfovisCore-6.3 -lvtkViewsQt-6.3 -lvtkViewsInfovis-6.3 -lvtkCommonColor-6.3 -lvtkFiltersImaging-6.3 \
            -lvtkImagingGeneral-6.3 -lvtkImagingSources-6.3 -lvtkFiltersModeling-6.3 -lvtkInfovisLayout-6.3 \
            -lvtkRenderingContext2D-6.3 -lvtkRenderingLabel-6.3 -lvtkViewsCore-6.3 -lvtkInteractionWidgets-6.3 \
            -lvtkFiltersHybrid-6.3 -lvtkRenderingAnnotation-6.3 -lvtkImagingColor-6.3 -lvtkRenderingVolume-6.3 \
            -lvtkIOGeometry-6.3 -lvtkIOLegacy-6.3 -lvtkRenderingFreeType-6.3 -lvtkRenderingVolumeOpenGL-6.3

}
