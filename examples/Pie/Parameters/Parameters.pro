SOURCES= main.cpp

TARGET = PieParameters

KDCHART_PATH = ../../..
INCLUDEPATH += \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools
DEPENDPATH += \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/examles/tools

LIBS        += -L$$KDCHART_PATH/lib -lkdchart -ltesttools

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

