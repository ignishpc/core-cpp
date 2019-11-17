
#ifndef IGNIS_IGENERALMODULETEST_H
#define IGNIS_IGENERALMODULETEST_H

#include "IModuleTest.h"
#include "executor/core/modules/IGeneralModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {

                class IGeneralModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IGeneralModuleTest);
                        CPPUNIT_TEST(sortIntTest);
                        CPPUNIT_TEST(sortStringTest);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    IGeneralModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void sortIntTest() { sortTest<int>("SortInt", 2, "Memory"); }

                    void sortStringTest() { sortTest<std::string>("SortString", 2, "RawMemory"); }

                private:
                    template<typename Tp>
                    void sortTest(const std::string &name, int cores, const std::string &partitionType);

                    std::shared_ptr<IGeneralModule> general;
                };
            }
        }
    }
}

#include "IGeneralModuleTest.tcc"

#endif

