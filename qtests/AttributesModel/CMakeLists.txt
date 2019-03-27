add_executable(AttributesModel-test Data.qrc main.cpp)
target_link_libraries(AttributesModel-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME AttributesModel-test COMMAND AttributesModel-test)
