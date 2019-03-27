add_executable(LineDiagrams-test Data.qrc main.cpp)
target_link_libraries(LineDiagrams-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME LineDiagrams-test COMMAND LineDiagrams-test)
