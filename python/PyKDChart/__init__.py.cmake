#
# This file is part of KDChart.
#
# SPDX-FileCopyrightText: 2019-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
# Author: Renato Araujo Oliveira Filho <renato.araujo@kdab.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#

import sys

__all__ = ['KDChart']

# Preload PySide2 libraries to avoid missing libraries while loading KDChart
try:
    from PySide@PYSIDE_MAJOR_VERSION@ import QtCore
    # Create a alias for PySide module so we can use a single import in source files
    import PySide@PYSIDE_MAJOR_VERSION@
    sys.modules["PySide"] = PySide@PYSIDE_MAJOR_VERSION@
except Exception:
    print("Failed to load PySide")
    raise
