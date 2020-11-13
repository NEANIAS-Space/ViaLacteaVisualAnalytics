#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T11:03:07
#
#-------------------------------------------------

QT       += core gui network printsupport xml  widgets concurrent webenginewidgets webchannel
#QT       += core gui network printsupport xml  widgets concurrent webkitwidgets
#CONFIG   += static
QMAKE_MAC_SDK = macosx10.15
CONFIG-=app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisIVODesktop-vtk6_qt5
TEMPLATE = app


ICON = logo.icns


INCLUDEPATH += /usr/local/include/vtk-9.0/\
               /path_to/cfitsio-3.49/include/ \
               include/\
               src/\
               /usr/local/include/\


#LIBS += -L/opt/vtk-9.0.0/lib

LIBS += -L/path_to/cfitsio-3.49/lib/ -lcfitsio

#LIBS += /usr/local/Cellar/curl/7.73.0/lib/libcurl.a
LIBS += -L/usr/local/lib -lvtkChartsCore-9.0 -lvtkCommonColor-9.0 -lvtkCommonComputationalGeometry-9.0 -lvtkCommonCore-9.0 -lvtkCommonDataModel-9.0 -lvtkCommonExecutionModel-9.0 -lvtkCommonMath-9.0 -lvtkCommonMisc-9.0 -lvtkCommonSystem-9.0 -lvtkCommonTransforms-9.0 -lvtkDICOMParser-9.0 -lvtkDomainsChemistry-9.0 -lvtkFiltersAMR-9.0 -lvtkFiltersCore-9.0 -lvtkFiltersExtraction-9.0 -lvtkFiltersFlowPaths-9.0 -lvtkFiltersGeneral-9.0 -lvtkFiltersGeneric-9.0 -lvtkFiltersGeometry-9.0 -lvtkFiltersHybrid-9.0 -lvtkFiltersHyperTree-9.0 -lvtkFiltersImaging-9.0 -lvtkFiltersModeling-9.0 -lvtkFiltersParallel-9.0 -lvtkFiltersParallelImaging-9.0 -lvtkFiltersProgrammable-9.0 -lvtkFiltersSMP-9.0 -lvtkFiltersSelection-9.0 -lvtkFiltersSources-9.0 -lvtkFiltersStatistics-9.0 -lvtkFiltersTexture-9.0 -lvtkFiltersVerdict-9.0 -lvtkGUISupportQt-9.0 -lvtkGUISupportQtSQL-9.0 -lvtkGeovisCore-9.0 -lvtkIOAMR-9.0 -lvtkIOCore-9.0 -lvtkIOEnSight-9.0 -lvtkIOExodus-9.0 -lvtkIOExport-9.0 -lvtkIOGeometry-9.0 -lvtkIOImage-9.0 -lvtkIOImport-9.0 -lvtkIOInfovis-9.0 -lvtkIOLSDyna-9.0 -lvtkIOLegacy-9.0 -lvtkIOMINC-9.0 -lvtkIOMovie-9.0 -lvtkIOPLY-9.0 -lvtkIOParallel-9.0 -lvtkIOParallelXML-9.0 -lvtkIOSQL-9.0 -lvtkIOVideo-9.0 -lvtkIOXML-9.0 -lvtkIOXMLParser-9.0 -lvtkImagingColor-9.0 -lvtkImagingCore-9.0 -lvtkImagingFourier-9.0 -lvtkImagingGeneral-9.0 -lvtkImagingHybrid-9.0 -lvtkImagingMath-9.0 -lvtkImagingMorphological-9.0 -lvtkImagingSources-9.0 -lvtkImagingStatistics-9.0 -lvtkImagingStencil-9.0 -lvtkInfovisCore-9.0 -lvtkInfovisLayout-9.0 -lvtkInteractionImage-9.0 -lvtkInteractionStyle-9.0 -lvtkInteractionWidgets-9.0 -lvtkParallelCore-9.0 -lvtkRenderingAnnotation-9.0 -lvtkRenderingContext2D-9.0  -lvtkRenderingCore-9.0 -lvtkRenderingFreeType-9.0 -lvtkRenderingFreeType-9.0 -lvtkRenderingGL2PSOpenGL2-9.0 -lvtkRenderingImage-9.0 -lvtkRenderingLOD-9.0 -lvtkRenderingLabel-9.0 -lvtkRenderingOpenGL2-9.0 -lvtkRenderingQt-9.0 -lvtkRenderingVolume-9.0 -lvtkRenderingVolumeOpenGL2-9.0 -lvtkViewsContext2D-9.0 -lvtkViewsCore-9.0 -lvtkViewsInfovis-9.0 -lvtkViewsQt-9.0 -lvtkexodusII-9.0 -lvtkfreetype-9.0 -lvtkgl2ps-9.0  -lvtkjsoncpp-9.0 -lvtkmetaio-9.0 -lvtkogg-9.0  -lvtksqlite-9.0 -lvtksys-9.0 -lvtkverdict-9.0


