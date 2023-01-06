##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

if(NOT ${PROJECT_NAME}_QT6)
    install(FILES kdchart.prf DESTINATION ${INSTALL_DATADIR}/mkspecs/features)
endif()
