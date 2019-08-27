add_executable(Measure-test Data.qrc main.cpp)
target_link_libraries(Measure-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME Measure-test COMMAND Measure-test)
