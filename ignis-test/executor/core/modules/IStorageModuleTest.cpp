
#include "IStorageModuleTest.h"
#include "../../../../ignis/data/IMemoryBuffer.h"
#include <unordered_map>

using namespace ignis::executor::core::modules;

void IStorageModuleTest::setUp() {
    executor_data->getContext()["ignis.executor.storage"] = "rawMemory";
    executor_data->getContext()["ignis.executor.storage.compression"] = "6";
    storage_module = std::make_shared<IStorageModule>(executor_data);
    auto manager = std::make_shared<api::IManager<int>>();
    executor_data->
            loadObject(storage_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager));

    std::srand(0);
    auto writer = executor_data->loadObject()->writeIterator();

    for (int i = 0; i < 1001; i++) {
        int value = std::rand() % 100;
        input.push_back(value);
        writer->write((storage::IObject::Any &) value);
    }
}

void IStorageModuleTest::tearDown() {
}

void IStorageModuleTest::take() {
    std::string taked;
    storage_module->take(taked, 1, "", 100, true);
    auto manager = std::make_shared<api::IManager<int>>();
    auto takedObj = storage_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    auto buffer = std::make_shared<data::IMemoryBuffer>((uint8_t *) taked.c_str(), taked.size());
    takedObj->read(buffer);

    auto reader = takedObj->readIterator();
    CPPUNIT_ASSERT_EQUAL(100ul, takedObj->getSize());
    for (int i = 0; i < 100; i++) {
        CPPUNIT_ASSERT_EQUAL(input[i], (int &) reader->next());
    }
}

void IStorageModuleTest::takeSampleWithRemplacement() {
    std::string sample;
    storage_module->takeSample(sample, 1, "", 100, true, 0, true);
    auto manager = std::make_shared<api::IManager<int>>();
    auto sampleObj = storage_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    auto buffer = std::make_shared<data::IMemoryBuffer>((uint8_t *) sample.c_str(), sample.size());
    sampleObj->read(buffer);

    auto reader = sampleObj->readIterator();
    CPPUNIT_ASSERT_EQUAL(100ul, sampleObj->getSize());
    while (reader->hasNext()) {
        CPPUNIT_ASSERT(std::find(input.begin(), input.end(), (int &) reader->next()) != input.end());
    }
}

void IStorageModuleTest::takeSampleWithoutRemplacement() {
    std::string sample;
    storage_module->takeSample(sample, 1, "", 100, false, 0, true);
    auto manager = std::make_shared<api::IManager<int>>();
    auto sampleObj = storage_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    auto buffer = std::make_shared<data::IMemoryBuffer>((uint8_t *) sample.c_str(), sample.size());
    sampleObj->read(buffer);

    std::unordered_map<int, int> count;
    for (int elem:input) {
        count[elem]++;
    }

    auto reader = sampleObj->readIterator();
    CPPUNIT_ASSERT_EQUAL(100ul, sampleObj->getSize());
    while (reader->hasNext()) {
        int value = (int &) reader->next();
        CPPUNIT_ASSERT(std::find(input.begin(), input.end(), value) != input.end());
        CPPUNIT_ASSERT(count[value]-- > 0);
    }
}

void IStorageModuleTest::collect() {
    std::string collected;
    storage_module->collect(collected, 1, "", true);
    auto manager = std::make_shared<api::IManager<int>>();
    auto collectedObj = storage_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    auto buffer = std::make_shared<data::IMemoryBuffer>((uint8_t *) collected.c_str(), collected.size());
    collectedObj->read(buffer);

    auto reader = collectedObj->readIterator();
    CPPUNIT_ASSERT_EQUAL(input.size(), collectedObj->getSize());
    for (auto &elem: input) {
        CPPUNIT_ASSERT_EQUAL(elem, (int &) reader->next());
    }
}


