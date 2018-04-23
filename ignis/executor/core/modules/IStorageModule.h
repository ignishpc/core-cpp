
#ifndef IGNIS_ISTORAGEMODULE_H
#define IGNIS_ISTORAGEMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IStorageModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IStorageModule : public IgnisModule, public ignis::rpc::executor::IStorageModuleIf{
                public:
                    IStorageModule(std::shared_ptr<IExecutorData> &executor_data);

                    void cache(const int64_t id, const std::string &storage) override;

                    void uncache(const int64_t id) override;

                    void load(const int64_t id) override;

                    virtual ~IStorageModule();
                };
            }
        }
    }
}

#endif
