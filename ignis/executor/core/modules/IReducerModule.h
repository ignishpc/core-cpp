
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

                    void getKeys(std::unordered_map<int64_t, int64_t> &_return,
                                 const rpc::executor::IFunction &funct, const bool single) override;

                    void
                    setExecutorKeys(const std::string &host, const int32_t port, const std::vector<int64_t> &keys_id,
                                    const int64_t msg_id) override;

                    void joinData(const std::vector<int64_t> &msg_ids) override;

                    void reduceByKey() override;

                    void reset() override;

                    virtual ~IReducerModule();

                private:
                    void joinDataAux(storage::IObject &obj, std::unordered_map<int64_t, std::vector<std::pair<
                            std::shared_ptr<storage::IObject>, std::shared_ptr<storage::ICoreWriteIterator<storage::IObject::Any>>>>> &keys);

                    std::shared_ptr<data::IManager<storage::IObject::Any>> manager;
                    std::shared_ptr<IDinamicObject<api::IReducer<storage::IObject::Any, storage::IObject::Any, storage::IObject::Any>>> function;
                    std::unordered_map<int64_t, std::shared_ptr<storage::IObject>> hash_value;
                    std::vector<std::shared_ptr<storage::IObject>> key_value;
                };
            }
        }
    }
}


#endif
