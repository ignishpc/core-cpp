
#include "IFilesModule.h"
#include "../ILog.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMemoryObject.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;

IFilesModule::IFilesModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IFilesModule::readFile(const int64_t data_id, const std::string &path, const int64_t offset, const int64_t len,
                            const int64_t lines) {
    string storage = executor_data->getContext()["ignis.executor.storage"];
    int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
    shared_ptr<IObject> object;
    auto type_handle = make_shared<data::serialization::ITypeHandleBase<string>>();
    auto cast_type_handle = (std::shared_ptr<data::serialization::ITypeHandleBase<IObject::Any>>&)type_handle;
    if(storage == "disk"){
        storage = "raw memory";//TODO create IDiskObject
        object = make_shared<IRawMemoryObject>(compression, len);
        object->setTypeHandle(cast_type_handle);
    }else if(storage == "raw memory"){
        storage = "raw memory";
        object = make_shared<IRawMemoryObject>(compression, len);
        object->setTypeHandle(cast_type_handle);
    }else{
        storage = "memory";
        object = make_shared<IMemoryObject>(cast_type_handle, lines);
    }
    IGNIS_LOG(info) << "IFileModule reading id: " << data_id
                    << " path: " << path
                    << " offset: " << offset
                    << " len: " << len
                    << " storage: " << storage
                    << " compression: " << compression;
    //TODO

}

void IFilesModule::saveFile(const int64_t data_id, const std::string &path, const bool joined) {

}

void IFilesModule::saveJson(const int64_t data_id, const std::string &path) {

}

IFilesModule::~IFilesModule() {

}
