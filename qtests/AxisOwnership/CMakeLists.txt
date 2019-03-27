add_executable(AxisOwnership-test Data.qrc main.cpp)
target_link_libraries(AxisOwnership-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME AxisOwnership-test COMMAND AxisOwnership-test)
