
#include "IModule.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMemoryObject.h"
#include "../ICompileManager.h"
#include "../../api/IValue.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core::storage;

IgnisModule::IgnisModule(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

std::shared_ptr<IObject> IgnisModule::getIObject(size_t bytes) {
    return getIObject(std::shared_ptr<api::IManager<IObject::Any>>(), 0, bytes);
}

std::shared_ptr<IObject>
IgnisModule::getIObject(const std::shared_ptr<api::IManager<IObject::Any>> &m, size_t elems, size_t bytes) {
    std::string storage = executor_data->getParser().getString("ignis.executor.storage");
    return getIObject(m, elems, bytes, storage);
}

std::shared_ptr<IObject>
IgnisModule::getIObject(const std::shared_ptr<api::IManager<IObject::Any>> &m, size_t elems, size_t bytes,
                        const std::string &type) {
    std::shared_ptr<IObject> object;
    if (false) {
        int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
        object = std::make_shared<IRawMemoryObject>(compression, bytes);
    } else if (type == IRawMemoryObject::TYPE || !m) {
        int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
        object = std::make_shared<IRawMemoryObject>(compression, bytes);
    } else {
        object = std::make_shared<IMemoryObject>(m, elems);
    }
    if (m) {
        object->setManager(m);
    }
    return object;
}

std::shared_ptr<ignis::executor::api::IManager<IObject::Any>> IgnisModule::getManager(IObject &object) {
    auto manager = object.getManager();
    if (!manager) {
        IGNIS_LOG(warning) << "IModule binary data has not type, trying to compile it";
        ICompileManager cm(object);
        auto lib = cm.compile();
        if (lib.length() > 0){
            IGNIS_LOG(info) << "IModule binary data type compiled successfully";
            rpc::ISource source;
            source.__set_name(lib);
            auto value =  loadSource<api::IValue<IObject::Any>>(source);
            return value->type_t();
        }
        IGNIS_LOG(error) << "IModule binary data type compiled fails";
        throw exceptions::IInvalidArgument("C++ requires type this data before using it");
    }
    return manager;
}

std::shared_ptr<IObject> IgnisModule::memoryObject(const std::shared_ptr<IObject> &object) {
    if (object->getType() != storage::IMemoryObject::TYPE) {
        auto object_aux = getIObject(getManager(*object), object->getSize(), 0, storage::IMemoryObject::TYPE);
        object->moveTo(*object_aux);
        return object_aux;
    }
    return object;
}

IgnisModule::~IgnisModule() {

}
