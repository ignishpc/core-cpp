
#include "IGeneralModuleTest.h"

#define IGeneralModuleTestClass ignis::executor::core::modules::IGeneralModuleTest

IGeneralModuleTestClass::IGeneralModuleTest() : general(std::make_shared<IGeneralModule>(executor_data)) {
    auto &props = executor_data->getContext().props();
    props["ignis.modules.sort.samples"] = "0.1";
    props["ignis.modules.sort.resampling"] = "False";
}

void IGeneralModuleTestClass::setUp() {}

void IGeneralModuleTestClass::tearDown() {}

void IGeneralModuleTestClass::executeToTest(const std::string &name, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    general->executeTo(newSource(name));
    auto result = getFromPartitions<int>();
    CPPUNIT_ASSERT_EQUAL(100, (int) result.size());
    for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(i, result[i]); }
}

template<typename Tp>
void IGeneralModuleTestClass::mapTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->map_(newSource(name));
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]); }
}

template<typename Tp>
void IGeneralModuleTestClass::filterTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->filter(newSource(name));
    auto result = getFromPartitions<Tp>();

    int j = 0;
    for (int i = 0; i < elems.size(); i++) {
        if (elems[i] % 2 == 0) { CPPUNIT_ASSERT_EQUAL(elems[i], result[j++]); }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::flatmapTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->flatmap(newSource(name));
    auto result = getFromPartitions<Tp>();

    CPPUNIT_ASSERT_EQUAL(elems.size() * 2, result.size());
    for (int i = 0; i < elems.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(elems[i], result[2 * i]);
        CPPUNIT_ASSERT_EQUAL(elems[i], result[2 * i + 1]);
    }
}


void IGeneralModuleTestClass::keyByTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<std::string>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->keyBy(newSource(name));
    auto result = getFromPartitions<std::pair<int, std::string>>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL((int) elems[i].length(), result[i].first);
        CPPUNIT_ASSERT_EQUAL(elems[i], result[i].second);
    }
}

template<typename Tp>
void IGeneralModuleTestClass::mapPartitionsTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->mapPartitions(newSource(name));
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]); }
}

template<typename Tp>
void IGeneralModuleTestClass::mapPartitionsWithIndexTest(const std::string &name, int cores,
                                                         const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->mapPartitionsWithIndex(newSource(name), false);
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]); }
}

template<typename Tp>
void IGeneralModuleTestClass::mapExecutorTest(const std::string &name, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto elems = IElements<Tp>().create(100 * 2 * 2, 0);
    loadToPartitions(elems, 5);
    general->mapExecutor(newSource(name));
    auto result = getFromPartitions<Tp>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(elems[i] + 1, result[i]); }
}

template<typename Tp>
void IGeneralModuleTestClass::mapExecutorToTest(const std::string &name, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto elems = IElements<Tp>().create(100 * 2 * 2, 0);
    loadToPartitions(elems, 5);
    general->mapExecutorTo(newSource(name));
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]); }
}


void IGeneralModuleTestClass::groupByTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<std::string>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    general->groupBy(newSource(name), cores * 2);
    auto result = getFromPartitions<std::pair<int, api::IVector<std::string>>>();

    std::unordered_map<int, int64_t> counts;

    for (auto &elem : elems) { counts[elem.length()]++; }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<int, api::IVector<std::string>>>())[0]), 0);

    result = getFromPartitions<std::pair<int, api::IVector<std::string>>>();

    if (executor_data->mpi().isRoot(0)) {
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(counts[result[i].first], (int64_t) result[i].second.size());
        }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::sortTest(const std::string &name, int cores, const std::string &partitionType, bool rs) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->getContext().props()["ignis.modules.sort.resampling"] = rs ? "true" : "false";

    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    general->sortBy(newSource(name), true);
    auto result = getFromPartitions<Tp>();

    for (int i = 1; i < result.size(); i++) { CPPUNIT_ASSERT_GREATEREQUAL(result[i - 1], result[i]); }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);

    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
        std::sort(elems.begin(), elems.end());
        for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(elems[i], result[i]); }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::distinctTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    std::set<Tp> distinct(elems.begin(), elems.end());
    registerType<Tp>();
    loadToPartitions(elems, cores * 2);

    general->distinct(4);

    auto result = getFromPartitions<Tp>();
    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(distinct.size(), result.size());
        CPPUNIT_ASSERT(std::set<Tp>(result.begin(), result.end()) == distinct);
    }
}

