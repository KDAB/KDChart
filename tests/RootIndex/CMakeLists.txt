add_executable(RootIndex-manual-test main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(RootIndex-manual-test ${QT_LIBRARIES} kdchart testtools)
