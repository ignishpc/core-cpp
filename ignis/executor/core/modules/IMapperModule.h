
#ifndef IGNIS_IMAPPERMODULE_H
#define IGNIS_IMAPPERMODULE_H

#include "IModule.h"
#include "../../../rpc/executor/IMapperModule.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IMapperModule : public IgnisModule, public ignis::rpc::executor::IMapperModuleIf{
                public:
                    IMapperModule(std::shared_ptr<IExecutorData> &executor_data);

                    void onemap(const ::ignis::rpc::executor::IFunction &funct) override;

                    void flatmap(const ::ignis::rpc::executor::IFunction &funct) override;

                    virtual ~IMapperModule();

                };
            }
        }
    }
}

#endif
