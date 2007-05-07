include( variables.pri )

CONFIG += ordered

TEMPLATE=subdirs
SUBDIRS = src examples uitools plugins

# forward make test calls to qtests:
test.target=test
test.commands=(echo "Disabled for now.")
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test
