#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

set(DEMO_SOURCES
    axissettings.cpp
    axissettings.ui
    colorslider.cpp
    datasetsettings.cpp
    datasetsettings.ui
    datavaluesettings.cpp
    datavaluesettings.ui
    demo.qrc
    diagramsettings.cpp
    diagramsettings.ui
    diagramtypedialog.cpp
    diagramtypedialog.ui
    gradientdialog.cpp
    gradientdialog.ui
    main.cpp
    mainwindow.cpp
)
if(EMSCRIPTEN)
    qt_add_executable(demo-example ${DEMO_SOURCES})
    # Output wasm file to same location as .html
    # file created by qt_add_executable
    set_target_properties(
        demo-example
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ""
                   RUNTIME_OUTPUT_DIRECTORY_RELEASE ""
                   RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ""
                   RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ""
    )
else()
    add_executable(demo-example ${DEMO_SOURCES})
endif()
target_link_libraries(
    demo-example
    PUBLIC ${QT_LIBRARIES} kdchart testtools
)
