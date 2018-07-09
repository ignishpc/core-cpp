
#include "IDriverModule.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;

IDriverModule::IDriverModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

IDriverModule::~IDriverModule() {

}
