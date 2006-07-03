KDCHART_PATH = ../..


# Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
# to avoid name clashes between debug/non-debug versions of the
# KD Chart library:
KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}


HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
RESOURCES = BarChart.qrc

INCLUDEPATH += \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools
DEPENDPATH += \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools

LIBS        += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB -ltesttools
message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )



unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  UI_DIR = .ui
}
!unix {
  MOC_DIR = _moc
  OBJECTS_DIR = _obj
  UI_DIR = _ui
}

