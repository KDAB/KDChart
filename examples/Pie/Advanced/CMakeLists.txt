add_executable(PieAdvanced-example Pie.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(PieAdvanced-example ${QT_LIBRARIES} kdchart testtools)
