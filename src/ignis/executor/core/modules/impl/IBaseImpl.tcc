
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
                out.add(in[p]);
            } else {
                in[p].reset();
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
}

#undef IBaseImplClass