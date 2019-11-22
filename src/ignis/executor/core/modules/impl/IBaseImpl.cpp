
#include "IBaseImpl.h"

using namespace ignis::executor::core::modules::impl;

IBaseImpl::IBaseImpl(std::shared_ptr<IExecutorData> &executor_data) : executor_data(executor_data) {}

IBaseImpl::~IBaseImpl() {}