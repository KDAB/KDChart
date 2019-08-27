add_executable(Ganttlegend_example-example entrydelegate.cpp entrydialog.cpp entrydialog.ui main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(Ganttlegend_example-example ${QT_LIBRARIES} kdchart testtools)
