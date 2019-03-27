add_executable(ChartElementOwnership-test main.cpp)
target_link_libraries(ChartElementOwnership-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME ChartElementOwnership-test COMMAND ChartElementOwnership-test)
