
#ifndef IGNIS_IMAPPERMODULETEST_H
#define IGNIS_IMAPPERMODULETEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMapperModuleTest : public CPPUNIT_NS::TestCase {

                CPPUNIT_TEST_SUITE(IMapperModuleTest);
                        //CPPUNIT_TEST();
                    CPPUNIT_TEST_SUITE_END();

                    virtual void setUp();

                    virtual void tearDown();

                };
            }
        }
    }
}

#endif
