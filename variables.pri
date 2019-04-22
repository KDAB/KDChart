CONFIG += qt warn_on

exists( g++.pri ):include( g++.pri )

# Note: The following KDCHART_MASTER_CVS definition is used to distinguish
#       between Klaraelvdalens Datakonsult AB Master CVS and other CVS
#       repositories holding copies of the sources that might have to
#       use special statements like include "KDFrame.moc" in the code.

DEFINES += USE_EXCEPTIONS KDCHART_VERSION=200 KDCHART_MASTER_CVS QT_FATAL_ASSERT

DEFINES += QT_NO_STL QT_NO_CAST_TO_ASCII QBA_NO_CAST_TO_VOID QBA_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

solaris-cc:DEFINES += SUN7

#  NOMINMAX avoids conflicting definitions of min and max
#  _USE_MATH_DEFINES makes M_PI defined
win32 {
!win32-g++ {
    DEFINES += NOMINMAX _USE_MATH_DEFINES
    QMAKE_CXXFLAGS += /GR /EHsc /wd4251
}
}
CONFIG += depend_includepath

contains(TEMPLATE, lib) {
  DESTDIR = $${TOP_BUILD_DIR}/lib
}
contains(TEMPLATE, app) {
  DESTDIR = $${TOP_BUILD_DIR}/bin
}

CONFIG( static ) {
  DEFINES += KDCHART_STATICLIB
} else {
  DEFINES += KDCHART_SHAREDLIB
  win32 {
    DLLDESTDIR = $$PWD/bin
    CONFIG += dll
  }
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  UI_DIR = .ui
  RCC_DIR = .qrc
}
!unix {
  debug:MODE=debug
  release:MODE=release
  MOC_DIR = $${MODE}/_moc
  OBJECTS_DIR = $${MODE}/_obj
  UI_DIR = $${MODE}/_ui
  RCC_DIR = _qrc
}
