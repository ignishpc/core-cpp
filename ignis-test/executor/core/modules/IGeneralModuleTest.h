
#ifndef IGNIS_IGENERALMODULETEST_H
#define IGNIS_IGENERALMODULETEST_H

#include "IModuleTest.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {

                class IGeneralModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IGeneralModuleTest);
                        // CPPUNIT_TEST();
                    CPPUNIT_TEST_SUITE_END();
                public:
                };
            }
        }
    }
}

#include "IGeneralModuleTest.tcc"

#endif

