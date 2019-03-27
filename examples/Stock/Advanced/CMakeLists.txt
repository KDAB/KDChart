add_executable(StockAdvanced-example Resources.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(StockAdvanced-example ${QT_LIBRARIES} kdchart testtools)
