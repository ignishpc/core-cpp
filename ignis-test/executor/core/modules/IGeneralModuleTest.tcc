
#include "IGeneralModuleTest.h"

#define IGeneralModuleTestClass ignis::executor::core::modules::IGeneralModuleTest

IGeneralModuleTestClass::IGeneralModuleTest() : general(std::make_shared<IGeneralModule>(executor_data)) {
    auto &props = executor_data->getContext().props();
    props["ignis.modules.sort.samples"] = "2";
}

void IGeneralModuleTestClass::setUp() {}

void IGeneralModuleTestClass::tearDown() {}

template<typename Tp>
void  IGeneralModuleTestClass::sortTest(const std::string &name, int cores, const std::string &partitionType) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto np = executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    executor_data->setCores(cores);
    auto elems = IElements<Tp>().create(100 * cores * 2 * np, 0);
    auto local_elems = rankVector(elems);
    loadToPartitions(local_elems, cores * 2);
    general->sortBy(newSource(name), true);
    auto result = getFromPartitions<Tp>();

    for (int i = 1; i < result.size(); i++) {
        //std::cerr << result[i] << std::endl;
        CPPUNIT_ASSERT_GREATEREQUAL(result[i - 1], result[i]);
    }
}

#undef IGeneralModuleTestClass