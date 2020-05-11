
#include "IExecutorData.h"

#define IExecutorDataClass ignis::executor::core::IExecutorData

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>> IExecutorDataClass::getPartitions(bool no_check) {
    auto group = std::static_pointer_cast<storage::IPartitionGroup < Tp>>
    (partitions);
    if (!group) {
        throw exception::IInvalidArgument("Error: no partition loaded");
    }
    if (!no_check) {
        if (group->partitions() > 0 && (*group)[0]->type() == storage::IVoidPartition::TYPE) {
            IGNIS_LOG(info) << "Creating real partitions from void partitions";
            auto new_group = partition_tools.newPartitionGroup<Tp>(group->partitions());
            for (int64_t i = 0; i < new_group->partitions(); i++) {
                auto &void_partition = reinterpret_cast<storage::IVoidPartition &>(*(*group)[i]);
                void_partition.write(*(*new_group)[i]);
            }
            std::swap(*new_group, *group);
        } else if (group->elemType() != RTTInfo::from<Tp>()) {
            throw exception::IInvalidArgument("Error: " + RTTInfo::from<Tp>().getStandardName() + " is not " +
                                              group->elemType().getStandardName());
        }
    }
    return group;
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>>
IExecutorDataClass::setPartitions(const std::shared_ptr<storage::IPartitionGroup < Tp>>

&group) {
auto old = partitions;
partitions = std::static_pointer_cast<void>(group);
return std::static_pointer_cast<storage::IPartitionGroup < Tp>>(old);
}

template<typename Tp>
void IExecutorDataClass::setVariable(const std::string key, const Tp &value) {
    auto it = variables.find(key);
    if (it == variables.end()) {
        variables[key] = std::make_shared<IBasicVariable < Tp>>
        (value);
    } else {
        variables[key]->basic<Tp>().get() = value;
    }
}

template<typename Tp>
void IExecutorDataClass::setVariable(const std::string key, Tp &&value) {
    auto it = variables.find(key);
    if (it == variables.end()) {
        variables[key] = std::make_shared<IBasicVariable < Tp>>
        (value);
    } else {
        variables[key]->basic<Tp>().get() = value;
    }
}

template<typename Tp>
Tp &IExecutorDataClass::getVariable(const std::string key) {
    auto it = variables.find(key);
    if (it != variables.end()) {
        return it->second->basic<Tp>().get();
    }
    throw exception::ILogicError("variable " + key + " not found");
}

#undef IExecutorDataClass