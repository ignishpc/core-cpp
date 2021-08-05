
#include "IBaseImpl.h"

#define IBaseImplClass ignis::executor::core::modules::impl::IBaseImpl

template<typename Tp>
void IBaseImplClass::exchange(storage::IPartitionGroup<Tp> &in, storage::IPartitionGroup<Tp> &out) {
    auto executors = executor_data->mpi().executors();
    if (executors == 1) {
        for (auto &part : in) {
            part->fit();
            out.add(part);
        }
        return;
    }
    auto type = executor_data->getProperties().exchangeType();
    bool sync;
    if (type == "sync") {
        sync = true;
    } else if (type == "sync") {
        sync = false;
    } else {
        IGNIS_LOG(info) << "Base: detecting exchange type";
        int64_t data[2] = {in.partitions(), 0};
        for (auto part : in) {
            if (part->empty()) { data[1]++; }
        }
        executor_data->mpi().native().Reduce(MPI::IN_PLACE, data, 2, MPI::LONG, MPI::SUM, 0);
        if (executor_data->mpi().isRoot(0)) {
            int64_t n = data[0];
            int64_t n_zero = data[1];
            int64_t e = executor_data->getContext().executors();
            sync = n_zero < (n / e);
        }
        executor_data->mpi().native().Bcast(&sync, 1, MPI::BOOL, 0);
    }

    if (sync) {
        IGNIS_LOG(info) << "Base: using synchronous exchange";
        exchange_sync(in, out);
    } else {
        IGNIS_LOG(info) << "Base: using asynchronous exchange";
        exchange_async(in, out);
    }
}

template<typename Tp>
void IBaseImplClass::exchange_sync(storage::IPartitionGroup<Tp> &in, storage::IPartitionGroup<Tp> &out) {
    auto executors = executor_data->mpi().executors();
    auto numPartitions = in.partitions();
    int64_t block = numPartitions / executors;
    int64_t remainder = numPartitions % executors;
    std::vector<std::pair<int64_t, int64_t>> parts_targets;

    for (int64_t i = 0; i < block; i++) {
        for (int64_t j = 0; j < executors; j++) {
            if (j < remainder) {
                parts_targets.emplace_back(block * j + i + j, j);
            } else {
                parts_targets.emplace_back(block * j + i + remainder, j);
            }
        }
    }

    if (block > 0) {
        for (int64_t j = 0; j < remainder; j++) { parts_targets.emplace_back(block * j + block, j); }
    } else {
        for (int64_t j = 0; j < remainder; j++) { parts_targets.emplace_back(j, j); }
    }

    executor_data->enableMpiCores();
    int64_t mpiCores = executor_data->getMpiCores();

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(mpiCores)
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(static, 1)
        for (int64_t i = 0; i < numPartitions; i++) {
            int64_t p = parts_targets[i].first;
            int64_t target = parts_targets[i].second;

            executor_data->mpi().gather(*in[p], target);
            if (executor_data->mpi().isRoot(target)) {
                in[p]->fit();
            } else {
                in[p].reset();
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    for (int64_t i = 0; i < numPartitions; i++) {
        if (in[i]) { out.add(in[i]); }
    }
    in.clear();
}

template<typename Tp>
void IBaseImplClass::exchange_async(storage::IPartitionGroup<Tp> &in, storage::IPartitionGroup<Tp> &out) {
    auto executors = executor_data->mpi().executors();
    int64_t me = executor_data->getContext().executorId();
    auto rank = executor_data->mpi().rank();
    auto numPartitions = in.partitions();
    int64_t block = numPartitions / executors;
    int64_t remainder = numPartitions % executors;
    std::vector<std::pair<int64_t, int64_t>> ranges;
    std::vector<int64_t> queue;

    int64_t last = 0;
    int64_t end;
    for (int64_t i = 0; i < executors; i++) {
        end = last + block + 1;
        if (i < remainder) { end += 1; }
        ranges.emplace_back(last, end);
    }

    int64_t m = executors % 2 == 0 ? executors : executors + 1;
    int64_t id = 0;
    int64_t id2 = m * m - 2;
    for (int64_t i = 0; i < m - 1; i++) {
        if (rank == id % (m - 1)) { queue.push_back(m - 1); }
        if (rank == m - 1) { queue.push_back(id % (m - 1)); }
        id += 1;
        for (int64_t j = 0; j < m / 2; j++) {
            if (rank == id % (m - 1)) { queue.push_back(id2 % (m - 1)); }
            if (rank == id2 % (m - 1)) { queue.push_back(id % (m - 1)); }
            id += 1;
            id2 -= 1;
        }
    }

    executor_data->enableMpiCores();
    int64_t mpiCores = executor_data->getMpiCores();

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(mpiCores)
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(static)
        for (int64_t i = 0; i < queue.size(); i++) {
            int64_t other = queue[i];
            bool ignore = true;
            bool ignore_other;
            if (other == executors) { continue; }
            for (int64_t j = ranges[other].first; j < ranges[other].second; j++) { ignore &= in[j]->empty(); }
            executor_data->mpi().native().Sendrecv(&ignore, 1, MPI::BOOL, other, 0, &ignore_other, 1, MPI::BOOL, other,
                                                   1);
            if (ignore && ignore_other) { continue; }
            int64_t other_part = ranges[other].first;
            int64_t other_end = ranges[other].second;
            int64_t me_part = ranges[me].first;
            int64_t me_end = ranges[me].second;
            int64_t its = std::max(other_end - other_part, me_end - me_part);

            for (int64_t j = 0; j < its; j++) {
                if (other_part >= other_end || me_part >= me_end) {
                    if (other_part >= other_end) {
                        executor_data->mpi().recv(*in[me_part], other, 0);
                    } else if (me_part >= me_end) {
                        executor_data->mpi().send(*in[other_part], other, 0);
                    } else {
                        continue;
                    }
                } else {
                    executor_data->mpi().sendRcv(*in[other_part], *in[me_part], other, 0);
                }
                in[other_part].reset();
                other_part++;
                me_part++;
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    for (int64_t i = 0; i < numPartitions; i++) {
        if (in[i]) {
            in[i]->fit();
            out.add(in[i]);
        }
    }
    in.clear();
}

#undef IBaseImplClass