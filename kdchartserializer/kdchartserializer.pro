include( kdchartserializer.pri )

CONFIG += ordered

TEMPLATE=subdirs
SUBDIRS = src examples

SUBDIRS += qtests

# forward make test calls to qtests:
test.target=test
test.commands=(cd qtests && $(MAKE) test)
test.depends = all $(TARGET)
QMAKE_EXTRA_TARGETS += test
