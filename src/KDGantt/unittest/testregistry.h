/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef __KDAB__UNITTEST__TESTREGISTRY_H__
#define __KDAB__UNITTEST__TESTREGISTRY_H__

#ifndef KDAB_NO_UNIT_TESTS

#include "../kdganttglobal.h"

#include <string>
#include <map>
#include <vector>
#include <cassert>

namespace KDAB {
namespace UnitTest {

    class Test;
    class TestFactory;

    class KDGANTT_EXPORT TestRegistry
    {
        friend class ::KDAB::UnitTest::TestFactory;
        static TestRegistry *mSelf;
        TestRegistry();
        ~TestRegistry();

    public:
        static TestRegistry *instance();
        static void deleteInstance();

        void registerTestFactory(const TestFactory *tf, const char *group);

        /*! runs all tests in all groups.
           @return the number of failed tests (if any) */
        unsigned int run() const;
        /*! runs only tests in group \a group
            @return the number of failed tests (if any) */
        unsigned int run(const char *group) const;

    private:
        std::map<std::string, std::vector<const TestFactory *>> mTests;
    };

    class KDGANTT_EXPORT Runner
    {
    public:
        ~Runner();
        unsigned int run(const char *group = nullptr) const;
    };
}
}

#endif // KDAB_NO_UNIT_TESTS

#endif // __KDAB__UNITTEST__TESTREGISTRY_H__
