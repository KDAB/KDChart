add_executable(ModelViewTableView-example ModelView.qrc main.cpp mainwindow.cpp)
target_link_libraries(ModelViewTableView-example ${QT_LIBRARIES} kdchart testtools)
