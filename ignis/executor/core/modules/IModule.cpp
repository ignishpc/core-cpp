
#include "IModule.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMemoryObject.h"

using namespace std;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;

IgnisModule::IgnisModule(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

std::shared_ptr<IObject> IgnisModule::getIObject(size_t bytes) {
    return getIObject(shared_ptr<data::IManager<IObject::Any>>(), bytes, 0);
}

std::shared_ptr<IObject>
IgnisModule::getIObject(shared_ptr<data::IManager<IObject::Any>> m, size_t lines, size_t bytes) {
    shared_ptr<IObject> object;
    string storage = executor_data->getParser().getString("ignis.executor.storage");
    int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
    if (storage == "disk" && false) {
        IGNIS_LOG(info) << "Creating Object storage: " << storage << ", compression: " << compression;
        object = make_shared<IRawMemoryObject>(compression, bytes);
    } else if (storage == "raw memory" || !m) {
        IGNIS_LOG(info) << "Creating Object storage: " << storage << ", compression: " << compression;
        object = make_shared<IRawMemoryObject>(compression, bytes);
    } else {
        IGNIS_LOG(info) << "Creating Object storage: memory";
        object = make_shared<IMemoryObject>(m, lines);
    }
    if (m) {
        object->setManager(m);
    }
    return object;
}

IgnisModule::~IgnisModule() {

}
