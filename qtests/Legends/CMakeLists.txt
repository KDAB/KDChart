add_executable(Legends-test main.cpp)
target_link_libraries(Legends-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME Legends-test COMMAND Legends-test)
