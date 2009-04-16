TEMPLATE = lib
TARGET = kdchartpluginlib
CONFIG(debug, debug|release) {
  !unix:TARGET = kdchartpluginlibd
}

include( ../variables.pri )

DEFINES += KDCHART_BUILD_PLUGIN_LIB
CONFIG += designer

# Remove when all strings have been surrounded by tr() or QLatin1String()
DEFINES -= QT_NO_CAST_FROM_ASCII

# can't do that, man! 
# KDCHARTDIR = ../
KDCHARTBASE = ../

LIBS += -L$$KDCHARTBASE/lib -l$$KDCHARTLIB -l$$KDCHARTUITOOLSLIB

INCLUDEPATH +=  $$KDCHARTBASE/uitools \
                $$KDCHARTBASE/include \
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

