#ifndef IGNIS_IGENERALACTIONMODULETEST_H
#define IGNIS_IGENERALACTIONMODULETEST_H

#include "IModuleTest.h"
#include "ignis/executor/core/modules/IGeneralActionModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IGeneralActionModuleTest : public IModuleTest {
                    CPPUNIT_TEST_SUITE(IGeneralActionModuleTest);
                    CPPUNIT_TEST(executeNoneTest);
                    CPPUNIT_TEST(loadAndRefNoneTest);
                    CPPUNIT_TEST(reduceIntTest);
                    CPPUNIT_TEST(reduceStringTest);
                    CPPUNIT_TEST(treeReduceIntTest);
                    CPPUNIT_TEST(treeReduceStringTest);
                    CPPUNIT_TEST(aggregateIntToStringTest);
                    CPPUNIT_TEST(treeAggregateStringTest);
                    CPPUNIT_TEST(foldIntTest);
                    CPPUNIT_TEST(treeFoldStringTest);
                    CPPUNIT_TEST(takeStringTest);
                    CPPUNIT_TEST(foreachIntTest);
                    CPPUNIT_TEST(foreachPartitionStringTest);
                    CPPUNIT_TEST(foreachExecutorIntTest);
                    CPPUNIT_TEST(topIntTest);
                    CPPUNIT_TEST(customTopStringTest);
                    CPPUNIT_TEST(takeOrderedStringTest);
                    CPPUNIT_TEST(customTakeOrderedIntTest);
                    CPPUNIT_TEST(keysIntStringTest);
                    CPPUNIT_TEST(valuesStringIntTest);
                    CPPUNIT_TEST_SUITE_END();

                public:
                    IGeneralActionModuleTest();

                    void setUp() override;

                    void tearDown() override;

                    void executeNoneTest() { executeTest("NoneFunction"); }

                    void loadAndRefNoneTest() { loadAndRefTest("NoneFunction"); }

                    void reduceIntTest() { reduceTest<int>("ReduceInt", 2, "Memory"); }

                    void reduceStringTest() { reduceTest<std::string>("ReduceString", 2, "RawMemory"); }

                    void treeReduceIntTest() { treeReduceTest<int>("ReduceInt", 2, "Memory"); }

                    void treeReduceStringTest() { treeReduceTest<std::string>("ReduceString", 2, "RawMemory"); }

                    void aggregateIntToStringTest() {
                        aggregateTest<int>("ZeroString", "ReduceIntToString", "ReduceString", 2, "Memory");
                    }

                    void treeAggregateStringTest() {
                        treeAggregateTest<std::string>("ZeroString", "ReduceString", "ReduceString", 2, "RawMemory");
                    }

                    void foldIntTest() { foldTest<int>("ZeroInt", "ReduceInt", 2, "Memory"); }

                    void treeFoldStringTest() {
                        treeFoldTest<std::string>("ZeroString", "ReduceString", 2, "RawMemory");
                    }

                    void takeStringTest() { takeTest<std::string>("Memory"); }

                    void foreachIntTest() { foreachTest<int>("ForeachInt", 2, "Memory"); }

                    void foreachPartitionStringTest() {
                        foreachPartitionTest<std::string>("ForeachPartitionString", 2, "RawMemory");
                    }

                    void foreachExecutorIntTest() {
                        foreachExecutorTest<std::string>("ForeachExecutorString", 2, "RawMemory");
                    }

                    void topIntTest() { topTest<int>(2, "Memory"); }

                    void customTopStringTest() { customTopTest<std::string>("SortString", 2, "RawMemory"); }

                    void takeOrderedStringTest() { takeOrderedTest<std::string>(2, "RawMemory"); }

                    void customTakeOrderedIntTest() { customTakeOrderedTest<int>("SortInt", 2, "Memory"); }

                    void keysIntStringTest() { keysTest<int, std::string>(2, "Memory"); }

                    void valuesStringIntTest() { valuesTest<std::string, int>(2, "RawMemory"); }

                private:
                    template<typename Tp2>
                    Tp2 normalize(Tp2 e) {
                        return e;
                    }

                    std::string normalize(std::string e) {
                        std::sort(e.begin(), e.end());
                        return e;
                    }

                    void executeTest(const std::string &name);

                    void loadAndRefTest(const std::string &name);

                    template<typename Tp>
                    void reduceTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void treeReduceTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void aggregateTest(const std::string &zero, const std::string &seq, const std::string &comb,
                                       int cores, const std::string &partitionType);

                    template<typename Tp>
                    void treeAggregateTest(const std::string &zero, const std::string &seq, const std::string &comb,
                                           int cores, const std::string &partitionType);

                    template<typename Tp>
                    void foldTest(const std::string &zero, const std::string &name, int cores,
                                  const std::string &partitionType);

                    template<typename Tp>
                    void treeFoldTest(const std::string &zero, const std::string &name, int cores,
                                      const std::string &partitionType);

                    template<typename Tp>
                    void takeTest(const std::string &partitionType);

                    template<typename Tp>
                    void foreachTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void foreachPartitionTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void foreachExecutorTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void topTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void customTopTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Tp>
                    void takeOrderedTest(int cores, const std::string &partitionType);

                    template<typename Tp>
                    void customTakeOrderedTest(const std::string &name, int cores, const std::string &partitionType);

                    template<typename Key, typename Value>
                    void keysTest(int cores, const std::string &partitionType);

                    template<typename Key, typename Value>
                    void valuesTest(int cores, const std::string &partitionType);

                    std::shared_ptr<IGeneralActionModule> generalAction;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IGeneralActionModuleTest.tcc"

#endif
