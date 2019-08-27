add_executable(LinesAdvanced-example LineChart.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(LinesAdvanced-example ${QT_LIBRARIES} kdchart testtools)
