
#include "IStorageModule.h"

ignis::executor::core::modules::IStorageModule::IStorageModule(
        std::shared_ptr<ignis::executor::core::IExecutorData> &executor_data) : IgnisModule(executor_data) {}

ignis::executor::core::modules::IStorageModule::~IStorageModule() {

}
