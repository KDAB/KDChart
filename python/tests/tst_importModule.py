## This file is part of the KD Chart library.
##
## SPDX-FileCopyrightText: 2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
##
## SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
##
## Licensees holding valid commercial KD Chart licenses may use this file in
## accordance with the KD Chart Commercial License Agreement provided with
## the Software.
##
## Contact info@kdab.com if any conditions of this licensing are not
## clear to you.
##

import unittest
import importlib
import inspect

from config import TstConfig

class TestImportModules(unittest.TestCase):
    def test_importModules(self):
        m = importlib.import_module(TstConfig.bindingsNamespace + '.KDGantt')
        moduleSymbols = []
        for t in inspect.getmembers(m):
            moduleSymbols.append(t[0])

        symbols = ['AbstractGrid', 'EndTimeRole', 'Span', 'TreeViewRowController']
        for symbol in symbols:
            self.assertIn(symbol, moduleSymbols)

if __name__ == '__main__':
    TstConfig.initLibraryPath()
    unittest.main()
