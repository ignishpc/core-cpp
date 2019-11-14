
#include "IGeneralActionModule.h"

using namespace ignis::executor::core::modules;


IGeneralActionModule::IGeneralActionModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData),
                                                                                           reduce_impl(executorData) {}

IGeneralActionModule::~IGeneralActionModule() {}

void IGeneralActionModule::reduce(const rpc::ISource &function) {
    executor_data->loadLibrary(function)->general_action->reduce(reduce_impl);
}

void IGeneralActionModule::treeReduce(const rpc::ISource &function, int64_t depth) {
    executor_data->loadLibrary(function)->general_action->treeReduce(reduce_impl, depth);
}

void IGeneralActionModule::collect() {}

void IGeneralActionModule::aggregate(const rpc::ISource &seqOp, const rpc::ISource &combOp) {}

void IGeneralActionModule::treeAggregate(const rpc::ISource &seqOp, const rpc::ISource &combOp, int64_t depth) {}

void IGeneralActionModule::fold(const rpc::ISource &function) {}

void IGeneralActionModule::take(int64_t num) {}

void IGeneralActionModule::foreach(const rpc::ISource &function) {}

void IGeneralActionModule::foreachPartition(const rpc::ISource &function) {}

void IGeneralActionModule::top(int64_t num) {}

void IGeneralActionModule::top(int64_t num, const rpc::ISource &comp) {}