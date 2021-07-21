
find_package(Python3 3.6 REQUIRED COMPONENTS Interpreter Development)
find_package(Shiboken2 REQUIRED)
find_package(PySide2 ${Qt5Widgets_VERSION} EXACT REQUIRED)
include(PySide2ModuleBuild)

add_subdirectory(PyKDChart)
