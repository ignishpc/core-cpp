
#include "IStorageModule.h"
#include "../storage/IObjectWrapper.h"
#include "../ILog.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;


IStorageModule::IStorageModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

IStorageModule::~IStorageModule() {

}

void IStorageModule::cache(const int64_t id, const std::string &storage) {
    try {
        auto loaded = executor_data->getSharedLoadObject();
        if (loaded->getType() == storage) {
            IGNIS_LOG(info) << "IStorageModule cache object " << id;
            executor_data->getStoredObjects()[id] = loaded;
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
        executor_data->getStoredObjects().erase(id);
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

void IStorageModule::load(const int64_t id) {
    try {
        IGNIS_LOG(info) << "IStorageModule load cache object " << id;
        executor_data->loadObject(executor_data->getStoredObjects()[id]);
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
