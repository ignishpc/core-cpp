
#ifndef IGNIS_IGNISMODULE_H
#define IGNIS_IGNISMODULE_H

#include "executor/core/selector/ISelector.h"
#include "executor/core/IExecutorData.h"
#include "executor/core/ILog.h"
#include "rpc/IExecutorException_types.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IModule {

                public:
                    IModule(std::shared_ptr<IExecutorData> &executor_data);

                    std::shared_ptr<selector::ITypeSelector> typeFromPartition();

                    std::shared_ptr<selector::ITypeSelector> typeFromHeader(std::string &header);

                    virtual ~IModule();

                protected:
                    std::shared_ptr<IExecutorData> executor_data;
                };
            }
        }
    }
}


#endif
