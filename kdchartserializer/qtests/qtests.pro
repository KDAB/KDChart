TEMPLATE = subdirs
SUBDIRS +=  AttributesSerializer \
            AttributesModelSerializer \
            TextAreaSerializer \
            AxesSerializer \
            LegendsSerializer \
            DiagramsSerializer \
#            CoordPlanesSerializer \
#            Serializer

test.target=test
unix:!macx:test.commands=for d in $${SUBDIRS}; do cd "\$$d" && LD_LIBRARY_PATH=../../../lib && $(MAKE) test && cd .. || exit -1; done
unix:macx:test.commands=for d in $${SUBDIRS}; do cd "\$$d" && DYLD_LIBRARY_PATH=../../../lib && $(MAKE) test && cd .. || exit -1; done
test.depends = all $(TARGET)
QMAKE_EXTRA_TARGETS += test
