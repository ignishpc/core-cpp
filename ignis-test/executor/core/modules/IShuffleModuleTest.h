
#ifndef IGNIS_ISHUFFLEMODULETEST_H
#define IGNIS_ISHUFFLEMODULETEST_H

#include "IModuleTest.h"
#include "../../../../ignis/executor/core/modules/IShuffleModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IShuffleModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IShuffleModuleTest);
                        CPPUNIT_TEST(shuffle);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    virtual void setUp();

                    virtual void shuffle();

                    virtual void tearDown();

                private:
                    std::shared_ptr<IShuffleModule> shuffle_module;
                };
            }
        }
    }
}

#endif
