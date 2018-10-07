
#include "IObjectTest.h"

using namespace ignis::executor::core::storage;

void IObjectTest::setUp() {
    object = createObject();
    auto manager = std::make_shared<api::IManager<std::string>>();
    //object->setManager((std::shared_ptr<api::IManager<IObject::Any>>&)manager);
}

void IObjectTest::testIterator() {



}

void IObjectTest::tearDown() {
    object.reset();
}