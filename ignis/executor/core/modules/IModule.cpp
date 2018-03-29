
#include "IModule.h"

using namespace ignis::executor::core::modules;

IgnisModule::IgnisModule(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

IgnisModule::~IgnisModule() {

}
