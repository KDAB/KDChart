add_executable(Cloning-test main.cpp)
target_link_libraries(Cloning-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME Cloning-test COMMAND Cloning-test)
