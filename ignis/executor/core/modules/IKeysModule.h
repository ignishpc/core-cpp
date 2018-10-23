
#ifndef IGNIS_IKEYMODULE_H
#define IGNIS_IKEYMODULE_H

#include "IModule.h"
#include <map>
#include "../../../rpc/executor/IKeysModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IKeysModule : public IgnisModule, public ignis::rpc::executor::IKeysModuleIf{
                public:
                    IKeysModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual void getKeys(std::vector<int64_t> & _return) override;

                    virtual void getKeysWithCount(std::unordered_map<int64_t, int64_t>& _return) override;

                    virtual void prepareKeys(const std::vector<ignis::rpc::executor::IExecutorKeys> & executorKeys) override;

                    virtual void reduceByKey(const rpc::ISource &funct) override;

                    virtual ~IKeysModule();

                private:

                    std::shared_ptr<int64_t> hashes;
                };
            }
        }
    }
}

#endif
