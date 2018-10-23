
#include "IReducerModule.h"

using namespace ignis::executor::core::modules;

IReducerModule::IReducerModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

IReducerModule::~IReducerModule() {}
