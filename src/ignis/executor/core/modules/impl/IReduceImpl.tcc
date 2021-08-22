
#include "IReduceImpl.h"

#define IReduceImplClass ignis::executor::core::modules::impl::IReduceImpl

template<typename Function, typename Tp>
inline void IReduceImplClass::basicReduce(Function &f, storage::IMemoryPartition<Tp> &result) {
    auto input = executor_data->getAndDeletePartitions<typename Function::_T1_type>();
    IGNIS_LOG(info) << "Reduce: reducing " << input->partitions() << " partitions locally";

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        Tp acum;
        bool acum_flag = false;

#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            if ((*input)[p]->size() == 0) { continue; }
            if (acum_flag) {
                aggregatePartition(f, *(*input)[p], acum);
            } else {
                acum = reducePartition(f, *(*input)[p]);
                acum_flag = true;
            }
            (*input)[p].reset();
        }
        if (acum_flag) {
#pragma omp critical
            { result.writeIterator()->write(std::move(acum)); }
        }

        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
}

template<typename Function, typename Tp>
inline void IReduceImplClass::basicFold(Function &f, storage::IMemoryPartition<Tp> &result) {
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type>();
    auto input = executor_data->getAndDeletePartitions<typename Function::_T2_type>();
    IGNIS_LOG(info) << "Reduce: folding " << input->partitions() << " partitions locally";

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto acum = executor_data->getVariable<typename Function::_T1_type>("zero");
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            if ((*input)[p]->size() == 0) { continue; }
            aggregatePartition(f, *(*input)[p], acum);
            (*input)[p].reset();
        }
#pragma omp critical
        { result.writeIterator()->write(std::move(acum)); }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
}


template<typename Function>
void IReduceImplClass::reduce() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    auto elem_part = executor_data->getPartitionTools().newMemoryPartition<typename Function::_R_type>(1);
    basicReduce<Function, typename Function::_R_type>(function, *elem_part);
    finalReduce<Function, typename Function::_R_type>(function, *elem_part);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Function>
void IReduceImplClass::treeReduce() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    auto elem_part = executor_data->getPartitionTools().newMemoryPartition<typename Function::_R_type>(1);
    basicReduce<Function, typename Function::_R_type>(function, *elem_part);
    finalTreeReduce<Function, typename Function::_R_type>(function, *elem_part);
    function.after(context);
    IGNIS_CATCH()
}


template<typename Function>
void IReduceImplClass::zero() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function f;
    f.before(context);
    executor_data->setVariable("zero", f.call(context));
    f.after(context);
    IGNIS_CATCH()
}

template<typename Function>
void IReduceImplClass::aggregate() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type>();
    auto input = executor_data->getAndDeletePartitions<typename Function::_T2_type>();
    auto partial_reduce = executor_data->getPartitionTools().newMemoryPartition<typename Function::_R_type>(
            executor_data->getCores());
    IGNIS_LOG(info) << "Reduce: aggregating " << input->partitions() << " partitions locally";

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto acum = executor_data->getVariable<typename Function::_T1_type>("zero");
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            if ((*input)[p]->size() == 0) { continue; }
            aggregatePartition(function, *(*input)[p], acum);
            (*input)[p].reset();
        }
#pragma omp critical
        { partial_reduce->writeIterator()->write(std::move(acum)); }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    function.after(context);
    output->add(partial_reduce);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}


template<typename Function>
void IReduceImplClass::fold() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    auto partial_reduce = executor_data->getPartitionTools().newMemoryPartition<typename Function::_R_type>();
    basicFold<Function, typename Function::_R_type>(function, *partial_reduce);
    finalReduce<Function, typename Function::_R_type>(function, *partial_reduce);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Function>
void IReduceImplClass::treeFold() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    auto partial_reduce = executor_data->getPartitionTools().newMemoryPartition<typename Function::_R_type>();
    basicFold<Function, typename Function::_R_type>(function, *partial_reduce);
    finalTreeReduce<Function, typename Function::_R_type>(function, *partial_reduce);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Tp>
