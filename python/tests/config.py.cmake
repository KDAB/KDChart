#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2021-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

import os
import sys

class TstConfig(object):
    bindingsNamespace = "PyKDChart"

    def initLibraryPath():
        if sys.platform == 'win32' and sys.version_info[0] == 3 and sys.version_info[1] >= 8:
            os.add_dll_directory("@CMAKE_RUNTIME_OUTPUT_DIRECTORY@")
