##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    CartesianPlanes-test
    Data.qrc main.cpp
)
target_link_libraries(
    CartesianPlanes-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME CartesianPlanes-test COMMAND CartesianPlanes-test)
