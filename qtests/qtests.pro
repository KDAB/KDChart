TEMPLATE = subdirs
SUBDIRS +=  ChartElementOwnership \
            #AxisOwnership \
            AttributesModel \
            Measure \
            RelativePosition \
            Palette \
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
#           ParamVsParam \
#           DrawIntoPainter \
###################################
#  adding the serializer tests here            
###################################
            AttributesSerializer \
            AttributesModelSerializer \
            TextAreaSerializer \
            AxesSerializer \
            LegendsSerializer \
            DiagramsSerializer \
#           CoordPlanesSerializer \
#           Serializer	   


unix:SUBDIRS += AxisOwnership
release:WIN_BINDIR=release
debug:WIN_BINDIR=debug
debug_and_release:WIN_BINDIR=debug
test.target=test
unix:!macx:test.commands=for d in $${SUBDIRS}; do cd "\$$d" && LD_LIBRARY_PATH=../../lib && $(MAKE) test && cd .. || exit -1; done
unix:macx:test.commands=for d in $${SUBDIRS}; do ( cd "\$$d" && export DYLD_LIBRARY_PATH=../../lib && $(MAKE) test ) || exit -1; done
win32:test.commands=for %d in ($${SUBDIRS}); do runTest.bat "%d" $$WIN_BINDIR || exit -1; done
test.depends = $(TARGET)

QMAKE_EXTRA_TARGETS += test


