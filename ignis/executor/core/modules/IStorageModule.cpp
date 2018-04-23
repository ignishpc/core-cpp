
#include "IStorageModule.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;


IStorageModule::IStorageModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

IStorageModule::~IStorageModule() {

}

void IStorageModule::cache(const int64_t id, const std::string &storage) {
    try {


    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IStorageModule::uncache(const int64_t id) {
    try {


    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IStorageModule::load(const int64_t id) {
    try {


    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}
