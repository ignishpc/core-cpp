
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

                        template<typename Tp>
                        void top(int64_t n);

                        template<typename Function>
                        void topBy(int64_t n);

                        template<typename Tp>
                        void takeOrdered(int64_t n);

                        template<typename Function>
                        void takeOrderedBy(int64_t n);

                        template<typename Tp>
                        void max();

                        template<typename Tp>
                        void min();

                        template<typename Function>
                        void maxBy();

                        template<typename Function>
                        void minBy();

                        template<typename Tp>
                        void sortByKey(bool ascending);

                        template<typename Tp>
                        void sortByKey(bool ascending, int64_t partitions);

                        template<typename Tp, typename Function>
                        void sortByKeyBy(bool ascending);

                        template<typename Tp, typename Function>
                        void sortByKeyBy(bool ascending, int64_t partitions);

                    private:
                        /*Primary Functions*/
                        template<typename Tp, typename Cmp>
                        void sort_impl(Cmp comparator, int64_t partitions, bool local_sort=true);

                        template<typename Tp, typename Cmp>
                        void parallelLocalSort(storage::IPartitionGroup<Tp> &group, Cmp comparator);

                        template<typename Tp>
                        std::shared_ptr<storage::IMemoryPartition<Tp>> selectPivots(storage::IPartitionGroup<Tp> &group,
                                                                                    int64_t samples);

                        template<typename Tp>
                        std::shared_ptr<storage::IMemoryPartition<Tp>> parallelSelectPivots(int64_t samples);

                        template<typename Tp, typename Cmp>
                        std::shared_ptr<storage::IPartitionGroup<Tp>>
                        generateRanges(storage::IPartitionGroup<Tp> &group, storage::IMemoryPartition<Tp> &pivots,
                                       Cmp comparator);

                        /*Auxiliary functions*/
                        template<typename Tp, typename Cmp>
                        void sortPartition(storage::IMemoryPartition<Tp> &part, Cmp comparator);

                        template<typename Tp>
                        std::shared_ptr<storage::IMemoryPartition<Tp>>
                        selectMemoryPivots(storage::IPartitionGroup<Tp> &group, int64_t samples);

                        template<typename Tp, typename Cmp>
                        std::shared_ptr<storage::IPartitionGroup<Tp>>
                        generateMemoryRanges(storage::IPartitionGroup<Tp> &group, storage::IMemoryPartition<Tp> &pivots,
                                             Cmp comparator);

                        template<typename Tp, typename Cmp>
                        int64_t searchRange(Tp &elem, storage::IMemoryPartition<Tp> &pivots, Cmp comparator);

                        template<typename Tp, typename Cmp>
                        void take_ordered_impl(Cmp comparator, int64_t n);

                        template<typename Tp, typename Cmp>
                        void take_ordered_add(Cmp comparator, storage::IMemoryPartition<Tp> &top, Tp &elem, int64_t n);

                        template<typename Tp, typename Cmp>
                        void max_impl(Cmp comparator);
                    };
                }// namespace impl
            }    // namespace modules
        }        // namespace core
    }            // namespace executor
}// namespace ignis

#include "ISortImpl.tcc"

#endif
