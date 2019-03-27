add_executable(AxisParameters-example Axes.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(AxisParameters-example ${QT_LIBRARIES} kdchart testtools)