void IReduceImplClass::groupByKey(int64_t numPartitions) {
    IGNIS_TRY()
    keyHashing<Tp>(numPartitions);
    keyExchanging<Tp>();

    auto input = executor_data->getAndDeletePartitions<Tp>();
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);
    const bool cache = input->cache();
    auto output =
            executor_data->getPartitionTools()
                    .newPartitionGroup<std::pair<typename Tp::first_type, api::IVector<typename Tp::second_type>>>(
                            numPartitions);
    IGNIS_LOG(info) << "Reduce: reducing key elements";

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        std::unordered_map<typename Tp::first_type, api::IVector<typename Tp::second_type>> acum;

#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto &part = *(*input)[p];
            if (isMemory) {
                auto &men_part = executor_data->getPartitionTools().toMemory(part);
                for (int64_t i = 0; i < men_part.size(); i++) {
                    auto &elem = men_part[i];
                    acum[std::move(elem.first)].push_back(std::move(elem.second));
                }
                auto writer = (*output)[p]->writeIterator();
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                for (auto &elem : acum) { men_writer.write(std::move(elem)); }
            } else {
                auto reader = part.readIterator();
                for (int64_t i = 0; i < part.size(); i++) {
                    auto &elem = reader->next();
                    acum[std::move(elem.first)].push_back(std::move(elem.second));
                }
                auto writer = (*output)[p]->writeIterator();
                for (auto &elem : acum) { writer->write(std::move(elem)); }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
            acum.clear();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Tp, typename Function>
void IReduceImplClass::reduceByKey(int64_t numPartitions, bool localReduce) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function f;
    f.before(context);
    if (localReduce) {
        IGNIS_LOG(info) << "Reduce: local reducing key elements";
        localReduceByKey<Function, Tp>(f);
    }
    keyHashing<Tp>(numPartitions);
    keyExchanging<Tp>();
    IGNIS_LOG(info) << "Reduce: reducing key elements";

    localReduceByKey<Function, Tp>(f);
    f.after(context);
    IGNIS_CATCH()
}


template<typename Tp, typename Function>
void IReduceImplClass::aggregateByKey(int64_t numPartitions, bool hashing) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function f;
    f.before(context);
    if (hashing) {
        keyHashing<Tp>(numPartitions);
        keyExchanging<Tp>();
    }
    IGNIS_LOG(info) << "Reduce: aggregating key elements";

    localAggregateByKey<Function, Tp>(f);
    f.after(context);

    IGNIS_CATCH()
}

template<typename Tp, typename Function>
void IReduceImplClass::foldByKey(int64_t numPartitions, bool localFold) {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function f;
    f.before(context);
    if (localFold) {
        IGNIS_LOG(info) << "Reduce: local folding key elements";
        localAggregateByKey<Function, Tp>(f);
        keyHashing<Tp>(numPartitions);
        keyExchanging<Tp>();
        IGNIS_LOG(info) << "Reduce: folding key elements";
        localReduceByKey<Function, Tp>(f);
    } else {
        keyHashing<Tp>(numPartitions);
        keyExchanging<Tp>();
        IGNIS_LOG(info) << "Reduce: folding key elements";
        localAggregateByKey<Function, Tp>(f);
    }
    f.after(context);
    IGNIS_CATCH()
}

