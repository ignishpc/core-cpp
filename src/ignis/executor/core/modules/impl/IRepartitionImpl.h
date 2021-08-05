
#ifndef IGNIS_IREPARTITIONIMPL_H
#define IGNIS_IREPARTITIONIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IRepartitionImpl : public IBaseImpl {
                    public:
                        IRepartitionImpl(std::shared_ptr<IExecutorData> &executorData);

                        virtual ~IRepartitionImpl();

                        template<typename Tp>
                        void repartition(int64_t numPartitions, bool preserveOrdering, bool global);

                        template<typename Tp>
                        void repartitionByRandom(int64_t numPartitions);

                        template<typename Tp>
                        void repartitionByHash(int64_t numPartitions);

                        template<typename Function>
                        void repartitionBy(int64_t numPartitions);

                    private:

                        template<typename Tp, typename Particioner>
                        void repartitionBy_impl(Particioner f, int64_t numPartitions);

                        template<typename Tp>
                        void local_repartition(int64_t numPartitions);

                        template<typename Tp>
                        void ordered_repartition(int64_t numPartitions);

                        template<typename Tp>
                        void unordered_repartition(int64_t numPartitions);

                    };
                }// namespace impl
            }    // namespace modules
        }        // namespace core
    }            // namespace executor
}// namespace ignis

#include "IRepartitionImpl.tcc"

#endif//IGNIS_IREPARTITIONIMPL_H
