
#include "IStorageModule.h"
#include "../storage/IObjectWrapper.h"
#include "../../../exceptions/IInvalidArgument.h"
#include "../ILog.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;


IStorageModule::IStorageModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

IStorageModule::~IStorageModule() {

}

int64_t IStorageModule::count(){
    return executor_data->getLoadObject().getSize();
}

void IStorageModule::cache(const int64_t id, const std::string &storage) {
    try {
        auto loaded = executor_data->getSharedLoadObject();
        if (loaded->getType() == storage) {
            IGNIS_LOG(info) << "IStorageModule cache object " << id;
            object_cache[id] = loaded;
        } else {
            IGNIS_LOG(info) << "IStorageModule cache object " << id << " from: " << loaded->getType() << ", to: "
                            << storage;
            auto obj_cache = getIObject(loaded->getManager());
            auto wrapper = std::make_shared<storage::IObjectWrapper>(loaded, obj_cache);
            executor_data->loadObject(wrapper);
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IStorageModule::uncache(const int64_t id) {
    try {
        IGNIS_LOG(info) << "IStorageModule uncache object " << id;
        object_cache.erase(id);
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
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
        iex.__set_stack(ex.toString());
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
        object_context[id] = executor_data->getSharedLoadObject();
        IGNIS_LOG(info) << "IStorageModule context save" << id;
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
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
        bool already_loaded = object_context[id] == executor_data->getSharedLoadObject();

        if(!already_loaded){
            executor_data->loadObject(object_context[id]);
            IGNIS_LOG(info) << "IStorageModule context loaded" << id;
        }
        object_context.erase(id);

    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}
