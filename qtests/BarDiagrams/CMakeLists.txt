add_executable(BarDiagrams-test Data.qrc main.cpp)
target_link_libraries(BarDiagrams-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME BarDiagrams-test COMMAND BarDiagrams-test)
