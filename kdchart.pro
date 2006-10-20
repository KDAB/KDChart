include( variables.pri )

CONFIG += ordered

TEMPLATE=subdirs
SUBDIRS = src examples uitools plugins tests qtests 
unix: SUBDIRS += kdchart1support

