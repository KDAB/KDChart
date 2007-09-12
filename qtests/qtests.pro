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
            CartesianDiagramDataCompressor \
            PieDiagrams \
            PolarDiagrams 
#           ParamVsParam \
#           DrawIntoPainter \
###################################
#  adding the serializer tests here
# depending on the Qt versions             
###################################
contains($$list($$[QT_VERSION]), 4.[2-9].*) { 
SUBDIRS +=  AttributesSerializer \
            AttributesModelSerializer \
            TextAreaSerializer \
            AxesSerializer \
            LegendsSerializer \
            DiagramsSerializer \
	    XmlToolsTests \
#           CoordPlanesSerializer \
#           Serializer
}

unix:SUBDIRS += AxisOwnership
release:WIN_BINDIR=release
debug:WIN_BINDIR=debug
#debug_and_release:WIN_BINDIR=release
test.target=test
unix:!macx {
    LIB_PATH=../../lib:\$\$LD_LIBRARY_PATH
    test.commands=for d in $${SUBDIRS}; do cd "\$$d" && LD_LIBRARY_PATH=$$LIB_PATH && $(MAKE) test && cd .. || exit 1; done
}
unix:macx {
    LIB_PATH=../../lib:\$\$DYLD_LIBRARY_PATH
    test.commands=for d in $${SUBDIRS}; do ( cd "\$$d" && export DYLD_LIBRARY_PATH=$$LIB_PATH && $(MAKE) test ) || exit 1; done
}
win32:test.commands=for %d in ($${SUBDIRS}); do runTest.bat "%d" $$WIN_BINDIR || exit -1; done
test.depends = $(TARGET)

QMAKE_EXTRA_TARGETS += test


