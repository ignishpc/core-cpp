
#include "ICacheImpl.h"

#define ICacheImplClass ignis::executor::core::modules::impl::ICacheImpl

template<typename Tp>
void ICacheImplClass::cache(const int64_t id, const int8_t level) {
    IGNIS_TRY()
    std::shared_ptr<storage::IPartitionGroup<Tp>> group_cache;
    if (level == 0) {//NO_CACHE
        auto value = _cache.find(id);
        if (value == _cache.end()) {
            IGNIS_LOG(warning) << "CacheContext: removing non existent cache " << id;
            return;
        }
        group_cache = std::static_pointer_cast<storage::IPartitionGroup<Tp>>(value->second);
        if (group_cache->partitions() == 0) {
            IGNIS_LOG(info) << "CacheContext: removing partition in cache";
        } else {
            IGNIS_LOG(info) << "CacheContext: removing partition in " << (*group_cache)[0]->type() << " cache";
        }
        _cache.erase(value);
        auto group = std::static_pointer_cast<storage::IPartitionGroup<Tp>>(value->second);

        if (executor_data->getPartitionTools().isDisk(*group)) {
            std::vector<std::string> lines;
            std::fstream file_cache(fileCache(), std::ios::binary | std::ios::in);
            std::string line;
            bool flag = false;
            while (!file_cache.eof()) {
                std::getline(file_cache, line, '\n');
                if(!flag){
                    std::string line_id = line.c_str();//std::string ignore all chars after \0
                    if (std::stol(line_id) == id) {
                        flag = true;
                        continue;
                    }
                }
                lines.push_back(line);
            }
            file_cache.close();
            file_cache.open(fileCache(), std::ios::binary | std::ios::out | std::ios::trunc);
            for (auto &line : lines) { file_cache << line << "\n"; }
            file_cache.close();
        }
        return;
    }

    group_cache = executor_data->getPartitions<Tp>();
    int8_t level_sel = level;

    if (level == 1) {//PRESERVE
        if (executor_data->getPartitionTools().isDisk(*group_cache)) {
            level_sel = 4;
        } else if (executor_data->getPartitionTools().isRawMemory(*group_cache)) {
            level_sel = 3;
        } else {
            level_sel = 2;
        }
    }

    if (level_sel == 2) {//MEMORY
        IGNIS_LOG(info) << "CacheContext: saving partition in " << storage::IMemoryPartition<Tp>::TYPE << " cache";
        if (!executor_data->getPartitionTools().isMemory(*group_cache)) {
            auto group = std::make_shared<storage::IPartitionGroup<Tp>>();
            for (int64_t i = 0; i < group_cache->partitions(); i++) {
                auto part = executor_data->getPartitionTools().newMemoryPartition<Tp>((*group)[i]->size());
                (*group_cache)[i]->copyTo(*part);
                group->add(part);
            }
            group_cache = group;
        }
    } else if (level_sel == 3) {//RAW_MEMORY
        IGNIS_LOG(info) << "CacheContext: saving partition in " << storage::IRawMemoryPartition<Tp>::TYPE << " cache";
        if (!executor_data->getPartitionTools().isRawMemory(*group_cache)) {
            auto group = std::make_shared<storage::IPartitionGroup<Tp>>();
            for (int64_t i = 0; i < group_cache->partitions(); i++) {
                auto part = executor_data->getPartitionTools().newRawMemoryPartition<Tp>((*group)[i]->bytes());
                (*group_cache)[i]->copyTo(*part);
                group->add(part);
            }
            group_cache = group;
        }
    } else if (level_sel == 4) {//DISK
        IGNIS_LOG(info) << "CacheContext: saving partition in " << storage::IDiskPartition<Tp>::TYPE << " cache";
        if (!executor_data->getPartitionTools().isDisk(*group_cache)) {
            for (auto &part : *group_cache) {
                auto group = std::make_shared<storage::IPartitionGroup<Tp>>();
                for (int64_t i = 0; i < group_cache->partitions(); i++) {
                    auto part = executor_data->getPartitionTools().newDiskPartition<Tp>("", true);
                    (*group_cache)[i]->copyTo(*part);
                    group->add(part);
                }
                group_cache = group;
            }
        } else {
            for (auto &part : *group_cache) {
                auto &disk = static_cast<storage::IDiskPartition<Tp> &>(*part);
                disk.persist(true);
            }
        }
        std::ofstream file_cache(fileCache(), std::ios_base::app);
        file_cache << std::to_string(id) << 0 << group_cache->elemType().getStandardName();
        for (auto &part : *group_cache) {
            auto &disk = static_cast<storage::IDiskPartition<Tp> &>(*part);
            file_cache << 0 << disk.getPath();
        }
        file_cache << "\n";
    } else {
        throw exception::IInvalidArgument("CacheContext: cache level " + std::to_string(level) + " is not valid");
    }

    group_cache->cache() = true;
    _cache[id] = std::static_pointer_cast<void>(group_cache);

    IGNIS_CATCH()
}

template<typename Tp>
void ICacheImplClass::loadFromDisk(const std::vector<std::string> &groupInfo) {
    IGNIS_TRY()
    auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    auto cmp = executor_data->getProperties().partitionCompression();
    int64_t id = std::stol(groupInfo[0]);
    for (int64_t i = 2; i < groupInfo.size(); i++) {
        group->add(std::make_shared<storage::IDiskPartition<Tp>>(groupInfo[i], cmp, true, true));
    }
    _cache[id] = std::static_pointer_cast<void>(group);
    IGNIS_CATCH()
}

#undef ICacheImplClass