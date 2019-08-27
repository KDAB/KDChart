add_executable(PlotterBubbleChart-example main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(PlotterBubbleChart-example ${QT_LIBRARIES} kdchart testtools)
