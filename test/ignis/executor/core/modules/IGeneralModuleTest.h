
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
                    CPPUNIT_TEST(mapIntTest);
                    CPPUNIT_TEST(filterIntTest);
                    CPPUNIT_TEST(flatmapStringTest);
                    CPPUNIT_TEST(keyByStringIntTest);
                    CPPUNIT_TEST(mapPartitionsIntTest);
                    CPPUNIT_TEST(mapPartitionWithIndexIntTest);
                    CPPUNIT_TEST(mapExecutorIntTest);
                    CPPUNIT_TEST(mapExecutorToStringTest);
                    CPPUNIT_TEST(groupByIntStringTest);
                    CPPUNIT_TEST(sortIntTest);
                    CPPUNIT_TEST(sortStringTest);
                    CPPUNIT_TEST(resamplingSortIntTest);
                    CPPUNIT_TEST(flatMapValuesIntTest);
                    CPPUNIT_TEST(mapValuesIntTest);
                    CPPUNIT_TEST(groupByKeyIntStringTest);
                    CPPUNIT_TEST(reduceByKeyIntStringTest);
                    CPPUNIT_TEST(aggregateByKeyIntIntTest);
                    CPPUNIT_TEST(foldByKeyIntIntTest);
                    CPPUNIT_TEST(sortByKeyIntStringTest);
                    CPPUNIT_TEST_SUITE_END();

                public:
                    IGeneralModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void mapIntTest() { mapTest<int>("MapInt", 2, "Memory"); }

                    void filterIntTest() { filterTest<int>("FilterInt", 2, "RawMemory"); }

                    void flatmapStringTest() { flatmapTest<std::string>("FlatmapString", 2, "Memory"); }

                    void keyByStringIntTest() { keyByTest("KeyByString", 2, "RawMemory"); }

                    void mapPartitionsIntTest() { mapPartitionsTest<int>("MapPartitionsInt", 2, "Memory"); }

                    void mapPartitionWithIndexIntTest() {
                        mapPartitionsWithIndexTest<int>("MapPartitionWithIndexInt", 2, "RawMemory");
                    }

                    void mapExecutorIntTest() { mapExecutorTest<int>("MapExecutorInt", "Memory"); }

                    void mapExecutorToStringTest() { mapExecutorToTest<int>("MapExecutorToString", "RawMemory"); }

                    void groupByIntStringTest() { groupByTest("GroupByIntString", 2, "Memory"); }

                    void sortIntTest() { sortTest<int>("SortInt", 2, "Memory"); }

                    void sortStringTest() { sortTest<std::string>("SortString", 2, "RawMemory"); }

                    void resamplingSortIntTest() { sortTest<int>("SortInt", 2, "Memory", true); }

                    void flatMapValuesIntTest() { flatMapValuesTest<int>("FlatMapValuesInt", 2, "Memory"); }

                    void mapValuesIntTest() { mapValuesTest<int>("MapValuesInt", 2, "RawMemory"); }

                    void groupByKeyIntStringTest() { groupByKeyTest(2, "Memory"); }

                    void reduceByKeyIntStringTest() {
                        reduceByKeyTest<int, std::string>("ReduceString", 2, "RawMemory");
                    }

                    void aggregateByKeyIntIntTest() {
                        aggregateByKeyTest<int, int>("ZeroString", "ReduceIntToString", "ReduceString", 2, "Memory");
                    }

                    void foldByKeyIntIntTest() { foldByKeyTest<int, int>("ZeroInt", "ReduceInt", 2, "Memory"); }

                    void sortByKeyIntStringTest() { sortByKeyTest<int, std::string>(2, "Memory"); }

                private:
                    template<typename Tp>
                    void mapTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void filterTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void flatmapTest(const std::string &name, int cores, const std::string &partitionType);

                    void keyByTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapPartitionsTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapPartitionsWithIndexTest(const std::string &name, int cores,
                                                    const std::string &partitionType);

                    template<typename Tp>
                    void mapExecutorTest(const std::string &name, const std::string &partitionType);

                    template<typename Tp>
                    void mapExecutorToTest(const std::string &name, const std::string &partitionType);


                    void groupByTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void sortTest(const std::string &name, int cores, const std::string &partitionType,
                                  bool resampling = false);

                    template<typename Tp>
                    void flatMapValuesTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapValuesTest(const std::string &name, int cores, const std::string &partitionType);

                    void groupByKeyTest(int cores, const std::string &partitionType);

                    template<typename Key, typename Value>
                    void reduceByKeyTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Key, typename Value>
                    void aggregateByKeyTest(const std::string &zero, const std::string &seq, const std::string &comb,
                                            int cores, const std::string &partitionType);

                    template<typename Key, typename Value>
                    void foldByKeyTest(const std::string &zero, const std::string &name, int cores,
                                       const std::string &partitionType);

                    template<typename Key, typename Value>
                    void sortByKeyTest(int cores, const std::string &partitionType);

                    std::shared_ptr<IGeneralModule> general;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IGeneralModuleTest.tcc"

#endif
