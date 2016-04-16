# Generated by CMake 3.3.2

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget vtksys vtkCommonCore vtkCommonMath vtkCommonMisc vtkCommonSystem vtkCommonTransforms vtkCommonDataModel vtkCommonColor vtkCommonExecutionModel vtkFiltersCore vtkCommonComputationalGeometry vtkFiltersGeneral vtkImagingCore vtkImagingFourier vtkalglib vtkFiltersStatistics vtkFiltersExtraction vtkInfovisCore vtkFiltersGeometry vtkFiltersSources vtkRenderingCore vtkzlib vtkfreetype vtkftgl vtkRenderingFreeType vtkRenderingContext2D vtkChartsCore vtkDICOMParser vtkIOCore vtkIOGeometry vtkexpat vtkIOXMLParser vtkIOXML vtkDomainsChemistry vtkIOLegacy vtkHashSource vtkParallelCore vtkFiltersAMR vtkFiltersFlowPaths vtkFiltersGeneric vtkImagingSources vtkFiltersHybrid vtkFiltersHyperTree vtkImagingGeneral vtkFiltersImaging vtkFiltersModeling vtkFiltersParallel vtkFiltersParallelImaging vtkFiltersProgrammable vtkFiltersSMP vtkFiltersSelection vtkFiltersTexture verdict vtkFiltersVerdict vtkInteractionStyle vtkmetaio vtkjpeg vtkpng vtkmkg3states vtktiff vtkIOImage vtkImagingHybrid vtkParseOGLExt vtkEncodeString vtkRenderingOpenGL vtkGUISupportQt vtkGUISupportQtOpenGL vtksqlite vtkIOSQL vtkGUISupportQtSQL vtkInfovisLayout vtkRenderingLabel vtkImagingColor vtkRenderingAnnotation vtkRenderingVolume vtkInteractionWidgets vtkViewsCore vtkViewsInfovis vtkViewsQt vtkGUISupportQtWebkit vtkproj4 vtkGeovisCore vtkhdf5 vtkhdf5_hl vtkIOAMR vtkIOEnSight vtkNetCDF vtkNetCDF_cxx vtkexoIIc vtkIOExodus vtkRenderingContextOpenGL vtkgl2ps vtkRenderingGL2PS vtkIOExport vtkIOImport vtklibxml2 vtkIOInfovis vtkIOLSDyna vtkIOMINC vtkoggtheora vtkIOMovie vtkIONetCDF vtkIOPLY vtkjsoncpp vtkIOParallel vtkIOParallelXML vtkIOVideo vtkImagingMath vtkImagingMorphological vtkImagingStatistics vtkImagingStencil vtkInteractionImage vtkRenderingImage vtkRenderingLIC vtkRenderingLOD vtkRenderingQt vtkRenderingVolumeOpenGL vtkViewsContext2D)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)

# Create imported target vtksys
add_library(vtksys STATIC IMPORTED)

# Create imported target vtkCommonCore
add_library(vtkCommonCore STATIC IMPORTED)

set_target_properties(vtkCommonCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "$<LINK_ONLY:vtksys>"
)

# Create imported target vtkCommonMath
add_library(vtkCommonMath STATIC IMPORTED)

set_target_properties(vtkCommonMath PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonCore"
)

# Create imported target vtkCommonMisc
add_library(vtkCommonMisc STATIC IMPORTED)

set_target_properties(vtkCommonMisc PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonMath"
)

# Create imported target vtkCommonSystem
add_library(vtkCommonSystem STATIC IMPORTED)

