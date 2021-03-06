
#include "ISortImpl.h"
#include <algorithm>
#include <cmath>

#define ISortImplClass ignis::executor::core::modules::impl::ISortImpl


template<typename Tp>
void ISortImplClass::sort(bool ascending) {
    sort<Tp>(ascending, -1);
}

template<typename Tp>
void ISortImplClass::sort(bool ascending, int64_t partitions) {
    IGNIS_TRY()
    std::less<Tp> comparator;
    sort_impl<Tp>([&comparator, ascending](const Tp &lhs, const Tp &rhs) { return comparator(lhs, rhs) == ascending; },
                  partitions);
    IGNIS_CATCH()
}

template<typename Function>
void ISortImplClass::sortBy(bool ascending) {
    sortBy<Function>(ascending, -1);
}

template<typename Function>
void ISortImplClass::sortBy(bool ascending, int64_t partitions) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    sort_impl<typename Function::_T1_type>(
            [&context, &function, ascending](const typename Function::_T1_type &lhs,
                                             const typename Function::_T2_type &rhs) {
                return function.call(const_cast<typename Function::_T1_type &>(lhs),
                                     const_cast<typename Function::_T2_type &>(rhs), context) == ascending;
            },
            partitions);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Tp>
void ISortImplClass::top(int64_t n) {
    IGNIS_TRY()
    std::less<Tp> comparator;
    take_ordered_impl<Tp>([&comparator](const Tp &lhs, const Tp &rhs) { return !comparator(lhs, rhs); }, n);
    IGNIS_CATCH()
}

template<typename Function>
void ISortImplClass::topBy(int64_t n) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    take_ordered_impl<typename Function::_T1_type>(
            [&context, &function](const typename Function::_T1_type &lhs, const typename Function::_T2_type &rhs) {
                return function.call(const_cast<typename Function::_T1_type &>(rhs),
                                      const_cast<typename Function::_T2_type &>(lhs), context);
            },
            n);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Tp>
void ISortImplClass::takeOrdered(int64_t n) {
    IGNIS_TRY()
    std::less<Tp> comparator;
    take_ordered_impl<Tp>([&comparator](const Tp &lhs, const Tp &rhs) { return comparator(lhs, rhs); }, n);
    IGNIS_CATCH()
}

template<typename Function>
void ISortImplClass::takeOrderedBy(int64_t n) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    take_ordered_impl<typename Function::_T1_type>(
            [&context, &function](const typename Function::_T1_type &lhs, const typename Function::_T2_type &rhs) {
                return function.call(const_cast<typename Function::_T1_type &>(lhs),
                                     const_cast<typename Function::_T2_type &>(rhs), context);
            },
            n);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Tp>
void ISortImplClass::max() {
    IGNIS_TRY()
    std::less<Tp> comparator;
    max_impl<Tp>([&comparator](const Tp &lhs, const Tp &rhs) { return comparator(lhs, rhs); });
    IGNIS_CATCH()
}

template<typename Tp>
void ISortImplClass::min() {
    IGNIS_TRY()
    std::less<Tp> comparator;
    max_impl<Tp>([&comparator](const Tp &lhs, const Tp &rhs) { return comparator(rhs, lhs); });
    IGNIS_CATCH()
}

template<typename Function>
void ISortImplClass::maxBy() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    max_impl<typename Function::_T1_type>(
            [&context, &function](const typename Function::_T1_type &lhs, const typename Function::_T2_type &rhs) {
                return function.call(const_cast<typename Function::_T1_type &>(lhs),
                                     const_cast<typename Function::_T2_type &>(rhs), context);
            });
    function.after(context);
    IGNIS_CATCH()
}

template<typename Function>
void ISortImplClass::minBy() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    max_impl<typename Function::_T1_type>(
            [&context, &function](const typename Function::_T1_type &lhs, const typename Function::_T2_type &rhs) {
                return function.call(const_cast<typename Function::_T1_type &>(rhs),
                                      const_cast<typename Function::_T2_type &>(lhs), context);
            });
    function.after(context);
    IGNIS_CATCH()
}

template<typename Tp>
void ISortImplClass::sortByKey(bool ascending) {
    sortByKey<Tp>(ascending, -1);
}

