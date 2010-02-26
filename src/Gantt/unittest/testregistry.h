#ifndef __KDAB__UNITTEST__TESTREGISTRY_H__
#define __KDAB__UNITTEST__TESTREGISTRY_H__

#ifndef KDAB_NO_UNIT_TESTS

#include "../../kdchart_export.h"
#include "../kdganttglobal.h"

#include <string>
#include <map>
#include <vector>
#include <cassert>

namespace KDAB {
namespace UnitTest {

    class Test;
    class TestFactory;

    class KDCHART_EXPORT TestRegistry {
        friend class ::KDAB::UnitTest::TestFactory;
        static TestRegistry * mSelf;
        TestRegistry();
        ~TestRegistry();
    public:
        static TestRegistry * instance();
        static void deleteInstance();

        void registerTestFactory( const TestFactory * tf, const char * group );

        /*! runs all tests in all groups.
           @return the number of failed tests (if any) */
        unsigned int run() const;
        /*! runs only tests in group \a group
            @return the number of failed tests (if any) */
        unsigned int run( const char * group ) const;

    private:
        std::map< std::string, std::vector<const TestFactory*> > mTests;
    };

    class KDCHART_EXPORT Runner {
    public:
        ~Runner();
        unsigned int run( const char * group=0 ) const;
    };

}
}

#endif // KDAB_NO_UNIT_TESTS

#endif // __KDAB__UNITTEST__TESTREGISTRY_H__
