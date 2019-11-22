
#include "IPipeImpl.h"

using namespace ignis::executor::core::modules::impl;

IPipeImpl::IPipeImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

IPipeImpl::~IPipeImpl() {}