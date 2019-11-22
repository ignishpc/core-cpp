
#include "IGeneralModule.h"
#include "ignis/executor/core/selector/ISelector.h"

using namespace ignis::executor::core::modules;


IGeneralModule::IGeneralModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData),
                                                                               group_by_impl(executorData),
                                                                               pipe_impl(executorData),
                                                                               sort_impl(executorData) {}

IGeneralModule::~IGeneralModule() {}

void IGeneralModule::map_(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->map(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::filter(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->filter(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::flatmap(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->flatmap(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapPartitions(const rpc::ISource &function, bool preservesPartitioning) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->mapPartitions(pipe_impl, preservesPartitioning);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapPartitionsWithIndex(const rpc::ISource &function, bool preservesPartitioning) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->mapPartitionsWithIndex(pipe_impl, preservesPartitioning);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::applyPartition(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->applyPartition(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::groupBy(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->groupBy(group_by_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::groupBy2(const rpc::ISource &function, int64_t numPartitions) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->groupBy(group_by_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sort(bool ascending) {
    IGNIS_RPC_TRY()
        typeFromPartition()->sort(sort_impl, ascending);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sort2(bool ascending, int64_t numPartitions) {
    IGNIS_RPC_TRY()
        typeFromPartition()->sort(sort_impl, ascending, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sortBy(const rpc::ISource &function, bool ascending) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->sortBy(sort_impl, ascending);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sortBy3(const rpc::ISource &function, bool ascending, int64_t numPartitions) {
    IGNIS_RPC_TRY()
        executor_data->loadLibrary(function)->general->sortBy(sort_impl, ascending, numPartitions);
    IGNIS_RPC_CATCH()
}


