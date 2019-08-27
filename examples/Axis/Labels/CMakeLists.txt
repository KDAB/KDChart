add_executable(AxisLabels-example AdjustedCartesianAxis.cpp BarChart.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(AxisLabels-example ${QT_LIBRARIES} kdchart testtools)
