include(../variables.pri)

TEMPLATE=subdirs
SUBDIRS=gfxview view listview apireview headers

!contains(CONFIG,KDAB_NO_UNIT_TESTS) {
  SUBDIRS += unittest
}
