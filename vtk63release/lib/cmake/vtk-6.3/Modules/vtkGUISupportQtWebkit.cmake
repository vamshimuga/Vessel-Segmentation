set(vtkGUISupportQtWebkit_LOADED 1)
set(vtkGUISupportQtWebkit_DEPENDS "vtkViewsQt")
set(vtkGUISupportQtWebkit_LIBRARIES "vtkGUISupportQtWebkit")
set(vtkGUISupportQtWebkit_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-6.3")
set(vtkGUISupportQtWebkit_LIBRARY_DIRS "")
set(vtkGUISupportQtWebkit_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/lib")
set(vtkGUISupportQtWebkit_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkGUISupportQtWebkitHierarchy.txt")
set(vtkGUISupportQtWebkit_EXCLUDE_FROM_WRAPPING 1)

if(NOT Qt5WebKitWidgets_DIR)
  set(Qt5WebKitWidgets_DIR "/Users/chuwa/Qt/5.5/clang_64/lib/cmake/Qt5WebKitWidgets")
endif()
find_package(Qt5WebKitWidgets REQUIRED QUIET)

