KDCHART_PATH = ../..

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui

INCLUDEPATH += $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools
DEPENDPATH += $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools

LIBS        += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB -l$TESTTOOLSLIB
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




