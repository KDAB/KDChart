add_executable(QLayout-test main.cpp)
target_link_libraries(QLayout-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME QLayout-test COMMAND QLayout-test)
