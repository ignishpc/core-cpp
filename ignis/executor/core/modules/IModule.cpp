
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
IgnisModule::getIObject(std::shared_ptr<api::IManager<IObject::Any>> m, size_t lines, size_t bytes) {
    std::string storage = executor_data->getParser().getString("ignis.executor.storage");
    return getIObject(std::shared_ptr<api::IManager<IObject::Any>>(), 0, bytes, storage);
}

std::shared_ptr<IObject>
IgnisModule::getIObject(std::shared_ptr<api::IManager<IObject::Any>> m, size_t lines, size_t bytes, std::string storage) {
    std::shared_ptr<IObject> object;
    int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
    if (storage == "disk" && false) {
        IGNIS_LOG(info) << "Creating Object storage: " << storage << ", compression: " << compression;
        object = std::make_shared<IRawMemoryObject>(compression, bytes);
    } else if (storage == "raw memory" || !m) {
        IGNIS_LOG(info) << "Creating Object storage: " << storage << ", compression: " << compression;
        object = std::make_shared<IRawMemoryObject>(compression, bytes);
    } else {
        IGNIS_LOG(info) << "Creating Object storage: memory";
        object = std::make_shared<IMemoryObject>(m, lines);
    }
    if (m) {
        object->setManager(m);
    }
    return object;
}

IgnisModule::~IgnisModule() {

}
