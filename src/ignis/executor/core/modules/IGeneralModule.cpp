
#include "IGeneralModule.h"

using namespace ignis::executor::core::modules;


IGeneralModule::IGeneralModule(std::shared_ptr<IExecutorData> &executorData)
    : IModule(executorData), pipe_impl(executorData), sort_impl(executorData), reduce_impl(executorData) {}

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

void IGeneralModule::keyBy(const ::ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->general->keyBy(pipe_impl);
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

void IGeneralModule::mapExecutor(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general->mapExecutor(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapExecutorTo(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general->mapExecutorTo(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::groupBy(const rpc::ISource &function, int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general->keyBy(pipe_impl);
    typeFromPartition()->groupByKey(reduce_impl, numPartitions);
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

void IGeneralModule::flatMapValues(const ::ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->key->flatMapValues(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapValues(const ::ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->key->mapValues(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::groupByKey(const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    typeFromPartition()->groupByKey(reduce_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::groupByKey2(const int64_t numPartitions, const ::ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->groupByKey(reduce_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::reduceByKey(const ::ignis::rpc::ISource &src, const int64_t numPartitions, bool localReduce) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->key->reduceByKey(reduce_impl, numPartitions, localReduce);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::aggregateByKey(const ::ignis::rpc::ISource &zero, const ::ignis::rpc::ISource &seqOp,
                                    const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(seqOp)->key->aggregateByKey(reduce_impl, numPartitions, true);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::aggregateByKey4(const ::ignis::rpc::ISource &zero, const ::ignis::rpc::ISource &seqOp,
                                     const ::ignis::rpc::ISource &combOp, const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(seqOp)->key->aggregateByKey(reduce_impl, numPartitions, false);
    executor_data->loadLibrary(combOp)->key->reduceByKey(reduce_impl, numPartitions, false);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::foldByKey(const ::ignis::rpc::ISource &zero, const ::ignis::rpc::ISource &src,
                               const int64_t numPartitions, bool localFold) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(src)->key->foldByKey(reduce_impl, numPartitions, localFold);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sortByKey(const bool ascending) {
    IGNIS_RPC_TRY()
    typeFromPartition()->sortByKey(sort_impl, ascending);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sortByKey2a(const bool ascending, const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    typeFromPartition()->sortByKey(sort_impl, ascending, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sortByKey2b(const ::ignis::rpc::ISource &src, const bool ascending) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->value->sortByKey(sort_impl, ascending);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sortByKey3(const ::ignis::rpc::ISource &src, const bool ascending, const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->value->sortByKey(sort_impl, ascending, numPartitions);
    IGNIS_RPC_CATCH()
}
