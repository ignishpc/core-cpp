
#include "ICacheImpl.h"
#include <boost/filesystem.hpp>

using namespace ignis::executor::core::modules::impl;


ICacheImpl::ICacheImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

std::vector<std::vector<std::string>> ICacheImpl::getCacheFromDisk(){
     auto cache = fileCache();
    std::vector<std::vector<std::string>> groups;
    if (boost::filesystem::exists(cache)) {
        IGNIS_LOG(info) << "CacheContext: cache file found, loading";
        std::ifstream file_cache(fileCache(), std::ios::binary);
        std::vector<std::string> parts;
        std::string line;
        while (!file_cache.eof()) {
            parts.clear();
            std::getline(file_cache, line, '\n');
            auto p = line.c_str();
            do {
                parts.push_back(std::string(p));
                p += parts.back().size() + 1;
            } while (p - line.c_str() < line.length());
        }
    }
    return groups;
}

std::string ICacheImpl::fileCache() {
    return executor_data->infoDirectory() + "/cache" + std::to_string(executor_data->getContext().executorId()) +
           ".bak";
}

int64_t ICacheImpl::saveContext() {
    IGNIS_TRY()
        int64_t id;
        if (context.size() <= 10) {//small contexts reuse ids
            for (int i = 0; i <= 10; i++) {
                if (context.find(i) == context.end()) {
                    id = i;
                    break;
                }
            }
        } else {
            id = next_context_id++;
        }
        IGNIS_LOG(info) << "CacheContext: saving context " << id;
        context[id] = std::static_pointer_cast<void>(executor_data->getPartitions<char>(true));
    IGNIS_CATCH()
}

void ICacheImpl::loadContext(const int64_t id) {
    IGNIS_TRY()
        auto value = context.find(id);
        if (value != context.end() && value->second.get() == executor_data->getPartitions<char>(true).get()) {
            return;
        }
        IGNIS_LOG(info) << "CacheContext: loading context " << id;

        if (value == context.end()) {
            throw exception::IInvalidArgument("context " + std::to_string(id) + " not found");
        }
        executor_data->setPartitions<char>(std::static_pointer_cast<storage::IPartitionGroup<char>>(value->second));
    IGNIS_CATCH()
}


void ICacheImpl::loadCache(const int64_t id) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "CacheContext: loading partition from cache";
        auto value = _cache.find(id);
        if (value == _cache.end()) {
            IGNIS_LOG(error) << "CacheContext: cache " << id << " not found";
            throw exception::IInvalidArgument("cache " + std::to_string(id) + " not found");
        }
        executor_data->setPartitions<char>(std::static_pointer_cast<storage::IPartitionGroup<char>>(value->second));
    IGNIS_CATCH()
}