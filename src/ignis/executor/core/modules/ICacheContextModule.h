
#ifndef IGNIS_ICACHECONTEXTMODULE_H
#define IGNIS_ICACHECONTEXTMODULE_H

#include "IModule.h"
#include <map>
#include <memory>
#include "ignis/rpc/executor/ICacheContextModule.h"
#include "impl/ICacheImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ICacheContextModule : public IModule, public rpc::executor::ICacheContextModuleIf {
                public:

                    ICacheContextModule(std::shared_ptr<IExecutorData> &executor_data);

                    int64_t saveContext();

                    void loadContext(const int64_t id);

                    void cache(const int64_t id, const int8_t level);

                    void loadCache(const int64_t id);

                    virtual ~ICacheContextModule();

                private:
                    impl::ICacheImpl impl;
                };
            }
        }
    }
}

#endif