template<typename Key, typename Value>
void IGeneralModuleTestClass::joinTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->getContext().executors();
    auto elems = IElements<std::pair<Key, Value>>().create(100 * cores * 2, 0);
    auto elems2 = IElements<std::pair<Key, Value>>().create(100 * cores * 2, 1);
    auto local_elems = rankVector(elems);
    auto local_elems2 = rankVector(elems2);

    registerType<std::pair<Key, Value>>();
    loadToPartitions(local_elems2, 2);
    executor_data->setVariable("other", executor_data->getPartitions<std::pair<Key, Value>>());
    loadToPartitions(local_elems, 2);

    general->join("other", 2);
    auto result = getFromPartitions<std::pair<Key, std::pair<Value, Value>>>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<Key, std::pair<Value, Value>>>())[0]), 0);
    result = getFromPartitions<std::pair<Key, std::pair<Value, Value>>>();

    if (executor_data->mpi().isRoot(0)) {
        std::map<Key, api::IVector<Value>> m1;
        for (auto &entry : elems) { m1[entry.first].push_back(entry.second); }

        api::IVector<std::pair<Key, std::pair<Value, Value>>> expected;
        for (auto &entry : elems2) {
            if (m1.find(entry.first) != m1.end()) {
                for (auto &value : m1[entry.first]) {
                    expected.emplace_back(entry.first, std::make_pair(entry.second, value));
                }
            }
        }
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        CPPUNIT_ASSERT(result == expected);
    }
}

template<typename Tp>
void IGeneralModuleTestClass::unionTest(int cores, const std::string &partitionType, bool preserveOrder) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->getContext().executors();
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    auto elems2 = IElements<Tp>().create(100 * cores * 2, 1);
    auto local_elems = rankVector(elems);
    auto local_elems2 = rankVector(elems2);

    registerType<Tp>();
    loadToPartitions(local_elems2, 2);
    executor_data->setVariable("other", executor_data->getPartitions<Tp>());
    loadToPartitions(local_elems, 2);

    general->union_("other", preserveOrder);
    auto result = getFromPartitions<Tp>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size() + elems2.size(), result.size());

        api::IVector<Tp> expected(elems.begin(), elems.end());
        expected.insert(expected.end(), elems2.begin(), elems2.end());

        if (!preserveOrder) {
            std::sort(result.begin(), result.end());
            std::sort(expected.begin(), expected.end());
        }
        CPPUNIT_ASSERT(result == expected);
    }
}

template<typename Tp>
void IGeneralModuleTestClass::flatMapValuesTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<std::pair<int, Tp>>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->flatMapValues(newSource(name));
    auto result = getFromPartitions<std::pair<int, std::string>>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(elems[i].first, result[i].first);
        CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i].second), result[i].second);
    }
}

template<typename Tp>
void IGeneralModuleTestClass::mapValuesTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<std::pair<int, Tp>>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->mapValues(newSource(name));
    auto result = getFromPartitions<std::pair<int, std::string>>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(elems[i].first, result[i].first);
        CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i].second), result[i].second);
    }
}

void IGeneralModuleTestClass::groupByKeyTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<std::pair<int, std::string>>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<std::pair<int, std::string>>();
    general->groupByKey(cores * 2);
    auto result = getFromPartitions<std::pair<int, api::IVector<std::string>>>();

    std::unordered_map<int, int64_t> counts;

    for (auto &elem : elems) { counts[elem.first]++; }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<int, api::IVector<std::string>>>())[0]), 0);

    result = getFromPartitions<std::pair<int, api::IVector<std::string>>>();

    if (executor_data->mpi().isRoot(0)) {
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(counts[result[i].first], (int64_t) result[i].second.size());
        }
    }
}

template<typename Key, typename Value>
void IGeneralModuleTestClass::reduceByKeyTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<std::pair<Key, Value>>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<std::pair<int, std::string>>();
    general->reduceByKey(newSource(name), cores * 2, true);
    auto result = getFromPartitions<std::pair<Key, Value>>();

    std::unordered_map<Key, Value> acums;

    for (auto &elem : elems) {
        if (acums.find(elem.first) == acums.end()) {
            acums[elem.first] = elem.second;
        } else {
            acums[elem.first] += elem.second;
        }
    }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<Key, Value>>())[0]), 0);

    result = getFromPartitions<std::pair<Key, Value>>();

    if (executor_data->mpi().isRoot(0)) {
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(normalize(acums[result[i].first]), normalize(result[i].second));
        }
    }
}

template<typename Key, typename Value>
void IGeneralModuleTestClass::aggregateByKeyTest(const std::string &zero, const std::string &seq,
                                                 const std::string &comb, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<std::pair<Key, Value>>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<std::pair<int, std::string>>();
    general->aggregateByKey4(newSource(zero), newSource(seq), newSource(comb), cores * 2);
    auto result = getFromPartitions<std::pair<Key, std::string>>();

    std::unordered_map<Key, std::string> acums;

    for (auto &elem : elems) {
        if (acums.find(elem.first) == acums.end()) { acums[elem.first] = ""; }
        acums[elem.first] += std::to_string(elem.second);
    }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<Key, std::string>>())[0]), 0);

    result = getFromPartitions<std::pair<Key, std::string>>();

    if (executor_data->mpi().isRoot(0)) {
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(normalize(acums[result[i].first]), normalize(result[i].second));
        }
    }
}

