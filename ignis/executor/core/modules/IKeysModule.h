
#ifndef IGNIS_IKEYMODULE_H
#define IGNIS_IKEYMODULE_H

#include "IModule.h"
#include <map>
#include "../../../rpc/executor/IReducerModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IKeysModule : public IgnisModule{
                public:
                    IKeysModule(std::shared_ptr<IExecutorData> &executor_data);

                    void getKeys(std::unordered_map<int64_t, int64_t> &_return, const bool single);

                    void sendPairs(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id,
                                    const int64_t msg_id);

                    void joinPairs(const std::vector<int64_t> &msg_ids);

                    void reset();

                    virtual ~IKeysModule();

                private:

                    std::unordered_map<int64_t, int64_t> counts;
                };
            }
        }
    }
}

#endif