template<typename Tp>
void ISortImplClass::sortByKey(bool ascending, int64_t partitions) {
    IGNIS_TRY()
    std::less<typename Tp::first_type> comparator;
    sort_impl<Tp>([&comparator, ascending](const Tp &lhs,
                                           const Tp &rhs) { return comparator(lhs.first, rhs.first) == ascending; },
                  partitions);
    IGNIS_CATCH()
}

template<typename Tp, typename Function>
void ISortImplClass::sortByKeyBy(bool ascending) {
    sortByKeyBy<Tp, Function>(ascending, -1);
}

template<typename Tp, typename Function>
void ISortImplClass::sortByKeyBy(bool ascending, int64_t partitions) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    sort_impl<Tp>(
            [&context, &function, ascending](const Tp &lhs, const Tp &rhs) {
                return function.call(const_cast<Tp &>(lhs).first, const_cast<Tp &>(rhs).first, context) == ascending;
            },
            partitions);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Tp, typename Cmp>
void ISortImplClass::sort_impl(Cmp comparator, int64_t partitions, bool local_sort) {
    auto input = executor_data->getPartitions<Tp>();
    auto executors = executor_data->mpi().executors();
    /*Copy the data if they are reused*/
    if (input->cache()) {
        /*Work directly on the array to improve performance*/
        if (executor_data->getPartitionTools().isMemory(*input)) {
            input = input->clone();
        } else {
            /*Only group will be affected*/
            input = input->shadowCopy();
        }
    }

    /*Sort each partition*/
    IGNIS_LOG(info) << "Sort: sorting " << input->partitions() << " partitions locally";
    parallelLocalSort(*input, comparator);

    int64_t localPartitions = input->partitions();
    int64_t totalPartitions;
    executor_data->mpi().native().Allreduce(&localPartitions, &totalPartitions, 1, MPI::LONG_LONG, MPI::SUM);
    if (totalPartitions < 2) {
        executor_data->setPartitions(input);
        return;
    }
    if (partitions < 0) { partitions = totalPartitions; }

    /*Generates pivots to separate the elements in order*/
    double sr = executor_data->getProperties().sortSamples();
    int64_t samples;
    if (sr > 1 || sr == 0) {
        samples = (int64_t) sr;
    } else {
        int64_t send[]{0, 0};
        int64_t rcv[2];
        send[0] = input->partitions();
        for (auto part : *input) { send[1] += part->size(); }
        executor_data->mpi().native().Allreduce(send, rcv, 2, MPI::LONG, MPI::SUM);
        samples = std::ceil((double) rcv[1] / rcv[0] * sr);
    }

    samples = std::max(partitions, samples);
    IGNIS_LOG(info) << "Sort: selecting " << samples << " pivots";
    auto pivots = selectPivots(*input, samples);

    bool resampling = executor_data->getProperties().sortResampling();
    if (sr < 1 && resampling && executors > 1 && local_sort) {
        IGNIS_LOG(info) << "Sort: -- resampling pivots begin --";
        auto tmp = executor_data->getPartitionTools().newPartitionGroup<Tp>(0);
        tmp->add(pivots);
        executor_data->setPartitions<Tp>(tmp);
        sort_impl<Tp, Cmp>(comparator, executors * executor_data->getCores(), false);
        IGNIS_LOG(info) << "Sort: -- resampling pivots end --";
        samples = partitions - 1;
        IGNIS_LOG(info) << "Sort: selecting " << samples << " partition pivots";
        pivots = parallelSelectPivots<Tp>(samples);
        IGNIS_LOG(info) << "Sort: collecting pivots";
        executor_data->mpi().gather(*pivots, 0);
    } else {
        IGNIS_LOG(info) << "Sort: collecting pivots";
        executor_data->mpi().gather(*pivots, 0);

        if (executor_data->mpi().isRoot(0)) {
            auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(0);
            group->add(pivots);
            parallelLocalSort(*group, comparator);
            samples = partitions - 1;

            IGNIS_LOG(info) << "Sort: selecting " << samples << " partition pivots";
            pivots = selectPivots(*group, samples);
        }
    }

    IGNIS_LOG(info) << "Sort: broadcasting pivots ranges";
    executor_data->mpi().bcast(*pivots, 0);

    auto ranges = generateRanges(*input, *pivots, comparator);
    pivots.reset();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();

    IGNIS_LOG(info) << "Sort: exchanging ranges";
    exchange<Tp>(*ranges, *output);

    /*Sort final partitions*/
    IGNIS_LOG(info) << "Sort: sorting again " << output->partitions() << " partitions locally";
    parallelLocalSort(*output, comparator);
    executor_data->setPartitions(output);
}

