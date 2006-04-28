TEMPLATE = subdirs
SUBDIRS +=  ChartElementOwnership \
            KDChartAttributesModel \
            KDChartPalette \

test.target=test
test.commands=for d in $${SUBDIRS}; do (cd "\$$d" && make test); done
test.depends = all $(TARGET)
QMAKE_EXTRA_TARGETS += test

