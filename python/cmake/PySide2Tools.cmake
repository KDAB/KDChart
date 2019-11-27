if(WIN32)
    set(PATH_SEP "\;")
else()
    set(PATH_SEP ":")
endif()
set(GENERATOR_EXTRA_FLAGS --generator-set=shiboken
                          --enable-parent-ctor-heuristic
                          --enable-pyside-extensions
                          --enable-return-value-heuristic
                          --use-isnull-as-nb_nonzero
                          -std=c++${CMAKE_CXX_STANDARD}
)

# 2017-04-24 The protected hack can unfortunately not be disabled, because
# Clang does produce linker errors when we disable the hack.
# But the ugly workaround in Python is replaced by a shiboken change.
if(WIN32 OR DEFINED AVOID_PROTECTED_HACK)
    message(STATUS "PySide2 will be generated avoiding the protected hack!")
    set(GENERATOR_EXTRA_FLAGS ${GENERATOR_EXTRA_FLAGS} --avoid-protected-hack)
    add_definitions(-DAVOID_PROTECTED_HACK)
else()
    message(STATUS "PySide will be generated using the protected hack!")
endif()

macro(make_path varname)
    # accepts any number of path variables
    string(REPLACE ";" "${PATH_SEP}" ${varname} "${ARGN}")
endmacro()

list(GET Qt5Core_INCLUDE_DIRS 0 QT_INCLUDE_DIR)
# On macOS, check if Qt is a framework build. This affects how include paths should be handled.
get_target_property(QtCore_is_framework Qt5::Core FRAMEWORK)

if(QtCore_is_framework)
    # Get the path to the framework dir.
    get_filename_component(QT_FRAMEWORK_INCLUDE_DIR "${QT_INCLUDE_DIR}/../" ABSOLUTE)

    # QT_INCLUDE_DIR points to the QtCore.framework directory, so we need to adjust this to point
    # to the actual include directory, which has include files for non-framework parts of Qt.
    get_filename_component(QT_INCLUDE_DIR "${QT_INCLUDE_DIR}/../../include" ABSOLUTE)
endif()

macro(CREATE_PYTHON_BINDINGS
    LIBRARY_NAME
    TYPESYSTEM_PATHS
    INCLUDE_PATHS
    OUTPUT_SOURCES
    TARGET_INCLUDE_DIRS
    TARGET_LINK_LIBRARIES
    GLOBAL_INCLUDE
    TYPESYSTEM_XML
    DEPENDS
        OUTPUT_DIR)

    # Transform the path separators into something shiboken understands.
    make_path(shiboken_include_dirs ${INCLUDE_PATHS})
    make_path(shiboken_typesystem_dirs ${TYPESYSTEM_PATHS})
    get_property(raw_python_dir_include_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
    make_path(python_dir_include_dirs ${raw_python_dir_include_dirs})
    set(shiboken_include_dirs "${shiboken_include_dirs}${PATH_SEP}${python_dir_include_dirs}")

    set(shiboken_framework_include_dirs_option "")
    if(CMAKE_HOST_APPLE)
        set(shiboken_framework_include_dirs "${QT_FRAMEWORK_INCLUDE_DIR}")
        make_path(shiboken_framework_include_dirs ${shiboken_framework_include_dirs})
        set(shiboken_framework_include_dirs_option "--framework-include-paths=${shiboken_framework_include_dirs}")
    endif()

    set_property(SOURCE ${OUTPUT_SOURCES} PROPERTY SKIP_AUTOGEN ON)
    add_custom_command(OUTPUT ${OUTPUT_SOURCES}
        COMMAND ${SHIBOKEN_BINARY} ${GENERATOR_EXTRA_FLAGS} ${GLOBAL_INCLUDE}
            --include-paths=${shiboken_include_dirs}
            --typesystem-paths=${shiboken_typesystem_dirs}
            ${shiboken_framework_include_dirs_option}
            --output-directory=${CMAKE_CURRENT_BINARY_DIR}
            ${TYPESYSTEM_XML}
            --api-version="5.12"
        DEPENDS ${TYPESYSTEM_XML} ${DEPENDS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running generator for ${LIBRARY_NAME} binding..."
    )

    set(TARGET_NAME "Py${LIBRARY_NAME}")
    set(MODULE_NAME "${LIBRARY_NAME}")
    add_library(${TARGET_NAME} MODULE ${OUTPUT_SOURCES})
    set_target_properties(${TARGET_NAME} PROPERTIES
        PREFIX ""
        OUTPUT_NAME ${MODULE_NAME}
        LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/
    )

    if(WIN32)
        set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".pyd")
        get_filename_component(PYTHON3_PATH_LIB ${PYTHON_LIBRARIES} DIRECTORY)
        set(PYTHON_WINDOWS_LIBRARIES ${PYTHON3_PATH_LIB}/python3.lib)
    else()
        set(PYTHON_WINDOWS_LIBRARIES "")
    endif()

    target_include_directories(${TARGET_NAME} PUBLIC
        ${TARGET_INCLUDE_DIRS}
        ${SHIBOKEN_INCLUDE_DIR}
        ${PYSIDE_INCLUDE_DIR}
        ${PYSIDE_EXTRA_INCLUDES}
        ${PYTHON_INCLUDE_DIRS}
    )

    target_link_libraries(${TARGET_NAME}
        ${PYTHON_LIBRARIES}
        ${PYTHON_WINDOWS_LIBRARIES}
        ${TARGET_LINK_LIBRARIES}
        ${SHIBOKEN_LIBRARY}
        ${PYSIDE_LIBRARY}
    )

    target_compile_definitions(${TARGET_NAME}
        PRIVATE Py_LIMITED_API=0x03050000
    )

    install(TARGETS ${TARGET_NAME}
        LIBRARY DESTINATION ${INSTALL_PYTHON_SITE_PACKAGES}/${PYKDCHART_MODULE_NAME}
    )

    if(APPLE)
        set_property(TARGET ${TARGET_NAME} APPEND PROPERTY
            LINK_FLAGS "-undefined dynamic_lookup")
    endif()
endmacro()

