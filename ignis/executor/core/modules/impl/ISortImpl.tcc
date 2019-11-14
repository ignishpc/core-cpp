
#include "ISortImpl.h"
#include <algorithm>

#define ISortImplClass ignis::executor::core::modules::impl::ISortImpl


template<typename Tp>
void ISortImplClass::sort(bool ascending) {
    sort < Tp > (ascending, executor_data->getPartitions<Tp>()->partitions());
}

template<typename Tp>
void ISortImplClass::sort(bool ascending, int64_t partitions) {
    std::less<Tp> comparator;
    sort_impl<Tp>([&comparator, ascending](const Tp &lhs, const Tp &rhs) {
        return comparator(lhs, rhs) == ascending;
    }, partitions);
}

template<typename Function>
void ISortImplClass::sortBy(bool ascending) {
    sortBy < Function > (ascending, executor_data->getPartitions<typename Function::_T_type>()->partitions());
}

template<typename Function>
void ISortImplClass::sortBy(bool ascending, int64_t partitions) {
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    sort_impl<Function::_T_type>(
            [&context, &function, ascending](const typename Function::_T_type &lhs,
                                             const typename Function::_T_type &rhs) {
                return function.call(
                        const_cast<typename Function::_T_type>(lhs),
                        const_cast<typename Function::_T_type>(rhs), context
                ) == ascending;
            }, partitions);
    function.after(context);
}

template<typename Tp, typename Cmp>
void ISortImplClass::sort_impl(Cmp comparator, int64_t partitions) {
    bool native = true;
    int8_t comp = 0;
    try {
//        auto input = executor_data->getPartitions<Tp>();
//        decltype(input) output;
//        bool inMemory = false;
//        /*Work directly on the array to improve performance*/
//        if (isMemory(*input)) {
//            /*Copy the data if they are reused*/
//            if (input->cache()) {
//                output = input->clone();
//            } else {
//                output = input;
//            }
//            executor_data->setPartitions(output);
//            inMemory = true;
//        } else {
//            auto output = newPartitionGroup<Tp>();
//        }
//
//        /*Sort each partition*/
//        parallelSort(*output, comparator, inMemory);
//
//        auto my_pivots = newMemoryPartition<Tp>();
//        auto n_ranges = executor_data->getContext().executors() - 1;
//        auto ranges = newMemoryPartition<Tp>(n_ranges);
//        auto writer = std::static_pointer_cast<storage::IMemoryWriteIterator < Tp>>
//        (my_pivots->writeIterator());
//
//        /*Generates pivots to separate the elements in order*/
//        if (inMemory) {
//            selectPivotsWithIndex(*output, *writer, executor_data->getProperties().sortSamples());
//        } else {
//            selectPivots(*output, *writer, executor_data->getProperties().sortSamples());
//        }
//
//        auto pivots_out = my_pivots->createMessage(native);
//        std::vector<int> sizes;
//        int my_size = pivots_out->size();
//        int rank = executor_data->getContext().mpiGroup().Get_rank();
//        if (rank == 0) {
//            sizes.resize(executor_data->getContext().executors());
//        }
//
//        /*Collect pivots size from every executor*/
//        executor_data->getContext().mpiGroup().Gather(&my_size, 1, MPI::INT,
//                                                      &sizes[0], 1, MPI::INT, 0);
//        /*Collect pivots*/
//        int all_size = 0;
//        std::vector<int> displs;
//        decltype(my_pivots) all_pivots;
//        decltype(pivots_out) pivots_in;
//        if (rank == 0) {
//            displs.push_back(sizes[0]);
//            for (int i = 1; i < sizes.size(); i++) {
//                displs[i] += sizes[i - 1] + displs[i - 1];
//            }
//            all_pivots = newMemoryPartition<Tp>();
//            pivots_in = all_pivots->createMessage(native);
//            pivots_in->resize(displs.back());
//        } else {
//            pivots_in = std::make_shared<storage::IVoidMessage>();
//        }
//
//        executor_data->getContext().mpiGroup().Gatherv(pivots_out->ptr(comp), pivots_out->size(), MPI::BYTE,
//                                                       pivots_in->ptr(comp), &sizes[0], &displs[0], MPI::BYTE, 0);
//        pivots_out.reset();
//        pivots_in.reset();
//        my_pivots->clear();
//
//        if (rank == 0) {
//            pivots_in->flush();
//
//            /*Sort pivots*/
//            sortIndexedPartition(*all_pivots, comparator);
//
//
//            writer = std::static_pointer_cast<storage::IMemoryWriteIterator < Tp>>
//            (ranges->writeIterator());
//
//            auto tmp_group = newPartitionGroup<Tp>(0);
//            auto tmp_part = std::static_pointer_cast<storage::IPartition < Tp>>
//            (all_pivots);
//            tmp_group->add(tmp_part);
//            selectPivotsWithIndex(*tmp_group, *writer, n_ranges);
//        }
//
//
//        auto ranges_msg = all_pivots->createMessage(native);
//        ranges_msg->resize(n_ranges);
//
//        /*Broadcast ranges*/
//        executor_data->getContext().mpiGroup().Bcast(ranges_msg->ptr(comp), ranges_msg->size(), MPI::BYTE, 0);
//        ranges_msg->flush();
//
//        /*Repartition*/
//        decltype(output) range_group;
//        if (inMemory) {
//            range_group = generateRanges(*output, *all_pivots);
//        } else {
//            range_group = generateRangesWithIndex(*output, *all_pivots);
//        }
//
//        for (int i = 0; i < executor_data->getContext().executors(); i++) {
//            //TODO
//        }
//        /*Generate final partitions*/
//
//
//        if (inMemory) {
//            output = generateRanges(*output, *my_pivots);
//        } else {
//            output = generateRangesWithIndex(*output, *my_pivots);
//        }
//
//        /*Sort final partitions*/
//        parallelSort(*output, comparator, inMemory);


    } catch (std::exception &ex) {
        throw exception::IException(ex);
    }
}

