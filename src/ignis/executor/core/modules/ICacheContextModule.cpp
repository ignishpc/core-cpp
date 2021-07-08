
#include "ICacheContextModule.h"

using namespace ignis::executor::core::modules;

ICacheContextModule::ICacheContextModule(std::shared_ptr<IExecutorData> &executor_data)
    : IModule(executor_data), impl(executor_data) {
    try {
        executor_data->reloadLibraries();
        //load partition cache when the executor has previously crashed
        auto disk_cache = impl.getCacheFromDisk();
        for (auto &group : disk_cache) {
            typeFromName(group[1])->loadFromDisk(impl, group);//group[0] is id, group[1] is type name...
        }
    } catch (exception::IException &ex) { IGNIS_LOG(error) << ex.toString(); } catch (std::exception &ex) {
        IGNIS_LOG(error) << ex.what();
    }
}

ICacheContextModule::~ICacheContextModule() {}

int64_t ICacheContextModule::saveContext() {
    IGNIS_RPC_TRY()
    return impl.saveContext();
    IGNIS_RPC_CATCH()
}

void ICacheContextModule::clearContext() {
    IGNIS_RPC_TRY()
    impl.clearContext();
    IGNIS_RPC_CATCH()
}

void ICacheContextModule::loadContext(const int64_t id) {
    IGNIS_RPC_TRY()
    impl.loadContext(id);
    IGNIS_RPC_CATCH()
}

void ICacheContextModule::loadContextAsVariable(const int64_t id, const std::string &name) {
    IGNIS_RPC_TRY()
    impl.loadContextAsVariable(id, name);
    IGNIS_RPC_CATCH()
}

void ICacheContextModule::cache(const int64_t id, const int8_t level) {
    IGNIS_RPC_TRY()
    typeFromPartition()->cache(impl, id, level);
    IGNIS_RPC_CATCH()
}

void ICacheContextModule::loadCache(const int64_t id) {
    IGNIS_RPC_TRY()
    impl.loadCache(id);
    IGNIS_RPC_CATCH()
}
