
#ifndef IGNIS_ISTORAGEMODULE_H
#define IGNIS_ISTORAGEMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IStorageModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IStorageModule : public IgnisModule, public ignis::rpc::executor::IStorageModuleIf {
                public:
                    IStorageModule(std::shared_ptr<IExecutorData> &executor_data);

                    void cache(const int64_t id, const std::string &storage) override;

                    void uncache(const int64_t id) override;

                    void restore(const int64_t id) override;

                    void saveContext(const int64_t id) override;

                    void loadContext(const int64_t id) override;

                    virtual ~IStorageModule();

                private:
                    std::unordered_map<size_t, std::shared_ptr<storage::IObject>> object_cache;
                    std::unordered_map<size_t, std::shared_ptr<storage::IObject>> object_context;
                };
            }
        }
    }
}

#endif
