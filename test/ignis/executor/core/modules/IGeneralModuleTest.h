
#ifndef IGNIS_IGENERALMODULETEST_H
#define IGNIS_IGENERALMODULETEST_H

#include "IModuleTest.h"
#include "ignis/executor/core/modules/IGeneralModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {

                class IGeneralModuleTest : public IModuleTest {
                CPPUNIT_TEST_SUITE(IGeneralModuleTest);
                        CPPUNIT_TEST(mapInt);
                        CPPUNIT_TEST(filterInt);
                        CPPUNIT_TEST(flatmapInt);
                        CPPUNIT_TEST(mapPartitionsInt);
                        CPPUNIT_TEST(mapPartitionWithIndexInt);
                        CPPUNIT_TEST(sortIntTest);
                        CPPUNIT_TEST(sortStringTest);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    IGeneralModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void mapInt() { map<int>("MapInt", 2, "Memory"); }

                    void filterInt() { filter<int>("FilterInt", 2, "Memory"); }

                    void flatmapInt() { flatmap<int>("FlatmapInt", 2, "Memory"); }

                    void mapPartitionsInt() { mapPartitions<int>("MapPartitionsInt", 2, "Memory"); }

                    void mapPartitionWithIndexInt() {
                        mapPartitionsWithIndex<int>("MapPartitionWithIndexInt", 2, "Memory");
                    }

                    void sortIntTest() { sortTest<int>("SortInt", 2, "Memory"); }

                    void sortStringTest() { sortTest<std::string>("SortString", 2, "RawMemory"); }

                private:
                    template<typename Tp>
                    void map(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void filter(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void flatmap(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapPartitions(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapPartitionsWithIndex(const std::string &name, int cores, const std::string &partitionType);

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

