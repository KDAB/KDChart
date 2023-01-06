#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#


import sys
import os

__all__ = ['KDChart']

def setupLibraryPath():
    if sys.platform != 'win32':
        return

    if "@PYSIDE_MAJOR_VERSION@" == "6":
        from shiboken@PYSIDE_MAJOR_VERSION@ import Shiboken
    else:
        from shiboken@PYSIDE_MAJOR_VERSION@ import shiboken@PYSIDE_MAJOR_VERSION@ as Shiboken

    from PySide@PYSIDE_MAJOR_VERSION@ import QtCore
    extra_dll_dirs = [ os.path.abspath(os.path.dirname(Shiboken.__file__)),
                       os.path.abspath(os.path.dirname(QtCore.__file__)),
                       os.path.abspath(os.path.dirname(__file__)) ]

    if sys.version_info[0] == 3 and sys.version_info[1] >= 8:
        for dll_dir in extra_dll_dirs:
            os.add_dll_directory(dll_dir)

    for dll_dir in extra_dll_dirs:
        os.environ['PATH'] = os.fspath(dll_dir) + os.pathsep + os.environ['PATH']

# Preload PySide libraries to avoid missing libraries while loading KDChart
try:
    from PySide@PYSIDE_MAJOR_VERSION@ import QtCore
    # Create a alias for PySide module so we can use a single import in source files
    import PySide@PYSIDE_MAJOR_VERSION@
    sys.modules["PySide"] = PySide@PYSIDE_MAJOR_VERSION@
except Exception:
    print("Failed to load PySide")
    raise

setupLibraryPath()
