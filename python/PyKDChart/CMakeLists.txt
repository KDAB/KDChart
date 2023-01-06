#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

set(BINDING_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(KDGantt)

# Make module import from build dir work
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.cmake ${CMAKE_CURRENT_BINARY_DIR}/__init__.py @ONLY)

# install
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py $<TARGET_FILE:KDAB::kdchart>
    DESTINATION ${${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX}
)
if(NOT WIN32)
    install(FILES $<TARGET_LINKER_FILE:KDAB::kdchart> $<TARGET_SONAME_FILE:KDAB::kdchart>
            DESTINATION ${${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX}
    )
endif()
