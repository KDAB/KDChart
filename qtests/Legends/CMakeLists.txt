# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    Legends-test
    main.cpp
)
target_link_libraries(
    Legends-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME Legends-test COMMAND Legends-test)
