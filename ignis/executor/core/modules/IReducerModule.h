
#ifndef IGNIS_IREDUCERMODULE_H
#define IGNIS_IREDUCERMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IReducerModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IReducerModule : public IgnisModule, public ignis::rpc::executor::IReducerModuleIf {
                public:
                    IReducerModule(std::shared_ptr<IExecutorData> &executor_data);

                    virtual ~IReducerModule();

                };
            }
        }
    }
}


#endif
