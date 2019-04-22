include( $${TOP_SOURCE_DIR}/examples/examples.pri )

SOURCES= main.cpp \
    mainwindow.cpp \
    datasetsettings.cpp \
    gradientdialog.cpp \
    colorslider.cpp \
    diagramsettings.cpp \
    datavaluesettings.cpp \
    diagramtypedialog.cpp \
    axissettings.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )

HEADERS += \
    mainwindow.h \
    datasetsettings.h \
    gradientdialog.h \
    colorslider.h \
    diagramsettings.h \
    datavaluesettings.h \
    diagramtypedialog.h \
    axissettings.h

RESOURCES += \
    demo.qrc

FORMS += \
    datasetsettings.ui \
    gradientdialog.ui \
    diagramsettings.ui \
    datavaluesettings.ui \
    diagramtypedialog.ui \
    axissettings.ui
