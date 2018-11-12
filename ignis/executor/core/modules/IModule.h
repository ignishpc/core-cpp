
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
                            size_t elems = 1000,
                            size_t bytes = 50 * 1024 * 1024);

                    std::shared_ptr<storage::IObject> getIObject(
                            const std::shared_ptr<api::IManager<storage::IObject::Any>> &m,
                            size_t elems,
                            size_t bytes,
                            const std::string& type);

                    template<typename T>
                    std::shared_ptr<T> loadFunction(const rpc::ISource &funct) {
                        IGNIS_LOG(info) << "IModule loading function";
                        if (funct.__isset.bytes) {
                            throw exceptions::IInvalidArgument("C++ not support function handle");
                        }
                        auto cache = executor_data->libraries.find(funct.name);
                        std::shared_ptr<T> result;
                        if(cache != executor_data->libraries.end()){
                            result = std::static_pointer_cast<T>(cache->second);
                        }else{
                            result = IObjectLoader::load<T>(funct.name);
                        }
                        IGNIS_LOG(info) << "IModule function loaded";
                        return result;
                    }

                    std::shared_ptr<api::IManager<storage::IObject::Any>> getManager(storage::IObject& object);

                    std::shared_ptr<storage::IObject> memoryObject(const std::shared_ptr<storage::IObject>& object);

                    virtual ~IgnisModule();

                protected:
                    std::shared_ptr<IExecutorData> executor_data;

                };
            }
        }
    }
}


#endif
