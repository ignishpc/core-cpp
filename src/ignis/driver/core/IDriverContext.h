
#ifndef IGNIS_IDRIVERCONTEXT_H
#define IGNIS_IDRIVERCONTEXT_H

#include <memory>
#include <vector>
#include <map>
#include <mutex>

#include "ignis/rpc/ISource_types.h"
#include "ignis/executor/core/modules/IModule.h"
#include "ignis/rpc/executor/ICacheContextModule.h"

namespace ignis {
    namespace driver {
        namespace core {
            class IDriverContext
                    : public executor::core::modules::IModule, public rpc::executor::ICacheContextModuleIf {
            public:

                IDriverContext(std::shared_ptr<executor::core::IExecutorData> &executor_data);

                int64_t saveContext();

                void loadContext(const int64_t id);

                void cache(const int64_t id, const int8_t level);

                void loadCache(const int64_t id);

                template<typename Tp>
                rpc::ISource registerType();

                template<typename C>
                int64_t parallelize(const C &collection, int64_t partitions);

                template<typename Tp>
                std::vector<Tp> collect(int64_t id);

                template<typename Tp>
                Tp collect1(int64_t id);

                virtual ~IDriverContext();

            private:
                int64_t next_context_id;
                std::map<int64_t, std::shared_ptr<void>> context;
                std::mutex mutex;
            };
        }
    }
}

#include "IDriverContext.tcc"

#endif
