
#include "IDriverContext.h"

#include "ignis/driver/api/IDriverException.h"
#include "ignis/executor/core/selector/ITypeSelector.h"
#include "ignis/rpc/driver/IDriverException_types.h"

#define IDriverContextClass ignis::driver::core::IDriverContext

template<typename Tp>
ignis::rpc::ISource IDriverContextClass::registerType() {
    IGNIS_RPC_TRY()
    auto type = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<Tp>>();
    executor_data->registerType(type);
    rpc::ISource src;
    src.obj.__set_name(type->info().getStandardName());
    return src;
    IGNIS_RPC_CATCH()
}

template<typename C>
int64_t IDriverContextClass::parallelize(const C &collection) {
    try {
        auto elems = collection.size();
        auto group = executor_data->getPartitionTools().newPartitionGroup<typename C::value_type>();
        auto partition = std::make_shared<executor::core::storage::IMemoryPartition<typename C::value_type>>(elems);
        auto writer = partition->writeIterator();
        auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
        auto it = collection.begin();

        for (int64_t i = 0; i < elems; i++) {
            men_writer.write(*it);
            it++;
        }

        group->add(partition);

        std::lock_guard<std::mutex> lock(mutex);
        executor_data->setPartitions<typename C::value_type>(group);
        return this->saveContext();
    } catch (executor::core::exception::IException &ex) {
        throw api::IDriverException(ex.what(), ex.toString());
    } catch (std::exception &ex) { throw api::IDriverException(ex.what()); }
}

template<typename Tp>
int64_t IDriverContextClass::parallelize(const std::vector<Tp> &&collection) {
    return parallelize<Tp>(static_cast<const executor::api::IVector<Tp> &&>(collection));
}

template<typename Tp>
int64_t IDriverContextClass::parallelize(const executor::api::IVector<Tp> &&collection) {
    try {
        auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>();
        auto partition = std::make_shared<executor::core::storage::IMemoryPartition<Tp>>(collection);
        group->add(partition);

        std::lock_guard<std::mutex> lock(mutex);
        executor_data->setPartitions<Tp>(group);
        return this->saveContext();
    } catch (executor::core::exception::IException &ex) {
        throw api::IDriverException(ex.what(), ex.toString());
    } catch (std::exception &ex) { throw api::IDriverException(ex.what()); }
}

template<typename Tp>
ignis::executor::api::IVector<Tp> IDriverContextClass::collect(int64_t id) {
    try {
        std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>> group;
        {
            std::lock_guard<std::mutex> lock(mutex);
            this->loadContext(id);
            group = executor_data->getPartitions<Tp>();
            executor_data->deletePartitions();
        }
        int64_t elems = 0;
        for (auto &tp : *group) { elems += tp->size(); }
        executor::api::IVector<Tp> result;
        result.reserve(elems);

        for (auto &tp : *group) {
            auto reader = tp->readIterator();
            while (reader->hasNext()) { result.push_back(std::move(reader->next())); }
        }
        return std::move(result);
    } catch (executor::core::exception::IException &ex) {
        throw api::IDriverException(ex.what(), ex.toString());
    } catch (std::exception &ex) { throw api::IDriverException(ex.what()); }
}

template<typename Tp>
Tp IDriverContextClass::collect1(int64_t id) {
    return collect<Tp>(id)[0];
}