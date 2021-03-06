
#ifndef IGNIS_IREDUCEIMPL_H
#define IGNIS_IREDUCEIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IReduceImpl : public IBaseImpl {
                    public:
                        IReduceImpl(std::shared_ptr<IExecutorData> &executorData);

                        template<typename Function>
                        void reduce();

                        template<typename Function>
                        void treeReduce();

                        template<typename Function>
                        void zero();

                        template<typename Function>
                        void aggregate();

                        template<typename Function>
                        void fold();

                        template<typename Function>
                        void treeFold();

                        template<typename Tp>
                        void groupByKey(int64_t numPartitions);

                        template<typename Tp, typename Function>
                        void reduceByKey(int64_t numPartitions, bool localReduce);

                        template<typename Tp, typename Function>
                        void aggregateByKey(int64_t numPartitions, bool hashing);

                        template<typename Tp, typename Function>
                        void foldByKey(int64_t numPartitions, bool localFold);

                        template<typename Tp>
                        void union_(const std::string &other, bool preserveOrder);

                        template<typename Tp>
                        void join(const std::string &other, int64_t numPartitions);

                        template<typename Tp>
                        void distinct(int64_t numPartitions);

                        virtual ~IReduceImpl();

                    private:
                        template<typename Function, typename Tp>
                        inline void basicReduce(Function &f, storage::IMemoryPartition<Tp> &result);

                        template<typename Function, typename Tp>
                        inline void basicFold(Function &f, storage::IMemoryPartition<Tp> &result);

                        template<typename Function, typename Tp>
                        inline Tp reducePartition(Function &f, storage::IPartition<Tp> &part);

                        template<typename Function, typename Tp>
                        inline void finalReduce(Function &f, storage::IMemoryPartition<Tp> &partial);

                        template<typename Function, typename Tp>
                        inline void finalTreeReduce(Function &f, storage::IMemoryPartition<Tp> &partial);

                        template<typename Function, typename Tp, typename Tp2>
                        inline void aggregatePartition(Function &f, storage::IPartition<Tp> &part, Tp2 &acum);

                        template<typename Function, typename Tp>
                        inline void localReduceByKey(Function &f);

                        template<typename Function, typename Tp>
                        inline void localAggregateByKey(Function &f);

                        template<typename Tp>
                        inline void keyHashing(int64_t numPartitions);

                        template<typename Tp>
                        inline void keyExchanging();

                        template<typename Tp>
                        inline void distinctFilter(storage::IPartitionGroup<Tp>& parts);

                    };
                }// namespace impl
            }    // namespace modules
        }        // namespace core
    }            // namespace executor
}// namespace ignis

#include "IReduceImpl.tcc"

#endif
