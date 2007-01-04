include( variables.pri )

CONFIG += ordered

TEMPLATE=subdirs
SUBDIRS = src examples uitools plugins tests qtests 

# (Mirko) removed in coordination with khz, will be finished after Jan 08:
# unix: SUBDIRS += kdchart1support

