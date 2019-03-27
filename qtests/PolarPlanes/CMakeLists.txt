add_executable(PolarPlanes-test Data.qrc main.cpp)
target_link_libraries(PolarPlanes-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME PolarPlanes-test COMMAND PolarPlanes-test)
