
#include "IKeysModuleTest.h"

using namespace ignis::executor::core::modules;

void IKeysModuleTest::setUp() {
    keys_module = std::make_shared<IKeysModule>(executor_data);
    executor_data->getContext()["ignis.executor.storage"] = "raw memory";
    executor_data->getContext()["ignis.executor.storage.compression"] = "6";
}

void IKeysModuleTest::tearDown() {

}

void IKeysModuleTest::keysTest() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    auto manager = std::make_shared<api::IPairManager<std::string, int>>();
    auto object = keys_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    rpc::ISource f;
    f.__set_name(library + ":reduceByKeyFunction");

    auto writer = object->writeIterator();
    std::unordered_map<std::string, int> reduction;
    std::srand(0);
    for (int i = 0; i < 100; i++) {
        std::pair<std::string, int> value(std::to_string(rand()%10), 1);
        reduction[value.first] += value.second;
        writer->write((storage::IObject::Any &) value);
    }

    executor_data->loadObject(object);

    keys_module->reduceByKey(f);

    object = executor_data->loadObject();
    auto reader = object->readIterator();

    CPPUNIT_ASSERT_EQUAL(reduction.size(),object->getSize());
    while(reader->hasNext()){
        auto& value = (std::pair<std::string, int>&)reader->next();
        CPPUNIT_ASSERT(reduction.find(value.first) != reduction.end());
        CPPUNIT_ASSERT_EQUAL(reduction[value.first],value.second);
    }
}
