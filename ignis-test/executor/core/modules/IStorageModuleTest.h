
#ifndef IGNIS_ISTORAGEMODULETEST_H
#define IGNIS_ISTORAGEMODULETEST_H


#include "IModuleTest.h"
#include "../../../../ignis/executor/core/modules/IStorageModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IStorageModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IStorageModuleTest);
                        CPPUNIT_TEST(take);
                        CPPUNIT_TEST(takeSampleWithRemplacement);
                        CPPUNIT_TEST(takeSampleWithoutRemplacement);
                        CPPUNIT_TEST(collect);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    virtual void setUp();

                    virtual void take();

                    virtual void takeSampleWithRemplacement();

                    virtual void takeSampleWithoutRemplacement();

                    virtual void collect();

                    virtual void tearDown();

                private:
                    std::vector<int> input;
                    std::shared_ptr<IStorageModule> storage_module;
                };
            }
        }
    }
}


#endif
