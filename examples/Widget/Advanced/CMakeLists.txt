add_executable(WidgetAdvanced-example main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(WidgetAdvanced-example ${QT_LIBRARIES} kdchart testtools)
