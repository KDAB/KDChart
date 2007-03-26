# Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
# to avoid name clashes between debug/non-debug versions of the
# KD Chart library:
KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}

QT += xml

CONFIG+=qtestlib
TEMPLATE = app
INCLUDEPATH +=  $$KDCHART_PATH/examples/tools \
                $$KDCHART_PATH/include \
                $$KDCHART_PATH/kdchartserializer/include \
                $$KDCHART_PATH/kdchartserializer/src
DEPENDPATH += . $$KDCHART_PATH/src \
                $$KDCHART_PATH/lib \
                $$KDCHART_PATH/examples/tools \
                $$KDCHART_PATH/include \
                $$KDCHART_PATH/kdchartserializer/include \
                $$KDCHART_PATH/kdchartserializer/src

test.target = test
test.commands = LD_LIBRARY_PATH=$$KDCHART_PATH/lib ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test

LIBS += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB -lkdchartserializer -ltesttools
