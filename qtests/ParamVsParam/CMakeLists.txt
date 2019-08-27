add_executable(ParamVsParam-test ModelParamVsParam.cpp ModelParamVsParamPlot.cpp main.cpp mainwindow.cpp)
target_link_libraries(ParamVsParam-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME ParamVsParam-test COMMAND ParamVsParam-test)
