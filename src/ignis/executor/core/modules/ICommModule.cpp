
#include "ICommModule.h"

using namespace ignis::executor::core::modules;

ICommModule::ICommModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData) {}

ICommModule::~ICommModule() {}

void ICommModule::createGroup(std::string &_return){
    IGNIS_RPC_TRY()
    //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::joinGroupMembers(const std::string &group, const int64_t size){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::joinToGroup(const std::string &group, const std::string &id){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

bool ICommModule::hasGroup(const std::string &id){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::destroyGroup(const std::string &id){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::destroyGroups(){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::getPartitions(std::vector<std::string> &_return){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::setPartitions(const std::vector<std::string> &partitions){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::driverGather(const std::string &id){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::driverGather0(const std::string &id){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void ICommModule::driverScatter(const std::string &id){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}