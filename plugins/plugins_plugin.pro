TEMPLATE = lib
CONFIG += plugin
TARGET = kdchart_plugin
CONFIG(debug, debug|release) {
    !unix:TARGET = kdchart_plugind
}

# can't do that, man:
# KDCHARTDIR = ../
KDCHARTBASE = ../

include( $$KDCHARTBASE/variables.pri )

INCLUDEPATH += $$KDCHARTBASE/include
DEFINES += PLUGINS_PLUGIN_DLL

CONFIG += designer debug_and_release

load(kdchart)

staticlib {
  CONFIG += kdchart_static
  CONFIG -= staticlib
}

HEADERS += plugins.h
SOURCES += plugins.cpp

unix {
  LIBS += -L$$KDCHARTBASE/lib -lkdchartpluginlib -lkdchartuitools -lkdchart
  kdchart_static:LIBS += -lkdchartuitools
} else {
  CONFIG(debug, debug|release) {
    LIBS += -lkdchartpluginlibd
    kdchart_static:LIBS += -lkdchartuitoolsd
  } else {
    LIBS += -lkdchartpluginlib
    kdchart_static:LIBS += -lkdchartuitools
  }
}

INCLUDEPATH += $$KDCHARTBASE/src \
               $$KDCHARTBASE/extra_include
DEPENDPATH +=  $$KDCHARTBASE/src

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
