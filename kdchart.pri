# Not used by KDChart itself. This is for use in other projects.
# Include this file directly, or copy the file into your project.

# copy from environment:
  isEmpty( KDCHARTDIR ):KDCHARTDIR=$$(KDCHARTDIR)
  !isEmpty( KDCHARTDIR ) {
    unix:isEmpty(QMAKE_EXTENSION_SHLIB) {
      static:QMAKE_EXTENSION_SHLIB=a
      else {
        macx:QMAKE_EXTENSION_SHLIB=dylib
        else:QMAKE_EXTENSION_SHLIB=so
      }
    }
    unix:!exists( $$KDCHARTDIR/lib/libkdchart.$$QMAKE_EXTENSION_SHLIB ):error( "Cannot find libkdchart.$$QMAKE_EXTENSION_SHLIB in $$KDCHARTDIR/lib" )
    unix:!exists( $$KDCHARTDIR/include/KDChart/KDChartGlobal ):error( "Cannot find KDChartGlobal in $$KDCHARTDIR/include/KDChart" )

    KDCHART_LIB_TYPE_SUFFIX = ""
    win32 {
        CONFIG(debug, debug|release) {
            KDCHART_LIB_TYPE_SUFFIX="d"
        }
    }

    LIBS += -L$$KDCHARTDIR/lib
    win32 {
      LIBS += -lkdchart$$KDCHART_LIB_TYPE_SUFFIX
    } else {
      !isEmpty(QMAKE_LFLAGS_RPATH):LIBS += $$QMAKE_LFLAGS_RPATH$$KDCHARTDIR/lib
      LIBS += -lkdchart
    }

    INCLUDEPATH += $$KDCHARTDIR/include/KDChart
    DEPENDPATH += $$KDCHARTDIR/include/KDChart

    CONFIG += have_kdchart
    DEFINES += HAVE_KDCHART

  } else:equals( WARN_IF_NOT_FOUND, "1" ) {
    message( "WARNING: kdchart not found. Please set KDCHARTDIR either as an environment variable or on the qmake command line if you want kdchart support")
  }
