TEMPLATE = subdirs
SUBDIRS +=  ChartElementOwnership \
            #AxisOwnership \
            KDChartAttributesModel \
            KDChartMeasure \
            KDChartPalette \
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
test.commands=for d in $${SUBDIRS}; do (cd "\$$d" && make test); done
test.depends = all $(TARGET)
QMAKE_EXTRA_TARGETS += test

