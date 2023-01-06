##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    Measure-test
    Data.qrc main.cpp
)
target_link_libraries(
    Measure-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME Measure-test COMMAND Measure-test)
