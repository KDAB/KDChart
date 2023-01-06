##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    Cloning-test
    main.cpp
)
target_link_libraries(
    Cloning-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME Cloning-test COMMAND Cloning-test)
