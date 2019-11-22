
#include "IGeneralActionModuleTest.h"

#define IGeneralActionModuleTestClass ignis::executor::core::modules::IGeneralActionModuleTest

IGeneralActionModuleTestClass::IGeneralActionModuleTest() :
        generalAction(std::make_shared<IGeneralActionModule>(executor_data)) {}

void IGeneralActionModuleTestClass::setUp() {}

void IGeneralActionModuleTestClass::tearDown() {}

template<typename Tp>
void  IGeneralActionModuleTestClass::reduceTest(const std::string &name, int cores, const std::string &partitionType) {
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
        for (int i = 1; i < elems.size(); i++) {
            expected_result += elems[i];
        }
        CPPUNIT_ASSERT_EQUAL(expected_result, result[0]);
    } else {
        CPPUNIT_ASSERT_EQUAL(0, (int) result.size());
    }
}

#undef IGeneralActionModuleTestClass