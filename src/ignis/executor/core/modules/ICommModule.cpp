
#include "ICommModule.h"

using namespace ignis::executor::core::modules;

ICommModule::ICommModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData) {}

ICommModule::~ICommModule() {}
