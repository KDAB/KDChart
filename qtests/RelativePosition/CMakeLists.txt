add_executable(RelativePosition-test main.cpp)
target_link_libraries(RelativePosition-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME RelativePosition-test COMMAND RelativePosition-test)
