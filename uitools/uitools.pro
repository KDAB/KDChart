
include( ../variables.pri )

UI_DIR = .
TEMPLATE = lib
TARGET = kdchartuitools
DESTDIR = ../lib
QT += xml designer
DLLDESTDIR = ../bin
KDCHART_PATH = ..
INCLUDEPATH+= ../include \
$(QTDIR)/src/3rdparty/zlib \
$$KDCHART_PATH/include \
$$KDCHART_PATH/src \
.
DEPENDPATH += ../include \
$(QTDIR)/src/3rdparty/zlib \
$$KDCHART_PATH/include \
.
LIBS += -L$$KDCHART_PATH/lib -lkdchart

win32{
  DLLDESTDIR = ../bin
  !static{
    DEFINES += KDCHART_MAKEDLL
  }
}

DEFINES += UITOOLS_BUILD_UITOOLS_LIB

KDAB_EVAL{
  HEADERS += ../evaldialog/evaldialog.h
  SOURCES += ../evaldialog/evaldialog.cpp
  DEFINES += KDAB_EVAL
}


FORMS += KDChartLegendPropertiesWidget.ui \
         KDChartHeaderFooterPropertiesWidget.ui \
         KDChartAxisPropertiesWidget.ui \
         KDChartDiagramPropertiesWidget.ui \
         
HEADERS += KDChartLegendPropertiesWidget.h \
           KDChartLegendPropertiesWidget_p.h \
           KDChartAxisPropertiesWidget.h \
           KDChartAxisPropertiesWidget_p.h \
           KDChartHeaderFooterPropertiesWidget.h \
           KDChartHeaderFooterPropertiesWidget_p.h \
           KDChartWidgetSerializer.h \
           KDChartChartSerializer.h \
           KDChartDiagramPropertiesWidget.h \
           KDChartDiagramPropertiesWidget_p.h \ 
           
SOURCES += KDChartLegendPropertiesWidget.cpp \
           KDChartAxisPropertiesWidget.cpp \
           KDChartHeaderFooterPropertiesWidget.cpp \
           KDChartWidgetSerializer.cpp \
           KDChartChartSerializer.cpp \ 
           KDChartDiagramPropertiesWidget.cpp \
