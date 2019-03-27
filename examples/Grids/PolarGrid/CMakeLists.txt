add_executable(GridsPolarGrid-example main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(GridsPolarGrid-example ${QT_LIBRARIES} kdchart testtools)
