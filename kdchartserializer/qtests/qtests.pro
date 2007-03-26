TEMPLATE = subdirs
SUBDIRS +=  AttributesSerializer 
#            TextAreaSerializer \
#            AbstractAreaBaseSerializer \
#            AttributesModelSerializer \
#            AxesSerializer \
#            DiagramsSerializer \
#            LegendsSerializer \
#            CoordPlanesSerializer \
#            Serializer

test.target=test
test.commands=for d in $${SUBDIRS}; do (cd "\$$d" && make test); done
test.depends = all $(TARGET)
QMAKE_EXTRA_TARGETS += test
