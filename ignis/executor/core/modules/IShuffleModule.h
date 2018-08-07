
#ifndef IGNIS_ISHUFFLEMODULE_H
#define IGNIS_ISHUFFLEMODULE_H

#include "IModule.h"
#include "../storage/ICoreIterator.h"
#include "../../../rpc/executor/IShuffleModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IShuffleModule : public IgnisModule, public ignis::rpc::executor::IShuffleModuleIf{
                public:
                    IShuffleModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual void createSplits();

                    virtual void nextSplit(const std::string& host, const int32_t port, const int64_t length, const bool local);

                    virtual void finishSplits();

                    virtual ~IShuffleModule();
                private:
                    std::shared_ptr<storage::ICoreReadIterator<storage::IObject::Any>> it;
                };
            }
        }
    }
}

#endif
