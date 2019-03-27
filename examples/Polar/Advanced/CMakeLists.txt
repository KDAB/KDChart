add_executable(PolarAdvanced-example main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(PolarAdvanced-example ${QT_LIBRARIES} kdchart testtools)
