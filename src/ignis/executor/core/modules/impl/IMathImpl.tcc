
#include "IMathImpl.h"
#include <random>

#define IMathImplClass ignis::executor::core::modules::impl::IMathImpl

template<typename Tp>
void IMathImplClass::sample(const bool withReplacement, const std::vector<int64_t> &num, const int32_t seed) {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<Tp>();
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>(input->partitions());
    auto &context = executor_data->getContext();

    IGNIS_LOG(info) << "Math: sample " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        std::mt19937 gen(seed + executor_data->getContext().threadId());
        std::uniform_int_distribution<int> dist;

#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto size = (*input)[p]->size();
            auto part = (*input)[p];
            if (!isMemory) {
                auto aux = executor_data->getPartitionTools().newMemoryPartition<Tp>(part->size());
                part->copyTo(*aux);
                part = aux;
            }
            auto &men = executor_data->getPartitionTools().toMemory(*part);
            if (withReplacement) {
                for (size_t i = 0; i < num[p]; i++) {
                    for (size_t j = 0; j < num[p]; j++) {
                        double prob = ((double) num[p]) / (size - j);
                        double random = ((double) dist(gen) / dist.max());
                        if (random < prob) {
                            writer->write(men[j]);
                            break;
                        }
                    }
                }
            } else {
                size_t picked = 0;
                for (size_t i = 0; i < size; i++) {
                    double prob = ((double) (num[p] - picked)) / (size - i);
                    double random = ((double) dist(gen) / dist.max());
                    if (random < prob) {
                        writer->write(men[i]);
                        picked += 1;
                    }
                }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Tp>
int64_t IMathImplClass::count() {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    IGNIS_LOG(info) << "Math: count " << input->partitions() << " partitions";
    int64_t count = 0;
    for (auto &part : *input) { count += part->size(); }
    return count;
    IGNIS_CATCH()
}

template<typename Tp>
int64_t IMathImplClass::sampleByKeyFilter() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto tmp = executor_data->getPartitionTools().newPartitionGroup<Tp>(input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*tmp);
    bool cache = input->cache() && isMemory;
    auto &context = executor_data->getContext();
    auto fractions = executor_data->getContext().var<std::unordered_map<typename Tp::first_type, double>>("fractions");

    auto check = [&fractions](const Tp &obj) { return fractions.find(obj.first) != fractions.end(); };

    IGNIS_LOG(info) << "Math: filtering key before sample " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*tmp)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (size_t i = 0; i < sz; i++) {
                    if (check(men_part[i])) {
                        if (cache) {
                            men_writer.write(men_part[i]);
                        } else {
                            men_writer.write(std::move(men_part[i]));
                        }
                    }
                }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) {
                    auto &elem = reader->next();
                    if (check(elem)) {
                        if (cache) {
                            writer->write(std::move(elem));
                        } else {
                            writer->write(elem);
                        }
                    }
                }
            }
            (*input)[p].reset();
            (*tmp)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    for (auto part : *tmp) {
        if (!part->empty()) { output->add(part); }
    }
    int64_t numPartitions = std::min(output->partitions(), (int64_t)fractions.size());
    executor_data->mpi().native().Allreduce(MPI_IN_PLACE, &numPartitions, 1, MPI::LONG, MPI::MAX);
    executor_data->setPartitions(output);
    return numPartitions;
    IGNIS_CATCH()
}

