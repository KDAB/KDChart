# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    BarsSimple-example
    main.cpp
)
target_link_libraries(
    BarsSimple-example ${QT_LIBRARIES} kdchart testtools
)
