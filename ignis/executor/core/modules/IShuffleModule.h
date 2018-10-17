
#ifndef IGNIS_ISHUFFLEMODULE_H
#define IGNIS_ISHUFFLEMODULE_H

#include "IModule.h"
#include "../storage/iterator/ICoreIterator.h"
#include "../../../rpc/executor/IShuffleModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IShuffleModule : public IgnisModule, public ignis::rpc::executor::IShuffleModuleIf {
                public:
                    IShuffleModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual void createSplits(const std::vector<ignis::rpc::executor::ISplit> &splits) override;

                    virtual void joinSplits(const std::vector<int64_t> &order) override;

                    virtual ~IShuffleModule();
                };
            }
        }
    }
}

#endif