template<typename Tp>
void IMathImplClass::sampleByKey(const bool withReplacement, const int32_t seed) {
    std::vector<int64_t> num;
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<
            std::pair<typename Tp::first_type, api::IVector<typename Tp::second_type>>>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    auto fractions = executor_data->getContext().var<std::unordered_map<typename Tp::first_type, double>>("fractions");
    num.resize(fractions.size());
    std::unordered_map<typename Tp::first_type, int64_t> pmap;
    for (auto &entry : fractions) {
        pmap[entry.first] = output->partitions();
        output->add(executor_data->getPartitionTools().newPartition<Tp>());
    }
    IGNIS_LOG(info) << "Math: sampleByKey copying values to single partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto reader = (*input)[p]->readIterator();
            while (reader->hasNext()) {
                auto &elem = reader->next();
                int64_t pos = pmap[elem.first];
                num[pos] = elem.second.size();
                auto writer = (*output)[pos]->writeIterator();
                for (const auto &value : elem.second) {
                    writer->write(Tp(elem.first, value));
                }
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(output);
    IGNIS_CATCH()
    sample<Tp>(withReplacement, num, seed);
}

template<typename Tp>
void IMathImplClass::countByKey() {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);
    auto threads = executor_data->getCores();

    std::unordered_map<typename Tp::first_type, int64_t> acum[threads];
    IGNIS_LOG(info) << "Math: counting local keys " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto thread = executor_data->getContext().threadId();
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto &part = *(*input)[p];
            if (isMemory) {
                auto &men_part = executor_data->getPartitionTools().toMemory(part);
                for (int64_t i = 0; i < men_part.size(); i++) { acum[thread][men_part[i].first]++; }
            } else {
                auto reader = part.readIterator();
                for (int64_t i = 0; i < part.size(); i++) { acum[thread][reader->next().first]++; }
            }
        }
        int64_t pivotUp = threads;
        int64_t pivotDown;
        while (pivotUp - 1 > thread) {
            pivotDown = (int64_t) std::floor(pivotUp / 2.0);
            pivotUp = (int64_t) std::ceil(pivotUp / 2.0);
            if (thread < pivotDown) {
                for (auto &entry : acum[thread + pivotUp]) { acum[thread][entry.first] += entry.second; }
            }
            acum[thread + pivotUp].clear();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    countByReduce(acum[0]);
    IGNIS_CATCH()
}

template<typename Tp>
void IMathImplClass::countByValue() {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);
    auto threads = executor_data->getCores();

    std::unordered_map<typename Tp::second_type, int64_t> acum[threads];
    IGNIS_LOG(info) << "Math: counting local values " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto thread = executor_data->getContext().threadId();
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto &part = *(*input)[p];
            if (isMemory) {
                auto &men_part = executor_data->getPartitionTools().toMemory(part);
                for (int64_t i = 0; i < men_part.size(); i++) { acum[thread][men_part[i].second]++; }
            } else {
                auto reader = part.readIterator();
                for (int64_t i = 0; i < part.size(); i++) { acum[thread][reader->next().second]++; }
            }
        }
        int64_t pivotUp = threads;
        int64_t pivotDown;
        while (pivotUp - 1 > thread) {
            pivotDown = (int64_t) std::floor(pivotUp / 2.0);
            pivotUp = (int64_t) std::ceil(pivotUp / 2.0);
            if (thread < pivotDown) {
                for (auto &entry : acum[thread + pivotUp]) { acum[thread][entry.first] += entry.second; }
            }
            acum[thread + pivotUp].clear();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    countByReduce(acum[0]);
    IGNIS_CATCH()
}

template<typename Tp>
void IMathImplClass::countByReduce(std::unordered_map<Tp, int64_t> &acum) {
    IGNIS_LOG(info) << "Math: reducing global counting";
    auto elem_part = executor_data->getPartitionTools().newMemoryPartition<std::pair<Tp, int64_t>>();
    auto rank = executor_data->mpi().rank();
    int64_t pivotUp;
    int64_t pivotDown;
    pivotUp = executor_data->mpi().executors();
    while (pivotUp > 1) {
        pivotDown = (int64_t) std::floor(pivotUp / 2.0);
        pivotUp = (int64_t) std::ceil(pivotUp / 2.0);
        if (rank < pivotDown) {
            executor_data->mpi().recv(*elem_part, rank + pivotUp, 0);
            for (int64_t i = 0; i < elem_part->size(); i++) { acum[(*elem_part)[i].first] += (*elem_part)[i].second; }
        } else if (rank >= pivotUp) {
            auto writer = elem_part->writeIterator();
            auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
            for (auto &entry : acum) { men_writer.write(std::move(entry)); }
            acum.clear();
            executor_data->mpi().send(*elem_part, rank - pivotUp, 0);
        }
        elem_part->clear();
    }

    auto output = executor_data->getPartitionTools().newPartitionGroup<std::pair<Tp, int64_t>>();
    if (executor_data->mpi().isRoot(0)) {
        auto writer = elem_part->writeIterator();
        auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
        for (auto &entry : acum) { men_writer.write(std::move(entry)); }
        acum.clear();
        output->add(elem_part);
    }

    executor_data->setPartitions(output);
}

#undef IMathImplClass
