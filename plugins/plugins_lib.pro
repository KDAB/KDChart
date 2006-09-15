TEMPLATE = lib

srcdir = ../plugins

DESTDIR = ../lib

TARGET = kdchartpluginlib

THIS_DLL=PLUGINS_LIB_DLL

include( ../kdchart.pri )

CONFIG += designer message-target

KDCHARTDIR = ../
load(kdchart)

HEADERS = \
	kdchartdesignerextension.h \
	kdchartwidgetdesignercustomeditor.h \ 

SOURCES = \
	kdchartdesignerextension.cpp \
	kdchartwidgetdesignercustomeditor.cpp \ 

FORMS = \
        kdchartwidgetdesignercustomeditor.ui \
