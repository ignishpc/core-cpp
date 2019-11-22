
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
    auto np = executor_data->getContext().executors();
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
    auto np = executor_data->getContext().executors();
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
    auto np = executor_data->getContext().executors();
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

template<typename Tp>
void IGeneralModuleTestClass::mapPartitions(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
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
    auto np = executor_data->getContext().executors();
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
}

#undef IGeneralModuleTestClass