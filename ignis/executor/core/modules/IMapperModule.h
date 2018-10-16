
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

                    virtual void _map(const rpc::ISourceFunction &sf) override;

                    virtual void flatmap(const rpc::ISourceFunction &sf) override;

                    virtual void filter(const rpc::ISourceFunction &sf) override;

                    virtual void keyBy(const rpc::ISourceFunction &sf);

                    virtual void streamingMap(const rpc::ISourceFunction &sf, bool ordered) override;

                    virtual void streamingFlatmap(const rpc::ISourceFunction &sf, bool ordered) override;

                    virtual void streamingFilter(const rpc::ISourceFunction &sf, bool ordered) override;

                    virtual void streamingKeyBy(const rpc::ISourceFunction &sf, bool ordered);

                    virtual ~IMapperModule();

                private:

                    template<template<typename...> typename F, bool filter = false, bool key = false>
                    void pipe(const rpc::ISourceFunction &sf);

                    template<template<typename...> typename F, bool filter = false, bool key = false>
                    void streaming(const rpc::ISourceFunction &sf, bool ordered);

                };
            }
        }
    }
}

#endif
