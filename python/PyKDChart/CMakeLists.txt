set(BINDING_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
  DESTINATION ${${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX}/${PYKDCHART_MODULE_NAME})

#install on build dir to allow import without install
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
  ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
  COPYONLY
)

add_subdirectory(KDGantt)
