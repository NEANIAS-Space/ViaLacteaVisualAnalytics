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
vtkIOAMR
vtkIOCore
vtkIOEnSight
vtkIOExodus
vtkIOExport
vtkIOGeometry
vtkIOImage
vtkIOImport
vtkIOInfovis
#vtkIOLSDyna
#vtkIOLegacy
vtkIOMINC
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
vtkexodusII
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


file(GLOB SOURCES_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# make libwcs include a separete
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

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