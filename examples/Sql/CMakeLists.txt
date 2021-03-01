
find_package(Qt6 COMPONENTS Sql REQUIRED)

add_executable(Sql-example main.cpp)
target_link_libraries(Sql-example ${QT_LIBRARIES} Qt6::Sql kdchart testtools)
