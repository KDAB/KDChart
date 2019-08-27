add_executable(PolarDiagrams-test Data.qrc main.cpp)
target_link_libraries(PolarDiagrams-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME PolarDiagrams-test COMMAND PolarDiagrams-test)
