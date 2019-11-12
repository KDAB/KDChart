project(PyKdChart)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

# Python
set(Python_ADDITIONAL_VERSIONS 3.6)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
execute_process (
	COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
       	OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

set(PYKDCHART_VERSION_MAJOR "2")
set(PYKDCHART_VERSION_MINOR "6")
set(PYKDCHART_VERSION_PATCH "1")
set(PYKDCHART_VERSION "${PYKDCHART_VERSION_MAJOR}.${PYKDCHART_VERSION_MINOR}.${PYKDCHART_VERSION_PATCH}")
set(PYKDCHART_VERSION_STRING "${FATCRM_VERSION}")
set(PYKDCHART_MODULE_NAME "PyKDChart")

# PySide
# Need same version  as c++ Qt library
find_package(PySide2 EXACT ${Qt5Core_VERSION} REQUIRED)
include(PySide2Tools)

add_subdirectory(PyKDChart)
