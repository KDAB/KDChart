add_executable(CartesianPlanes-test Data.qrc main.cpp)
target_link_libraries(CartesianPlanes-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME CartesianPlanes-test COMMAND CartesianPlanes-test)
