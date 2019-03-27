add_executable(WidgetElementOwnership-test main.cpp)
target_link_libraries(WidgetElementOwnership-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME WidgetElementOwnership-test COMMAND WidgetElementOwnership-test)
