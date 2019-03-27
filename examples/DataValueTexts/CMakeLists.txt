add_executable(DataValueTexts-example DataValueTexts.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(DataValueTexts-example ${QT_LIBRARIES} kdchart testtools)
