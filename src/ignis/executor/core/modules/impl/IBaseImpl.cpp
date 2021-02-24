
#include "IBaseImpl.h"

using namespace ignis::executor::core::modules::impl;

IBaseImpl::IBaseImpl(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

void IBaseImpl::registerType(const std::shared_ptr<selector::ITypeSelector> &type) {
    executor_data->registerType(type);
}

std::vector<int64_t> IBaseImpl::targetsChunk(int64_t n, int64_t chunks) {
    int64_t block = n / chunks;
    int64_t remainder = n % chunks;
    std::vector<int64_t> result;

    for (int64_t i = 0; i < chunks; i++) {
        result.resize(result.size() + block, i);
        if (i < remainder) { result.push_back(i); }
    }
    return result;
}

IBaseImpl::~IBaseImpl() {}