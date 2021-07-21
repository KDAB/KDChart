set(BINDING_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(PYSIDE_MAJOR_VERSION "2")

add_subdirectory(KDGantt)

# Make module import from build dir work
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.cmake ${CMAKE_CURRENT_BINARY_DIR}/__init__.py @ONLY)

# install
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
    DESTINATION ${${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX}/PyKDChart)
