
#include "IGeneralActionModuleTest.h"

#define IGeneralActionModuleTestClass ignis::executor::core::modules::IGeneralActionModuleTest

IGeneralActionModuleTestClass::IGeneralActionModuleTest()
    : generalAction(std::make_shared<IGeneralActionModule>(executor_data)) {}

void IGeneralActionModuleTestClass::setUp() {}

void IGeneralActionModuleTestClass::tearDown() {}

void IGeneralActionModuleTestClass::executeTest(const std::string &name) {
    generalAction->execute(newSource(name));
    CPPUNIT_ASSERT(executor_data->getContext().isVar(name));
}

void IGeneralActionModuleTestClass::loadAndRefTest(const std::string &name) {
    executor_data->loadLibrary(newSource(name))->loadClass(executor_data->getContext());
    rpc::ISource source;
    source.obj.name = name;
    generalAction->execute(source);
}

template<typename Tp>
void IGeneralActionModuleTestClass::reduceTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    generalAction->reduce(newSource(name));
    auto result = getFromPartitions<Tp>();

    if (rank == 0) {
        CPPUNIT_ASSERT_EQUAL(1, (int) result.size());
        Tp expected_result = elems[0];
        for (int i = 1; i < elems.size(); i++) { expected_result += elems[i]; }
        CPPUNIT_ASSERT_EQUAL(normalize(expected_result), normalize(result[0]));
    } else {
        CPPUNIT_ASSERT_EQUAL(0, (int) result.size());
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::treeReduceTest(const std::string &name, int cores,
                                                   const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    generalAction->treeReduce(newSource(name));
    auto result = getFromPartitions<Tp>();

    if (rank == 0) {
        CPPUNIT_ASSERT_EQUAL(1, (int) result.size());
        Tp expected_result = elems[0];
        for (int i = 1; i < elems.size(); i++) { expected_result += elems[i]; }
        expected_result = normalize(expected_result);
        result[0] = normalize(result[0]);
        CPPUNIT_ASSERT_EQUAL(expected_result, result[0]);
    } else {
        CPPUNIT_ASSERT_EQUAL(0, (int) result.size());
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::aggregateTest(const std::string &zero, const std::string &seq,
                                                  const std::string &comb, int cores,
                                                  const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    generalAction->aggregate(newSource(zero), newSource(seq), newSource(comb));
    auto result = getFromPartitions<std::string>();

    if (rank == 0) {
        CPPUNIT_ASSERT_EQUAL(1, (int) result.size());
        std::stringstream expected_result;
        for (int i = 0; i < elems.size(); i++) { expected_result << elems[i]; }
        CPPUNIT_ASSERT_EQUAL(normalize(expected_result.str()), normalize(result[0]));
    } else {
        CPPUNIT_ASSERT_EQUAL(0, (int) result.size());
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::treeAggregateTest(const std::string &zero, const std::string &seq,
                                                      const std::string &comb, int cores,
                                                      const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    generalAction->treeAggregate(newSource(zero), newSource(seq), newSource(comb));
    auto result = getFromPartitions<std::string>();

    if (rank == 0) {
        CPPUNIT_ASSERT_EQUAL(1, (int) result.size());
        std::stringstream expected_result;
        for (int i = 0; i < elems.size(); i++) { expected_result << elems[i]; }
        std::string expected_result_str = expected_result.str();
        expected_result_str = normalize(expected_result_str);
        result[0] = normalize(result[0]);
        CPPUNIT_ASSERT_EQUAL(expected_result_str, result[0]);
    } else {
        CPPUNIT_ASSERT_EQUAL(0, (int) result.size());
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::foldTest(const std::string &zero, const std::string &name, int cores,
                                             const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    generalAction->fold(newSource(zero), newSource(name));
    auto result = getFromPartitions<Tp>();

    if (rank == 0) {
        CPPUNIT_ASSERT_EQUAL(1, (int) result.size());
        Tp expected_result = elems[0];
        for (int i = 1; i < elems.size(); i++) { expected_result += elems[i]; }
        CPPUNIT_ASSERT_EQUAL(normalize(expected_result), normalize(result[0]));
    } else {
        CPPUNIT_ASSERT_EQUAL(0, (int) result.size());
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::treeFoldTest(const std::string &zero, const std::string &name, int cores,
                                                 const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    generalAction->treeFold(newSource(zero), newSource(name));
    auto result = getFromPartitions<Tp>();

    if (rank == 0) {
        CPPUNIT_ASSERT_EQUAL(1, (int) result.size());
        Tp expected_result = elems[0];
        for (int i = 1; i < elems.size(); i++) { expected_result += elems[i]; }
        expected_result = normalize(expected_result);
        result[0] = normalize(result[0]);
        CPPUNIT_ASSERT_EQUAL(expected_result, result[0]);
    } else {
        CPPUNIT_ASSERT_EQUAL(0, (int) result.size());
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::takeTest(const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    int64_t n = 30;
    auto elems = IElements<Tp>().create(100, 0);
    loadToPartitions(elems, 5);
    registerType<Tp>();
    generalAction->take(n);
    auto result = getFromPartitions<Tp>();

    CPPUNIT_ASSERT_EQUAL(n, (int64_t) result.size());
    for (int64_t i = 0; i < n; i++) { CPPUNIT_ASSERT_EQUAL(elems[i], result[i]); }
}

template<typename Tp>
void IGeneralActionModuleTestClass::foreachTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    generalAction->foreach_(newSource(name));
    CPPUNIT_ASSERT(executor_data->getContext().isVar(name));
}

template<typename Tp>
void IGeneralActionModuleTestClass::foreachPartitionTest(const std::string &name, int cores,
                                                         const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    generalAction->foreachPartition(newSource(name));
    CPPUNIT_ASSERT(executor_data->getContext().isVar(name));
}

template<typename Tp>
void IGeneralActionModuleTestClass::foreachExecutorTest(const std::string &name, int cores,
                                                        const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    generalAction->foreachExecutor(newSource(name));
    CPPUNIT_ASSERT(executor_data->getContext().isVar(name));
}

template<typename Tp>
void IGeneralActionModuleTestClass::topTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    int64_t n = 30;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();
    generalAction->top(n);
    auto result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(n, (int64_t) result.size());
        std::sort(elems.begin(), elems.end());
        for (int i = 0; i < n; i++) { CPPUNIT_ASSERT_EQUAL(elems[elems.size() - i - 1], result[i]); }
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::customTopTest(const std::string &name, int cores,
                                                  const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    int64_t n = 30;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();
    generalAction->top2(n, newSource(name));
    auto result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(n, (int64_t) result.size());
        std::sort(elems.begin(), elems.end());
        for (int i = 0; i < n; i++) { CPPUNIT_ASSERT_EQUAL(elems[elems.size() - i - 1], result[i]); }
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::takeOrderedTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    int64_t n = 30;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();
    generalAction->takeOrdered(n);
    auto result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(n, (int64_t) result.size());
        std::sort(elems.begin(), elems.end());
        for (int i = 0; i < n; i++) { CPPUNIT_ASSERT_EQUAL(elems[i], result[i]); }
    }
}

template<typename Tp>
void IGeneralActionModuleTestClass::customTakeOrderedTest(const std::string &name, int cores,
                                                          const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    int64_t n = 30;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();
    generalAction->takeOrdered2(n, newSource(name));
    auto result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(n, (int64_t) result.size());
        std::sort(elems.begin(), elems.end());
        for (int i = 0; i < n; i++) { CPPUNIT_ASSERT_EQUAL(elems[i], result[i]); }
    }
}

template<typename Key, typename Value>
void IGeneralActionModuleTestClass::keysTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto elems = IElements<std::pair<Key, Value>>().create(100 * cores, 0);
    loadToPartitions(elems, cores * 2);
    registerType<std::pair<Key, Value>>();
    generalAction->keys();
    auto result = getFromPartitions<Key>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int64_t i = 0; i < elems.size(); i++) { CPPUNIT_ASSERT_EQUAL(elems[i].first, result[i]); }
}

template<typename Key, typename Value>
void IGeneralActionModuleTestClass::valuesTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto elems = IElements<std::pair<Key, Value>>().create(100 * cores, 0);
    loadToPartitions(elems, cores * 2);
    registerType<std::pair<Key, Value>>();
    generalAction->values();
    auto result = getFromPartitions<Value>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int64_t i = 0; i < elems.size(); i++) { CPPUNIT_ASSERT_EQUAL(elems[i].second, result[i]); }
}

#undef IGeneralActionModuleTestClass