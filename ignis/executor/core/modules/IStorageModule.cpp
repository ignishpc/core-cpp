
#include "IStorageModule.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../ILog.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;


IStorageModule::IStorageModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

IStorageModule::~IStorageModule() {

}

int64_t IStorageModule::count(){
    return executor_data->loadObject()->getSize();
}

void IStorageModule::cache(const int64_t id, const std::string &storage) {
}

void IStorageModule::uncache(const int64_t id) {
    try {
        IGNIS_LOG(info) << "IStorageModule uncache object " << id;
        object_cache.erase(id);
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IStorageModule::restore(const int64_t id) {
    try {
        IGNIS_LOG(info) << "IStorageModule load cache object " << id;
        if (object_cache.find(id) == object_cache.end()) {
            throw exceptions::IInvalidArgument("IStorageModule object not found");
        }
        executor_data->loadObject(object_cache[id]);
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IStorageModule::saveContext(const int64_t id) {
    try {
        object_context[id] = executor_data->loadObject();
        IGNIS_LOG(info) << "IStorageModule context save" << id;
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IStorageModule::loadContext(const int64_t id) {
    try {
        if (object_context.find(id) == object_context.end()) {
            throw exceptions::IInvalidArgument("IStorageModule object not found");
        }
        bool already_loaded = object_context[id] == executor_data->loadObject();

        if(!already_loaded){
            executor_data->loadObject(object_context[id]);
            IGNIS_LOG(info) << "IStorageModule context loaded" << id;
        }
        object_context.erase(id);

    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}
