add_executable(SharedAbscissaSeparateDiagrams-example SharedAbscissa.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(SharedAbscissaSeparateDiagrams-example ${QT_LIBRARIES} kdchart testtools)
