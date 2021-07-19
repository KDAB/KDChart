find_package(Qt5Sql REQUIRED)
set(QT_LIBRARIES Qt5::Sql)

add_executable(Sql-example main.cpp)
target_link_libraries(Sql-example ${QT_LIBRARIES} kdchart testtools)
