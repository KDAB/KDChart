include( variables.pri )

CONFIG += ordered

TEMPLATE=subdirs
SUBDIRS = src examples uitools plugins tests qtests benchmarks

# (Mirko) removed in coordination with khz, will be finished after Jan 08:
# unix: SUBDIRS += kdchart1support

# forward make test calls to qtests:
test.target=test
test.commands=(cd qtests && $(MAKE) test)
test.depends = all $(TARGET)
QMAKE_EXTRA_TARGETS += test