template<typename Tp, typename Cmp>
void ISortImplClass::parallelSort(storage::IPartitionGroup <Tp> &group, Cmp comparator, bool inMemory) {
    IGNIS_OMP_EXCEPTION_INIT()
    #pragma omp parallel num_threads(executor_data->getContext().cores())
    {
        IGNIS_OMP_TRY()
            /*Sort each partition locally*/
            #pragma omp for schedule(dynamic)
            for (int64_t p = 0; p < group.partitions(); p++) {
                if (inMemory) {
                    sortIndexedPartition<Tp>(reinterpret_cast<storage::IMemoryPartition <Tp> &>(*group[p]),
                                             comparator);
                } else {
                    #pragma omp critical
                    {
                        auto sorted = sortPartition<Tp>(*group[p], comparator);
                        group.add(sorted);
                    }
                }
            }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
}

template<typename Tp, typename Cmp>
void ISortImplClass::sortIndexedPartition(storage::IMemoryPartition <Tp> &part, Cmp comparator) {
    std::stable_sort(part.begin(), part.end(), comparator);
}

template<typename Tp, typename Cmp>
std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>>
ISortImplClass::sortPartition(storage::IPartition <Tp> &part, Cmp comparator) {
    auto read = part.readIterator();
    std::vector<std::shared_ptr<storage::IPartition < Tp>> > stack;

    int64_t i = 0;
    while (true) {
        while (i < part.size() && (stack.size() < 2 || stack.back()->size() != stack[stack.size() - 2]->size())) {
            stack.push_back(newPartition<Tp>());
            stack.back()->writeIterator()->write(read->next());
            i++;
        }
        if (stack[0]->size() == part.size()) {
            break;
        }

        auto sorted = mergePartitions(*stack.back(), *stack[stack.size() - 2], comparator);

        stack.pop_back();
        stack.pop_back();
        stack.push_back(sorted);
    }
    return stack.back();
}


template<typename Tp, typename Cmp>
std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>> ISortImplClass::mergePartitions(
        storage::IPartition <Tp> &p1, storage::IPartition <Tp> &p2, Cmp comparator) {
    auto sorted = newPartition<Tp>();
    auto sw = sorted->writeIterator();
    auto r1 = p1.readIterator();
    auto r2 = p2.readIterator();
    auto head1 = r1->nextShared();
    auto head2 = r2->nextShared();

    while (true) {
        if (comparator(*head1, *head2)) {
            sw->write(*head1);
            if (r1->hasNext()) {
                head1 = r1->nextShared();
            } else {
                sw->write(*head2);
                storage::copy(*r2, *sw);
                break;
            }
        } else {
            sw->write(*head2);
            if (r2->hasNext()) {
                head2 = r2->nextShared();
            } else {
                sw->write(*head1);
                storage::copy(*r1, *sw);
                break;
            }
        }
    }
    return sorted;
}

template<typename Tp>
void
ISortImplClass::selectPivotsWithIndex(storage::IPartitionGroup <Tp> &group,
                                      storage::IMemoryWriteIterator <Tp> &pivots,
                                      int samples) {

}

template<typename Tp>
void ISortImplClass::selectPivots(storage::IPartitionGroup <Tp> &group, storage::IMemoryWriteIterator <Tp> &pivots,
                                  int samples) {

}


template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>>
ISortImplClass::generateRanges(storage::IPartitionGroup <Tp> &group, storage::IMemoryPartition <Tp> &pivots) {

}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>>
ISortImplClass::generateRangesWithIndex(storage::IPartitionGroup <Tp> &group, storage::IMemoryPartition <Tp> &pivots) {

}

#undef ISortImplClass