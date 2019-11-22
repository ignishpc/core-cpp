
#ifndef IGNIS_ISORTIMPL_H
#define IGNIS_ISORTIMPL_H

#include "IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class ISortImpl : public IBaseImpl {
                    public:
                        ISortImpl(std::shared_ptr<IExecutorData> &executorData);

                        virtual ~ISortImpl();

                        template<typename Tp>
                        void sort(bool ascending);

                        template<typename Tp>
                        void sort(bool ascending, int64_t partitions);

                        template<typename Function>
                        void sortBy(bool ascending);

                        template<typename Function>
                        void sortBy(bool ascending, int64_t partitions);

                    private:
                        /*Primary Functions*/
                        template<typename Tp, typename Cmp>
                        void sort_impl(Cmp comparator, int64_t partitions);

                        template<typename Tp, typename Cmp>
                        void parallelLocalSort(storage::IPartitionGroup<Tp> &group, Cmp comparator);

                        template<typename Tp>
                        std::shared_ptr<storage::IMemoryPartition<Tp>>
                        selectPivots(storage::IPartitionGroup<Tp> &group, int64_t samples);

                        template<typename Tp>
                        std::shared_ptr<storage::IPartitionGroup<Tp>>
                        generateRanges(storage::IPartitionGroup<Tp> &group, storage::IMemoryPartition<Tp> &pivots);

                        /*Auxiliary functions*/
                        template<typename Tp, typename Cmp>
                        void sortMemoryPartition(storage::IMemoryPartition<Tp> &part, Cmp comparator);

                        template<typename Tp, typename Cmp>
                        std::shared_ptr<storage::IPartition<Tp>>
                        sortPartition(storage::IPartition<Tp> &part, Cmp comparator);

                        template<typename Tp, typename Cmp>
                        std::shared_ptr<storage::IPartition<Tp>>
                        mergePartitions(storage::IPartition<Tp> &p1, storage::IPartition<Tp> &p2, Cmp comparator);

                        template<typename Tp>
                        std::shared_ptr<storage::IMemoryPartition<Tp>>
                        selectMemoryPivots(storage::IPartitionGroup<Tp> &group, int64_t samples);

                        template<typename Tp>
                        std::shared_ptr<storage::IPartitionGroup<Tp>>
                        generateMemoryRanges(storage::IPartitionGroup<Tp> &group,
                                             storage::IMemoryPartition<Tp> &pivots);

                        template<typename Tp>
                        int64_t searchRange(Tp& elem, storage::IMemoryPartition<Tp> &pivots);
                    };
                }
            }
        }
    }
}

#include "ISortImpl.tcc"

#endif