macx:LIBS +=  -framework \
    Foundation \
    -framework \
    Cocoa \
    -framework \
    GLUT \
    -framework \
    QTKit \
    -framework \
    OpenGL \
    -framework \
    AGL \
    -framework \
    IOKit \
    -framework \
    QtPrintSupport


SOURCES += src/main.cpp\
           src/mainwindow.cpp \
    src/treemodel.cpp \
    src/treeitem.cpp \
    src/vtkfitsreader.cpp \
    src/vispoint.cpp \
    src/observedobject.cpp \
    src/operationqueue.cpp \
    src/sednode.cpp \
    src/sed.cpp \
    src/pointspipe.cpp \
    src/pipe.cpp \
    src/vtkellipse.cpp \
    src/base64.cpp \
    src/astroutils.cpp \
    src/libwcs/fitsfile.c \
    src/libwcs/hget.c \
    src/libwcs/fileutil.c \
    src/libwcs/fitswcs.c \
    src/libwcs/distort.c \
    src/libwcs/hput.c \
    src/libwcs/iget.c \
    src/libwcs/imio.c \
    src/libwcs/imhfile.c \
    src/libwcs/dateutil.c \
    src/libwcs/wcsinit.c \
    src/libwcs/dsspos.c \
    src/libwcs/wcs.c \
    src/libwcs/wcstrig.c \
    src/libwcs/wcscon.c \
    src/libwcs/lin.c \
    src/libwcs/platepos.c \
    src/libwcs/tnxpos.c \
    src/libwcs/wcslib.c \
    src/libwcs/cel.c \
    src/libwcs/proj.c \
    src/libwcs/sph.c \
    src/libwcs/worldpos.c \
    src/vialacteasource.cpp \
    src/vlkbquery.cpp \
    src/loadingwidget.cpp \
    src/sedvisualizerplot.cpp \
    src/qcustomplot.cpp \
    src/sedplotpointcustom.cpp \
    src/sedfitgrid_thin.cpp \
    src/sedfitgrid_thick.cpp \
    src/luteditor.cpp \
    src/vtklegendscaleactor.cpp \
    src/vtktoolswidget.cpp \
    src/color.cpp \
    src/vtkfitstoolswidget.cpp \
    src/higalselectedsources.cpp \
    src/plotwindow.cpp \
    src/vlkbquerycomposer.cpp \
    src/vlkbtable.cpp \
    src/fitsimagestatisiticinfo.cpp \
    src/vlkbsimplequerycomposer.cpp \
    src/dbquery.cpp \
    src/xmlparser.cpp \
    src/vialactea_fileload.cpp \
    src/selectedsourcefieldsselect.cpp \
    src/downloadmanager.cpp \
    src/viewselectedsourcedataset.cpp \
    src/vialactea.cpp \
    src/contour.cpp \
    src/histogram.cpp \
    src/lutselector.cpp \
    src/vtkwindow_new.cpp \
    src/vtkfitstoolwidget_new.cpp \
    src/vtkfitstoolwidgetobject.cpp \
    src/vialacteainitialquery.cpp \
    src/settingform.cpp \
    src/aboutform.cpp \
    src/selectedsourcesform.cpp \
    src/lutcustomize.cpp \
    src/vtkextracthistogram.cpp \
    src/extendedglyph3d.cpp \
 #   src/vosamp.cpp \
    src/visivoimporterdesktop.cpp \
    src/vstabledesktop.cpp \
    src/visivoutilsdesktop.cpp \
    src/vsobjectdesktop.cpp \
    src/filtercustomize.cpp \
    src/vialacteastringdictwidget.cpp


