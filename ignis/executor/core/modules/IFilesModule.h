
#ifndef IGNIS_IFILESMODULE_H
#define IGNIS_IFILESMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IFilesModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IFilesModule : public IgnisModule, public ignis::rpc::executor::IFilesModuleIf{
                public:
                    IFilesModule(std::shared_ptr<IExecutorData> &executor_data);

                };
            }
        }
    }
}

#endif
