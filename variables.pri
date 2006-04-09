CONFIG += qt warn_on zlib debug

exists( g++.pri):include( g++.pri )

# Note: The following KDCHART_MASTER_CVS definition is used to distinguish
#       between Klaraelvdalens Datakonsult AB Master CVS and other CVS
#       repositories holding copies of the sources that might have to
#       use special statements like include "KDFrame.moc" in the code.

DEFINES += USE_EXCEPTIONS KDCHART_VERSION=200 KDCHART_MASTER_CVS QT_FATAL_ASSERT

QT += xml

solaris-cc:DEFINES += SUN7

#  NOMINMAX avoids conflicting definitions of min and max
#  _USE_MATH_DEFINES makes M_PI defined
win32:DEFINES += NOMINMAX _USE_MATH_DEFINES

linux-g++:QMAKE_CXXFLAGS += -pedantic -Wno-long-long

CONFIG += depend_includepath

# If CONFIG += qsa is set, KDChart will be compiled for QSA
qsa {
  load(qsa)
  DEFINES += QSA
}

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
