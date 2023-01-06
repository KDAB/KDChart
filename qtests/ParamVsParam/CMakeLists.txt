##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    ParamVsParam-test
    main.cpp mainwindow.cpp ModelParamVsParam.cpp ModelParamVsParamPlot.cpp
)
target_link_libraries(
    ParamVsParam-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME ParamVsParam-test COMMAND ParamVsParam-test)
