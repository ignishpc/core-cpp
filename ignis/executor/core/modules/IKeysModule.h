
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

                    virtual void getKeys(std::unordered_map<int64_t, int64_t> &_return, const bool single) override ;

                    virtual void sendPairs(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id) override ;

                    virtual void joinPairs() override ;

                    virtual void reset() override;

                    virtual ~IKeysModule();

                private:

                    std::unordered_map<int64_t, int64_t> counts;
                };
            }
        }
    }
}

#endif
