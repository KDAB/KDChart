TEMPLATE = lib


CONFIG += plugin

# Necessary when Qt was build with debug_and_release, but breaks if Qt was not.
# CONFIG += build_all

TARGET = kdchartwidget_plugins
DESTDIR = ../lib

CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,d)
}

THIS_DLL=PLUGINS_PLUGIN_DLL

include( ../kdchart.pri )

CONFIG += designer message-target debug_and_release
KDCHARTDIR = ../
load(kdchart)

HEADERS += plugins.h
SOURCES += plugins.cpp

win32:LIBS += kdchartpluginlib.lib
unix:LIBS += -lkdchartpluginlib

target.path = $$[QT_INSTALL_PLUGINS]/designer

templates.path = $$[QT_INSTALL_PREFIX]/bin/templates
templates.files = ../templates/*.ui

INSTALLS += target templates
