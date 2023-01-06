/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
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
