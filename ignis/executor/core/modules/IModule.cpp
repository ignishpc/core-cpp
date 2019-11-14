
#include "IModule.h"

using namespace ignis::executor::core::modules;

IModule::IModule(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

IModule::~IModule() {}

std::shared_ptr<ignis::executor::core::selector::IArgsType> IModule::typeFromPartition() {
    IGNIS_LOG(info) << "Cheeking partition type";
    auto type = executor_data->getPartitions<int>()->elemType();
    if (type.isVoid()) {
        throw exception::ILogicError("The C ++ executor cannot identify the type of this partition");
    }
    return executor_data->getType(type.getStandardName());
}