set_target_properties(vtkCommonSystem PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonCore;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkCommonTransforms
add_library(vtkCommonTransforms STATIC IMPORTED)

set_target_properties(vtkCommonTransforms PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonCore;vtkCommonMath"
)

# Create imported target vtkCommonDataModel
add_library(vtkCommonDataModel STATIC IMPORTED)

set_target_properties(vtkCommonDataModel PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonMath;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkCommonColor
add_library(vtkCommonColor STATIC IMPORTED)

set_target_properties(vtkCommonColor PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel"
)

# Create imported target vtkCommonExecutionModel
add_library(vtkCommonExecutionModel STATIC IMPORTED)

set_target_properties(vtkCommonExecutionModel PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel"
)

# Create imported target vtkFiltersCore
add_library(vtkFiltersCore STATIC IMPORTED)

set_target_properties(vtkFiltersCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkCommonMath;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms"
)

# Create imported target vtkCommonComputationalGeometry
add_library(vtkCommonComputationalGeometry STATIC IMPORTED)

set_target_properties(vtkCommonComputationalGeometry PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonMath;vtkCommonSystem"
)

# Create imported target vtkFiltersGeneral
add_library(vtkFiltersGeneral STATIC IMPORTED)

set_target_properties(vtkFiltersGeneral PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonComputationalGeometry;vtkFiltersCore"
)

# Create imported target vtkImagingCore
add_library(vtkImagingCore STATIC IMPORTED)

set_target_properties(vtkImagingCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonExecutionModel;vtkCommonMath;vtkCommonSystem;vtkCommonTransforms"
)

# Create imported target vtkImagingFourier
add_library(vtkImagingFourier STATIC IMPORTED)

set_target_properties(vtkImagingFourier PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingCore;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkalglib
add_library(vtkalglib STATIC IMPORTED)

# Create imported target vtkFiltersStatistics
add_library(vtkFiltersStatistics STATIC IMPORTED)

set_target_properties(vtkFiltersStatistics PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkCommonMath;vtkCommonMisc;vtkCommonTransforms;vtkImagingFourier;vtkalglib"
)

# Create imported target vtkFiltersExtraction
add_library(vtkFiltersExtraction STATIC IMPORTED)

set_target_properties(vtkFiltersExtraction PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonExecutionModel;vtkFiltersCore;vtkFiltersGeneral;vtkFiltersStatistics"
)

# Create imported target vtkInfovisCore
add_library(vtkInfovisCore STATIC IMPORTED)

set_target_properties(vtkInfovisCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonSystem;vtkFiltersExtraction;vtkFiltersGeneral"
)

# Create imported target vtkFiltersGeometry
add_library(vtkFiltersGeometry STATIC IMPORTED)

set_target_properties(vtkFiltersGeometry PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersCore"
)

# Create imported target vtkFiltersSources
add_library(vtkFiltersSources STATIC IMPORTED)

set_target_properties(vtkFiltersSources PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonComputationalGeometry;vtkFiltersGeneral"
)

# Create imported target vtkRenderingCore
add_library(vtkRenderingCore STATIC IMPORTED)

set_target_properties(vtkRenderingCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonColor;vtkCommonExecutionModel;vtkCommonTransforms;$<LINK_ONLY:vtkFiltersSources>;$<LINK_ONLY:vtkFiltersGeometry>;$<LINK_ONLY:vtkFiltersExtraction>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkzlib
add_library(vtkzlib STATIC IMPORTED)

# Create imported target vtkfreetype
add_library(vtkfreetype STATIC IMPORTED)

set_target_properties(vtkfreetype PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkzlib;-framework ApplicationServices -framework CoreServices"
)

# Create imported target vtkftgl
add_library(vtkftgl STATIC IMPORTED)

set_target_properties(vtkftgl PROPERTIES
  INTERFACE_LINK_LIBRARIES "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/OpenGL.framework;vtkfreetype"
)

# Create imported target vtkRenderingFreeType
add_library(vtkRenderingFreeType STATIC IMPORTED)

set_target_properties(vtkRenderingFreeType PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingCore;vtkRenderingCore;vtkfreetype;vtkftgl"
)

# Create imported target vtkRenderingContext2D
add_library(vtkRenderingContext2D STATIC IMPORTED)

set_target_properties(vtkRenderingContext2D PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingCore;$<LINK_ONLY:vtkCommonDataModel>;$<LINK_ONLY:vtkCommonMath>;$<LINK_ONLY:vtkCommonTransforms>;$<LINK_ONLY:vtkRenderingFreeType>"
)

# Create imported target vtkChartsCore
add_library(vtkChartsCore STATIC IMPORTED)

set_target_properties(vtkChartsCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingContext2D;$<LINK_ONLY:vtkCommonColor>;$<LINK_ONLY:vtkInfovisCore>"
)

# Create imported target vtkDICOMParser
add_library(vtkDICOMParser STATIC IMPORTED)

# Create imported target vtkIOCore
add_library(vtkIOCore STATIC IMPORTED)

set_target_properties(vtkIOCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonExecutionModel;vtkCommonMisc;$<LINK_ONLY:vtkzlib>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkIOGeometry
add_library(vtkIOGeometry STATIC IMPORTED)

set_target_properties(vtkIOGeometry PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonMisc;vtkCommonSystem;vtkIOCore;$<LINK_ONLY:vtkzlib>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkexpat
add_library(vtkexpat STATIC IMPORTED)

# Create imported target vtkIOXMLParser
add_library(vtkIOXMLParser STATIC IMPORTED)

set_target_properties(vtkIOXMLParser PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonMisc;vtkCommonSystem;vtkIOCore;$<LINK_ONLY:vtkexpat>"
)

# Create imported target vtkIOXML
add_library(vtkIOXML STATIC IMPORTED)

set_target_properties(vtkIOXML PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkIOGeometry;vtkIOXMLParser;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkDomainsChemistry
add_library(vtkDomainsChemistry STATIC IMPORTED)

set_target_properties(vtkDomainsChemistry PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkRenderingCore;$<LINK_ONLY:vtkIOXML>;$<LINK_ONLY:vtkFiltersSources>"
)

# Create imported target vtkIOLegacy
add_library(vtkIOLegacy STATIC IMPORTED)

set_target_properties(vtkIOLegacy PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonMisc;vtkCommonSystem;vtkIOCore;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkHashSource
add_executable(vtkHashSource IMPORTED)

# Create imported target vtkParallelCore
add_library(vtkParallelCore STATIC IMPORTED)

set_target_properties(vtkParallelCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonCore;vtkIOLegacy;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkFiltersAMR
add_library(vtkFiltersAMR STATIC IMPORTED)

set_target_properties(vtkFiltersAMR PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersGeneral;vtkParallelCore"
)

# Create imported target vtkFiltersFlowPaths
add_library(vtkFiltersFlowPaths STATIC IMPORTED)

set_target_properties(vtkFiltersFlowPaths PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkFiltersGeneral;vtkFiltersSources;vtkIOCore"
)

# Create imported target vtkFiltersGeneric
add_library(vtkFiltersGeneric STATIC IMPORTED)

set_target_properties(vtkFiltersGeneric PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersCore;vtkFiltersSources"
)

# Create imported target vtkImagingSources
add_library(vtkImagingSources STATIC IMPORTED)

set_target_properties(vtkImagingSources PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingCore"
)

# Create imported target vtkFiltersHybrid
add_library(vtkFiltersHybrid STATIC IMPORTED)

set_target_properties(vtkFiltersHybrid PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersGeneral;vtkImagingSources;vtkRenderingCore"
)

# Create imported target vtkFiltersHyperTree
add_library(vtkFiltersHyperTree STATIC IMPORTED)

set_target_properties(vtkFiltersHyperTree PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersGeneral"
)

# Create imported target vtkImagingGeneral
add_library(vtkImagingGeneral STATIC IMPORTED)

set_target_properties(vtkImagingGeneral PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingSources"
)

# Create imported target vtkFiltersImaging
add_library(vtkFiltersImaging STATIC IMPORTED)

set_target_properties(vtkFiltersImaging PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersStatistics;vtkImagingGeneral;vtkImagingSources"
)

# Create imported target vtkFiltersModeling
add_library(vtkFiltersModeling STATIC IMPORTED)

set_target_properties(vtkFiltersModeling PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersGeneral;vtkFiltersSources"
)

# Create imported target vtkFiltersParallel
add_library(vtkFiltersParallel STATIC IMPORTED)

set_target_properties(vtkFiltersParallel PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersExtraction;vtkFiltersGeometry;vtkFiltersModeling;vtkParallelCore;vtkRenderingCore"
)

# Create imported target vtkFiltersParallelImaging
add_library(vtkFiltersParallelImaging STATIC IMPORTED)

set_target_properties(vtkFiltersParallelImaging PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersImaging;vtkFiltersParallel;vtkIOLegacy;vtkImagingCore;vtkParallelCore"
)

# Create imported target vtkFiltersProgrammable
add_library(vtkFiltersProgrammable STATIC IMPORTED)

set_target_properties(vtkFiltersProgrammable PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel"
)

# Create imported target vtkFiltersSMP
add_library(vtkFiltersSMP STATIC IMPORTED)

set_target_properties(vtkFiltersSMP PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersCore;vtkFiltersGeneral"
)

# Create imported target vtkFiltersSelection
add_library(vtkFiltersSelection STATIC IMPORTED)

set_target_properties(vtkFiltersSelection PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonComputationalGeometry;vtkFiltersCore"
)

# Create imported target vtkFiltersTexture
add_library(vtkFiltersTexture STATIC IMPORTED)

set_target_properties(vtkFiltersTexture PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersGeneral"
)

# Create imported target verdict
add_library(verdict STATIC IMPORTED)

# Create imported target vtkFiltersVerdict
add_library(vtkFiltersVerdict STATIC IMPORTED)

set_target_properties(vtkFiltersVerdict PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;verdict"
)

# Create imported target vtkInteractionStyle
add_library(vtkInteractionStyle STATIC IMPORTED)

set_target_properties(vtkInteractionStyle PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingCore;$<LINK_ONLY:vtkFiltersSources>;$<LINK_ONLY:vtkFiltersExtraction>"
)

# Create imported target vtkmetaio
add_library(vtkmetaio STATIC IMPORTED)

set_target_properties(vtkmetaio PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkzlib"
)

# Create imported target vtkjpeg
add_library(vtkjpeg STATIC IMPORTED)

# Create imported target vtkpng
add_library(vtkpng STATIC IMPORTED)

set_target_properties(vtkpng PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkzlib;-lm"
)

# Create imported target vtkmkg3states
add_executable(vtkmkg3states IMPORTED)

# Create imported target vtktiff
add_library(vtktiff STATIC IMPORTED)

set_target_properties(vtktiff PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkzlib;vtkjpeg;-lm"
)

# Create imported target vtkIOImage
add_library(vtkIOImage STATIC IMPORTED)

set_target_properties(vtkIOImage PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonExecutionModel;vtkCommonMath;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;vtkIOCore;$<LINK_ONLY:vtkjpeg>;$<LINK_ONLY:vtkpng>;$<LINK_ONLY:vtktiff>;$<LINK_ONLY:vtkmetaio>;$<LINK_ONLY:vtkDICOMParser>;$<LINK_ONLY:vtkzlib>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkImagingHybrid
add_library(vtkImagingHybrid STATIC IMPORTED)

set_target_properties(vtkImagingHybrid PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkIOImage;vtkImagingCore"
)

# Create imported target vtkParseOGLExt
add_executable(vtkParseOGLExt IMPORTED)

# Create imported target vtkEncodeString
add_executable(vtkEncodeString IMPORTED)

# Create imported target vtkRenderingOpenGL
add_library(vtkRenderingOpenGL STATIC IMPORTED)

set_target_properties(vtkRenderingOpenGL PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingCore;$<LINK_ONLY:vtkImagingHybrid>;$<LINK_ONLY:vtksys>;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/AGL.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/OpenGL.framework;-framework Cocoa"
)

# Create imported target vtkGUISupportQt
add_library(vtkGUISupportQt STATIC IMPORTED)

set_target_properties(vtkGUISupportQt PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkImagingCore;vtkInteractionStyle;vtkRenderingOpenGL;$<LINK_ONLY:vtkFiltersExtraction>;Qt5::Widgets"
)

# Create imported target vtkGUISupportQtOpenGL
add_library(vtkGUISupportQtOpenGL STATIC IMPORTED)

set_target_properties(vtkGUISupportQtOpenGL PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkGUISupportQt;vtkRenderingOpenGL;$<LINK_ONLY:Qt5::OpenGL>"
)

# Create imported target vtksqlite
add_library(vtksqlite STATIC IMPORTED)

# Create imported target vtkIOSQL
add_library(vtkIOSQL STATIC IMPORTED)

set_target_properties(vtkIOSQL PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkIOCore;$<LINK_ONLY:vtksys>;$<LINK_ONLY:vtksqlite>"
)

# Create imported target vtkGUISupportQtSQL
add_library(vtkGUISupportQtSQL STATIC IMPORTED)

set_target_properties(vtkGUISupportQtSQL PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkIOSQL;$<LINK_ONLY:vtksys>;$<LINK_ONLY:Qt5::Widgets>;$<LINK_ONLY:Qt5::Sql>"
)

# Create imported target vtkInfovisLayout
add_library(vtkInfovisLayout STATIC IMPORTED)

set_target_properties(vtkInfovisLayout PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkFiltersModeling;vtkImagingHybrid;vtkInfovisCore"
)

# Create imported target vtkRenderingLabel
add_library(vtkRenderingLabel STATIC IMPORTED)

set_target_properties(vtkRenderingLabel PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingFreeType;$<LINK_ONLY:vtkFiltersExtraction>"
)

# Create imported target vtkImagingColor
add_library(vtkImagingColor STATIC IMPORTED)

set_target_properties(vtkImagingColor PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingCore"
)

# Create imported target vtkRenderingAnnotation
add_library(vtkRenderingAnnotation STATIC IMPORTED)

set_target_properties(vtkRenderingAnnotation PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingColor;vtkRenderingFreeType;$<LINK_ONLY:vtkFiltersSources>"
)

# Create imported target vtkRenderingVolume
add_library(vtkRenderingVolume STATIC IMPORTED)

set_target_properties(vtkRenderingVolume PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingCore;vtkRenderingCore"
)

# Create imported target vtkInteractionWidgets
add_library(vtkInteractionWidgets STATIC IMPORTED)

set_target_properties(vtkInteractionWidgets PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersHybrid;vtkFiltersModeling;vtkImagingGeneral;vtkImagingHybrid;vtkInteractionStyle;vtkRenderingAnnotation;vtkRenderingFreeType;vtkRenderingVolume"
)

# Create imported target vtkViewsCore
add_library(vtkViewsCore STATIC IMPORTED)

set_target_properties(vtkViewsCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkInteractionWidgets;vtkRenderingCore"
)

# Create imported target vtkViewsInfovis
add_library(vtkViewsInfovis STATIC IMPORTED)

set_target_properties(vtkViewsInfovis PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkChartsCore;vtkCommonColor;vtkFiltersImaging;vtkFiltersModeling;vtkInfovisLayout;vtkInteractionStyle;vtkRenderingContext2D;vtkRenderingLabel;vtkViewsCore;$<LINK_ONLY:vtkFiltersGeometry>"
)

# Create imported target vtkViewsQt
add_library(vtkViewsQt STATIC IMPORTED)

set_target_properties(vtkViewsQt PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkGUISupportQt;vtkViewsInfovis;$<LINK_ONLY:Qt5::Widgets>"
)

# Create imported target vtkGUISupportQtWebkit
add_library(vtkGUISupportQtWebkit STATIC IMPORTED)

set_target_properties(vtkGUISupportQtWebkit PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkViewsQt;$<LINK_ONLY:Qt5::WebKitWidgets>"
)

# Create imported target vtkproj4
add_library(vtkproj4 STATIC IMPORTED)

set_target_properties(vtkproj4 PROPERTIES
  INTERFACE_LINK_LIBRARIES "m"
)

# Create imported target vtkGeovisCore
add_library(vtkGeovisCore STATIC IMPORTED)

set_target_properties(vtkGeovisCore PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkIOXML;vtkInfovisLayout;vtkInteractionStyle;vtkInteractionWidgets;vtkRenderingCore;vtkViewsCore;vtkproj4"
)

# Create imported target vtkhdf5
add_library(vtkhdf5 STATIC IMPORTED)

set_target_properties(vtkhdf5 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
  INTERFACE_LINK_LIBRARIES "m;dl;vtkzlib"
)

# Create imported target vtkhdf5_hl
add_library(vtkhdf5_hl STATIC IMPORTED)

set_target_properties(vtkhdf5_hl PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
  INTERFACE_LINK_LIBRARIES "vtkhdf5"
)

# Create imported target vtkIOAMR
add_library(vtkIOAMR STATIC IMPORTED)

set_target_properties(vtkIOAMR PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersAMR;vtkParallelCore;$<LINK_ONLY:vtkhdf5_hl>;$<LINK_ONLY:vtkhdf5>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkIOEnSight
add_library(vtkIOEnSight STATIC IMPORTED)

set_target_properties(vtkIOEnSight PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel"
)

# Create imported target vtkNetCDF
add_library(vtkNetCDF STATIC IMPORTED)

set_target_properties(vtkNetCDF PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkhdf5_hl;vtkhdf5;m"
)

# Create imported target vtkNetCDF_cxx
add_library(vtkNetCDF_cxx STATIC IMPORTED)

set_target_properties(vtkNetCDF_cxx PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkNetCDF"
)

# Create imported target vtkexoIIc
add_library(vtkexoIIc STATIC IMPORTED)

set_target_properties(vtkexoIIc PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkNetCDF;vtkNetCDF_cxx"
)

# Create imported target vtkIOExodus
add_library(vtkIOExodus STATIC IMPORTED)

set_target_properties(vtkIOExodus PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersGeneral;vtkIOXML;$<LINK_ONLY:vtkexoIIc>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkRenderingContextOpenGL
add_library(vtkRenderingContextOpenGL STATIC IMPORTED)

set_target_properties(vtkRenderingContextOpenGL PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingContext2D;vtkRenderingOpenGL;$<LINK_ONLY:vtkRenderingFreeType>"
)

# Create imported target vtkgl2ps
add_library(vtkgl2ps STATIC IMPORTED)

set_target_properties(vtkgl2ps PROPERTIES
  INTERFACE_LINK_LIBRARIES "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/AGL.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/OpenGL.framework;m;vtkzlib;vtkpng;m"
)

# Create imported target vtkRenderingGL2PS
add_library(vtkRenderingGL2PS STATIC IMPORTED)

set_target_properties(vtkRenderingGL2PS PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingContextOpenGL;$<LINK_ONLY:vtkRenderingOpenGL>;$<LINK_ONLY:vtkRenderingFreeType>;$<LINK_ONLY:vtkgl2ps>"
)

# Create imported target vtkIOExport
add_library(vtkIOExport STATIC IMPORTED)

set_target_properties(vtkIOExport PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonCore;vtkImagingCore;vtkRenderingAnnotation;vtkRenderingContext2D;vtkRenderingCore;vtkRenderingFreeType;vtkRenderingGL2PS;vtkRenderingLabel;vtkRenderingOpenGL;$<LINK_ONLY:vtkIOImage>;$<LINK_ONLY:vtkFiltersGeometry>;$<LINK_ONLY:vtkgl2ps>"
)

# Create imported target vtkIOImport
add_library(vtkIOImport STATIC IMPORTED)

set_target_properties(vtkIOImport PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonCore;vtkRenderingCore;vtksys;$<LINK_ONLY:vtkFiltersSources>;$<LINK_ONLY:vtkIOImage>"
)

# Create imported target vtklibxml2
add_library(vtklibxml2 STATIC IMPORTED)

set_target_properties(vtklibxml2 PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkzlib;dl;m"
)

# Create imported target vtkIOInfovis
add_library(vtkIOInfovis STATIC IMPORTED)

set_target_properties(vtkIOInfovis PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonMisc;vtkCommonSystem;vtkIOCore;vtkIOLegacy;vtkIOXML;vtkInfovisCore;$<LINK_ONLY:vtklibxml2>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkIOLSDyna
add_library(vtkIOLSDyna STATIC IMPORTED)

set_target_properties(vtkIOLSDyna PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkIOXML;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkIOMINC
add_library(vtkIOMINC STATIC IMPORTED)

set_target_properties(vtkIOMINC PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkFiltersHybrid;vtkIOGeometry;vtkIOImage;vtkRenderingCore;$<LINK_ONLY:vtksys>;$<LINK_ONLY:vtkNetCDF>;$<LINK_ONLY:vtkNetCDF_cxx>"
)

# Create imported target vtkoggtheora
add_library(vtkoggtheora STATIC IMPORTED)

# Create imported target vtkIOMovie
add_library(vtkIOMovie STATIC IMPORTED)

set_target_properties(vtkIOMovie PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonExecutionModel;vtkCommonSystem;vtkIOCore;vtkoggtheora"
)

# Create imported target vtkIONetCDF
add_library(vtkIONetCDF STATIC IMPORTED)

set_target_properties(vtkIONetCDF PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonSystem;vtkIOCore;$<LINK_ONLY:vtksys>;$<LINK_ONLY:vtkNetCDF>;$<LINK_ONLY:vtkNetCDF_cxx>"
)

# Create imported target vtkIOPLY
add_library(vtkIOPLY STATIC IMPORTED)

set_target_properties(vtkIOPLY PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;vtkCommonMisc;vtkIOGeometry"
)

# Create imported target vtkjsoncpp
add_library(vtkjsoncpp STATIC IMPORTED)

# Create imported target vtkIOParallel
add_library(vtkIOParallel STATIC IMPORTED)

set_target_properties(vtkIOParallel PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersParallel;vtkIOImage;vtkIONetCDF;vtkIOXML;vtkParallelCore;$<LINK_ONLY:vtkexoIIc>;$<LINK_ONLY:vtkjsoncpp>;$<LINK_ONLY:vtkNetCDF>;$<LINK_ONLY:vtkNetCDF_cxx>;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkIOParallelXML
add_library(vtkIOParallelXML STATIC IMPORTED)

set_target_properties(vtkIOParallelXML PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkIOXML;vtkParallelCore;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkIOVideo
add_library(vtkIOVideo STATIC IMPORTED)

set_target_properties(vtkIOVideo PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonExecutionModel;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkImagingMath
add_library(vtkImagingMath STATIC IMPORTED)

set_target_properties(vtkImagingMath PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonDataModel;vtkCommonExecutionModel"
)

# Create imported target vtkImagingMorphological
add_library(vtkImagingMorphological STATIC IMPORTED)

set_target_properties(vtkImagingMorphological PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingCore;vtkImagingGeneral"
)

# Create imported target vtkImagingStatistics
add_library(vtkImagingStatistics STATIC IMPORTED)

set_target_properties(vtkImagingStatistics PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingCore"
)

# Create imported target vtkImagingStencil
add_library(vtkImagingStencil STATIC IMPORTED)

set_target_properties(vtkImagingStencil PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkCommonComputationalGeometry;vtkImagingCore"
)

# Create imported target vtkInteractionImage
add_library(vtkInteractionImage STATIC IMPORTED)

set_target_properties(vtkInteractionImage PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingColor;vtkInteractionStyle;vtkInteractionWidgets;vtkRenderingCore;vtkRenderingFreeType"
)

# Create imported target vtkRenderingImage
add_library(vtkRenderingImage STATIC IMPORTED)

set_target_properties(vtkRenderingImage PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkImagingCore;vtkRenderingCore"
)

# Create imported target vtkRenderingLIC
add_library(vtkRenderingLIC STATIC IMPORTED)

set_target_properties(vtkRenderingLIC PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkIOLegacy;vtkIOXML;vtkImagingSources;vtkRenderingOpenGL;$<LINK_ONLY:vtksys>"
)

# Create imported target vtkRenderingLOD
add_library(vtkRenderingLOD STATIC IMPORTED)

set_target_properties(vtkRenderingLOD PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkFiltersModeling;vtkRenderingCore"
)

# Create imported target vtkRenderingQt
add_library(vtkRenderingQt STATIC IMPORTED)

set_target_properties(vtkRenderingQt PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingLabel;$<LINK_ONLY:vtkFiltersTexture>;$<LINK_ONLY:vtkFiltersSources>;$<LINK_ONLY:vtkGUISupportQt>;$<LINK_ONLY:Qt5::Widgets>"
)

# Create imported target vtkRenderingVolumeOpenGL
add_library(vtkRenderingVolumeOpenGL STATIC IMPORTED)

set_target_properties(vtkRenderingVolumeOpenGL PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingOpenGL;vtkRenderingVolume;$<LINK_ONLY:vtksys>;$<LINK_ONLY:vtkFiltersGeneral>;$<LINK_ONLY:vtkFiltersSources>"
)

# Create imported target vtkViewsContext2D
add_library(vtkViewsContext2D STATIC IMPORTED)

set_target_properties(vtkViewsContext2D PROPERTIES
  INTERFACE_LINK_LIBRARIES "vtkRenderingContext2D;vtkViewsCore"
)

if(CMAKE_VERSION VERSION_LESS 2.8.12)
  message(FATAL_ERROR "This file relies on consumers using CMake 2.8.12 or greater.")
endif()

# Load information for each installed configuration.
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB CONFIG_FILES "${_DIR}/VTKTargets-*.cmake")
foreach(f ${CONFIG_FILES})
  include(${f})
endforeach()

# Cleanup temporary variables.
set(_IMPORT_PREFIX)

# Loop over all imported files and verify that they actually exist
foreach(target ${_IMPORT_CHECK_TARGETS} )
  foreach(file ${_IMPORT_CHECK_FILES_FOR_${target}} )
    if(NOT EXISTS "${file}" )
      message(FATAL_ERROR "The imported target \"${target}\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    endif()
  endforeach()
  unset(_IMPORT_CHECK_FILES_FOR_${target})
endforeach()
unset(_IMPORT_CHECK_TARGETS)

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
