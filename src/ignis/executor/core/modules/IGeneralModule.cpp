
#include "IGeneralModule.h"

using namespace ignis::executor::core::modules;


IGeneralModule::IGeneralModule(std::shared_ptr<IExecutorData> &executorData)
    : IModule(executorData), pipe_impl(executorData), sort_impl(executorData), reduce_impl(executorData),
      repartition_impl(executorData) {}

IGeneralModule::~IGeneralModule() {}

void IGeneralModule::executeTo(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general->executeTo(pipe_impl);
    IGNIS_RPC_CATCH()
}

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

void IGeneralModule::keyBy(const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->general->keyBy(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapWithIndex(const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->general->mapWithIndex(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapPartitions(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general->mapPartitions(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapPartitionsWithIndex(const rpc::ISource &function) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(function)->general->mapPartitionsWithIndex(pipe_impl);
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

void IGeneralModule::union_(const std::string &other, const bool preserveOrder) {
    IGNIS_RPC_TRY()
    typeFromPartition()->union_(reduce_impl, other, preserveOrder);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::union2(const std::string &other, const bool preserveOrder, const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->union_(reduce_impl, other, preserveOrder);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::join(const std::string &other, const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    typeFromPartition()->join(reduce_impl, other, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::join3(const std::string &other, const int64_t numPartitions, const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->join(reduce_impl, other, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::distinct(const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    typeFromPartition()->distinct(reduce_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::distinct2(const int64_t numPartitions, const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->distinct(reduce_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::repartition(const int64_t numPartitions, const bool preserveOrdering, const bool global_) {
    IGNIS_RPC_TRY()
    typeFromPartition()->repartition(repartition_impl, numPartitions, preserveOrdering, global_);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::partitionByRandom(const int64_t numPartitions, const int32_t seed) {
    IGNIS_RPC_TRY()
    typeFromPartition()->partitionByRandom(repartition_impl, numPartitions, seed);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::partitionByHash(const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    typeFromPartition()->partitionByHash(repartition_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::partitionBy(const rpc::ISource &src, const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->general->partitionBy(repartition_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::flatMapValues(const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->key->flatMapValues(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::mapValues(const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->key->mapValues(pipe_impl);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::groupByKey(const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    typeFromPartition()->groupByKey(reduce_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::groupByKey2(const int64_t numPartitions, const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->groupByKey(reduce_impl, numPartitions);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::reduceByKey(const rpc::ISource &src, const int64_t numPartitions, bool localReduce) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->key->reduceByKey(reduce_impl, numPartitions, localReduce);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::aggregateByKey(const rpc::ISource &zero, const rpc::ISource &seqOp, const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(seqOp)->key->aggregateByKey(reduce_impl, numPartitions, true);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::aggregateByKey4(const rpc::ISource &zero, const rpc::ISource &seqOp, const rpc::ISource &combOp,
                                     const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(zero)->general_action->zero(reduce_impl);
    executor_data->loadLibrary(seqOp)->key->aggregateByKey(reduce_impl, numPartitions, false);
    executor_data->loadLibrary(combOp)->key->reduceByKey(reduce_impl, numPartitions, false);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::foldByKey(const rpc::ISource &zero, const rpc::ISource &src, const int64_t numPartitions,
                               bool localFold) {
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

void IGeneralModule::sortByKey2b(const rpc::ISource &src, const bool ascending) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->value->sortByKey(sort_impl, ascending);
    IGNIS_RPC_CATCH()
}

void IGeneralModule::sortByKey3(const rpc::ISource &src, const bool ascending, const int64_t numPartitions) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->value->sortByKey(sort_impl, ascending, numPartitions);
    IGNIS_RPC_CATCH()
}
