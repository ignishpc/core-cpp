
#ifndef IGNIS_IKEYSMODULETEST_H
#define IGNIS_IKEYSMODULETEST_H

#include "IModuleTest.h"
#include "../../../../ignis/executor/core/modules/IKeysModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IKeysModuleTest : public IModuleTest{
                CPPUNIT_TEST_SUITE(IKeysModuleTest);
                        CPPUNIT_TEST(reduceByKeyTest);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    virtual void setUp();

                    virtual void reduceByKeyTest();

                    virtual void tearDown();

                private:
                    std::shared_ptr<IKeysModule> keys_module;
                };
            }
        }
    }
}

#endif
