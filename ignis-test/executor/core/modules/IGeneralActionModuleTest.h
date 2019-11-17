#ifndef IGNIS_IGENERALACTIONMODULETEST_H
#define IGNIS_IGENERALACTIONMODULETEST_H

#include "IModuleTest.h"
#include "executor/core/modules/IGeneralActionModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IGeneralActionModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IGeneralActionModuleTest);
                        CPPUNIT_TEST(reduceIntTest);
                        CPPUNIT_TEST(reduceStringTest);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    IGeneralActionModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void reduceIntTest() { reduceTest<int>("ReduceInt", 2, "Memory"); }

                    void reduceStringTest() { reduceTest<std::string>("ReduceString", 2, "RawMemory"); }

                private:
                    template<typename Tp>
                    void reduceTest(const std::string &name, int cores, const std::string& partitionType);

                    std::shared_ptr<IGeneralActionModule> generalAction;
                };
            }
        }
    }
}

#include "IGeneralActionModuleTest.tcc"

#endif

