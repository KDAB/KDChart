##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    ChartElementOwnership-test
    main.cpp
)
target_link_libraries(
    ChartElementOwnership-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME ChartElementOwnership-test COMMAND ChartElementOwnership-test)