template<typename Key, typename Value>
void IGeneralModuleTestClass::foldByKeyTest(const std::string &zero, const std::string &name, int cores,
                                            const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<std::pair<Key, Value>>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<std::pair<int, std::string>>();
    general->foldByKey(newSource(zero), newSource(name), cores * 2, true);
    auto result = getFromPartitions<std::pair<Key, Value>>();

    std::unordered_map<Key, Value> acums;

    for (auto &elem : elems) {
        if (acums.find(elem.first) == acums.end()) {
            acums[elem.first] = elem.second;
        } else {
            acums[elem.first] += elem.second;
        }
    }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<Key, Value>>())[0]), 0);

    result = getFromPartitions<std::pair<Key, Value>>();

    if (executor_data->mpi().isRoot(0)) {
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(normalize(acums[result[i].first]), normalize(result[i].second));
        }
    }
}

template<typename Key, typename Value>
void IGeneralModuleTestClass::sortByKeyTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);

    auto elems = IElements<std::pair<Key, Value>>().create(100 * cores * 2 * np, 0);
    api::IVector<Key> elems_keys;
    for (auto &elem : elems) { elems_keys.push_back(elem.first); }
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<std::pair<Key, Value>>();
    general->sortByKey(true);
    auto result = getFromPartitions<std::pair<Key, Value>>();

    for (int i = 1; i < result.size(); i++) { CPPUNIT_ASSERT_GREATEREQUAL(result[i - 1].first, result[i].first); }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<Key, Value>>())[0]), 0);

    result = getFromPartitions<std::pair<Key, Value>>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
        std::sort(elems_keys.begin(), elems_keys.end());
        for (int i = 0; i < result.size(); i++) { CPPUNIT_ASSERT_EQUAL(elems_keys[i], result[i].first); }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::repartitionTest(int cores, const std::string &partitionType, bool preserveOrdering,
                                              bool global) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->mpi().rank();

    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    if (rank == 0) { local_elems.insert(local_elems.begin(), elems.begin(), elems.begin() + 100); }
    elems.insert(elems.begin(), elems.begin(), elems.begin() + 100);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();

    general->repartition(2 + np - 1, preserveOrdering, global);

    for (auto &part : *executor_data->getPartitions<Tp>()) { CPPUNIT_ASSERT_GREATER((size_t) 50, part->size()); }

    auto result = getFromPartitions<Tp>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
        if (preserveOrdering || !global) {
            CPPUNIT_ASSERT(elems == result);
        } else {
            std::unordered_map<Tp, int64_t> expected;
            for (auto &e : elems) { expected[e]++; }
            for (auto &e : result) { expected[e]--; }
            for (auto &entry : expected) { CPPUNIT_ASSERT_EQUAL((int64_t) 0, entry.second); }
        }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::partitionByTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->mpi().rank();

    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    if (rank == 0) { local_elems.insert(local_elems.begin(), elems.begin(), elems.begin() + 100); }
    elems.insert(elems.begin(), elems.begin(), elems.begin() + 100);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();

    general->partitionBy(newSource(name), 2 * np - 1);

    auto result = getFromPartitions<Tp>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
        std::unordered_map<Tp, int64_t> expected;
        for (auto &e : elems) { expected[e]++; }
        for (auto &e : result) { expected[e]--; }
        for (auto &entry : expected) { CPPUNIT_ASSERT_EQUAL((int64_t) 0, entry.second); }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::partitionByRandomTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->mpi().rank();

    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    if (rank == 0) { local_elems.insert(local_elems.begin(), elems.begin(), elems.begin() + 100); }
    elems.insert(elems.begin(), elems.begin(), elems.begin() + 100);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();

    general->partitionByRandom(2 * np - 1);

    auto result = getFromPartitions<Tp>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
        std::unordered_map<Tp, int64_t> expected;
        for (auto &e : elems) { expected[e]++; }
        for (auto &e : result) { expected[e]--; }
        for (auto &entry : expected) { CPPUNIT_ASSERT_EQUAL((int64_t) 0, entry.second); }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::partitionByHashTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->mpi().rank();

    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    if (rank == 0) { local_elems.insert(local_elems.begin(), elems.begin(), elems.begin() + 100); }
    elems.insert(elems.begin(), elems.begin(), elems.begin() + 100);
    loadToPartitions(local_elems, cores * 2);
    registerType<Tp>();

    general->partitionByHash(2 * np - 1);

    auto result = getFromPartitions<Tp>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
        std::unordered_map<Tp, int64_t> expected;
        for (auto &e : elems) { expected[e]++; }
        for (auto &e : result) { expected[e]--; }
        for (auto &entry : expected) { CPPUNIT_ASSERT_EQUAL((int64_t) 0, entry.second); }
    }
}

#undef IGeneralModuleTestClass