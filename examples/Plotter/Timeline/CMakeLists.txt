add_executable(PlotterTimeline-example main.cpp timeaxis.cpp timechartmodel.cpp)
target_link_libraries(PlotterTimeline-example ${QT_LIBRARIES} kdchart testtools)
