include( ../../variables.pri )

QT += xml

TEMPLATE	= app
HEADERS         = KDChart1_Presentation.h
SOURCES         = KDChart1_Presentation.cpp \
                  main.cpp
TARGET          = KDChart1_Presentation
INTERFACES	=
INCLUDEPATH += ../include \
               ../../include

load(kdchart)

unix:LIBS += -L../../lib -lkdchart1support
win32:LIBS += ..\..\lib\kdchart1support.lib
linux-ipaq-g++:INCLUDEPATH += $(QPEDIR)/library
linux-ipaq-g++:LIBS += -ljpeg -lqpe
DEPENDPATH += ../include
#linux-g++:TMAKE_CXX=insure
#linux-g++:TMAKE_LINK=insure

# To avoid a splash screen being shown, run qmake like this:
# qmake CONFIG+=no-splash -after CONFIG+=thread
!no-splash {
   DEFINES+=USE_SPLASH_SCREEN
}
# in case CONFIG+=dll was added to the qmake command line in the main folder
dll {
	CONFIG -= dll
}
