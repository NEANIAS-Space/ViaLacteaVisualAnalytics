project(ViaLactea)

# CMake version requirements
cmake_minimum_required(VERSION 2.8.7)

# Allow custom cmake overrides
set(VIALACTEA_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
set(CMAKE_MODULE_PATH ${VIALACTEA_CMAKE_DIR} ${CMAKE_MODULE_PATH})


# Third party library dependencies

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/lib)


 
  # Find VTK and get VTK configuration information
  find_package(VTK REQUIRED)

  # define VTK_VERSION if not already defined
  if(NOT DEFINED VTK_VERSION)
    set(VTK_VERSION ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION})
  endif()
  if(VTK_VERSION VERSION_LESS 8.90)
    include(${VTK_USE_FILE})
  endif()
  if(VTK_VERSION VERSION_LESS 5.8)
    message(FATAL_ERROR "VTK 5.8 or later is required.")
  endif()

  # We are being built as a stand-alone package
  set(VTK_VIALACTEA_LINK_TARGET vtkVialactea)

  # When CMake 3.0.0 is required, remove this block (see CMP0042).
  if(NOT DEFINED CMAKE_MACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
  endif()

  option(BUILD_SHARED_LIBS "Build shared libraries." OFF)



  # Wrap Python option
  if(VTK_WRAP_PYTHON)
    option(BUILD_PYTHON_WRAPPERS "Build python wrappers" ON)
  else()
    unset(BUILD_PYTHON_WRAPPERS CACHE)
  endif()


  # Defines CMAKE_INSTALL_LIBDIR etcetera
  include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})





  # Look for fitsio
  find_library(
    cfitsio 
    cfitsio
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/lib)


  if(NOT VTK_VERSION VERSION_LESS 8.90)
    # When building as a stand-alone module for VTK 9
    message(STATUS "vtkVialactea: Building as an External VTK Module")

    # Clear VTK_LIBS, linking will be handled by modular build
    set(VTK_LIBS)
    set(VTK_VIALACTEA_LINK_TARGET VTK::VIALACTEA)



    vtk_module_scan(
      MODULE_FILES        "${CMAKE_CURRENT_SOURCE_DIR}/src/fits.module"
      REQUEST_MODULES     "VTK::VIALACTEA"
      PROVIDES_MODULES    VIALACTEA_modules
      )

    vtk_module_build(
      MODULES             ${VIALACTEA_modules}
      INSTALL_EXPORT       VIALACTEA
      #  ARCHIVE_DESTINATION "lib"
      #  HEADERS_DESTINATION "include"
      CMAKE_DESTINATION   "${CMAKE_INSTALL_LIBDIR}/cmake/vtk"
      #  LICENSE_DESTINATION "${CMAKE_INSTALL_LIBDIR}/vtk"
      #  HIERARCHY_DESTINATION "${CMAKE_INSTALL_LIBDIR}/vtk/hierarchy/${CMAKE_PROJECT_NAME}"
     
)


if(VTK_WRAP_PYTHON)

     vtk_module_wrap_python(
        MODULES         ${VIALACTEA_modules}
        INSTALL_EXPORT  VIALACTEA
        PYTHON_PACKAGE  "vtkmodules"
        #MODULE_DESTINATION  ""
        CMAKE_DESTINATION   "${CMAKE_INSTALL_LIBDIR}/cmake/vtk"
        LIBRARY_DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        BUILD_STATIC    OFF)
    endif()

    
  endif() #(NOT VTK_VERSION VERSION_LESS 8.90)




if(Module_vtkVialactea)
  set(VIALACTEA_BIN_DIR ${VTK_INSTALL_RUNTIME_DIR})
  set(VIALACTEA_LIB_DIR ${VTK_INSTALL_LIBRARY_DIR})
  set(VIALACTEA_ARC_DIR ${VTK_INSTALL_ARCHIVE_DIR})
  set(VIALACTEA_INC_DIR ${VTK_INSTALL_INCLUDE_DIR})
endif()

# Set up our target directory structure for "make install"
set(VIALACTEA_BIN_DIR "bin") # for executables and ".dll" libraries
if(NOT VIALACTEA_LIB_DIR)
  set(VIALACTEA_LIB_DIR "lib") # for unix/linux/osx shared libraries
endif()
if(NOT VIALACTEA_ARC_DIR)
  set(VIALACTEA_ARC_DIR "${VIALACTEA_LIB_DIR}") # for static or ".lib" libraries
endif()
if(NOT VIALACTEA_INC_DIR)
  set(VIALACTEA_INC_DIR "include") # for header files
endif()


# Set the full paths to the install tree
set(VIALACTEA_RUNTIME_INSTALL_DEST ${CMAKE_INSTALL_PREFIX}/${VIALACTEA_BIN_DIR})
set(VIALACTEA_LIBRARY_INSTALL_DEST ${CMAKE_INSTALL_PREFIX}/${VIALACTEA_LIB_DIR})
set(VIALACTEA_ARCHIVE_INSTALL_DEST ${CMAKE_INSTALL_PREFIX}/${VIALACTEA_ARC_DIR})
set(VIALACTEA_INCLUDE_INSTALL_DEST ${CMAKE_INSTALL_PREFIX}/${VIALACTEA_INC_DIR})


