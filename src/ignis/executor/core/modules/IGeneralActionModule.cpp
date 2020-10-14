
#include "IGeneralActionModule.h"
#include "ignis/executor/core/selector/ISelector.h"

using namespace ignis::executor::core::modules;


IGeneralActionModule::IGeneralActionModule(std::shared_ptr<IExecutorData> &executorData)
    : IModule(executorData), pipe_impl(executorData), sort_impl(executorData), reduce_impl(executorData) {}

IGeneralActionModule::~IGeneralActionModule() {}

void IGeneralActionModule::reduce(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general_action->reduce(reduce_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::treeReduce(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general_action->treeReduce(reduce_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::collect() {
    //Do nothing
}

void IGeneralActionModule::aggregate(const rpc::ISource &zero, const rpc::ISource &seqOp, const rpc::ISource &combOp) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(seqOp)->general_action->aggregate(reduce_impl);
    executor_data->loadLibrary(combOp)->general_action->reduce(reduce_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::treeAggregate(const rpc::ISource &zero, const rpc::ISource &seqOp,
                                         const rpc::ISource &combOp) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(seqOp)->general_action->aggregate(reduce_impl);
    executor_data->loadLibrary(combOp)->general_action->treeReduce(reduce_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::fold(const rpc::ISource &zero, const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(function)->general_action->fold(reduce_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::treeFold(const rpc::ISource &zero, const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(function)->general_action->treeFold(reduce_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::take(int64_t num) {
    IGNIS_RPC_TRY()
    typeFromPartition()->take(pipe_impl, num);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::foreach_(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general_action->foreach (pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::foreachPartition(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general_action->foreachPartition(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::top(int64_t num) {
    IGNIS_RPC_TRY()
    typeFromPartition()->top(sort_impl, num);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::top2(int64_t num, const rpc::ISource &comp) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(comp)->general_action->top(sort_impl, num);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::takeOrdered(int64_t num) {
    IGNIS_RPC_TRY()
    typeFromPartition()->takeOrdered(sort_impl, num);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::takeOrdered2(int64_t num, const rpc::ISource &comp) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(comp)->general_action->takeOrdered(sort_impl, num);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::keys() {
    IGNIS_RPC_TRY()
    typeFromPartition()->keys(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralActionModule::values() {
    IGNIS_RPC_TRY()
    typeFromPartition()->values(pipe_impl);
    IGNIS_RPC_CATCH()
}