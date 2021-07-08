
#include "IDriverContext.h"
#include "ignis/executor/api/IVector.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"

using namespace ignis::driver::core;
using namespace ignis::executor::core;

IDriverContext::IDriverContext(std::shared_ptr<executor::core::IExecutorData> &executor_data)
    : IModule(executor_data) {}

void IDriverContext::cache(const int64_t id, const int8_t level) {
    IGNIS_RPC_TRY()
        throw exception::ILogicError("Driver does not implement cache");
    IGNIS_RPC_CATCH()
}

void IDriverContext::loadCache(const int64_t id) {
    IGNIS_RPC_TRY()
        std::lock_guard<std::mutex> lock(mutex);
        auto value = data.find(id);
        if (value == data.end()) { throw exception::IInvalidArgument("data " + std::to_string(id) + " not found"); }
        executor_data->setPartitions<char>(std::static_pointer_cast<storage::IPartitionGroup<char>>(value->second()));
    IGNIS_RPC_CATCH()

}

IDriverContext::~IDriverContext() {}

int64_t IDriverContext::addData(const std::function<std::shared_ptr<void>()> &f) {
    auto id = next_id++;
    data[id] = f;
    return id;
}

std::shared_ptr<void> IDriverContext::getContext(int64_t id) {
    auto value = context.find(id);
    if (value == context.end()) { throw exception::IInvalidArgument("context " + std::to_string(id) + " not found"); }
    context.erase(value);
    return value->second;
}

int64_t IDriverContext::saveContext() {
    IGNIS_RPC_TRY()
    std::lock_guard<std::mutex> lock(mutex);
    auto id = next_id++;
    context[id] = std::static_pointer_cast<void>(executor_data->getPartitions<char>(true));
    return id;
    IGNIS_RPC_CATCH()
}

void IDriverContext::clearContext() {
    IGNIS_RPC_TRY()
    std::lock_guard<std::mutex> lock(mutex);
    executor_data->deletePartitions();
    executor_data->clearVariables();
    IGNIS_RPC_CATCH()
}

void IDriverContext::loadContext(const int64_t id) {
    IGNIS_RPC_TRY()
    std::lock_guard<std::mutex> lock(mutex);
    executor_data->setPartitions<char>(std::static_pointer_cast<storage::IPartitionGroup<char>>(getContext(id)));
    IGNIS_RPC_CATCH()
}

void IDriverContext::loadContextAsVariable(const int64_t id, const std::string &name) {
    IGNIS_RPC_TRY()
    throw exception::ILogicError("Driver does not implement loadContextAsVariable");
    IGNIS_RPC_CATCH()
}

int64_t IDriverContext::parallelize(const std::vector<bool> &&collection) {
    try {

        auto get = [&]() {
            auto group = executor_data->getPartitionTools().newPartitionGroup<bool>();
            executor::api::IVector<bool> v(collection.begin(), collection.end());
            auto partition = std::make_shared<executor::core::storage::IMemoryPartition<bool>>(std::move(v));
            group->add(partition);
            return std::static_pointer_cast<void>(group);
        };

        std::lock_guard<std::mutex> lock(mutex);
        return addData(get);
    } catch (executor::core::exception::IException &ex) {
        throw api::IDriverException(ex.what(), ex.toString());
    } catch (std::exception &ex) { throw api::IDriverException(ex.what()); }
}