template<typename Tp>
void IReduceImplClass::union_(const std::string &other, bool preserveOrder) {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto input2 = executor_data->getContext().var<decltype(input)>(other);
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    IGNIS_LOG(info) << "Reduce: union " << input->partitions() << " and " << input2->partitions() << " partitions";
    std::string storage;
    if (input->partitions() > 0) {
        storage = (*input)[0]->type();
        if (input2->partitions() > 0) {
            if ((*input)[0]->type() != (*input2)[0]->type()) {
                for (int64_t i = 0; i < input2->partitions(); i++) {
                    auto new_part = executor_data->getPartitionTools().newPartition<Tp>((*input)[0]->type());
                    (*input2)[i]->copyTo(*new_part);
                    (*input2)[i] = new_part;
                }
            }
        }
    } else {
        storage = executor_data->getProperties().partitionType();
    }
    if (preserveOrder) {
        IGNIS_LOG(info) << "Reduce: union using order mode";
        int64_t executors = executor_data->getContext().executors();
        int64_t rank = executor_data->getContext().executorId();
        int64_t count = input->partitions();
        int64_t global_count = 0;
        int64_t offset = 0;
        std::vector<int64_t> exec_counts(executors);
        int64_t count2 = input2->partitions();
        int64_t global_count2 = 0;
        int64_t offset2 = 0;
        std::vector<int64_t> exec_counts2(executors);
        executor_data->mpi().native().Allgather(&count, 1, MPI::LONG_LONG, &exec_counts[0], 1, MPI::LONG_LONG);
        executor_data->mpi().native().Allgather(&count2, 1, MPI::LONG_LONG, &exec_counts2[0], 1, MPI::LONG_LONG);

        for (int64_t i = 0; i < executors; i++) {
            if (i == rank) {
                offset = global_count;
                offset2 = global_count2;
            }
            global_count += exec_counts[i];
            global_count2 += exec_counts2[i];
        }
        auto tmp = executor_data->getPartitionTools().newPartitionGroup<Tp>();
        auto create = [&](int64_t n) {
            for (int64_t i = 0; i < n; i++) { tmp->add(executor_data->getPartitionTools().newPartition<Tp>(storage)); }
        };

        create(offset);
        for (auto part : *input) { tmp->add(part); }
        create(global_count - tmp->partitions());
        create(offset2);
        for (auto part : *input2) { tmp->add(part); }
        create(global_count + global_count2 - tmp->partitions());
        exchange(*tmp, *output);
    } else {
        IGNIS_LOG(info) << "Reduce: union using fast mode";
        for (auto part : *input) { output->add(part); }
        for (auto part : *input2) { output->add(part); }
    }

    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Tp>
void IReduceImplClass::join(const std::string &other, int64_t numPartitions) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "Reduce: preparing first partitions";
    keyHashing<Tp>(numPartitions);
    keyExchanging<Tp>();
    auto input = executor_data->getAndDeletePartitions<Tp>();

    IGNIS_LOG(info) << "Reduce: preparing second partitions";
    executor_data->setPartitions(executor_data->getContext().var<decltype(input)>(other));
    keyHashing<Tp>(numPartitions);
    keyExchanging<Tp>();
    auto input2 = executor_data->getAndDeletePartitions<Tp>();

    IGNIS_LOG(info) << "Reduce: joining key elements";
    typedef std::pair<typename Tp::second_type, typename Tp::second_type> Value_Type;
    typedef std::pair<typename Tp::first_type, Value_Type> Return_Type;
    auto output = executor_data->getPartitionTools().newPartitionGroup<Return_Type>(numPartitions);

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        std::unordered_map<typename Tp::first_type, api::IVector<typename Tp::second_type>> acum;
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto reader = (*input)[p]->readIterator();
            while (reader->hasNext()) {
                auto &elem = reader->next();
                acum[elem.first].push_back(elem.second);
            }

            reader = (*input2)[p]->readIterator();
            while (reader->hasNext()) {
                auto &elem = reader->next();
                auto it = acum.find(elem.first);
                if (it != acum.end()) {
                    for (const auto &value : it->second) {
                        writer->write(Return_Type(it->first, Value_Type(value, elem.second)));
                    }
                }
            }
            acum.clear();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Tp>
void IReduceImplClass::distinct(int64_t numPartitions) {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    IGNIS_LOG(info) << "Reduce: distinct " << input->partitions() << " partitions";
    auto tmp = executor_data->getPartitionTools().newPartitionGroup<Tp>(numPartitions);
    const bool in_men = executor_data->getPartitionTools().isMemory(*input) &&
                        executor_data->getPartitionTools().isMemory(*tmp) && !input->cache();
    const std::hash<Tp> hash;
    IGNIS_LOG(info) << "Reduce: creating " << numPartitions << " new partitions with hashing";

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto thread_ranges = executor_data->getPartitionTools().newPartitionGroup<Tp>(tmp->partitions());
        std::vector<std::shared_ptr<api::IWriteIterator<Tp>>> writers;
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) {
            writers.push_back((*thread_ranges)[p]->writeIterator());
        }
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto reader = (*input)[p]->readIterator();
            if (in_men) {
                auto &men_reader = executor_data->getPartitionTools().toMemory(*reader);
                while (men_reader.hasNext()) {
                    auto &elem = men_reader.next();
                    static_cast<storage::IMemoryWriteIterator<Tp> &>(*writers[hash(elem) % numPartitions])
                            .write(std::move(elem));
                }

            } else {
                while (reader->hasNext()) {
                    auto &elem = reader->next();
                    writers[hash(elem) % numPartitions]->write(elem);
                }
            }
            (*input)[p].reset();
        }
