KDCHARTDIR = $$(KDCHARTDIR)
isEmpty( KDCHARTDIR ){
    error( "You need to set the KDCHARTDIR environment variable to point to a KDChart installation" )
}
KDCHARTDIR = "$$KDCHARTDIR"
INCLUDEPATH += $$KDCHARTDIR/include
DEPENDPATH += $$KDCHARTDIR/include
LIBS += -L$$KDCHARTDIR/lib -lkdchart

TEMPLATE = app

SOURCES += main.cpp \
           kdchartdemo.cpp \
           kdbasicsourcepanel.cpp \
           tablemodel.cpp \
           kdabstractsourcepanel.cpp \

HEADERS += kdchartdemo.h \
           kdbasicsourcepanel.h \
           tablemodel.h \
           kdabstractsourcepanel.h \

FORMS = kdchartdemo.ui \
        kdbasicsourcepanel.ui \

CONFIG += warn_on \
          thread \
          qt

QT += gui xml
