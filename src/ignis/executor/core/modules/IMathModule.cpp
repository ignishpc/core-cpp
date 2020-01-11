
#include "IMathModule.h"

using namespace ignis::executor::core::modules;

IMathModule::IMathModule(std::shared_ptr<IExecutorData> &executorData) : IModule(executorData) {}

IMathModule::~IMathModule() {
}

void IMathModule::sample(const bool withReplacement, const double fraction, const int32_t seed){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IMathModule::takeSample(const bool withReplacement, const double fraction, const int32_t seed){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

int64_t IMathModule::count(){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IMathModule::max(const rpc::ISource &cmp){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}

void IMathModule::min(const rpc::ISource &cmp){
    IGNIS_RPC_TRY()
        //TODO
    IGNIS_RPC_CATCH()
}
