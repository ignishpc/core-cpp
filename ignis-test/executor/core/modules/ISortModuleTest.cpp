
#include "ISortModuleTest.h"

using namespace ignis::executor::core::modules;


void ISortModuleTest::setUp() {
    sort_module = std::make_shared<ISortModule>(executor_data);
    executor_data->getContext()["ignis.executor.storage"] = "rawMemory";
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

void ISortModuleTest::sort() {
    auto manager = std::make_shared<api::IManager<int>>();
    size_t executors = 3;
    std::vector<std::vector<int>> input;
    std::vector<std::shared_ptr<storage::IObject>> object;
    std::vector<std::shared_ptr<storage::iterator::ICoreWriteIterator<storage::IObject::Any>>> writer;
    std::vector<decltype(executor_data->getPostBox().popOutBox())> msgs;
    std::vector<std::string> nodes;
    input.resize(executors);
    object.resize(executors);
    writer.resize(executors);
    msgs.resize(executors);

    std::srand(0);
    for (int e = 0; e < executors; e++) {
        nodes.push_back(std::to_string(e));
        input[e] = std::vector<int>();
        object[e] = sort_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
        writer[e] = object[e]->writeIterator();
        for (int i = 0; i < 10 + e; i++) {
            int value = std::rand() % 100;
            input[e].push_back(value);
            writer[e]->write((storage::IObject::Any &) value);
        }

    }

    for (int e = 0; e < executors; e++) {
        executor_data->loadObject(object[e]);
        sort_module->localSort(true);
        object[e] = executor_data->loadObject();
    }

    for (int e = 0; e < executors; e++) {
        executor_data->loadObject(object[e]);
        sort_module->sampling(executors, e, "addr0");
        msgs[e] = executor_data->getPostBox().popOutBox();
        CPPUNIT_ASSERT_EQUAL(executors, msgs[e][e].getObj()->getSize());
    }

    for (int e = 0; e < executors; e++) {
        executor_data->getPostBox().newInMessage(e, msgs[e][e]);
    }

    sort_module->getPivots();
    CPPUNIT_ASSERT_EQUAL(executors * executors, executor_data->loadObject()->getSize());
    sort_module->localSort(true);
    sort_module->findPivots(nodes);

    auto msgs_master = executor_data->getPostBox().popOutBox();
    CPPUNIT_ASSERT_EQUAL(executors -1, msgs_master.begin()->second.getObj()->getSize());

    for (int e = 0; e < executors; e++) {
        executor_data->getPostBox().newInMessage(e, msgs_master[e]);
        executor_data->loadObject(object[e]);
        sort_module->exchangePartitions(e, nodes);
        msgs[e] = executor_data->getPostBox().popOutBox();
    }

    for (int e = 0; e < executors; e++) {
        executor_data->loadObject(object[e]);
        for (auto &msgs_ex: msgs) {
            for (auto &msg:msgs_ex) {
                if (msg.second.getAddr() == nodes[e]) {
                    executor_data->getPostBox().newInMessage(msg.first, msg.second);
                }
            }
        }
        sort_module->mergePartitions();
        sort_module->localSort(true);
        object[e] = executor_data->loadObject();
    }

    size_t zero = 0;
    for (int e = 0; e < executors; e++) {
        zero += input[e].size()  - object[e]->getSize();
    }
    CPPUNIT_ASSERT_EQUAL(0ul, zero);

    int last = (int&)object[0]->readIterator()->next();
    for (int e = 0; e < executors; e++) {
        auto reader = object[e]->readIterator();
        while(reader->hasNext()){
            int next = (int &) reader->next();
            CPPUNIT_ASSERT_LESSEQUAL(next, last);
            last = next;

        }
    }

}
