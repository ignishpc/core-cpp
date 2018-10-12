
#include "IModule.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMemoryObject.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;

IgnisModule::IgnisModule(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

std::shared_ptr<IObject> IgnisModule::getIObject(size_t bytes) {
    return getIObject(std::shared_ptr<api::IManager<IObject::Any>>(), 0, bytes);
}

std::shared_ptr<IObject>
IgnisModule::getIObject(const std::shared_ptr<api::IManager<IObject::Any>>& m, size_t elems, size_t bytes) {
    std::string storage = executor_data->getParser().getString("ignis.executor.storage");
    return getIObject(std::shared_ptr<api::IManager<IObject::Any>>(), elems, bytes, storage);
}

std::shared_ptr<IObject>
IgnisModule::getIObject(const std::shared_ptr<api::IManager<IObject::Any>>& m, size_t elems, size_t bytes,const  std::string& type) {
    std::shared_ptr<IObject> object;
    int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
    if (type == "disk" && false) {
        IGNIS_LOG(info) << "Creating Object storage: " << type << ", compression: " << compression;
        object = std::make_shared<IRawMemoryObject>(compression, bytes);
    } else if (type == "raw memory" || !m) {
        IGNIS_LOG(info) << "Creating Object storage: " << type << ", compression: " << compression;
        object = std::make_shared<IRawMemoryObject>(compression, bytes);
    } else {
        IGNIS_LOG(info) << "Creating Object storage: memory";
        object = std::make_shared<IMemoryObject>(m, elems);
    }
    if (m) {
        object->setManager(m);
    }
    return object;
}

IgnisModule::~IgnisModule() {

}