template<typename Tp, typename Cmp>
void ISortImplClass::parallelLocalSort(storage::IPartitionGroup<Tp> &group, Cmp comparator) {
    bool inMemory = executor_data->getPartitionTools().isMemory(group);
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
/*Sort each partition locally*/
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < group.partitions(); p++) {
            if (inMemory) {
                sortPartition<Tp>(executor_data->getPartitionTools().toMemory(*group[p]), comparator);
            } else {
                storage::IMemoryPartition<Tp> tmp(group[p]->size());
                group[p]->copyTo(tmp);
                sortPartition<Tp>(tmp, comparator);
                group[p] = executor_data->getPartitionTools().newPartition(tmp);
                group[p]->copyFrom(tmp);
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IMemoryPartition<Tp>>
ISortImplClass::selectPivots(storage::IPartitionGroup<Tp> &group, int64_t samples) {
    if (executor_data->getPartitionTools().isMemory(group)) { return selectMemoryPivots(group, samples); }
    auto pivots = executor_data->getPartitionTools().newMemoryPartition<Tp>();
    auto writer = pivots->writeIterator();
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < group.partitions(); p++) {
            if (group[p]->size() < samples) {
                group[p]->copyTo(*pivots);
                writer = pivots->writeIterator();
                continue;
            }

            auto skip = (group[p]->size() - samples) / (samples + 1);
            auto rem = (group[p]->size() - samples) % (samples + 1);
            auto reader = group[p]->readIterator();
            for (int64_t n = 0; n < samples; n++) {
                for (int64_t i = 0; i < skip; i++) {
                    reader->next();
                }
                if (n < rem) { reader->next(); }
#pragma omp critical
                { writer->write(reader->next()); }
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    return pivots;
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IMemoryPartition<Tp>>
ISortImplClass::parallelSelectPivots(int64_t samples) {
    int rank = executor_data->mpi().rank();
    auto executors = executor_data->mpi().executors();
    auto result = executor_data->getPartitionTools().newMemoryPartition<Tp>(samples);
    std::vector<int64_t> aux(executors, 0);
    int64_t sz = 0, disp = 0, pos, sample;
    auto tmp = executor_data->getAndDeletePartitions<Tp>();
    for (auto &part : *tmp) { sz += part->size(); }

    executor_data->mpi().native().Allgather(&sz, 1, MPI::LONG_LONG, &aux[0], 1, MPI::LONG_LONG);

    sz = 0;
    for (int64_t i = 0; i < executors; i++) { sz += aux[i]; }
    for (int64_t i = 0; i < rank; i++) { disp += aux[i]; }

    auto skip = (sz - samples) / (samples + 1);
    auto rem = (sz - samples) % (samples + 1);

    pos = skip + (rem > 0 ? 1 : 0);
    for (sample = 0; sample < samples; sample++) {
        if (pos >= disp) { break; }
        if (sample < rem - 1) {
            pos += skip + 2;
        } else {
            pos += skip + 1;
        }
    }
    pos -= disp;

    auto writer = result->writeIterator();
    auto mwriter = executor_data->getPartitionTools().toMemory(*writer);

    for (auto part : *tmp) {
        auto array = executor_data->getPartitionTools().toMemory(*part);
        while (pos < array.size() && sample < samples) {
            mwriter.write(array[pos++]);
            pos += skip;
            if (sample < rem - 1) { pos++; }
            sample++;
        }
        pos -= array.size();
    }

    return result;
}

template<typename Tp, typename Cmp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>>
ISortImplClass::generateRanges(storage::IPartitionGroup<Tp> &group, storage::IMemoryPartition<Tp> &pivots,
                               Cmp comparator) {
    if (executor_data->getPartitionTools().isMemory(group)) { return generateMemoryRanges(group, pivots, comparator); }
    auto ranges = executor_data->getPartitionTools().newPartitionGroup<Tp>(pivots.size() + 1);

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto thread_ranges = executor_data->getPartitionTools().newPartitionGroup<Tp>(ranges->partitions());
        std::vector<std::shared_ptr<api::IWriteIterator<Tp>>> writers;
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) {
            writers.push_back((*thread_ranges)[p]->writeIterator());
        }

#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < group.partitions(); p++) {
            auto reader = group[p]->readIterator();
            while (reader->hasNext()) {
                auto &elem = reader->next();
                writers[searchRange(elem, pivots, comparator)]->write(elem);
            }
            group[p].reset();
        }
#pragma omp critical
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) { (*thread_ranges)[p]->moveTo(*((*ranges)[p])); }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    group.clear();
    return ranges;
}


template<typename Tp, typename Cmp>
void ISortImplClass::sortPartition(storage::IMemoryPartition<Tp> &part, Cmp comparator) {
    std::sort(part.begin(), part.end(), comparator);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IMemoryPartition<Tp>>
ISortImplClass::selectMemoryPivots(storage::IPartitionGroup<Tp> &group, int64_t samples) {
    auto pivots = executor_data->getPartitionTools().newMemoryPartition<Tp>();
    auto writer = pivots->writeIterator();
    auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);

    for (int64_t p = 0; p < group.partitions(); p++) {
        if (group[p]->size() < samples) {
            auto &men_part = executor_data->getPartitionTools().toMemory(*(group[p]));
            for (auto &elem : men_part) { men_writer.write(elem); }
            continue;
        }

        auto skip = (group[p]->size() - samples) / (samples + 1);
        auto rem = (group[p]->size() - samples) % (samples + 1);
        auto &part = executor_data->getPartitionTools().toMemory(*group[p]);
        auto pos = skip + (rem > 0 ? 1 : 0);
        for (int64_t n = 0; n < samples; n++) {
            men_writer.write(part[pos++]);
            pos += skip;
            if (n < rem - 1) { pos++; }
        }
    }
    return pivots;
}


template<typename Tp, typename Cmp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>>
ISortImplClass::generateMemoryRanges(storage::IPartitionGroup<Tp> &group, storage::IMemoryPartition<Tp> &pivots,
                                     Cmp comparator) {
    auto ranges = executor_data->getPartitionTools().newPartitionGroup<Tp>(pivots.size() + 1);

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto thread_ranges = executor_data->getPartitionTools().newPartitionGroup<Tp>(ranges->partitions());
        std::vector<std::shared_ptr<storage::IMemoryWriteIterator<Tp>>> writers(ranges->partitions());
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) {
            auto it = (*thread_ranges)[p]->writeIterator();
            writers[p] = executor_data->getPartitionTools().toMemory(it);
        }

#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < group.partitions(); p++) {
            auto &part = executor_data->getPartitionTools().toMemory(*group[p]);
            if (part.empty()) { continue; }
            std::vector<std::pair<int64_t, int64_t>> elems_stack;
            std::vector<std::pair<int64_t, int64_t>> ranges_stack;

            elems_stack.emplace_back(0, group[p]->size() - 1);
            ranges_stack.emplace_back(0, pivots.size());

            while (!elems_stack.empty()) {
                int64_t start = elems_stack.back().first;
                int64_t end = elems_stack.back().second;
                int64_t mid = (start + end) / 2;
                elems_stack.pop_back();

                int64_t firt = ranges_stack.back().first;
                int64_t last = ranges_stack.back().second;
                ranges_stack.pop_back();

                int64_t r = searchRange(part[mid], pivots, comparator);
                writers[r]->write(std::move(part[mid]));

                if (firt == r) {
                    for (int64_t i = start; i < mid; i++) { writers[r]->write(std::move(part[i])); }
                } else if (start < mid) {
                    elems_stack.emplace_back(start, mid - 1);
                    ranges_stack.emplace_back(firt, r);
                }

                if (r == last) {
                    for (int64_t i = mid + 1; i <= end; i++) { writers[r]->write(std::move(part[i])); }
                } else if (mid < end) {
                    elems_stack.emplace_back(mid + 1, end);
                    ranges_stack.emplace_back(r, last);
                }
            }
            group[p].reset();
        }
