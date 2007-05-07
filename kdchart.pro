include( variables.pri )

CONFIG += ordered

TEMPLATE=subdirs
SUBDIRS = src examples uitools plugins kdchartserializer

unix: SUBDIRS += kdchart1support

