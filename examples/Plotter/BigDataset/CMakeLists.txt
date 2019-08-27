add_executable(PlotterBigDataset-example Controls.ui MainWidget.cpp Model.cpp main.cpp)
target_link_libraries(PlotterBigDataset-example ${QT_LIBRARIES} kdchart testtools)
