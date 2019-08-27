add_executable(Palette-test main.cpp)
target_link_libraries(Palette-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME Palette-test COMMAND Palette-test)
