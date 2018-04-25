
#ifndef IGNIS_IGNISMODULE_H
#define IGNIS_IGNISMODULE_H

#include <memory>
#include "../storage/IObject.h"
#include "../IExecutorData.h"
#include "../IDinamicObject.h"
#include "../ILog.h"
#include "../../../rpc/executor/IFunction_types.h"
#include "../../../exceptions/IInvalidArgument.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IgnisModule {

                public:
                    IgnisModule(std::shared_ptr<IExecutorData> &executor_data);

                    std::shared_ptr<storage::IObject> getIObject(size_t bytes = 50 * 1024 * 1024);

                    std::shared_ptr<storage::IObject>
                    getIObject(std::shared_ptr<data::IManager<storage::IObject::Any>> m, size_t lines = 1000, size_t bytes = 50 * 1024 * 1024);

                    template<typename T>
                    std::shared_ptr<IDinamicObject<T>> loadFunction(const rpc::executor::IFunction &funct) {
                        IGNIS_LOG(info) << "IModule loading function";
                        if (funct.__isset.bytes) {
                            throw exceptions::IInvalidArgument("C++ not support function serialization");
                        }
                        return std::make_shared<IDinamicObject<T>>(funct.name);
                    }

                    virtual ~IgnisModule();

                protected:
                    std::shared_ptr<IExecutorData> executor_data;

                };
            }
        }
    }
}


#endif
