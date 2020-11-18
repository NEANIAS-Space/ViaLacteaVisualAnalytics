cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(ViaLactea)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Core Network GUI PrintSupport Xml Widgets Concurrent WebChannel WebEngineWidgets REQUIRED)
find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkChartsCore
  vtkCommonComputationalGeometry
  vtkCommonExecutionModel
  vtkCommonMisc
  vtkCommonMath
  vtkCommonSystem
  vtkCommonTransforms
  vtkDICOMParser
  vtkDomainsChemistry
vtkFiltersAMR
  vtkFiltersCore
vtkFiltersExtraction
vtkFiltersFlowPaths
vtkFiltersGeneral
vtkFiltersGeneric
vtkFiltersGeometry
vtkFiltersHybrid
vtkFiltersHyperTree
vtkFiltersImaging
vtkFiltersModeling
vtkFiltersParallel
vtkFiltersParallelImaging
vtkFiltersProgrammable
vtkFiltersSMP
vtkFiltersSelection
vtkFiltersSources
vtkFiltersStatistics
vtkFiltersTexture
vtkFiltersVerdict
  vtkCommonDataModel
  vtkInteractionStyle

vtkGUISupportQt
vtkGUISupportQtSQL
vtkGeovisCore
#vtkIOAMR
vtkIOCore
#vtkIOEnSight
#vtkIOExodus
vtkIOExport
vtkIOGeometry
vtkIOImage
vtkIOImport
vtkIOInfovis
#vtkIOLSDyna
#vtkIOLegacy
#vtkIOMINC
vtkIOMovie
vtkIOPLY
vtkIOParallel
vtkIOParallelXML
#vtkIOSQL
vtkIOVideo
vtkIOXML
vtkIOXMLParser
vtkImagingColor
vtkImagingCore
vtkImagingFourier
vtkImagingGeneral
vtkImagingHybrid
vtkImagingMath
vtkImagingMorphological
vtkImagingSources
vtkImagingStatistics
vtkImagingStencil
vtkInfovisCore
vtkInfovisLayout
vtkInteractionImage
vtkInteractionStyle
vtkInteractionWidgets
vtkParallelCore
vtkRenderingAnnotation
vtkRenderingContext2D
vtkRenderingFreeType
vtkRenderingFreeType
vtkRenderingImage
vtkRenderingLOD
vtkRenderingLabel
vtkRenderingQt
vtkRenderingVolume
vtkRenderingVolumeOpenGL2
vtkViewsContext2D
vtkViewsCore
vtkViewsInfovis
vtkViewsQt
#vtkexodusII
vtkfreetype
vtkgl2ps
vtkjsoncpp
vtkmetaio
vtkogg
vtksqlite
vtksys
vtkverdict
vtkRenderingCore
vtkRenderingFreeType
#vtkRenderingContextOpenGL2
  vtkRenderingGL2PSOpenGL2
  vtkViewsQt
  vtkRenderingOpenGL2 QUIET)


if (NOT VTK_FOUND)
  message("Skipping ViaLactea as: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
 endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")


set(SOURCES_WCS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
           ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/treemodel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/treeitem.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtkfitsreader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vispoint.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/observedobject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/operationqueue.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sednode.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sed.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pointspipe.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pipe.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtkellipse.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base64.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/astroutils.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sedvisualizerplot.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcustomplot.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sedplotpointcustom.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sedfitgrid_thin.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sedfitgrid_thick.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/luteditor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtklegendscaleactor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtktoolswidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/color.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtkfitstoolswidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/higalselectedsources.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/plotwindow.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vlkbquerycomposer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vlkbtable.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fitsimagestatisiticinfo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vlkbsimplequerycomposer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dbquery.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/xmlparser.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vialactea_fileload.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/selectedsourcefieldsselect.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/downloadmanager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/viewselectedsourcedataset.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vialactea.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/contour.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/histogram.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lutselector.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtkwindow_new.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtkfitstoolwidget_new.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtkfitstoolwidgetobject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vialacteainitialquery.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/settingform.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aboutform.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/selectedsourcesform.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lutcustomize.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vtkextracthistogram.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extendedglyph3d.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visivoimporterdesktop.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vstabledesktop.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visivoutilsdesktop.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vsobjectdesktop.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filtercustomize.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vialacteastringdictwidget.cpp )

file(GLOB HEADERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB FORMS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)

#file(GLOB SOURCES_WCS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/*.c)


set(SOURCES_WCS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/fitsfile.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/hget.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/fileutil.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/fitswcs.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/distort.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/hput.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/iget.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/imio.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/imhfile.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/dateutil.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/wcsinit.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/dsspos.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/wcs.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/wcstrig.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/wcscon.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/lin.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/platepos.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/tnxpos.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/wcslib.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/cel.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/proj.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/sph.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/worldpos.c )

# a separate include folder for h files
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# make libwcs include a separete
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/lib)

find_library(
    cfitsio 
    cfitsio
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/lib)

# Set up Qt forms/resources
# Should be set manually because Qt is old 
qt5_wrap_ui(${CMAKE_CURRENT_SOURCE_DIR}/src/aboutform.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ui/aboutform.ui )
qt5_wrap_ui( ${FORMS_SRC})

qt5_add_resources(RESOURCE_FILE resources.qrc)
include_directories (${CMAKE_BINARY_DIR})

add_executable(ViaLactea
    ${SOURCES_SRC}
    ${SOURCES_WCS_SRC}
    ${HEADERS_SRC}
    ${FORMS_SRC}
    visivo.qrc
)

FIND_PACKAGE(Boost REQUIRED)

if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS}) 
   
   
endif()


target_link_libraries(ViaLactea  Qt5::Gui  Qt5::Network  Qt5::PrintSupport  Qt5::Xml Qt5::Widgets  Qt5::Concurrent  Qt5::WebEngineWidgets)
target_link_libraries(ViaLactea ${VTK_LIBRARIES} ${cfitsio})

#target_link_libraries(ViaLactea ${Boost_LIBRARIES})
vtk_module_autoinit(
    TARGETS ViaLactea
    MODULES ${VTK_LIBRARIES}
    )