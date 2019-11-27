set(BINDING_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
	DESTINATION ${INSTALL_PYTHON_SITE_PACKAGES}/${PYKDCHART_MODULE_NAME})

#intall on build dir to allow import without install
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
    ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
    COPYONLY
)

set(PY_QT_API_VERSION "${Qt5Core_VERSION_MAJOR}.${Qt5Core_VERSION_MINOR}")
add_subdirectory(KDGantt)
