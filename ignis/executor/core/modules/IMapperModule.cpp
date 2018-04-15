
#include "IMapperModule.h"

ignis::executor::core::modules::IMapperModule::IMapperModule(
        std::shared_ptr<ignis::executor::core::IExecutorData> &executor_data) : IgnisModule(executor_data) {}
