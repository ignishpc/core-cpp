
#include "IPostmanModuleTest.h"

using namespace ignis::executor::core::modules;

void IPostmanModuleTest::setUp() {
    postman_module = std::make_shared<IPostmanModule>(executor_data);
    executor_data->getContext()["ignis.executor.storage"] = "rawMemory";
    executor_data->getContext()["ignis.executor.storage.compression"] = "6";
    executor_data->getContext()["ignis.executor.transport.port"] = "54321";
    executor_data->getContext()["ignis.executor.transport.threads"] = "4";
    executor_data->getContext()["ignis.executor.transport.compression"] = "1";
    executor_data->getContext()["ignis.executor.transport.reconnections"] = "0";
}

void IPostmanModuleTest::tearDown() {

}

void IPostmanModuleTest::test(std::string addr) {
    auto manager = std::make_shared<api::IManager<int>>();
    auto object = postman_module->getIObject((std::shared_ptr<api::IManager<storage::IObject::Any>> &) manager);
    size_t id = 2000;

    auto writer = object->writeIterator();
    std::vector<int> input;
    std::srand(0);
    for (int i = 0; i < 1001; i++) {
        int value = std::rand() % 100;
        input.push_back(value);
        writer->write((storage::IObject::Any &) value);
    }
    executor_data->getPostBox().newOutMessage(id, IMessage(addr, object));
    postman_module->start();

    postman_module->sendAll();
    postman_module->stop();

    CPPUNIT_ASSERT(executor_data->getPostBox().getOutBox().empty());
    auto msg = executor_data->getPostBox().popInBox();
    CPPUNIT_ASSERT(!msg.empty());
    CPPUNIT_ASSERT(msg.find(id) != msg.end());

    auto result = msg[id].getObj();
    result->setManager(object->getManager());
    auto reader = result->readIterator();

    CPPUNIT_ASSERT_EQUAL(input.size(), result->getSize());
    for (auto &elem:input) {
        CPPUNIT_ASSERT_EQUAL(elem, (int &) reader->next());
    }
}

void IPostmanModuleTest::localTest() {
    test("local");
}

void IPostmanModuleTest::socketTest() {
    test("socket!localhost!" + executor_data->getContext()["ignis.executor.transport.port"]);
}

void IPostmanModuleTest::unixSocketTest() {
    test("unixSocket!/tmp/testIgnis.socket");
}

void IPostmanModuleTest::memoryBufferTest() {
    std::string path = "./";
    std::string block_size = std::to_string(10 * 1024);
    test("memoryBuffer!localhost!" + executor_data->getContext()["ignis.executor.transport.port"] + "!" + path + "!" +
         block_size);
}
