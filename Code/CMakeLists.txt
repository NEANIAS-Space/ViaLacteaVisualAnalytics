cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(ViaLactea)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Core Network GUI PrintSupport Xml Widgets Concurrent WebEngineWidgets REQUIRED)
find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkCommonDataModel
  vtkInteractionStyle
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingOpenGL2 QUIET)
if (NOT VTK_FOUND)
  message("Skipping ViaLactea: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
 endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")


file(GLOB SOURCES_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB FORMS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)

file(GLOB SOURCES_WCS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs/*.c)

# a separate include folder for h files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/libwcs)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/lib)

# Set up Qt forms/resources
# Should be set manually because Qt is old 
qt5_wrap_ui(${CMAKE_CURRENT_SOURCE_DIR}/src/aboutform.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ui/aboutform.ui )
qt5_wrap_ui( ${FORMS_SRC})

#qt5_add_resources(RESOURCE_FILE resources.qrc)
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
target_link_libraries(ViaLactea ${VTK_LIBRARIES} cfitsio)
#target_link_libraries(ViaLactea ${Boost_LIBRARIES})
vtk_module_autoinit(
    TARGETS ViaLactea
    MODULES ${VTK_LIBRARIES}
    )