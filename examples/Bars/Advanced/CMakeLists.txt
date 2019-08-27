add_executable(BarsAdvanced-example BarChart.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(BarsAdvanced-example ${QT_LIBRARIES} kdchart testtools)
