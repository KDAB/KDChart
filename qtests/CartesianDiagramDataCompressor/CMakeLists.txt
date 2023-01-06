##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    CartesianDiagramDataCompressor-test
    CartesianDiagramDataCompressorTests.cpp
)
target_link_libraries(
    CartesianDiagramDataCompressor-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME CartesianDiagramDataCompressor-test COMMAND CartesianDiagramDataCompressor-test)
