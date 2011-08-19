include( $$KDCHARTBASE/variables.pri )

TEMPLATE = lib
CONFIG += plugin
TARGET = kdchart_plugin
CONFIG(debug, debug|release) {
    !unix:TARGET = kdchart_plugind
}

# KDCHARTDIR = ../
KDCHARTBASE = ..

INCLUDEPATH += $$KDCHARTBASE/include
DEFINES += PLUGINS_PLUGIN_DLL

CONFIG += designer debug_and_release

CONFIG( static ) {
  CONFIG += kdchart_static
  CONFIG -= staticlib
}

HEADERS += plugins.h
SOURCES += plugins.cpp

LIBS += -L$$KDCHARTBASE/lib -l$$KDCHARTPLUGINLIBLIB -l$$KDCHARTLIB -l$$KDCHARTUITOOLSLIB
kdchart_static: LIBS += -l$$KDCHARTUITOOLSLIB

INCLUDEPATH += $$KDCHARTBASE/src \
               $$KDCHARTBASE/extra_include
DEPENDPATH +=  $$KDCHARTBASE/src

CONFIG(install-qt){
  target.path = $$[QT_INSTALL_PLUGINS]/designer
  message( "Installation of KD Chart Plug-in into" $$target.path)
} else {
  target.path = $$INSTALL_PREFIX/plugins/designer
  message( "" )
  message( "Installation of KD Chart Plug-in into" $$target.path)
  message( ( Use \"qmake CONFIG+=install-qt\" to install into $$[QT_INSTALL_PLUGINS]/designer ) )
  message( "" )
}
INSTALLS += target