#pragma omp critical
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) { (*thread_ranges)[p]->moveTo(*((*tmp)[p])); }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();

    distinctFilter<Tp>(*tmp);
    exchange(*tmp, *output);
    distinctFilter<Tp>(*output);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function, typename Tp>
inline Tp IReduceImplClass::reducePartition(Function &f, storage::IPartition<Tp> &part) {
    auto &context = executor_data->getContext();
    if (executor_data->getPartitionTools().isMemory(part)) {
        auto &men_part = executor_data->getPartitionTools().toMemory(part);
        auto acum = men_part[0];
        for (int64_t i = 1; i < men_part.size(); i++) { acum = f.call(acum, men_part[i], context); }
        return std::move(acum);
    } else {
        auto reader = part.readIterator();
        auto acum = reader->next();
        for (int64_t i = 1; i < part.size(); i++) { acum = f.call(acum, reader->next(), context); }
        return std::move(acum);
    }
}

template<typename Function, typename Tp>
inline void IReduceImplClass::finalReduce(Function &f, storage::IMemoryPartition<Tp> &partial) {
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type>();
    IGNIS_LOG(info) << "Reduce: reducing all elements in the executor";
    if (partial.size() > 1) {
        partial[0] = reducePartition(f, partial);
        partial.resize(1);
    }

    IGNIS_LOG(info) << "Reduce: gathering elements for an executor";
    executor_data->mpi().gather(partial, 0);
    if (executor_data->mpi().isRoot(0) && partial.size() > 0) {
        IGNIS_LOG(info) << "Reduce: final reduce";
        auto result = executor_data->getPartitionTools().newMemoryPartition<typename Function::_R_type>(1);
        result->writeIterator()->write(reducePartition(f, partial));
        output->add(result);
    }
    executor_data->setPartitions(output);
}

template<typename Function, typename Tp>
inline void IReduceImplClass::finalTreeReduce(Function &f, storage::IMemoryPartition<Tp> &partial) {
    auto executors = executor_data->mpi().executors();
    auto rank = executor_data->mpi().rank();
    auto &context = executor_data->getContext();
    int64_t pivotUp = executors;
    int64_t pivotDown;
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type>();

    IGNIS_LOG(info) << "Reduce: reducing all elements in the executor";
    if (partial.size() > 1) {
        if (executor_data->getCores() == 1) {
            partial[0] = reducePartition(f, partial);
        } else {
            IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
            {
                IGNIS_OMP_TRY()
                auto id = executor_data->getContext().threadId();
                int64_t n = partial.size();
                int64_t n2 = n / 2;
                while (n2 > id) {
#pragma omp for schedule(static)
                    for (int64_t i = 0; i < n2; i++) { partial[i] = f.call(partial[i], partial[n - i], context); }
                    n = (int64_t) std::ceil(n / 2.0);
                    n2 = n / 2;
                }
                IGNIS_OMP_CATCH()
            }
            IGNIS_OMP_EXCEPTION_END()
        }
        partial.resize(1);
    }

    IGNIS_LOG(info) << "Reduce: performing a final tree reduce";
    while (pivotUp > 1) {
        pivotDown = (int64_t) std::floor(pivotUp / 2.0);
        pivotUp = (int64_t) std::ceil(pivotUp / 2.0);
        if (rank < pivotDown) {
            executor_data->mpi().recv(partial, rank + pivotUp, 0);
            partial[0] = f.call(partial[0], partial[1], context);
            partial.resize(1);
        } else if (rank >= pivotUp) {
            executor_data->mpi().send(partial, rank - pivotUp, 0);
        }
    }
    if (executor_data->mpi().isRoot(0) && partial.size() > 0) {
        auto result = executor_data->getPartitionTools().newMemoryPartition<typename Function::_R_type>(1);
        result->writeIterator()->write(std::move(partial[0]));
        output->add(result);
    }
    executor_data->setPartitions(output);
}

