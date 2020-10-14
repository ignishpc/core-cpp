
#ifndef IGNIS_IGNISMODULE_H
#define IGNIS_IGNISMODULE_H

#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/ILog.h"
#include "ignis/executor/core/selector/ISelector.h"
#include "ignis/rpc/IExecutorException_types.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IModule {

                public:
                    IModule(std::shared_ptr<IExecutorData> &executor_data);

                    std::shared_ptr<selector::ITypeSelector> typeFromPartition();

                    std::shared_ptr<selector::ITypeSelector> typeFromHeader(const std::string &header);

                    std::shared_ptr<selector::ITypeSelector> typeFromName(const std::string &name);

                    std::shared_ptr<selector::ITypeSelector> typeFromSource(const rpc::ISource &source);

                    virtual ~IModule();

                protected:
                    std::shared_ptr<IExecutorData> executor_data;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis


#endif
