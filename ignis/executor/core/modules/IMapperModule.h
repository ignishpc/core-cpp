
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

                    virtual void _map(const rpc::ISource &sf) override;

                    virtual void flatmap(const rpc::ISource &sf) override;

                    virtual void filter(const rpc::ISource &sf) override;

                    virtual void keyBy(const rpc::ISource &sf) override;

                    virtual void streamingMap(const rpc::ISource &sf, bool ordered) override;

                    virtual void streamingFlatmap(const rpc::ISource &sf, bool ordered) override;

                    virtual void streamingFilter(const rpc::ISource &sf, bool ordered) override;

                    virtual void streamingKeyBy(const rpc::ISource &sf, bool ordered) override;

                    virtual ~IMapperModule();

                private:

                    template<template<typename...> class F, bool filter = false, bool key = false>
                    void pipe(const rpc::ISource &sf);

                    template<template<typename...> class F, bool filter = false, bool key = false>
                    void streaming(const rpc::ISource &sf, bool ordered);

                };
            }
        }
    }
}

#endif