#pragma omp critical
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) { (*thread_ranges)[p]->moveTo(*((*ranges)[p])); }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    group.clear();
    return ranges;
}

template<typename Tp, typename Cmp>
int64_t ISortImplClass::searchRange(Tp &elem, storage::IMemoryPartition<Tp> &pivots, Cmp comparator) {
    int64_t start = 0;
    int64_t end = pivots.size() - 1;
    int64_t mid;
    while (start < end) {
        mid = (start + end) / 2;
        if (comparator(elem, pivots[mid])) {
            end = mid - 1;
        } else {
            start = mid + 1;
        }
    }
    if (comparator(elem, pivots[start])) {
        return start;
    } else {
        return start + 1;
    }
}

template<typename Tp, typename Cmp>
void ISortImplClass::take_ordered_impl(Cmp comparator, int64_t n) {
    auto input = executor_data->getPartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    bool inMemory = executor_data->getPartitionTools().isMemory(*input);

    IGNIS_LOG(info) << "Sort: top/takeOrdered " << n << " elemens";
    auto top = executor_data->getPartitionTools().newMemoryPartition<Tp>(n * input->partitions());
    IGNIS_LOG(info) << "Sort: local partition top/takeOrdered";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        storage::IMemoryPartition<Tp> local_top(n);
/*Sort each partition locally*/
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            if (inMemory) {
                auto &men = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (int64_t i = 0; i < men.size(); i++) { take_ordered_add(comparator, local_top, men[i], n); }
            } else {
                auto it = (*input)[p]->readIterator();
                while (it->hasNext()) { take_ordered_add(comparator, local_top, it->next(), n); }
            }
        }
