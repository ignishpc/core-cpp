
#ifndef IGNIS_IDRIVERCONTEXT_H
#define IGNIS_IDRIVERCONTEXT_H

#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <functional>

#include "ignis/executor/api/IVector.h"
#include "ignis/executor/core/modules/IModule.h"
#include "ignis/rpc/ISource_types.h"
#include "ignis/rpc/executor/ICacheContextModule.h"

namespace ignis {
    namespace driver {
        namespace core {
            class IDriverContext : public executor::core::modules::IModule,
                                   public rpc::executor::ICacheContextModuleIf {
            public:
                IDriverContext(std::shared_ptr<executor::core::IExecutorData> &executor_data);

                int64_t saveContext();

                void clearContext();

                void loadContext(const int64_t id);

                void loadContextAsVariable(const int64_t id, const std::string& name);

                void cache(const int64_t id, const int8_t level);

                void loadCache(const int64_t id);

                template<typename Tp>
                rpc::ISource registerType();

                template<typename C>
                int64_t parallelize(const C &collection);

                template<typename Tp>
                int64_t parallelize(const std::vector<Tp> &&collection);

                int64_t parallelize(const std::vector<bool> &&collection);

                template<typename Tp>
                int64_t parallelize(const executor::api::IVector<Tp> &&collection);

                template<typename Tp>
                executor::api::IVector<Tp> collect(int64_t id);

                template<typename Tp>
                Tp collect1(int64_t id);

                virtual ~IDriverContext();

            private:
                int64_t addData(const std::function<std::shared_ptr<void>()>& f);

                std::shared_ptr<void> getContext(int64_t id);

                int64_t next_id;
                std::map<int64_t, std::shared_ptr<void>> context;
                std::map<int64_t, std::function<std::shared_ptr<void>()>> data;
                std::mutex mutex;
            };
        }// namespace core
    }    // namespace driver
}// namespace ignis

#include "IDriverContext.tcc"

#endif
