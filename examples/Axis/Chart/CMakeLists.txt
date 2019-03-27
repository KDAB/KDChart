add_executable(AxisChart-example BarChart.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(AxisChart-example ${QT_LIBRARIES} kdchart testtools)
