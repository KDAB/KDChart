# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    Palette-test
    main.cpp
)
target_link_libraries(
    Palette-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME Palette-test COMMAND Palette-test)
