KDCHART_PATH = ../..

include( $$KDCHART_PATH/examples/examples.pri )

SOURCES= main.cpp

QT += svg

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
