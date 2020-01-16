
#ifndef IGNIS_ICACHEIMPL_H
#define IGNIS_ICACHEIMPL_H

#include "IBaseImpl.h"
#include <fstream>

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class ICacheImpl : public IBaseImpl{
                    public:

                        ICacheImpl(std::shared_ptr<IExecutorData> &executorData);

                        int64_t saveContext();

                        void loadContext(const int64_t id);

                        template<typename Tp>
                        void cache(const int64_t id, const int8_t level);

                        void loadCache(const int64_t id);

                        std::vector<std::vector<std::string>> getCacheFromDisk();

                        template <typename Tp>
                        void loadFromDisk(const std::vector<std::string>& group);

                    private:
                        std::string fileCache();

                        std::map<int64_t, std::shared_ptr<void>> context;
                        int64_t next_context_id;
                        std::map<int64_t, std::shared_ptr<void>> _cache;
                    };
                }
            }
        }
    }
}

#include "ICacheImpl.tcc"

#endif
