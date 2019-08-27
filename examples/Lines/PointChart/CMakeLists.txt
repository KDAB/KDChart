add_executable(LinesPointChart-example LineChart.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(LinesPointChart-example ${QT_LIBRARIES} kdchart testtools)