template<typename Function, typename Tp, typename Tp2>
inline void IReduceImplClass::aggregatePartition(Function &f, storage::IPartition<Tp> &part, Tp2 &acum) {
    auto &context = executor_data->getContext();
    if (executor_data->getPartitionTools().isMemory(part)) {
        auto &men_part = executor_data->getPartitionTools().toMemory(part);
        for (int64_t i = 0; i < men_part.size(); i++) { acum = f.call(acum, men_part[i], context); }
    } else {
        auto reader = part.readIterator();
        for (int64_t i = 0; i < part.size(); i++) { acum = f.call(acum, reader->next(), context); }
    }
}

template<typename Function, typename Tp>
void IReduceImplClass::localReduceByKey(Function &f) {
    auto input = executor_data->getPartitions<Tp>();
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);
    auto output = input;
    if (output->cache()) {
        output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
        for (int64_t p = 0; p < input->partitions(); p++) {
            output->add(executor_data->getPartitionTools().newPartition<Tp>((*input)[0]->type()));
        }
    }

    auto &context = executor_data->getContext();
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        std::unordered_map<typename Tp::first_type, typename Tp::second_type> acum;

#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto &part_in = *(*input)[p];
            auto &part_out = *(*output)[p];
            if (isMemory) {
                auto &men_part = executor_data->getPartitionTools().toMemory(part_in);
                for (int64_t i = 0; i < men_part.size(); i++) {
                    auto &elem = men_part[i];
                    auto it = acum.find(elem.first);
                    if (it == acum.end()) {
                        acum.insert(it, std::move(elem));
                    } else {
                        it->second = f.call(it->second, elem.second, context);
                    }
                }
                part_out.clear();
                auto writer = part_out.writeIterator();
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                for (auto &elem : acum) { men_writer.write(std::move(elem)); }
            } else {
                auto reader = part_in.readIterator();
                for (int64_t i = 0; i < part_in.size(); i++) {
                    auto &elem = reader->next();
                    auto it = acum.find(elem.first);
                    if (it == acum.end()) {
                        acum.insert(it, std::move(elem));
                    } else {
                        it->second = f.call(it->second, elem.second, context);
                    }
                }
                part_out.clear();
                auto writer = part_out.writeIterator();
                for (auto &elem : acum) { writer->write(std::move(elem)); }
            }
            acum.clear();
            part_out.fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(output);
}

template<typename Function, typename Tp>
void IReduceImplClass::localAggregateByKey(Function &f) {
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto output = executor_data->getPartitionTools()
                          .newPartitionGroup<std::pair<typename Tp::first_type, typename Function::_R_type>>(
                                  input->partitions());
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);
    auto &context = executor_data->getContext();
    auto base_acum = executor_data->getVariable<typename Function::_T1_type>("zero");
    const bool inMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    const bool cache = input->cache();

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        std::unordered_map<typename Tp::first_type, typename Function::_R_type> acum;

