
#include "IGeneralModule.h"

using namespace ignis::executor::core::modules;


IGeneralModule::IGeneralModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData),
                                                                               group_by_impl(executorData),
                                                                               pipe_impl(executorData),
                                                                               sort_impl(executorData) {}

IGeneralModule::~IGeneralModule() {}

void IGeneralModule::map_(const rpc::ISource &function) {
    executor_data->loadLibrary(function)->general->map(pipe_impl);
}

void IGeneralModule::filter(const rpc::ISource &function) {
    executor_data->loadLibrary(function)->general->filter(pipe_impl);
}

void IGeneralModule::flatmap(const rpc::ISource &function) {
    executor_data->loadLibrary(function)->general->flatmap(pipe_impl);
}

void IGeneralModule::mapPartitions(const rpc::ISource &function, bool preservesPartitioning) {
    executor_data->loadLibrary(function)->general->mapPartitions(pipe_impl, preservesPartitioning);
}

void IGeneralModule::mapPartitionsWithIndex(const rpc::ISource &function, bool preservesPartitioning) {
    executor_data->loadLibrary(function)->general->mapPartitionsWithIndex(pipe_impl, preservesPartitioning);
}

void IGeneralModule::applyPartition(const rpc::ISource &function) {
    executor_data->loadLibrary(function)->general->applyPartition(pipe_impl);
}

void IGeneralModule::groupBy(const rpc::ISource &function) {
    executor_data->loadLibrary(function)->general->groupBy(group_by_impl);
}

void IGeneralModule::groupBy(const rpc::ISource &function, int64_t numPartitions) {
    executor_data->loadLibrary(function)->general->groupBy(group_by_impl, numPartitions);
}

void IGeneralModule::sort(bool ascending) {
    typeFromPartition()->sort(sort_impl, ascending);
}

void IGeneralModule::sortBy(const rpc::ISource &function, bool ascending) {
    executor_data->loadLibrary(function)->general->sortBy(sort_impl, ascending);
}


