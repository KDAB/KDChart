include( $${TOP_SOURCE_DIR}/examples/examples.pri )

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui
QT    += svg

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  UI_DIR = .ui
}
!unix {
  debug:MODE=debug
  release:MODE=release
  MOC_DIR = $${MODE}/_moc
  OBJECTS_DIR = $${MODE}/_obj
  UI_DIR = $${MODE}/_ui
}




