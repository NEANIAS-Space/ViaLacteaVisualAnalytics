


# Create the main library
set(LIB_NAME vtkVialactea)
  

# Sources in the current directory (library sources only!)
set(LIB_SRCS
  vtkFitsReader.cxx 
  vtkFitsUnstructuredReader.cxx 

)


# Third party library dependencies

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/INSTALL/CFITSIO/lib)


# Sources that are not vtkObjects
set(LIB_SPECIAL
    main.cpp
     extendedglyph3d.cpp 
     vtkellipse.cpp 
     vtkextracthistogram.cpp 
    #  vtklegendscaleactor.cpp 
     libwcs/fitsfile.c 
     libwcs/hget.c 
     libwcs/fileutil.c 
     libwcs/fitswcs.c 
     libwcs/distort.c 
     libwcs/hput.c 
     libwcs/iget.c 
     libwcs/imio.c 
     libwcs/imhfile.c 
     libwcs/dateutil.c 
     libwcs/wcsinit.c 
     libwcs/dsspos.c 
     libwcs/wcs.c 
     libwcs/wcstrig.c 
     libwcs/wcscon.c 
     libwcs/lin.c 
     libwcs/platepos.c 
     libwcs/tnxpos.c 
     libwcs/wcslib.c 
     libwcs/cel.c 
     libwcs/proj.c 
     libwcs/sph.c 
     libwcs/worldpos.c
)

# Headers without a matching .cxx file are listed here
set(LIB_HDRS
vtkFitsReader.h
vtkFitsUnstructuredReader.h
)


set_source_files_properties(${LIB_HDRS} ${LIB_SPECIAL}
  PROPERTIES WRAP_EXCLUDE ON)






# Create the library


  # When building as a module for VTK 8.90 or later
   set(LIB_SRC_HDRS)
   foreach(_src ${LIB_SRCS})
     get_filename_component(_base ${_src} NAME_WE)
     list(APPEND LIB_SRC_HDRS ${_base}.h)
   endforeach()


  vtk_module_add_module(VTK::VIALACTEA
     SOURCES ${LIB_SRCS}
     HEADERS ${LIB_SRC_HDRS} ${LIB_HDRS})
   vtk_module_link(VTK::VIALACTEA
     PRIVATE ${cfitsio} ${curl} )

