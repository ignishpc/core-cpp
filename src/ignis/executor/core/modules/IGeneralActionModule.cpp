
#include "IGeneralActionModule.h"
#include "ignis/executor/core/selector/ISelector.h"

using namespace ignis::executor::core::modules;


IGeneralActionModule::IGeneralActionModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData),
                                                                                           reduce_impl(executorData) {}

IGeneralActionModule::~IGeneralActionModule() {}

void IGeneralActionModule::reduce(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general_action->reduce(reduce_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::treeReduce(const rpc::ISource &function, int64_t depth) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general_action->treeReduce(reduce_impl, depth);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::collect() {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::aggregate(const rpc::ISource &seqOp, const rpc::ISource &combOp) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::treeAggregate(const rpc::ISource &seqOp, const rpc::ISource &combOp, int64_t depth) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::fold(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::take(int64_t num) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::foreach_(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::foreachPartition(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::top(int64_t num) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::top2(int64_t num, const rpc::ISource &comp) {
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}