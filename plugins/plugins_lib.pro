TEMPLATE = lib

srcdir = ../plugins

DESTDIR = ../lib

TARGET = kdchartpluginlib

THIS_DLL=PLUGINS_LIB_DLL

include( ../kdchart.pri )

CONFIG += designer message-target

KDCHARTDIR = ../
load(kdchart)

win32:LIBS += kdchartuitools.lib
unix:LIBS += -lkdchartuitools

INCLUDEPATH += ../uitools \
                ../src

DEPENDPATH += ../uitools

HEADERS = \
	kdchartdesignerextension.h \
	kdchartwidgetdesignercustomeditor.h \ 
        kdchartchartdesignercustomeditor.h \
        kdchartextrainfoextension.h \
        diagramtype.h \

SOURCES = \
	kdchartdesignerextension.cpp \
	kdchartwidgetdesignercustomeditor.cpp \ 
        kdchartchartdesignercustomeditor.cpp \
        kdchartextrainfoextension.cpp \
        diagramtype.cpp \

FORMS = \
        kdchartwidgetdesignercustomeditor.ui \
        kdchartchartdesignercustomeditor.ui \
        diagramtype.ui \
        
