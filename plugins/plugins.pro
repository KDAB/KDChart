TEMPLATE = subdirs
SUBDIRS = plugins_lib \
    plugins_plugin

plugins_lib.file = plugins_lib.pro

plugins_plugin.file = plugins_plugin.pro
plugins_plugin.depends = plugins_lib
