
#include "IRepartitionImpl.h"
#include <random>
#include <tuple>

#define IRepartitionImplClass ignis::executor::core::modules::impl::IRepartitionImpl


template<typename Tp>
void IRepartitionImplClass::repartition(int64_t numPartitions, bool preserveOrdering, bool global) {
    IGNIS_TRY()
    if (!global || executor_data->mpi().executors() == 1) {
        local_repartition<Tp>(numPartitions);
    } else if (preserveOrdering) {
        ordered_repartition<Tp>(numPartitions);
    } else {
        unordered_repartition<Tp>(numPartitions);
    }
    IGNIS_CATCH()
}

template<typename Tp>
void IRepartitionImplClass::ordered_repartition(int64_t numPartitions) {
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto executors = executor_data->getContext().executors();
    auto rank = executor_data->mpi().rank();
    std::vector<int64_t> executors_count(executors);
    std::vector<int64_t> local_offset(input->partitions());
    int64_t local_count = 0;
    int64_t global_count = 0;
    int64_t global_offset = 0;
    local_offset.push_back(0);
    for (auto part : *input) {
        local_count += part->size();
        local_offset.push_back(part->size());
    }
    executor_data->mpi().native().Allgather(&local_count, 1, MPI::LONG, &executors_count[0], 1, MPI::LONG);
    for (int64_t i = 0; i < executors; i++) {
        if (i < rank) { global_offset += executors_count[i]; }
        global_count += executors_count[i];
    }
    std::vector<int64_t> new_partitions_count(numPartitions);
    std::vector<int64_t> new_partitions_offset;
    int64_t new_partitions_first = -1;
    {
        int64_t block = global_count / numPartitions;
        int64_t remainder = global_count % numPartitions;
        new_partitions_offset.push_back(0);
        for (int64_t i = 0; i < numPartitions; i++) {
            new_partitions_count[i] = block;
            if (i < remainder) { new_partitions_count[i]++; }
            new_partitions_offset.push_back(new_partitions_offset.back() + new_partitions_count[i]);
            if (global_offset <= new_partitions_offset[i + 1] && new_partitions_first == -1) {
                new_partitions_first = i;
            }
        }
    }

    IGNIS_LOG(info) << "Repartition: ordered repartition from " << input->partitions() << " partitions";
    auto global = executor_data->getPartitionTools().newPartitionGroup<std::pair<int64_t, Tp>>(numPartitions);

    if (input->partitions() > 0) {
        std::vector<decltype(global)> locals;
        for (int64_t i = 0; i < executor_data->getCores(); i++) {
            locals.push_back(
                    executor_data->getPartitionTools().newPartitionGroup<std::pair<int64_t, Tp>>(numPartitions));
        }
        IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
        {
            auto local = locals[executor_data->getContext().threadId()];
            int64_t i = new_partitions_first;
            int64_t required = -1;
            IGNIS_OMP_TRY()
#pragma omp for schedule(static)
            for (int64_t p = 0; p < input->partitions(); p++) {
                if (required < 0) {
                    required = new_partitions_offset[i + 1] - global_offset;
                    for (int64_t j = 0; j < p; j++) { required -= (*input)[j]->size(); }
                    while (required < 0) { required += new_partitions_count[i++]; }
                }
                auto reader = (*input)[p]->readIterator();
                auto avail = (*input)[p]->size();
                std::shared_ptr<api::IWriteIterator<std::pair<int64_t, Tp>>> writer;

                while (avail > 0) {
                    if (required == 0) {
                        i += 1;
                        required = new_partitions_count[i];
                    }
                    writer = (*local)[i]->writeIterator();
                    auto its = std::min((int64_t) avail, required);
                    for (int64_t n = 0; n < its; n++) { writer->write(std::make_pair(rank, reader->next())); }
                    avail -= its;
                    required -= its;
                }
                (*input)[p].reset();
            }

#pragma omp for schedule(static)
            for (int64_t p = 0; p < global->partitions(); p++) {
                for (int64_t l = 0; l < executor_data->getCores(); l++) {
                    if ((*locals[l])[p]) { (*locals[l])[p]->moveTo(*((*global)[p])); }
                }
            }
            IGNIS_OMP_CATCH()
        }
        IGNIS_OMP_EXCEPTION_END()
    }

    IGNIS_LOG(info) << "Repartition: exchanging new partitions";
    auto tmp = executor_data->getPartitionTools().newPartitionGroup<std::pair<int64_t, Tp>>();
    exchange(*global, *tmp);
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>(tmp->partitions());

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {

        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < tmp->partitions(); p++) {
            auto part = (*tmp)[p];
            if (part->empty()) { continue; }
            auto writer = (*output)[p]->writeIterator();
            auto men_part = executor_data->getPartitionTools().newMemoryPartition<std::pair<int64_t, Tp>>(part->size());
            part->moveTo(*men_part);
            part.reset();
            auto men_part2 = *men_part;
            std::vector<int64_t> first(executors, 0);
            std::vector<int64_t> count(executors, 0);
            for (int64_t i = men_part2.size() - 1; i > -1; i--) {
                first[men_part2[i].first] = i;
                count[men_part2[i].first] += 1;
            }
            for (int64_t e = 0; e < executors; e++) {
                int64_t stop = first[e] + count[e];
                for (int64_t i = first[e]; i < stop; i++) { writer->write(std::move(men_part2[i].second)); }
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    executor_data->setPartitions(output);
}

template<typename Tp>
void IRepartitionImplClass::unordered_repartition(int64_t numPartitions) {
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto executors = executor_data->getContext().executors();
    auto rank = executor_data->mpi().rank();
    std::vector<int64_t> executors_count(executors);
    int64_t local_count = 0;
    int64_t global_count = 0;
    int64_t msg_max = 0;
    int64_t msg_num = 0;
    for (auto part : *input) {
        local_count += part->size();
        if (part->size() > msg_max) { msg_max = part->size(); }
    }
    IGNIS_LOG(info) << "Repartition: unordered repartition from " << input->partitions() << " partitions";
    executor_data->mpi().native().Allgather(&local_count, 1, MPI::LONG_LONG, &executors_count[0], 1, MPI::LONG_LONG);
    executor_data->mpi().native().Allreduce(MPI_IN_PLACE, &msg_max, 1, MPI::LONG_LONG, MPI::MAX);

    for (int64_t i = 0; i < executors; i++) { global_count += executors_count[i]; }
    int64_t average = global_count / executors;
    std::vector<int64_t> executors_overhead(executors);
    for (int64_t i = 0; i < executors; i++) { executors_overhead[i] = executors_count[i] - average; }
    std::vector<std::tuple<int64_t, int64_t, int64_t>> src_target_size;
    std::vector<int64_t> msg_count(executors, 0);
    int64_t other = 0;
    for (int64_t i = 0; i < executors; i++) {
        while (executors_overhead[i] > 0 && other < executors) {
            if (executors_overhead[other] >= 0) {
                other++;
                continue;
            }
            int64_t elems;
            if (executors_overhead[other] + executors_overhead[i] > 0) {
                elems = -executors_overhead[other];
                executors_overhead[other] = 0;
                executors_overhead[i] -= elems;
            } else {
                elems = executors_overhead[i];
                executors_overhead[i] = 0;
                executors_overhead[other] += elems;
            }

            while (elems > 0) {
                int64_t total = std::min(elems, msg_max);
                src_target_size.emplace_back(i, other, total);
                elems -= total;
                msg_count[i]++;
                if (msg_count[i] > msg_num) { msg_num = msg_count[i]; }
            }
        }
    }
    auto shared = executor_data->getPartitionTools().newPartitionGroup<Tp>(msg_num * executors);

    auto &src = *input;
    auto src_part = src[src.partitions() - 1];
    auto reader = src_part->readIterator();
    bool used = false;
    for (auto &entry : src_target_size) {
        if (std::get<0>(entry) == rank) {
            int64_t elems = std::get<2>(entry);
            int64_t target = msg_num * std::get<1>(entry);
            while (!(*shared)[target]->empty()) { target++; }
            auto writer = (*shared)[target]->writeIterator();

            while (elems > 0) {
                used = true;
                while (reader->hasNext() && elems > 0) {
                    writer->write(reader->next());
                    elems -= 1;
                }
                if (elems > 0) {
                    src.remove(src.partitions() - 1);
                    src_part = src[src.partitions() - 1];
                    reader = src_part->readIterator();
                }
            }
        }
    }

    if (used && reader->hasNext()) {
        auto new_part = executor_data->getPartitionTools().newPartition<Tp>(src_part->type());
        auto writer = new_part->writeIterator();
        while (reader->hasNext()) { writer->write(reader->next()); }
        src[src.partitions() - 1] = new_part;
    }
    IGNIS_LOG(info) << "Repartition: exchanging new partitions";

    auto tmp = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    exchange(*shared, *tmp);
    for (auto part : *input) { output->add(part); }
    for (auto part : *tmp) {
        if (!part->empty()) { output->add(part); }
    }
    executor_data->setPartitions(output);
    local_repartition<Tp>(numPartitions);
}

template<typename Tp>
void IRepartitionImplClass::local_repartition(int64_t numPartitions) {
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    int64_t executors = executor_data->getContext().executors();
    int64_t elements = 0;
    for (auto &part : (*input)) { elements += part->size(); }
    int64_t localPartitions = numPartitions / executors;
    if (numPartitions % executors > executor_data->getContext().executorId()) { localPartitions++; }
    IGNIS_LOG(info) << "Repartition: local repartition from " << input->partitions() << " to " << localPartitions
                    << " partitions";
    int64_t partition_elems = elements / localPartitions;
    int64_t remainder = elements % localPartitions;
    int64_t i = 1;
    int64_t ew = 0;
    int64_t er = (*input)[0]->size();
    auto it = (*input)[0]->readIterator();
    for (int64_t p = 0; p < localPartitions; p++) {
        auto part = executor_data->getPartitionTools().newPartition<Tp>();
        auto writer = part->writeIterator();
        ew = partition_elems;
        if (p < remainder) { ew++; }

        while (ew > 0 && (i < input->partitions() || er > 0)) {
            if (er == 0) {
                (*input)[i - 1].reset();
                er = (*input)[i]->size();
                it = (*input)[i++]->readIterator();
            }
            for (; ew > 0 && er > 0; ew--, er--) { writer->write(std::move(it->next())); }
        }
        part->fit();
        output->add(part);
    }

    executor_data->setPartitions(output);
}

template<typename Tp>
void IRepartitionImplClass::partitionByRandom(int64_t numPartitions, int32_t seed) {
    IGNIS_TRY()
    partitionBy_impl<Tp>(
            [seed](const Tp &obj) {
                static thread_local std::mt19937 gen(seed);
                static thread_local std::uniform_int_distribution<int> dist;
                return dist(gen);
            },
            numPartitions);
    IGNIS_CATCH()
}

template<typename Tp>
void IRepartitionImplClass::partitionByHash(int64_t numPartitions) {
    IGNIS_TRY()
    partitionBy_impl<Tp>(
            [](const Tp &obj) {
                const static std::hash<Tp> hash;
                return hash(obj);
            },
            numPartitions);
    IGNIS_CATCH()
}

template<typename Function>
void IRepartitionImplClass::partitionBy(int64_t numPartitions) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function f;
    f.before(context);
    partitionBy_impl<typename Function::_T_type>(
            [&f, &context](typename Function::_T_type &obj) { return f.call(obj, context); }, numPartitions);
    f.after(context);
    IGNIS_CATCH()
}

template<typename Tp, typename Particioner>
void IRepartitionImplClass::partitionBy_impl(Particioner f, int64_t numPartitions) {
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);

    IGNIS_LOG(info) << "Repartition: partitionBy in " << input->partitions() << " partitions";
    auto global = executor_data->getPartitionTools().newPartitionGroup<Tp>(numPartitions);
    if (input->partitions() > 0) {
        std::vector<decltype(global)> locals;
        for (int64_t i = 0; i < executor_data->getCores(); i++) {
            locals.push_back(executor_data->getPartitionTools().newPartitionGroup<Tp>(numPartitions));
        }

        IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
        {
            auto local = locals[executor_data->getContext().threadId()];
            std::vector<std::shared_ptr<api::IWriteIterator<Tp>>> writers;
            for (int64_t p = 0; p < local->partitions(); p++) { writers.push_back((*local)[p]->writeIterator()); }

            IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
            for (int64_t p = 0; p < input->partitions(); p++) {
                auto sz = (*input)[p]->size();
                if (isMemory) {
                    auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                    for (size_t i = 0; i < sz; i++) {
                        writers[((size_t) f(men_part[i])) % numPartitions]->write(men_part[i]);
                    }
                } else {
                    auto reader = (*input)[p]->readIterator();
                    for (size_t i = 0; i < sz; i++) {
                        auto &elem = reader->next();
                        writers[((size_t) f(elem)) % numPartitions]->write(elem);
                    }
                }
                (*input)[p].reset();
            }

#pragma omp for schedule(static)
            for (int64_t p = 0; p < global->partitions(); p++) {
                for (int64_t l = 0; l < executor_data->getCores(); l++) {
                    if ((*locals[l])[p]) { (*locals[l])[p]->moveTo(*((*global)[p])); }
                }
            }
            IGNIS_OMP_CATCH()
        }
        IGNIS_OMP_EXCEPTION_END()
    }

    IGNIS_LOG(info) << "Repartition: exchanging new partitions";
    exchange(*global, *output);
    executor_data->setPartitions(output);
}

#undef IRepartitionImplClass