add_executable(Ganttapireview-manual-test entrydelegate.cpp entrydialog.cpp entrydialog.ui main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(Ganttapireview-manual-test ${QT_LIBRARIES} kdchart testtools)
