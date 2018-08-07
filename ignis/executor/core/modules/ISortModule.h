
#ifndef IGNIS_ISORTMODULE_H
#define IGNIS_ISORTMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/ISortModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ISortModule : public IgnisModule, public ignis::rpc::executor::ISortModuleIf{
                public:
                    ISortModule(std::shared_ptr<IExecutorData> &executor_data);
                };
            }
        }
    }
}


#endif
