TEMPLATE = lib
CONFIG += plugin
TARGET = kdchart_plugin
CONFIG(debug, debug|release) {
    !unix:TARGET = kdchart_plugind
}

include( ../variables.pri )

DEFINES += PLUGINS_PLUGIN_DLL

# Remove when all strings have been surrounded by tr() or QLatin1String()
DEFINES -= QT_NO_CAST_FROM_ASCII

CONFIG += designer debug_and_release
KDCHARTDIR = ../
load(kdchart)

HEADERS += plugins.h
SOURCES += plugins.cpp

unix {
  LIBS += -lkdchartpluginlib
} else {
  CONFIG(debug, debug|release) {
    LIBS += -lkdchartpluginlibd
  } else {
    LIBS += -lkdchartpluginlib
  }
}

INCLUDEPATH += ../src
DEPENDPATH += ../src

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
