add_executable(ZoomKeyboard-example main.cpp mainwindow.cpp mainwindow.ui zoomwidget.cpp)
target_link_libraries(ZoomKeyboard-example ${QT_LIBRARIES} kdchart testtools)
