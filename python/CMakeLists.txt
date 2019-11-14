project(PyKdChart)

set(PYKDCHART_VERSION_MAJOR ${KDChart_VERSION_MAJOR})
set(PYKDCHART_VERSION_MINOR ${KDChart_VERSION_MAJOR})
set(PYKDCHART_VERSION_PATCH ${KDChart_VERSION_MAJOR})
set(PYKDCHART_VERSION "${PYKDCHART_VERSION_MAJOR}.${PYKDCHART_VERSION_MINOR}.${PYKDCHART_VERSION_PATCH}")
set(PYKDCHART_VERSION_STRING "${FATCRM_VERSION}")
set(PYKDCHART_MODULE_NAME "PyKDChart")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
set(BUILD_PYTHON_BINDINGS True)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

# Python
set(Python_ADDITIONAL_VERSIONS 3.6)
find_package(PythonInterp 3.6 QUIET)

if (PYTHONINTERP_FOUND)
    find_package(PythonLibs QUIET)
    if (PYTHONLIBS_FOUND)
        # PySide
        # Need same version  as c++ Qt library
	find_package(PySide2 EXACT ${Qt5Core_VERSION} QUIET)
        if (PySide2_FOUND)
            include(PySide2Tools)
            add_subdirectory(PyKDChart)
        else()
            message(WARNING "** PySide2 required libraries not found! Disabling build for bindings.")
        endif()
    else()
        message(WARNING "** Python libraries not found! Disabling build for bindings.")
    endif()
else()
    message(WARNING "** Python executable not found! Disabling build for bindings.")
endif()






