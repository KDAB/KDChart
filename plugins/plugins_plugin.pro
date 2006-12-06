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


# Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
# to avoid name clashes between debug/non-debug versions of the
# KD Chart library:
KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}
KDCHARTUITOOLSLIB = kdchartuitools
CONFIG(debug, debug|release) {
    !unix: KDCHARTUITOOLSLIB = "kdchartuitoolsd"
}

LIBS += -L$$KDCHARTDIR/lib -l$$KDCHARTLIB -l$$KDCHARTUITOOLSLIB


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

INCLUDEPATH += $$KDCHARTDIR/include \
               $$KDCHARTDIR/src \
               $$KDCHARTDIR/extra_include
DEPENDPATH +=  $$KDCHARTDIR/src

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
