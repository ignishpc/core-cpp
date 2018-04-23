
#include "IReducerModule.h"
#include "../ILog.h"
#include "../../../exceptions/IInvalidArgument.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::executor::IFunction;
using ignis::rpc::IRemoteException;

IReducerModule::IReducerModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IReducerModule::getKeys(std::vector<int64_t> &_return, const rpc::executor::IFunction &funct) {
    try {
        IGNIS_LOG(info) << "IReducerModule starting reducer";
        function = loadFunction<api::IReducer<storage::IObject::Any, storage::IObject::Any, storage::IObject::Any>>(funct);



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

void IReducerModule::setExecutorKeys(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id,
                                     const int64_t msg_id) {
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

void IReducerModule::joinData(const std::vector<int64_t> &msg_ids) {
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

void IReducerModule::reduceByKey() {
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

void IReducerModule::reset() {
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

IReducerModule::~IReducerModule() {

}
