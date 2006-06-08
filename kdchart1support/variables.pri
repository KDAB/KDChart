CONFIG += qt warn_on

# Note: The following KDCHART_MASTER_CVS definition is used to distinguish
#       between Klaraelvdalens Datakonsult AB Master CVS and other CVS
#       repositories holding copies of the sources that might have to
#       use special statements like include "KDFrame.moc" in the code.

DEFINES = USE_EXCEPTIONS KDCHART_VERSION=199 KDCHART_MASTER_CVS QT_FATAL_ASSERT

solaris-cc*:DEFINES += SUN7
macx-g++*:TMAKE_CXXFLAGS = -fexceptions
macx-g++*:LIBS += -lz

win32-msvc:TMAKE_CXXFLAGS += -GX

#TMAKE_CXX=codewizard
#TMAKE_LINK=codewizard
#linux-g++:TMAKE_CXX=insure
#linux-g++:TMAKE_LINK=insure

linux-g++*:QMAKE_CXXFLAGS += -pedantic -Wno-long-long

CONFIG += depend_includepath

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  UI_DIR= .ui
}
!unix {
  MOC_DIR = _moc
  OBJECTS_DIR = _obj
  UI_DIR= _ui
}

# If CONFIG += qsa is set, KDChart will be compiled for QSA
qsa {
  load(qsa)
  DEFINES += QSA
  QMAKE_CXXFLAGS_DEBUG= -g3
}
