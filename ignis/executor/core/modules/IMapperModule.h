
#ifndef IGNIS_IMAPPERMODULE_H
#define IGNIS_IMAPPERMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IMapperModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMapperModule : public IgnisModule, public ignis::rpc::executor::IMapperModuleIf {
                public:
                    IMapperModule(std::shared_ptr<IExecutorData> &executor_data);

                    void _map(const  ::ignis::rpc::executor::IFunction& funct) override;

                    void streamingMap(const  ::ignis::rpc::executor::IFunction& funct, bool ordered) override;

                    virtual ~IMapperModule();

                };
            }
        }
    }
}

#endif
