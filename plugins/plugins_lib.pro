include( $${TOP_SOURCE_DIR}/variables.pri )
include( $${TOP_SOURCE_DIR}/features/kdchart.prf )

TEMPLATE = lib
TARGET = kdchartpluginlib
DESTDIR =$${TOP_BUILD_DIR}/lib
CONFIG(debug, debug|release) {
  !unix:TARGET = kdchartpluginlibd
}

DEFINES += KDCHART_BUILD_PLUGIN_LIB
CONFIG += designer

# Remove when all strings have been surrounded by tr() or QLatin1String()
DEFINES -= QT_NO_CAST_FROM_ASCII

# load(kdchart)

LIBS += -L$${TOP_BUILD_DIR}/lib -l$$KDCHARTLIB -l$$KDCHARTUITOOLSLIB

INCLUDEPATH +=  $${TOP_SOURCE_DIR}/uitools \
                $${TOP_BUILD_DIR}/uitools \
                $${TOP_SOURCE_DIR}/include \
                $${TOP_SOURCE_DIR}/include/KDChart \
                $${TOP_SOURCE_DIR}/src \
                $${TOP_SOURCE_DIR}/extra_include

DEPENDPATH += $${TOP_SOURCE_DIR}/uitools

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

