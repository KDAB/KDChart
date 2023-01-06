##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2021-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

# dot should come with Doxygen find_package(Doxygen)
if(DOXYGEN_DOT_EXECUTABLE)
    set(HAVE_DOT "YES")
else()
    set(HAVE_DOT "NO")
    message(STATUS "Unable to provide inheritance diagrams for the API documentation. "
                   "To fix, install the graphviz project from https://www.graphviz.org"
    )
endif()

# qhelpgenerator
find_program(QHELPGEN_EXECUTABLE qhelpgenerator HINTS ${QT_INSTALL_BINS})
if(QHELPGEN_EXECUTABLE)
    set(HAVE_QHELPGEN "YES")
else()
    set(HAVE_QHELPGEN "NO")
    message(STATUS "Unable to generate the API documentation in qch format. "
                   "To fix, install the qthelpgenerator program which comes with Qt."
    )
endif()

find_file(QDOC_QTCORE_TAG qtcore.tags HINTS ${QT_INSTALL_DOCS}/qtcore ${QT_INSTALL_DATA}/doc/qtcore)
set(QDOC_TAG_DIR "<QDOC_TAG_DIR_not_found>")
if(QDOC_QTCORE_TAG)
    get_filename_component(QDOC_TAG_DIR ${QDOC_QTCORE_TAG} DIRECTORY)
    get_filename_component(QDOC_TAG_DIR ${QDOC_TAG_DIR} DIRECTORY)
endif()

file(GLOB _dox_deps *.dox *.html)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

#apidox generation using doxygen
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT_DIR}/qch/kdchart-api.qch
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    #copy some files by-hand that are referred to by the markdown README
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/INSTALL.txt ${DOXYGEN_OUTPUT_DIR}/html
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/LICENSES ${DOXYGEN_OUTPUT_DIR}/html/LICENSES
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/examples ${DOXYGEN_OUTPUT_DIR}/html/examples
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIR}/html/docs
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIR}/html/docs/manual
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/docs/manual/kdchart.pdf
            ${DOXYGEN_OUTPUT_DIR}/html/docs/manual
    DEPENDS ${_dox_deps} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generate the .qch file"
)
add_custom_target(
    kdchart-api.qch ALL
    DEPENDS ${DOXYGEN_OUTPUT_DIR}/qch/kdchart-api.qch
    COMMENT "Target to generate the .qch file"
)
add_custom_target(
    docs
    DEPENDS kdchart-api.qch
    COMMENT "Target to generate the documentation"
)

set(QCH_INSTALL_DIR
    ${INSTALL_DOC_DIR}
    CACHE STRING "Install location of Qt Assistant help files."
)
install(
    FILES ${DOXYGEN_OUTPUT_DIR}/qch/kdchart-api.qch
    DESTINATION ${QCH_INSTALL_DIR}
)
install(
    FILES ${DOXYGEN_OUTPUT_DIR}/kdchart.tags
    DESTINATION ${INSTALL_DOC_DIR}
)
