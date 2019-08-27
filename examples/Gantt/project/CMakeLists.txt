add_executable(Ganttproject-example main.cpp mainwindow.cpp projectmodel.cpp)
target_link_libraries(Ganttproject-example ${QT_LIBRARIES} kdchart testtools)
