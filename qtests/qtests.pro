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

unix:SUBDIRS += AxisOwnership
test.target=test
!win32:test.commands=for d in $${SUBDIRS}; do cd "\$$d" && LD_LIBRARY_PATH=../../lib && $(MAKE) test && cd .. || exit -1; done
win32:test.commands=for %d in ($${SUBDIRS}); do cd "%d" && PATH=..\..\lib; && $(MAKE) test && cd .. || exit -1; done
test.depends = all $(TARGET)

QMAKE_EXTRA_TARGETS += test


