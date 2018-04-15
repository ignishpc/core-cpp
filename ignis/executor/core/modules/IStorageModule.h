
#ifndef IGNIS_ISTORAGEMODULE_H
#define IGNIS_ISTORAGEMODULE_H

#include "IModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IStorageModule : public IgnisModule{
                public:
                    IStorageModule(std::shared_ptr<IExecutorData> &executor_data);
                };
            }
        }
    }
}

#endif
