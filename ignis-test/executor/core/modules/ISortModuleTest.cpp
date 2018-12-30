
#include "ISortModuleTest.h"

using namespace ignis::executor::core::modules;


void ISortModuleTest::setUp() {
    sort_module = std::make_shared<ISortModule>(executor_data);
    executor_data->getContext()["ignis.executor.storage"] = "raw memory";
    executor_data->getContext()["ignis.executor.storage.compression"] = "6";
}

void ISortModuleTest::tearDown() {

}

void ISortModuleTest::sequentialLocalSort() {
    executor_data->getContext()["ignis.executor.cores"] = "1";
    localSort();
}

void ISortModuleTest::parallelLocalSort() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    localSort();
}

void ISortModuleTest::localSort() {
    auto manager = std::make_shared<api::IManager<int>>();
    auto object = sort_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);

    auto writer = object->writeIterator();
    std::vector<int> input;
    std::srand(0);
    for (int i = 0; i < 1001; i++) {
        int value = std::rand() % 100;
        input.push_back(value);
        writer->write((storage::IObject::Any &) value);
    }

    executor_data->loadObject(object);
    sort_module->localSort(true);

    auto result = executor_data->loadObject();
    CPPUNIT_ASSERT_EQUAL(input.size(), result->getSize());
    auto reader = result->readIterator();
    int last = (int &) reader->next();
    while (reader->hasNext()) {
        int next = (int &) reader->next();
        CPPUNIT_ASSERT_LESSEQUAL(next, last);
        last = next;
    }

}
