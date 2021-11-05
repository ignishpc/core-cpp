
#ifndef IGNIS_ICOMMMODULETEST_H
#define IGNIS_ICOMMMODULETEST_H

#include "IModuleTest.h"
#include "ignis/executor/core/modules/ICommModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ICommModuleTest : public IModuleTest {
                    CPPUNIT_TEST_SUITE(ICommModuleTest);
                    CPPUNIT_TEST(getSetPartitionsEmptyTest);
                    CPPUNIT_TEST(getSetPartitionsSimpleTest);
                    CPPUNIT_TEST(getSetPartitionsHasMinimalTest);
                    CPPUNIT_TEST(getSetPartitionsAdvancedTest);
                    CPPUNIT_TEST(importDataTest);
                    CPPUNIT_TEST(importDataVoidTest);
                    CPPUNIT_TEST(importDataCoresTest);
                    CPPUNIT_TEST(importDataCoresVoidTest);
                    CPPUNIT_TEST_SUITE_END();

                public:
                    ICommModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void getSetPartitionsEmptyTest() { getSetPartitionsImpl<int>("RawMemory", 0, 4, true); }

                    void getSetPartitionsSimpleTest() { getSetPartitionsImpl<int>("Memory", 1, 4, true); }

                    void getSetPartitionsHasMinimalTest() {
                        getSetPartitionsImpl<std::string>("RawMemory", 4, 2, false);
                    }

                    void getSetPartitionsAdvancedTest() { getSetPartitionsImpl<int>("Memory", 2, 4, true); }

                    void importDataTest() { importDataTestImpl<int>(1, "Memory", 10, false); }

                    void importDataVoidTest() { importDataTestImpl<int>(1, "Memory", 10, true); }

                    void importDataCoresTest() { importDataTestImpl<int>(2, "Memory", 10, false); }

                    void importDataCoresVoidTest() { importDataTestImpl<int>(2, "Memory", 10, true); }

                private:
                    template<typename Tp>
                    void getSetPartitionsImpl(const std::string &partitionType, int64_t parts, int64_t getParts,
                                              bool setInt);

                    template<typename Tp>
                    void importDataTestImpl(int cores, const std::string &partitionType, int64_t parts, bool withVoid);

                    std::shared_ptr<ICommModule> comm;
                };

            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "ICommModuleTest.tcc"

#endif//IGNIS_ICOMMMODULETEST_H