#pragma omp critical
        { local_top.moveTo(*top); }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    IGNIS_LOG(info) << "Sort: local executor top/takeOrdered";
    sortPartition(*top, comparator);
    top->resize(n);
    IGNIS_LOG(info) << "Sort: global top/takeOrdered";
    executor_data->mpi().gather(*top, 0);
    if (executor_data->mpi().isRoot(0)) {
        sortPartition(*top, comparator);
        top->resize(n);
        output->add(top);
    }
    executor_data->setPartitions(output);
}

template<typename Tp, typename Cmp>
void ISortImplClass::take_ordered_add(Cmp comparator, storage::IMemoryPartition<Tp> &top, Tp &elem, int64_t n) {
    auto &inner = top.inner();
    if (inner.empty()) {
        inner.push_back(elem);
        return;
    }
    if (inner.size() == n) {
        if (comparator(inner.back(), elem) || !comparator(elem, inner.back())) {
            return;
        } else {
            inner.pop_back();
        }
    }
    auto i = searchRange(elem, top, comparator);
    inner.insert(inner.begin() + i, elem);
}

template<typename Tp, typename Cmp>
void ISortImplClass::max_impl(Cmp comparator) {
    auto input = executor_data->getPartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    bool inMemory = executor_data->getPartitionTools().isMemory(*input);

    IGNIS_LOG(info) << "Sort: max/min";
    Tp elem;
    bool empty = true;
    for (auto &part : *input) {
        if (part->size() > 0) {
            elem = part->readIterator()->next();
            empty = false;
            break;
        }
    }

    if (empty) {
        executor_data->setPartitions(output);
        return;
    }

    IGNIS_LOG(info) << "Sort: local max/min";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        Tp local_elem = elem;
/*Sort each partition locally*/
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            if (inMemory) {
                auto &men = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (int64_t i = 0; i < men.size(); i++) {
                    if (comparator(local_elem, men[i])) { local_elem = men[i]; }
                }
            } else {
                auto it = (*input)[p]->readIterator();
                while (it->hasNext()) {
                    auto &tmp = it->next();
                    if (comparator(local_elem, tmp)) { local_elem = tmp; }
                }
            }
        }
#pragma omp critical
        {
            if (comparator(elem, local_elem)) { elem = local_elem; }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    IGNIS_LOG(info) << "Sort: global max/min";
    auto result = executor_data->getPartitionTools().newMemoryPartition<Tp>(1);
    result->writeIterator()->write(elem);
    executor_data->mpi().gather(*result, 0);
    if (executor_data->mpi().isRoot(0)) {
        for(auto& item: *result){
            if (comparator(elem, item)) {
                elem = item;
            }
        }
        result->resize(1);
        (*result)[0] = elem;
        output->add(result);
    }
    executor_data->setPartitions(output);
}


#undef ISortImplClass