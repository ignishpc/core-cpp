
#include "IBaseImpl.h"

using namespace ignis::executor::core::modules::impl;

IBaseImpl::IBaseImpl(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

void IBaseImpl::registerType(const std::shared_ptr<selector::ITypeSelector> &type) {
    executor_data->registerType(type);
}

IBaseImpl::~IBaseImpl() {}