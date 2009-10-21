TEMPLATE=subdirs
SUBDIRS+=DelayedData Gantt

# Disabled for Windows cause compiling the RootIndex sample with
# mingw or nmake results in a Q_ASSERT in qmake (cause QList.first()
# is used while the QList is empty, removing the required
# OBJECTS_DIR from the RootIndex.pro fixes it) if qmake was
# compiled with debug enabled or in a crash if not.
!win32:SUBDIRS+=RootIndex
