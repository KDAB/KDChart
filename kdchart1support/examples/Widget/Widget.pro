FORMS   = mainwindow.ui
SOURCES = main.cpp mainwindow.cpp
HEADERS = mainwindow.h

QT += xml

KDCHART_PATH        = ../../..
KDCHART_COMPAT_PATH = ../..

load(kdchart)

LIBS += -L$$(KDCHART_PATH)/lib -lkdchart

INCLUDEPATH += $$KDCHART_PATH/include \
               $$KDCHART_COMPAT_PATH/include 
DEPENDPATH  += $$KDCHART_PATH/include \
               $$KDCHART_COMPAT_PATH/include


unix:LIBS  += -L$$KDCHART_PATH/lib -lkdchart1support
win32:LIBS += $$KDCHART_PATH\lib\kdchart1support.lib


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
