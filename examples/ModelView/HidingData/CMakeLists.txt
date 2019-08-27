add_executable(ModelViewHidingData-example ModelView.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(ModelViewHidingData-example ${QT_LIBRARIES} kdchart testtools)
