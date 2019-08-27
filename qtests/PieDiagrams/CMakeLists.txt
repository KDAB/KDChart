add_executable(PieDiagrams-test Data.qrc main.cpp)
target_link_libraries(PieDiagrams-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME PieDiagrams-test COMMAND PieDiagrams-test)
