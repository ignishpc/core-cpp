
#include "IGeneralModuleTest.h"

#define IGeneralModuleTestClass ignis::executor::core::modules::IGeneralModuleTest

IGeneralModuleTestClass::IGeneralModuleTest() : general(std::make_shared<IGeneralModule>(executor_data)) {
    auto &props = executor_data->getContext().props();
    props["ignis.modules.sort.samples"] = "2";
}

void IGeneralModuleTestClass::setUp() {}

void IGeneralModuleTestClass::tearDown() {}


template<typename Tp>
void IGeneralModuleTestClass::map(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->map_(newSource(name));
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]);
    }
}

template<typename Tp>
void IGeneralModuleTestClass::filter(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->filter(newSource(name));
    auto result = getFromPartitions<Tp>();

    int j = 0;
    for (int i = 0; i < result.size(); i++) {
        if (elems[i] % 2 == 0) {
            CPPUNIT_ASSERT_EQUAL(elems[i], result[j++]);
        }
    }
}

template<typename Tp>
void IGeneralModuleTestClass::flatmap(const std::string &name, int cores, const std::string &partitionType) {
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


void IGeneralModuleTestClass::keyBy(const std::string &name, int cores, const std::string &partitionType) {
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
void IGeneralModuleTestClass::mapPartitions(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->mapPartitions(newSource(name), false);
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]);
    }
}

template<typename Tp>
void
IGeneralModuleTestClass::mapPartitionsWithIndex(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2, 0);
    loadToPartitions(elems, cores * 2);
    general->mapPartitionsWithIndex(newSource(name), false);
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]);
    }
}

template<typename Tp>
void IGeneralModuleTestClass::mapExecutor(const std::string &name, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto elems = IElements<Tp>().create(100 * 2 * 2, 0);
    loadToPartitions(elems, 5);
    general->mapExecutor(newSource(name));
    auto result = getFromPartitions<Tp>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(elems[i] + 1, result[i]);
    }
}

template<typename Tp>
void IGeneralModuleTestClass::mapExecutorTo(const std::string &name, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto elems = IElements<Tp>().create(100 * 2 * 2, 0);
    loadToPartitions(elems, 5);
    general->mapExecutorTo(newSource(name));
    auto result = getFromPartitions<std::string>();

    CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
    for (int i = 0; i < result.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(std::to_string(elems[i]), result[i]);
    }
}


void IGeneralModuleTestClass::groupBy(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<std::string>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    general->groupBy(newSource(name), cores * 2);
    auto result = getFromPartitions < std::pair<int, api::IVector < std::string>>>();

    std::unordered_map<int, int64_t> counts;

    for (auto &elem:elems) {
        counts[elem.length()]++;
    }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(
            *((*executor_data->getPartitions < std::pair<int, api::IVector < std::string>>>())[0]), 0);

    result = getFromPartitions < std::pair<int, api::IVector < std::string>>>();

    if (executor_data->mpi().isRoot(0)) {
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(counts[result[i].first], (int64_t) result[i].second.size());
        }
    }
}

template<typename Tp>
void  IGeneralModuleTestClass::sortTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    general->sortBy(newSource(name), true);
    auto result = getFromPartitions<Tp>();

    for (int i = 1; i < result.size(); i++) {
        CPPUNIT_ASSERT_GREATEREQUAL(result[i - 1], result[i]);
    }

    loadToPartitions(result, 1);

    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);

    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(elems.size(), result.size());
        std::sort(elems.begin(), elems.end());
        for (int i = 0; i < result.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(elems[i], result[i]);
        }
    }
}

#undef IGeneralModuleTestClass