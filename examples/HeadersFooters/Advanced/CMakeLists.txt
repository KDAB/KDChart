add_executable(HeadersFootersAdvanced-example HeadersFooters.qrc addheaderdialog.ui main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(HeadersFootersAdvanced-example ${QT_LIBRARIES} kdchart testtools)
