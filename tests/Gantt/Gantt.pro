include(../variables.pri)

TEMPLATE=subdirs
SUBDIRS=gfxview view listview apireview headers
# Tests beginMoveRows() which was introduced in Qt 4.6
greaterThan(QT_MINOR_VERSION, 5) {
    SUBDIRS += reorder
}
SUBDIRS += unittest
