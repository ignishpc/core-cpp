
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
                        CPPUNIT_TEST(flatmapString);
                        CPPUNIT_TEST(keyByStringInt);
                        CPPUNIT_TEST(mapPartitionsInt);
                        CPPUNIT_TEST(mapPartitionWithIndexInt);
                        CPPUNIT_TEST(mapExecutorInt);
                        CPPUNIT_TEST(mapExecutorToString);
                        CPPUNIT_TEST(groupByIntString);
                        CPPUNIT_TEST(sortIntTest);
                        CPPUNIT_TEST(sortStringTest);
                        //CPPUNIT_TEST(sortBy);
                        //CPPUNIT_TEST(flatMapValues);
                        //CPPUNIT_TEST(mapValues);
                        //CPPUNIT_TEST(groupByKey);
                        //CPPUNIT_TEST(reduceByKey);
                        //CPPUNIT_TEST(aggregateByKey);
                        //CPPUNIT_TEST(foldByKey);
                        //CPPUNIT_TEST(sortByKey);
                        //CPPUNIT_TEST(customSortByKey);
                    CPPUNIT_TEST_SUITE_END();
                public:
                    IGeneralModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void mapInt() { map<int>("MapInt", 2, "Memory"); }

                    void filterInt() { filter<int>("FilterInt", 2, "RawMemory"); }

                    void flatmapString() { flatmap<std::string>("FlatmapString", 2, "Memory"); }

                    void keyByStringInt() { keyBy("KeyByString", 2, "RawMemory"); }

                    void mapPartitionsInt() { mapPartitions<int>("MapPartitionsInt", 2, "Memory"); }

                    void mapPartitionWithIndexInt() {
                        mapPartitionsWithIndex<int>("MapPartitionWithIndexInt", 2, "RawMemory");
                    }

                    void mapExecutorInt() { mapExecutor<int>("MapExecutorInt", "Memory"); }

                    void mapExecutorToString() { mapExecutorTo<int>("MapExecutorToString", "RawMemory"); }

                    void groupByIntString() { groupBy("GroupByIntString", 2, "Memory"); }

                    void sortIntTest() { sortTest<int>("SortInt", 2, "Memory"); }

                    void sortStringTest() { sortTest<std::string>("SortString", 2, "RawMemory"); }

                    void sortBy() {}

                    void flatMapValues() {}

                    void mapValues() {}

                    void groupByKey() {}

                    void reduceByKey() {}

                    void aggregateByKey() {}

                    void foldByKey() {}

                    void sortByKey() {}

                    void customSortByKey() {}

                private:
                    template<typename Tp>
                    void map(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void filter(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void flatmap(const std::string &name, int cores, const std::string &partitionType);

                    void keyBy(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapPartitions(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapPartitionsWithIndex(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void mapExecutor(const std::string &name, const std::string &partitionType);

                    template<typename Tp>
                    void mapExecutorTo(const std::string &name, const std::string &partitionType);


                    void groupBy(const std::string &name, int cores, const std::string &partitionType);

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

