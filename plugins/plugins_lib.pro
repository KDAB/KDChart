include( $${TOP_SOURCE_DIR}/variables.pri )
include( $${TOP_SOURCE_DIR}/features/kdchart.prf )

TEMPLATE = lib
TARGET = kdchartpluginlib
DESTDIR = ../lib
CONFIG(debug, debug|release) {
  !unix:TARGET = kdchartpluginlibd
}

DEFINES += KDCHART_BUILD_PLUGIN_LIB
CONFIG += designer

# Remove when all strings have been surrounded by tr() or QLatin1String()
DEFINES -= QT_NO_CAST_FROM_ASCII

# load(kdchart)

# KDCHARTDIR = ../
KDCHARTBASE = ../

LIBS += -L$$KDCHARTBASE/lib -l$$KDCHARTLIB -l$$KDCHARTUITOOLSLIB

INCLUDEPATH +=  $${TOP_SOURCE_DIR}/uitools \
                $${TOP_BUILD_DIR}/uitools \
                $$KDCHARTBASE/include \
                $$KDCHARTBASE/include/KDChart \
                $$KDCHARTBASE/src \
                $$KDCHARTBASE/extra_include

DEPENDPATH += $$KDCHARTBASE/uitools

HEADERS = \
	kdchartdesignerextension.h \
	kdchartwidgetdesignercustomeditor.h \ 
        kdchartchartdesignercustomeditor.h \
        kdchartextrainfoextension.h \
        #diagramtype.h \

SOURCES = \
	kdchartdesignerextension.cpp \
	kdchartwidgetdesignercustomeditor.cpp \ 
        kdchartchartdesignercustomeditor.cpp \
        kdchartextrainfoextension.cpp \
        #diagramtype.cpp \

FORMS = \
        kdchartwidgetdesignercustomeditor.ui \
        kdchartchartdesignercustomeditor.ui \
        #diagramtype.ui \

