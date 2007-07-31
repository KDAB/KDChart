include( variables.pri )

TEMPLATE=subdirs
SUBDIRS = src examples uitools plugins tests qtests
CONFIG += ordered

contains($$list($$[QT_VERSION]), 4.[2-9].*) { SUBDIRS += kdchartserializer }

SUBDIRS +=qtests

unix: SUBDIRS += kdchart1support

# forward make test calls to qtests:
test.target=test
test.commands=(cd qtests && $(MAKE) test)
test.depends = all $(TARGET)
QMAKE_EXTRA_TARGETS += test
