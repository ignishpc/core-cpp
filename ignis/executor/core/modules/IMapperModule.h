
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

                    void _map(const  rpc::ISourceFunction& sf) override;

                    void flatmap(const  rpc::ISourceFunction& sf) override;

                    void filter(const  rpc::ISourceFunction& sf) override;

                    void streamingMap(const  rpc::ISourceFunction& sf, bool ordered) override;

                    void streamingFlatmap(const  rpc::ISourceFunction& sf, bool ordered) override;

                    void streamingFilter(const  rpc::ISourceFunction& sf, bool ordered) override;

                    virtual ~IMapperModule();

                private:

                    void pipe(const  rpc::ISourceFunction& sf, bool filter);

                    void streaming(const  rpc::ISourceFunction& sf, bool ordered, bool filter);
                };
            }
        }
    }
}

#endif
