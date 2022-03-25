
#include "IBaseImpl.h"
#include <algorithm>

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
    } else if (type == "async") {
        sync = false;
    } else {
        IGNIS_LOG(info) << "Base: detecting exchange type";
        int64_t data[2] = {in.partitions(), 0};
        for (auto part : in) {
            if (part->empty()) { data[1]++; }
        }
        auto rank = executor_data->mpi().rank();
        executor_data->mpi().native().Reduce(rank == 0 ? MPI::IN_PLACE : data, data, 2, MPI::LONG, MPI::SUM, 0);
        if (executor_data->mpi().isRoot(0)) {
            int64_t n = data[0];
            int64_t n_zero = data[1];
            sync = n_zero < (n / executors);
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

    auto none = std::make_pair((int64_t) -1, (int64_t) -1);
    parts_targets.resize((block + 1) * executors, none);
    for (int64_t p = 0, i = 0; i < executors; i++) {
        for (int64_t j = 0; j < block; j++) { parts_targets[j * executors + i] = std::make_pair(p + j, i); }
        p += block;
        if (i < remainder) {
            parts_targets[block * executors + i] = std::make_pair(p, i);
            p += 1;
        }
    }
    std::remove_if(parts_targets.begin(), parts_targets.end(),
                   [&none](std::pair<int64_t, int64_t> &e) { return e == none; });

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
    auto rank = executor_data->mpi().rank();
    auto numPartitions = in.partitions();
    int64_t block = numPartitions / executors;
    int64_t remainder = numPartitions % executors;
    std::vector<std::pair<int64_t, int64_t>> ranges;
    std::vector<int64_t> queue;

    int64_t init, end;
    for (int64_t i = 0; i < executors; i++) {
        if (i < remainder) {
            init = (block + 1) * i;
            end = init + block + 1;
        } else {
            init = (block + 1) * remainder + block * (i - remainder);
            end = init + block;
        }
        ranges.emplace_back(init, end);
    }

    int64_t m = executors % 2 == 0 ? executors : executors + 1;
    int64_t id = 0;
    int64_t id2 = m * m - 2;
    for (int64_t i = 0; i < m - 1; i++) {
        if (rank == id % (m - 1)) { queue.push_back(m - 1); }
        if (rank == m - 1) { queue.push_back(id % (m - 1)); }
        id += 1;
        for (int64_t j = 1; j < m / 2; j++) {
            if (rank == id % (m - 1)) { queue.push_back(id2 % (m - 1)); }
            if (rank == id2 % (m - 1)) { queue.push_back(id % (m - 1)); }
            id += 1;
            id2 -= 1;
        }
    }

    executor_data->enableMpiCores();
    int64_t mpiCores = executor_data->getMpiCores();

    std::vector<bool> ignores(queue.size(), false);

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
            for (int64_t j = ranges[other].first; j < ranges[other].second; j++) { ignore = ignore && in[j]->empty(); }
            executor_data->mpi().native().Sendrecv(&ignore, 1, MPI::BOOL, other, 0, &ignore_other, 1, MPI::BOOL, other,
                                                   0);
            if(ignore && ignore_other){
                ignores[i] = true;
                for (int64_t j = ranges[other].first; j < ranges[other].second; j++) {
                    in[j].reset();
                }
            }
        }

        for (int64_t i = 0; i < queue.size(); i++) {
            int64_t other = queue[i];
            if (ignores[i] || other == executors) { continue; }
            int64_t other_part = ranges[other].first;
            int64_t other_end = ranges[other].second;
            int64_t me_part = ranges[rank].first;
            int64_t me_end = ranges[rank].second;
            int64_t its = std::max(other_end - other_part, me_end - me_part);

#pragma omp for schedule(static, 1)
            for (int64_t j = 0; j < its; j++) {
                me_part = ranges[rank].first + j;
                other_part = ranges[other].first + j;
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