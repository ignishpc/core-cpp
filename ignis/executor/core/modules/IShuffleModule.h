
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

                    virtual void nextSplit(const std::string& addr, const int64_t length);

                    virtual void finishSplits();

                    virtual void joinSplits(const std::vector<int64_t> & order);

                    virtual ~IShuffleModule();
                private:
                    std::shared_ptr<storage::ICoreReadIterator<storage::IObject::Any>> it;
                };
            }
        }
    }
}

#endif
