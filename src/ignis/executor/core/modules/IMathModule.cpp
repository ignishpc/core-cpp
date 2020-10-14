
#include "IMathModule.h"

using namespace ignis::executor::core::modules;

IMathModule::IMathModule(std::shared_ptr<IExecutorData> &executorData)
    : IModule(executorData), math_impl(executorData), sort_impl(executorData) {}

IMathModule::~IMathModule() {}

void IMathModule::sample(const bool withReplacement, const double fraction, const int32_t seed) {
    IGNIS_RPC_TRY()
    typeFromPartition()->sample(math_impl, withReplacement, fraction, seed);
    IGNIS_RPC_CATCH()
}

void IMathModule::takeSample(const bool withReplacement, const int64_t num, const int32_t seed) {
    IGNIS_RPC_TRY()
    typeFromPartition()->takeSample(math_impl, withReplacement, num, seed);
    IGNIS_RPC_CATCH()
}

int64_t IMathModule::count() {
    IGNIS_RPC_TRY()
    return typeFromPartition()->count(math_impl);
    IGNIS_RPC_CATCH()
}

void IMathModule::max() {
    IGNIS_RPC_TRY()
    typeFromPartition()->max(sort_impl);
    IGNIS_RPC_CATCH()
}

void IMathModule::min() {
    IGNIS_RPC_TRY()
    typeFromPartition()->min(sort_impl);
    IGNIS_RPC_CATCH()
}

void IMathModule::max1(const rpc::ISource &cmp) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(cmp)->general_action->max(sort_impl);
    IGNIS_RPC_CATCH()
}

void IMathModule::min1(const rpc::ISource &cmp) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(cmp)->general_action->min(sort_impl);
    IGNIS_RPC_CATCH()
}

void IMathModule::sampleByKey(const bool withReplacement, const rpc::ISource &fractions, const int32_t seed) {
    IGNIS_RPC_TRY()
    executor_data->loadParameters(fractions);
    typeFromPartition()->sampleByKey(math_impl, withReplacement, seed);
    IGNIS_RPC_CATCH()
}

void IMathModule::countByKey() {
    IGNIS_RPC_TRY()
    typeFromPartition()->countByKey(math_impl);
    IGNIS_RPC_CATCH()
}

void IMathModule::countByValue() {
    IGNIS_RPC_TRY()
    typeFromPartition()->countByValue(math_impl);
    IGNIS_RPC_CATCH()
}
