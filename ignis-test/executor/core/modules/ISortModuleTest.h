
#ifndef IGNIS_ISORTMODULETEST_H
#define IGNIS_ISORTMODULETEST_H

#include "IModuleTest.h"
#include "../../../../ignis/executor/core/modules/ISortModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ISortModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(ISortModuleTest);
                        CPPUNIT_TEST(sequentialLocalSort);
                        CPPUNIT_TEST(parallelLocalSort);
                        CPPUNIT_TEST(sort);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    virtual void setUp();

                    virtual void sequentialLocalSort();

                    virtual void parallelLocalSort();

                    virtual void sort();

                    virtual void tearDown();

                private:
                    void localSort();

                    std::shared_ptr<ISortModule> sort_module;
                };
            }
        }
    }
}


#endif