#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto &part_in = *(*input)[p];
            if (isMemory) {
                auto &men_part = executor_data->getPartitionTools().toMemory(part_in);
                for (int64_t i = 0; i < men_part.size(); i++) {
                    auto &elem = men_part[i];
                    auto it = acum.find(elem.first);
                    if (it == acum.end()) {
                        if (cache) {
                            acum.insert(it, std::pair<typename Tp::first_type, typename Function::_R_type>(
                                                    std::move(elem.first), f.call(base_acum, elem.second, context)));
                        } else {
                            acum.insert(it, std::pair<typename Tp::first_type, typename Function::_R_type>(
                                                    elem.first, f.call(base_acum, elem.second, context)));
                        }
                    } else {
                        it->second = f.call(it->second, elem.second, context);
                    }
                }
                auto writer = (*output)[p]->writeIterator();
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                for (auto &elem : acum) { men_writer.write(std::move(elem)); }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (int64_t i = 0; i < part_in.size(); i++) {
                    auto &elem = reader->next();
                    auto it = acum.find(elem.first);
                    if (it == acum.end()) {
                        acum.insert(it, std::pair<typename Tp::first_type, typename Function::_R_type>(
                                                std::move(elem.first), f.call(base_acum, elem.second, context)));
                    } else {
                        it->second = f.call(it->second, elem.second, context);
                    }
                }
                auto writer = (*output)[p]->writeIterator();
                for (auto &elem : acum) { writer->write(std::move(elem)); }
            }
            acum.clear();
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(output);
}

template<typename Tp>
void IReduceImplClass::keyHashing(int64_t numPartitions) {
    auto input = executor_data->getPartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>(numPartitions);

    const bool in_men =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    const bool cache = input->cache();
    const std::hash<typename Tp::first_type> hash;
    IGNIS_LOG(info) << "Reduce: creating " << numPartitions << " new partitions with key hashing";

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        auto thread_ranges = executor_data->getPartitionTools().newPartitionGroup<Tp>(output->partitions());
        std::vector<std::shared_ptr<api::IWriteIterator<Tp>>> writers;
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) {
            writers.push_back((*thread_ranges)[p]->writeIterator());
        }
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto reader = (*input)[p]->readIterator();
            if (in_men) {
                auto &men_reader = executor_data->getPartitionTools().toMemory(*reader);
                if (cache) {
                    while (men_reader.hasNext()) {
                        auto &elem = men_reader.next();
                        static_cast<storage::IMemoryWriteIterator<Tp> &>(*writers[hash(elem.first) % numPartitions])
                                .write(elem);
                    }
                } else {
                    while (men_reader.hasNext()) {
                        auto &elem = men_reader.next();
                        static_cast<storage::IMemoryWriteIterator<Tp> &>(*writers[hash(elem.first) % numPartitions])
                                .write(std::move(elem));
                    }
                    (*input)[p]->clear();
                }
            } else {
                while (reader->hasNext()) {
                    auto &elem = reader->next();
                    writers[hash(elem.first) % numPartitions]->write(std::move(elem));
                }
                if (!cache) { (*input)[p]->clear(); }
            }
        }
#pragma omp critical
        for (int64_t p = 0; p < thread_ranges->partitions(); p++) { (*thread_ranges)[p]->moveTo(*((*output)[p])); }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(output);
}

template<typename Tp>
void IReduceImplClass::keyExchanging() {
    auto input = executor_data->getPartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    auto numPartitions = input->partitions();
    IGNIS_LOG(info) << "Reduce: exchanging " << numPartitions << " partitions keys";

    exchange<Tp>(*input, *output);

    executor_data->setPartitions(output);
}

template<typename Tp>
inline void IReduceImplClass::distinctFilter(storage::IPartitionGroup<Tp> &parts) {

    const bool in_men = executor_data->getPartitionTools().isMemory(parts);
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
        std::unordered_set<Tp> distinct;
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < parts.partitions(); p++) {
            auto new_part = executor_data->getPartitionTools().newPartition<Tp>();
            auto writer = new_part->writeIterator();
            if(in_men){
                auto &men_part = executor_data->getPartitionTools().toMemory(*parts[p]);
                for (int64_t i = 0; i < men_part.size(); i++) {
                    auto &elem = men_part[i];
                    auto it = distinct.find(elem);
                    if(it == distinct.end()){
                        writer->write(elem);
                    }
                    distinct.insert(elem);
                }
            }else{
                auto reader = parts[p]->readIterator();
                while (reader->hasNext()) {
                    auto &elem = reader->next();
                    auto it = distinct.find(elem);
                    if(it == distinct.end()){
                        writer->write(elem);
                    }
                    distinct.insert(elem);
                }
            }
            parts[p] = new_part;
            distinct.clear();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
}

#undef IReduceImplClass