
#ifndef IGNIS_IREDUCERMODULE_H
#define IGNIS_IREDUCERMODULE_H

#include "IModule.h"
#include "../../api/IReducer.h"
#include "../../../rpc/executor/IReducerModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IReducerModule : public IgnisModule, public ignis::rpc::executor::IReducerModuleIf {
                public:
                    IReducerModule(std::shared_ptr<IExecutorData> &executor_data);

                    void reduceByKey() override;

                    virtual ~IReducerModule();

                    ///TO DELETE

                    void getKeys(std::unordered_map<int64_t, int64_t> &_return,
                                 const rpc::executor::IFunction &funct, const bool single) override{}
                    void
                    setExecutorKeys(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id,
                                    const int64_t msg_id) override{}
                    void joinData(const std::vector<int64_t> &msg_ids) override{}
                    void reset() override{}
                };
            }
        }
    }
}


#endif
