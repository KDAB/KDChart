include( ../variables.pri )

UI_DIR = .

TEMPLATE = lib
TARGET = kdchartuitools
DESTDIR	= ../lib

QT += xml

# use qmake CONFIG+=dll to compile the kdchart compat library as a DLL
dll {
	DEFINES += KDCHART_UITOOLS_DLL
}

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
LIBS        += -L$$KDCHART_PATH/lib -lkdchart


SOURCES = \
        KDChartLegendPropertiesWidget.cpp \
        KDChartAxisPropertiesWidget.cpp \
        KDChartHeaderFooterPropertiesWidget.cpp \

HEADERS = \
        KDChartLegendPropertiesWidget.h \
        KDChartLegendPropertiesWidget_p.h \
        KDChartAxisPropertiesWidget.h \
        KDChartAxisPropertiesWidget_p.h \
        KDChartHeaderFooterPropertiesWidget.h \
        KDChartHeaderFooterPropertiesWidget_p.h \

FORMS = \
        KDChartLegendPropertiesWidget.ui \
        KDChartHeaderFooterPropertiesWidget.ui \

KDAB_EVAL {
  HEADERS += ../evaldialog/evaldialog.h
  SOURCES += ../evaldialog/evaldialog.cpp
  DEFINES += KDAB_EVAL
}
