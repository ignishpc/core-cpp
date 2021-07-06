
#include "IDriverContext.h"
#include "ignis/executor/api/IVector.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"

using namespace ignis::driver::core;
using namespace ignis::executor::core;

IDriverContext::IDriverContext(std::shared_ptr<executor::core::IExecutorData> &executor_data)
    : IModule(executor_data) {}

void IDriverContext::cache(const int64_t id, const int8_t level) {}

void IDriverContext::loadCache(const int64_t id) {}

IDriverContext::~IDriverContext() {}

int64_t IDriverContext::saveContext() {
    IGNIS_RPC_TRY()
    std::lock_guard<std::recursive_mutex> lock(mutex);
    auto id = next_context_id++;
    context[id] = std::static_pointer_cast<void>(executor_data->getPartitions<char>(true));
    return id;
    IGNIS_RPC_CATCH()
}

void IDriverContext::clearContext() {
    IGNIS_RPC_TRY()
    std::lock_guard<std::recursive_mutex> lock(mutex);
    executor_data->deletePartitions();
    executor_data->clearVariables();
    IGNIS_RPC_CATCH()
}

void IDriverContext::loadContext(const int64_t id) {
    IGNIS_RPC_TRY()
    std::lock_guard<std::recursive_mutex> lock(mutex);
    auto value = context.find(id);
    if (value == context.end()) { throw exception::IInvalidArgument("context " + std::to_string(id) + " not found"); }
    executor_data->setPartitions<char>(std::static_pointer_cast<storage::IPartitionGroup<char>>(value->second));
    IGNIS_RPC_CATCH()
}

int64_t IDriverContext::parallelize(const std::vector<bool> &&collection) {
    try {
        auto group = executor_data->getPartitionTools().newPartitionGroup<bool>();
        executor::api::IVector<bool> v(collection.begin(), collection.end());
        auto partition = std::make_shared<executor::core::storage::IMemoryPartition<bool>>(std::move(v));
        group->add(partition);

        std::lock_guard<std::recursive_mutex> lock(mutex);
        executor_data->setPartitions<bool>(group);
        return this->saveContext();
    } catch (executor::core::exception::IException &ex) {
        throw api::IDriverException(ex.what(), ex.toString());
    } catch (std::exception &ex) { throw api::IDriverException(ex.what()); }
}