HEADERS  += include/mainwindow.h \
            include/singleton.h \
    include/treemodel.h \
    include/treeitem.h \
    include/vtkfitsreader.h \
    include/vispoint.h \
    include/observedobject.h \
    include/operationqueue.h \
    include/sednode.h \
    include/sed.h \
    include/pointspipe.h \
    include/pipe.h \
    include/vtkellipse.h \
    include/base64.h \
    include/astroutils.h \
    src/libwcs/fitsfile.h \
    src/libwcs/wcs.h \
    include/vialacteasource.h \
    include/vlkbquery.h \
    include/loadingwidget.h \
    include/sedvisualizerplot.h \
    include/qcustomplot.h \
    include/sedplotpointcustom.h \
    include/sedfitgrid_thin.h \
    include/sedfitgrid_thick.h \
    include/luteditor.h \
    include/vtklegendscaleactor.h \
    include/vtktoolswidget.h \
    include/color.h \
    include/vtkfitstoolswidget.h \
    include/higalselectedsources.h \
    include/plotwindow.h \
    include/vlkbquerycomposer.h \
    include/vlkbtable.h \
    include/fitsimagestatisiticinfo.h \
    include/vlkbsimplequerycomposer.h \
    include/dbquery.h \
    include/xmlparser.h \
    include/extendedglyph3d.h \
    include/vialactea_fileload.h \
    include/selectedsourcefieldsselect.h \
    include/downloadmanager.h \
    include/viewselectedsourcedataset.h \
    include/vialactea.h \
    include/contour.h   \
    include/histogram.h \
    include/lutselector.h \
    include/vtkwindow_new.h \
    include/vtkfitstoolwidget_new.h \
    include/vtkfitstoolwidgetobject.h \
    include/vialacteainitialquery.h \
    include/settingform.h \
    include/aboutform.h \
    include/selectedsourcesform.h \
    include/lutcustomize.h \
    include/vtkextracthistogram.h \
    include/osxhelper.h \
   # include/vosamp.h \
    include/visivoimporterdesktop.h \
    include/vstabledesktop.h \
    include/visivoutilsdesktop.h \
    include/vsobjectdesktop.h \
    include/visivofilterdesktop.h \
    include/filtercustomize.h \
    include/vialacteastringdictwidget.h


FORMS    += ui/mainwindow.ui \
    ui/operationqueue.ui \
    ui/vtkwindow.ui \
    ui/vlkbquery.ui \
    ui/loadingwidget.ui \
    ui/sedvisualizerplot.ui \
    ui/sedfitgrid_thin.ui \
    ui/sedfitgrid_thick.ui \
    ui/vtktoolswidget.ui \
    ui/vtkfitstoolswidget.ui \
    ui/higalselectedsources.ui \
    ui/plotwindow.ui \
    ui/vlkbquerycomposer.ui \
    ui/fitsimagestatisiticinfo.ui \
    ui/vlkbsimplequerycomposer.ui \
    ui/dbquery.ui \
    ui/vialactea_fileload.ui \
    ui/selectedsourcefieldsselect.ui \
    ui/viewselectedsourcedataset.ui \
    ui/vialactea.ui \
    ui/contour.ui \
    ui/vtkwindow_new.ui \
    ui/vtkfitstoolwidget_new.ui \
    ui/vialacteainitialquery.ui \
    ui/settingform.ui \
    ui/aboutform.ui \
    ui/selectedsourcesform.ui \
    ui/lutcustomize.ui \
    ui/filtercustomize.ui \
    ui/vialacteastringdictwidget.ui

RESOURCES += \
   visivo.qrc

DISTFILES +=

OBJECTIVE_SOURCES += \
    src/osxhelper.mm
