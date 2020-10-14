
#ifndef IGNIS_ICACHECONTEXTMODULE_H
#define IGNIS_ICACHECONTEXTMODULE_H

#include "IModule.h"
#include "ignis/rpc/executor/ICacheContextModule.h"
#include "impl/ICacheImpl.h"
#include <map>
#include <memory>

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class ICacheContextModule : public IModule, public rpc::executor::ICacheContextModuleIf {
                public:
                    ICacheContextModule(std::shared_ptr<IExecutorData> &executor_data);

                    int64_t saveContext();

                    void clearContext();

                    void loadContext(const int64_t id);

                    void cache(const int64_t id, const int8_t level);

                    void loadCache(const int64_t id);

                    virtual ~ICacheContextModule();

                private:
                    impl::ICacheImpl impl;
                };
            }// namespace modules
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
