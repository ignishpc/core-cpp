
#include "IMapperModule.h"

using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::executor::IFunction;

IMapperModule::IMapperModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IMapperModule::onemap(const int64_t data_id, const int64_t ge_data_id, const IFunction &funct) {

}

void IMapperModule::flatmap(const int64_t data_id, const int64_t ge_data_id, const IFunction &funct) {

}

IMapperModule::~IMapperModule() {

}
