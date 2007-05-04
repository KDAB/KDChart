TEMPLATE = subdirs
SUBDIRS +=  ChartElementOwnership \
            #AxisOwnership \
            AttributesModel \
            Measure \
            RelativePosition \
            Palette \
            Legends \
            CartesianPlanes \
            Cloning \
            PolarPlanes \
            QLayout \
            WidgetElementOwnership \
            BarDiagrams \
	    LineDiagrams \
            PieDiagrams \
            PolarDiagrams \
            ParamVsParam \
	    DrawIntoPainter \


# 1. Work around a qmake bug:
#
#    The bug would produce invalid *.exe files when KD Chart is linked
#    statically and examples are build right after building the lib.
staticlib {
  CONFIG -= staticlib
  DEFINES += KDCHART_STATICLIB
}


# 2. Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
#    to avoid name clashes between debug/non-debug versions of the
#    KD Chart library:
KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}


unix:SUBDIRS += AxisOwnership
test.target=test
!win32:test.commands=for d in $${SUBDIRS}; do cd "\$$d" && LD_LIBRARY_PATH=../../lib && $(MAKE) test && cd .. || exit -1; done
win32:test.commands=for %d in ($${SUBDIRS}); do cd "%d" && set PATH=..\..\lib; && $(MAKE) test && cd .. || exit -1; done
test.depends = all $(TARGET)

QMAKE_EXTRA_TARGETS += test


