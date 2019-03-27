add_executable(EmptyValues-example BarChart.qrc main.cpp mainwindow.cpp)
target_link_libraries(EmptyValues-example ${QT_LIBRARIES} kdchart testtools)
