
#include "ICommModule.h"

using namespace ignis::executor::core::modules;

ICommModule::ICommModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData), impl(executor_data) {}

ICommModule::~ICommModule() {}

void ICommModule::openGroup(std::string &_return) {
    IGNIS_RPC_TRY()
    _return = impl.openGroup();
    IGNIS_RPC_CATCH()
}

void ICommModule::closeGroup() {
    IGNIS_RPC_TRY()
    impl.closeGroup();
    IGNIS_RPC_CATCH()
}

void ICommModule::joinToGroup(const std::string &id, const bool leader) {
    IGNIS_RPC_TRY()
    impl.joinToGroup(id, leader);
    IGNIS_RPC_CATCH()
}

void ICommModule::joinToGroupName(const std::string &id, const bool leader, const std::string &name) {
    IGNIS_RPC_TRY()
    impl.joinToGroupName(id, leader, name);
    IGNIS_RPC_CATCH()
}

bool ICommModule::hasGroup(const std::string &name) {
    IGNIS_RPC_TRY()
    return impl.hasGroup(name);
    IGNIS_RPC_CATCH()
}

void ICommModule::destroyGroup(const std::string &name) {
    IGNIS_RPC_TRY()
    impl.destroyGroup(name);
    IGNIS_RPC_CATCH()
}

void ICommModule::destroyGroups() {
    IGNIS_RPC_TRY()
    impl.destroyGroups();
    IGNIS_RPC_CATCH()
}

int8_t ICommModule::getProtocol() {
    IGNIS_RPC_TRY()
    return impl.getProtocol();
    IGNIS_RPC_CATCH()
}

void ICommModule::getPartitions(std::vector<std::string> &_return, const int8_t protocol) {
    IGNIS_RPC_TRY()
    _return = typeFromPartition()->getPartitions(impl, protocol, 0);
    IGNIS_RPC_CATCH()
}

void ICommModule::getPartitions2(std::vector<std::string> &_return, const int8_t protocol, int64_t minPartitions) {
    IGNIS_RPC_TRY()
    _return = typeFromPartition()->getPartitions(impl, protocol, minPartitions);
    IGNIS_RPC_CATCH()
}

void ICommModule::setPartitions(const std::vector<std::string> &partitions) {
    IGNIS_RPC_TRY()
    impl.setPartitionsVoid(partitions);
    IGNIS_RPC_CATCH()
}

void ICommModule::setPartitions2(const std::vector<std::string> &partitions, const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->setPartitions(impl, partitions);
    IGNIS_RPC_CATCH()
}

void ICommModule::driverGather(const std::string &group, const ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    if (executor_data->hasPartitions()) {
        typeFromPartition()->driverGather(impl, group);
    } else {
        typeFromSource(src)->driverGather(impl, group);
    }
    IGNIS_RPC_CATCH()
}

void ICommModule::driverGather0(const std::string &group, const ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    if (executor_data->hasPartitions()) {
        typeFromPartition()->driverGather0(impl, group);
    } else {
        typeFromSource(src)->driverGather0(impl, group);
    }
    IGNIS_RPC_CATCH()
}

void ICommModule::driverScatter(const std::string &group, int64_t partitions) {
    IGNIS_RPC_TRY()
    if (executor_data->hasPartitions()) {
        typeFromPartition()->driverScatter(impl, group, partitions);
    } else {
        impl.driverScatterVoid(group, partitions);
    }
    IGNIS_RPC_CATCH()
}

void ICommModule::driverScatter3(const std::string &group, int64_t partitions, const ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    if (executor_data->hasPartitions()) {
        typeFromPartition()->driverScatter(impl, group, partitions);
    } else {
        typeFromSource(src)->driverScatter(impl, group, partitions);
    }
    IGNIS_RPC_CATCH()
}

void ICommModule::importData(const std::string &group, const bool source, const int64_t threads) {
    IGNIS_RPC_TRY()
    typeFromPartition()->importData(impl, group, source, threads);
    IGNIS_RPC_CATCH()
}

void ICommModule::importData4(const std::string &group, const bool source, const int64_t threads,
                              const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->importData(impl, group, source, threads);
    IGNIS_RPC_CATCH()
}
