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

unix:SUBDIRS += AxisOwnership
test.target=test
unix:!macx {
    LIB_PATH=$${TOP_BUILD_DIR}/lib:\$\$LD_LIBRARY_PATH
    test.commands=for d in $${SUBDIRS}; do cd "\$$d" && LD_LIBRARY_PATH=$$LIB_PATH $(MAKE) test || TEST_FAIL=1; cd ..; done; if [ -n \"\$$TEST_FAIL\" ]; then exit 1; fi
}
unix:macx {
    LIB_PATH=$${TOP_BUILD_DIR}/lib:\$\$DYLD_LIBRARY_PATH
    test.commands=for d in $${SUBDIRS}; do ( cd "\$$d" && export DYLD_LIBRARY_PATH=$$LIB_PATH && $(MAKE) test ) || TEST_FAIL=1; done; if [ -n \"\$$TEST_FAIL\" ]; then exit 1; fi
}

win32 {
    WIN_BINDIR=
    debug_and_release {
        WIN_BINDIR=release
    }
    RUNTEST=$${TOP_SOURCE_DIR}/qtests/runTest.bat
    RUNTEST=$$replace(RUNTEST, /, \\)
    test.commands=for %d in ($${SUBDIRS}); do $$RUNTEST "%d" $$WIN_BINDIR || exit 1; done
}

test.depends = $(TARGET)

QMAKE_EXTRA_TARGETS += test


