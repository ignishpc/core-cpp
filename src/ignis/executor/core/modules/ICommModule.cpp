
#include "ICommModule.h"

using namespace ignis::executor::core::modules;

ICommModule::ICommModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData), impl(executor_data) {}

ICommModule::~ICommModule() {}

void ICommModule::createGroup(std::string &_return) {
    IGNIS_RPC_TRY()
        _return = impl.createGroup();
    IGNIS_RPC_CATCH()
}

void ICommModule::joinGroupMembers(const std::string &group, const int64_t id, const int64_t size) {
    IGNIS_RPC_TRY()
        impl.joinGroupMembers(group, id, size);
    IGNIS_RPC_CATCH()
}

void ICommModule::joinToGroup(const std::string &group, const std::string &id) {
    IGNIS_RPC_TRY()
        impl.joinToGroup(group, id);
    IGNIS_RPC_CATCH()
}

bool ICommModule::hasGroup(const std::string &id) {
    IGNIS_RPC_TRY()
        return impl.hasGroup(id);
    IGNIS_RPC_CATCH()
}

void ICommModule::destroyGroup(const std::string &id) {
    IGNIS_RPC_TRY()
        impl.destroyGroup(id);
    IGNIS_RPC_CATCH()
}

void ICommModule::destroyGroups() {
    IGNIS_RPC_TRY()
        impl.destroyGroups();
    IGNIS_RPC_CATCH()
}

int8_t ICommModule::getProtocol(){
    IGNIS_RPC_TRY()
        impl.getProtocol();
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

void ICommModule::driverGather(const std::string &id, const ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
        if(executor_data->hasPartitions()){
            typeFromPartition()->driverGather(impl, id);
        }else{
            typeFromSource(src)->driverGather(impl, id);
        }
    IGNIS_RPC_CATCH()
}

void ICommModule::driverGather0(const std::string &id, const ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
        if(executor_data->hasPartitions()){
            typeFromPartition()->driverGather0(impl, id);
        }else{
            typeFromSource(src)->driverGather0(impl, id);
        }
    IGNIS_RPC_CATCH()
}

void ICommModule::driverScatter(const std::string &id, int64_t partitions) {
    IGNIS_RPC_TRY()
        if(executor_data->hasPartitions()){
            typeFromPartition()->driverScatter(impl, id, partitions);
        }else{
            impl.driverScatterVoid(id, partitions);
        }
    IGNIS_RPC_CATCH()
}

void ICommModule::driverScatter3(const std::string &id, int64_t partitions, const ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
        if(executor_data->hasPartitions()){
            typeFromPartition()->driverScatter(impl, id, partitions);
        }else {
            typeFromSource(src)->driverScatter(impl, id, partitions);
        }
    IGNIS_RPC_CATCH()
}
