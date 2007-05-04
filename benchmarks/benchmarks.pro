TEMPLATE = subdirs
SUBDIRS += DrawIntoPainter \

test.target=test
!win32:test.commands=for d in $${SUBDIRS}; do cd "\$$d" && $(MAKE) test && cd .. || exit -1; done
win32:test.commands=for %d in ($${SUBDIRS}); do cd "%d" && $(MAKE) test && cd .. || exit -1; done
test.depends = all $(TARGET)

QMAKE_EXTRA_TARGETS += test
