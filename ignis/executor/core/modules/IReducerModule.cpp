
#include "IReducerModule.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::executor::IFunction;

IReducerModule::IReducerModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IReducerModule::getKeys(std::vector<std::string> &_return, const int64_t data_id) {

}

void IReducerModule::setExecutorKeys(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id,
                                     const int64_t msg_id) {

}

void IReducerModule::joinData(const std::vector<int64_t> &msg_ids) {

}

void IReducerModule::reduceByKey(const int64_t data_id, const int64_t ge_data_id, const IFunction &funct) {

}

void IReducerModule::reset() {

}

IReducerModule::~IReducerModule() {

}
