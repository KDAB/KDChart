add_executable(ZoomScrollBars-example ZoomScrollBars.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(ZoomScrollBars-example ${QT_LIBRARIES} kdchart testtools)
