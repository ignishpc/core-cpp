
#include "IShuffleModuleTest.h"

using namespace ignis::executor::core::modules;
using ignis::rpc::executor::ISplit;

void IShuffleModuleTest::setUp() {
    shuffle_module = std::make_shared<IShuffleModule>(executor_data);
    executor_data->getContext()["ignis.executor.storage"] = "raw memory";
    executor_data->getContext()["ignis.executor.storage.compression"] = "6";
}

void IShuffleModuleTest::tearDown() {

}

ISplit newISplit(int64_t msg_id, const std::string &addr, int64_t length) {
    ISplit split;
    split.__set_msg_id(msg_id);
    split.__set_addr(addr);
    split.__set_length(length);
    return split;
}


void IShuffleModuleTest::shuffle() {
    auto manager = std::make_shared<api::IManager<int>>();
    auto object = shuffle_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);

    auto writer = object->writeIterator();
    std::vector<int> input;
    std::srand(0);
    for (int i = 0; i < 1001; i++) {
        int value = std::rand() % 100;
        input.push_back(value);
        writer->write((storage::IObject::Any &) value);
    }

    executor_data->loadObject(object);

    std::vector<ISplit> splits;
    std::vector<int64_t> order = {1, 3, 2, 4};
    splits.push_back(newISplit(1, "local", input.size() / 4));
    splits.push_back(newISplit(3, "local", input.size() / 4));
    splits.push_back(newISplit(2, "local", input.size() / 4));
    splits.push_back(newISplit(4, "local", input.size() / 4 + input.size() % 4));

    shuffle_module->createSplits(splits);

    auto mgsg = executor_data->getPostBox().popOutBox();
    for(int i=0;i<order.size();i++){
        CPPUNIT_ASSERT_EQUAL(splits[i].length, (int64_t)mgsg[order[i]].getObj()->getSize());
    }

    for(auto& msg : mgsg){
        executor_data->getPostBox().newInMessage(msg.first,msg.second);
    }

    shuffle_module->joinSplits(order);
    auto result = executor_data->getSharedLoadObject();
    CPPUNIT_ASSERT(object != result);
    auto reader = result->readIterator();

    CPPUNIT_ASSERT_EQUAL(input.size(), result->getSize());
    for (auto &elem:input) {
        CPPUNIT_ASSERT_EQUAL(elem, (int &) reader->next());
    }

}


