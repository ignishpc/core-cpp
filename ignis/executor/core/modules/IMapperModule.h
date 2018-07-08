
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

                    void _map(const  ::ignis::rpc::executor::IFunction& sf) override;

                    void flatmap(const  ::ignis::rpc::executor::IFunction& sf);

                    void streamingMap(const  ::ignis::rpc::executor::IFunction& sf, bool ordered) override;

                    void streamingFlatmap(const  ::ignis::rpc::executor::IFunction& sf, bool ordered);

                    virtual ~IMapperModule();

                private:

                    void mapAux(const  ::ignis::rpc::executor::IFunction& sf, bool flat);

                    void streamingMapAux(const  ::ignis::rpc::executor::IFunction& sf, bool ordered, bool flat);
                };
            }
        }
    }
}

#endif
