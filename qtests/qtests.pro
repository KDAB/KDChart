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

unix:SUBDIRS += AxisOwnership
test.target=test
!win32:test.commands=for d in $${SUBDIRS}; do cd "\$$d" && $(MAKE) test && cd .. || break; done
win32:test.commands=for %d in $${SUBDIRS}; do cd "%d" && $(MAKE) test && cd .. || break; done
test.depends = all $(TARGET)

QMAKE_EXTRA_TARGETS += test


