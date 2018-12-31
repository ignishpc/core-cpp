
#include "IKeysModuleTest.h"

using namespace ignis::executor::core::modules;

void IKeysModuleTest::setUp() {
    keys_module = std::make_shared<IKeysModule>(executor_data);
    executor_data->getContext()["ignis.executor.storage"] = "rawMemory";
    executor_data->getContext()["ignis.executor.storage.compression"] = "6";
}

void IKeysModuleTest::tearDown() {

}

void IKeysModuleTest::reduceByKeyTest() {
    executor_data->getContext()["ignis.executor.cores"] = "8";
    auto manager = std::make_shared<api::IManager<std::pair<std::string, int>>>();
    auto object = keys_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    rpc::ISource f;
    f.__set_name(library + ":reduceByKeyFunction");

    auto writer = object->writeIterator();
    std::unordered_map<std::string, int> reduction;
    std::srand(0);
    for (int i = 0; i < 1001; i++) {
        std::pair<std::string, int> value(std::to_string(rand() % 10), 1);
        reduction[value.first] += value.second;
        writer->write((storage::IObject::Any &) value);
    }

    executor_data->loadObject(object);

    keys_module->reduceByKey(f);

    object = executor_data->loadObject();
    auto reader = object->readIterator();

    CPPUNIT_ASSERT_EQUAL(reduction.size(), object->getSize());
    while (reader->hasNext()) {
        auto &value = (std::pair<std::string, int> &) reader->next();
        CPPUNIT_ASSERT(reduction.find(value.first) != reduction.end());
        CPPUNIT_ASSERT_EQUAL(reduction[value.first], value.second);
    }

    std::vector<int64_t> hashes;
    keys_module->getKeys(hashes);
    CPPUNIT_ASSERT_EQUAL(reduction.size(), hashes.size());

    std::vector<ignis::rpc::executor::IExecutorKeys> executorKeys;
    std::unordered_map<std::string, int> reduction2;
    auto object_rcva = keys_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    auto object_rcvb = keys_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    auto writer_rcva = object_rcva->writeIterator();
    auto writer_rcvb = object_rcvb->writeIterator();

    reader = object->readIterator();
    executorKeys.resize(4);
    for (int i = 0; i < hashes.size(); i++) {
        auto &value = (std::pair<std::string, int> &) reader->next();
        if (hashes[i] % 4 < 2) {
            if (hashes[i] % 2 == 1) {
                executorKeys[0].keys.push_back(hashes[i]);
                reduction2[value.first] = reduction[value.first];
                if (rand() % 2 == 0) {
                    auto value2 = std::pair<std::string, int>(value.first, i);
                    writer_rcva->write((storage::IObject::Any &) value2);
                    reduction2[value.first] += value2.second;
                } else {
                    auto value2 = std::pair<std::string, int>(value.first, i * 10);
                    writer_rcvb->write((storage::IObject::Any &) value2);
                    reduction2[value.first] += value2.second;
                }
            } else {
                executorKeys[1].keys.push_back(hashes[i]);
            }
        } else {
            if (hashes[i] % 2 == 1) {
                executorKeys[2].keys.push_back(hashes[i]);
            } else {
                executorKeys[3].keys.push_back(hashes[i]);
            }
        }
    }
    executorKeys[0].__set_msg_id(0);
    executorKeys[1].__set_msg_id(1);
    executorKeys[2].__set_msg_id(2);
    executorKeys[3].__set_msg_id(3);
    executorKeys[0].__set_addr("local");
    executorKeys[1].__set_addr("ip2");
    executorKeys[2].__set_addr("ip3");
    executorKeys[3].__set_addr("ip4");

    keys_module->prepareKeys(executorKeys);

    for (auto &entry:executor_data->getPostBox().popOutBox()) {
        if (entry.second.getAddr() == "local") {
            executor_data->getPostBox().newInMessage(entry.first, entry.second);
        }
    }
    executor_data->getPostBox().newInMessage(4, IMessage("local", object_rcva));
    executor_data->getPostBox().newInMessage(5, IMessage("local", object_rcvb));

    keys_module->collectKeys();

    keys_module->reduceByKey(f);

    object = executor_data->loadObject();
    reader = object->readIterator();

    CPPUNIT_ASSERT_EQUAL(reduction2.size(), object->getSize());
    while (reader->hasNext()) {
        auto &value = (std::pair<std::string, int> &) reader->next();
        CPPUNIT_ASSERT(reduction2.find(value.first) != reduction2.end());
        CPPUNIT_ASSERT_EQUAL(reduction2[value.first], value.second);
    }
}
