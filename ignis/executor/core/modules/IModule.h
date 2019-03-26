
#ifndef IGNIS_IGNISMODULE_H
#define IGNIS_IGNISMODULE_H

#include <memory>
#include "../storage/IObject.h"
#include "../IExecutorData.h"
#include "../IObjectLoader.h"
#include "../ILog.h"
#include "../../api/IManager.h"
#include "../../../rpc/ISource_types.h"
#include "../../../exceptions/IInvalidArgument.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IgnisModule {

                public:
                    IgnisModule(std::shared_ptr<IExecutorData> &executor_data);

                    std::shared_ptr<storage::IObject> getIObject(size_t bytes = 50 * 1024 * 1024);

                    std::shared_ptr<storage::IObject> getIObject(
                            const std::shared_ptr<api::IManager<storage::IObject::Any>> &m,
                            size_t elems = 1000);

                    std::shared_ptr<storage::IObject> getIObject(
                            const std::shared_ptr<api::IManager<storage::IObject::Any>> &m,
                            size_t elems,
                            size_t bytes);

                    std::shared_ptr<storage::IObject> getIObject(
                            const std::shared_ptr<api::IManager<storage::IObject::Any>> &m,
                            size_t elems,
                            size_t bytes,
                            const std::string &type);

                    template<typename T>
                    std::shared_ptr<T> loadSource(const rpc::ISource &source) {
                        IGNIS_LOG(info) << "IModule loading function";
                        if (source.obj.__isset.bytes) {
                            throw exceptions::IInvalidArgument("C++ not support function handle");
                        }
                        auto result = executor_data->getObjectLoader().load<T>(source.obj.name);
                        if(source.params.size()> 0){
                            IGNIS_LOG(info) << "IModule loading arguments";
                            for(auto& entry: source.params){
                                auto var = std::static_pointer_cast<void>(std::make_shared<std::string>(entry.second));
                                executor_data->getContext().getVariables()[entry.first] = std::make_pair(false,var);
                            }
                        }
                        IGNIS_LOG(info) << "IModule function loaded";
                        return result;
                    }

                    std::shared_ptr<api::IManager<storage::IObject::Any>> getManager(storage::IObject &object);

                    virtual ~IgnisModule();

                protected:
                    std::shared_ptr<IExecutorData> executor_data;

                };
            }
        }
    }
}


#endif
