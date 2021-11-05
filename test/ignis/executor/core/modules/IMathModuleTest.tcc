
#include "IMathModuleTest.h"
#include "ignis/driver/api.h"

#define IMathModuleTestClass ignis::executor::core::modules::IMathModuleTest


IMathModuleTestClass::IMathModuleTest() : math(std::make_shared<IMathModule>(executor_data)) {
    auto &props = executor_data->getContext().props();
    props["ignis.modules.sort.samples"] = "2";
}


void IMathModuleTestClass::setUp() {}

void IMathModuleTestClass::tearDown() {}

template<typename Tp>
void IMathModuleTestClass::sampleWithReplacementTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->mpi().executors();
    int64_t sum = 0;
    std::vector<int64_t> num;
    for (int i = 0; i < cores * 2; i++) {
        num.push_back(20 * np);
        sum += num.back();
    }

    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, num.size());
    registerType<Tp>();

    math->sample(true, num, 0);

    auto result = getFromPartitions<Tp>();
    CPPUNIT_ASSERT_EQUAL(sum, (int64_t) result.size());
}
template<typename Tp>
void IMathModuleTestClass::sampleWithoutReplacementTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->mpi().executors();
    int64_t sum = 0;
    std::vector<int64_t> num;
    for (int i = 0; i < cores * 2; i++) {
        num.push_back(20 * np);
        sum += num.back();
    }

    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, num.size());
    registerType<Tp>();

    math->sample(false, num, 0);

    auto result = getFromPartitions<Tp>();
    CPPUNIT_ASSERT_EQUAL(sum, (int64_t) result.size());
}
template<typename Tp>
void IMathModuleTestClass::sampleByKeyTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->mpi().executors();
    char word = 'a';
    api::IVector<std::string> keys;
    for (int i = 0; i < cores * 2; i++) { keys.emplace_back(1, word++); }
    auto values = IElements<Tp>().create(100 * cores * 2 * np, 0);
    api::IVector<std::pair<std::string, Tp>> elems;
    for (auto &key : keys) {
        for (auto &v : values) { elems.emplace_back(key, v); }
    }

    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, keys.size());
    registerType<std::pair<std::string, Tp>>();

    std::map<std::string, double> aux;
    aux["a"] = 0.5;
    auto fractions = ignis::driver::api::ISource("").addParam("fractions", aux).rpc();

    math->sampleByKey(true, fractions, 0);

    auto result = getFromPartitions<std::pair<std::string, Tp>>();
    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<std::string, Tp>>())[0]), 0);
    result = getFromPartitions<std::pair<std::string, Tp>>();

    if (executor_data->mpi().isRoot(0)) {
        std::unordered_map<std::string, int64_t> count;
        for (auto &entry : result) { count[entry.first]++; }
        CPPUNIT_ASSERT(count.find("a") != count.end());
        CPPUNIT_ASSERT(count.size() == 1);

        CPPUNIT_ASSERT_EQUAL(count["a"], (int64_t) values.size() / 2);
    }
}
template<typename Tp>
void IMathModuleTestClass::countTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);

    auto elems = IElements<Tp>().create(100 * 2, 0);
    loadToPartitions(elems, 2);

    CPPUNIT_ASSERT_EQUAL(math->count(), (int64_t) elems.size());
}
template<typename Tp>
void IMathModuleTestClass::maxTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;

    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);

    registerType<Tp>();
    math->max();

    auto result = getFromPartitions<Tp>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(1, (int)result.size());
        std::sort(elems.begin(), elems.end());
        CPPUNIT_ASSERT_EQUAL(elems.back(), result[0]);
    }
}
template<typename Tp>
void IMathModuleTestClass::minTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;

    auto np = executor_data->getContext().executors();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);

    registerType<Tp>();
    math->min();

    auto result = getFromPartitions<Tp>();

    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
    result = getFromPartitions<Tp>();

    if (executor_data->mpi().isRoot(0)) {
        CPPUNIT_ASSERT_EQUAL(1, (int)result.size());
        std::sort(elems.begin(), elems.end());
        CPPUNIT_ASSERT_EQUAL(elems[0], result[0]);
    }
}
template<typename Tp>
void IMathModuleTestClass::countByKeyTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->mpi().executors();

    auto elems = IElements<std::pair<Tp, Tp>>().create(100 * cores * 2 * np, 0);

    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<std::pair<Tp, Tp>>();

    math->countByKey();

    auto result = getFromPartitions<std::pair<Tp, int64_t>>();
    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<Tp, int64_t>>())[0]), 0);
    result = getFromPartitions<std::pair<Tp, int64_t>>();

    if (executor_data->mpi().isRoot(0)) {
        std::unordered_map<Tp, int64_t> count;
        for (auto &entry : elems) { count[entry.first]++; }
        CPPUNIT_ASSERT_EQUAL(count.size(), result.size());

        for(auto &entry: result){
            CPPUNIT_ASSERT_EQUAL(count[entry.first], entry.second);
        }
    }
}
template<typename Tp>
void IMathModuleTestClass::countByValueTest(int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(cores);
    auto np = executor_data->mpi().executors();

    auto elems = IElements<std::pair<Tp, Tp>>().create(100 * cores * 2 * np, 0);

    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    registerType<std::pair<Tp, Tp>>();

    math->countByValue();

    auto result = getFromPartitions<std::pair<Tp, int64_t>>();
    loadToPartitions(result, 1);
    executor_data->mpi().gather(*((*executor_data->getPartitions<std::pair<Tp, int64_t>>())[0]), 0);
    result = getFromPartitions<std::pair<Tp, int64_t>>();

    if (executor_data->mpi().isRoot(0)) {
        std::unordered_map<Tp, int64_t> count;
        for (auto &entry : elems) { count[entry.second]++; }
        CPPUNIT_ASSERT_EQUAL(count.size(), result.size());

        for(auto &entry: result){
            CPPUNIT_ASSERT_EQUAL(count[entry.first], entry.second);
        }
    }
}

#undef IMathModuleTestClass