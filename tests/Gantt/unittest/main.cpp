/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#include "../../../src/KDGantt/unittest/testregistry.h"

#include <QApplication>
#include <iostream>

int main(int argc, char **argv)
{

    QApplication app(argc, argv);

    KDAB::UnitTest::Runner r;
    unsigned int failed = 0;
    if (argc == 1)
        failed = r.run();
    else {
        for (int i = 1; i < argc; ++i)
            if (argv[i] && *argv[i])
                failed += r.run(argv[i]);
            else {
                std::cerr << argv[0] << ": skipping empty group name" << std::endl;
            }
    }
    std::cout << failed << " tests failed." << std::endl;
    return failed;
}
