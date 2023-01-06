##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2021-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

# Doxygen
find_package(Doxygen)
set_package_properties(
    Doxygen PROPERTIES
    TYPE OPTIONAL
    DESCRIPTION "API Documentation system"
    URL "https://www.doxygen.org"
    PURPOSE "Needed to build the API documentation."
)

if(DOXYGEN_FOUND)
    add_subdirectory(api)
endif()
