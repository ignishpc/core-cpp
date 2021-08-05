
#include "IPipeImpl.h"

#define IPipeImplCLass ignis::executor::core::modules::impl::IPipeImpl

template<typename Function>
void IPipeImplCLass::execute() {
    IGNIS_TRY()
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    IGNIS_LOG(info) << "General: execute";
    function.call(context);
    function.after(context);
    IGNIS_CATCH()
}

template<typename Function, typename R>
void IPipeImplCLass::executeTo() {
    IGNIS_TRY()
    auto output = executor_data->getPartitionTools().newPartitionGroup<R>();
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    IGNIS_LOG(info) << "General: executeTo";

    auto newParts = function.call(context);

    IGNIS_LOG(info) << "General: moving elements to partitions";
    for (auto &v : newParts) {
        auto part = executor_data->getPartitionTools().newMemoryPartition<R>(0);
        std::swap(v, part->inner());
        output->add(part);
    }

    if (executor_data->getProperties().partitionType() != storage::IMemoryPartition<R>::TYPE) {
        IGNIS_LOG(info) << "General: saving partitions from memory";
        for (int64_t i = 0; i < output->partitions(); i++) {
            auto &oldPart = (*output)[i];
            auto part = executor_data->getPartitionTools().newPartition<R>(*oldPart);
            oldPart->moveTo(*part);
            oldPart = part;
        }
    }
    function.after(context);
    executor_data->setPartitions(output);

    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::map() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T_type>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type>(input->partitions());
    auto &context = executor_data->getContext();
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: map " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (size_t i = 0; i < sz; i++) { men_writer.write(function.call(men_part[i], context)); }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) { writer->write(function.call(reader->next(), context)); }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::filter() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T_type>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_T_type>(input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    bool cache = input->cache() && isMemory;
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: filter " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (size_t i = 0; i < sz; i++) {
                    if (function.call(men_part[i], context)) {
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
                    if (function.call(elem, context)) {
                        if (cache) {
                            writer->write(std::move(elem));
                        } else {
                            writer->write(elem);
                        }
                    }
                }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::flatmap() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T_type>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type::value_type>(
            input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: flatmap " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (size_t i = 0; i < sz; i++) {
                    auto result = function.call(men_part[i], context);
                    for (auto it = result.begin(); it != result.end(); it++) { men_writer.write(std::move(*it)); }
                }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) {
                    auto result = function.call(reader->next(), context);
                    for (auto it = result.begin(); it != result.end(); it++) { writer->write(std::move(*it)); }
                }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::keyBy() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T_type>();
    auto output = executor_data->getPartitionTools()
                          .newPartitionGroup<std::pair<typename Function::_R_type, typename Function::_T_type>>(
                                  input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    bool cache = input->cache() && executor_data->getPartitionTools().isMemory(*input);
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: keyBy " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                if (cache) {
                    for (size_t i = 0; i < sz; i++) {
                        men_writer.write(std::pair<typename Function::_R_type, typename Function::_T_type>(
                                function.call(men_part[i], context), std::move(men_part[i])));
                    }
                } else {
                    for (size_t i = 0; i < sz; i++) {
                        men_writer.write(std::pair<typename Function::_R_type, typename Function::_T_type>(
                                function.call(men_part[i], context), men_part[i]));
                    }
                }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) {
                    auto &elem = reader->next();
                    writer->write(std::pair<typename Function::_R_type, typename Function::_T_type>(
                            function.call(elem, context), std::move(elem)));
                }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::mapPartitions() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T_type::value_type>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type::value_type>(
            input->partitions());
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: mapPartitions " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto reader = (*input)[p]->readIterator();
            auto result = function.call(*reader, context);
            for (auto it = result.begin(); it != result.end(); it++) { writer->write(std::move(*it)); }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::mapPartitionsWithIndex(bool preservesPartitioning) {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T2_type::value_type>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Function::_R_type::value_type>(
            input->partitions());
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: mapPartitionsWithIndex " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto reader = (*input)[p]->readIterator();
            auto result = function.call(p, *reader, context);
            for (auto it = result.begin(); it != result.end(); it++) { writer->write(std::move(*it)); }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function, typename Tp>
void IPipeImplCLass::mapExecutor() {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    auto inMemory = executor_data->getPartitionTools().isMemory(*input);
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    IGNIS_LOG(info) << "General: mapExecutor " << input->partitions() << " partitions";
    if (!inMemory || input->cache()) {
        IGNIS_LOG(info) << "General: loading partitions in memory";
        auto aux = executor_data->getPartitionTools().newPartitionGroup<Tp>();
        for (auto &part : *input) {
            auto men = executor_data->getPartitionTools().newMemoryPartition<Tp>(part->size());
            part->copyTo(*men);
            aux->add(men);
        }
        input = aux;
    }

    typename Function::_T_type arg;
    for (auto &part : *input) {
        auto &men = executor_data->getPartitionTools().toMemory(*part);
        arg.push_back(&men.inner());
    }

    function.call(arg, context);

    if (!inMemory) {
        IGNIS_LOG(info) << "General: saving partitions from memory";
        auto aux = executor_data->getPartitionTools().newPartitionGroup<Tp>();
        for (auto &men : *input) {
            auto part = executor_data->getPartitionTools().newPartition<Tp>(*men);
            men->copyTo(*part);
            aux->add(part);
        }
        input = aux;
    }
    function.after(context);
    executor_data->setPartitions(input);

    IGNIS_CATCH()
}

template<typename Function, typename Tp, typename R>
void IPipeImplCLass::mapExecutorTo() {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<R>();
    auto inMemory = executor_data->getPartitionTools().isMemory(*input);
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    IGNIS_LOG(info) << "General: mapExecutorTo " << input->partitions() << " partitions";
    if (!inMemory) {
        IGNIS_LOG(info) << "General: loading partitions in memory";
        auto aux = executor_data->getPartitionTools().newPartitionGroup<Tp>();
        for (auto &part : *input) {
            auto men = executor_data->getPartitionTools().newMemoryPartition<Tp>(part->size());
            part->copyTo(*men);
            aux->add(men);
        }
        input = aux;
    }

    typename Function::_T_type arg;
    for (auto &part : *input) {
        auto &men = executor_data->getPartitionTools().toMemory(*part);
        arg.push_back(&men.inner());
    }

    auto newParts = function.call(arg, context);

    IGNIS_LOG(info) << "General: moving elements to partitions";
    for (auto &v : newParts) {
        auto part = executor_data->getPartitionTools().newMemoryPartition<R>(0);
        std::swap(v, part->inner());
        output->add(part);
    }


    if (!inMemory) {
        IGNIS_LOG(info) << "General: saving partitions from memory";
        for (int64_t i = 0; i < output->partitions(); i++) {
            auto &oldPart = (*output)[i];
            auto part = executor_data->getPartitionTools().newPartition<R>(*oldPart);
            oldPart->moveTo(*part);
            oldPart = part;
        }
    }
    function.after(context);
    executor_data->setPartitions(output);

    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::foreach () {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T_type>();
    auto &context = executor_data->getContext();
    bool isMemory = executor_data->getPartitionTools().isMemory(*input);
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: foreach " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (size_t i = 0; i < sz; i++) { function.call(men_part[i], context); }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) { function.call(reader->next(), context); }
            }
            (*input)[p].reset();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->deletePartitions();
    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::foreachPartition() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<typename Function::_T_type::value_type>();
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: foreachPartition " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto reader = (*input)[p]->readIterator();
            function.call(*reader, context);
            (*input)[p].reset();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->deletePartitions();
    IGNIS_CATCH()
}

template<typename Function, typename Tp>
void IPipeImplCLass::foreachExecutor() {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    auto inMemory = executor_data->getPartitionTools().isMemory(*input);
    auto &context = executor_data->getContext();
    Function function;
    function.before(context);
    IGNIS_LOG(info) << "General: foreachExecutor " << input->partitions() << " partitions";
    if (!inMemory) {
        IGNIS_LOG(info) << "General: loading partitions in memory";
        auto aux = executor_data->getPartitionTools().newPartitionGroup<Tp>();
        for (auto &part : *input) {
            auto men = executor_data->getPartitionTools().newMemoryPartition<Tp>(part->size());
            part->copyTo(*men);
            aux->add(men);
        }
        input = aux;
    }

    typename Function::_T_type arg;
    for (auto &part : *input) {
        auto &men = executor_data->getPartitionTools().toMemory(*part);
        arg.push_back(&men.inner());
    }

    function.call(arg, context);
    function.after(context);

    IGNIS_CATCH()
}

template<typename Tp>
void IPipeImplCLass::take(int64_t num) {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<Tp>();
    int64_t count = 0;
    for (auto &part : *input) {
        if (part->size() + count > num) {
            if (executor_data->getPartitionTools().isMemory(*part) && !input->cache()) {
                executor_data->getPartitionTools().toMemory(*part).resize(num - count);
                output->add(part);
            } else {
                auto cut = executor_data->getPartitionTools().newPartition<Tp>(part->type());
                auto reader = part->readIterator();
                auto writer = cut->writeIterator();
                while (count != num) {
                    count++;
                    writer->write(reader->next());
                }
                output->add(cut);
            }
            break;
        }
        count += part->size();
        output->add(part);
    }
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Tp>
void IPipeImplCLass::keys() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Tp::first_type>(input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    bool cache = input->cache() && isMemory;

    IGNIS_LOG(info) << "General: keys " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                if (cache) {
                    for (size_t i = 0; i < sz; i++) { men_writer.write(men_part[i].first); }
                } else {
                    for (size_t i = 0; i < sz; i++) { men_writer.write(std::move(men_part[i].first)); }
                }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) { writer->write(reader->next().first); }
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
void IPipeImplCLass::values() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto output = executor_data->getPartitionTools().newPartitionGroup<typename Tp::second_type>(input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    bool cache = input->cache() && isMemory;

    IGNIS_LOG(info) << "General: values " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                if (cache) {
                    for (size_t i = 0; i < sz; i++) { men_writer.write(men_part[i].second); }
                } else {
                    for (size_t i = 0; i < sz; i++) { men_writer.write(std::move(men_part[i].second)); }
                }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) { writer->write(reader->next().second); }
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

template<typename Tp, typename Function>
void IPipeImplCLass::flatMapValues() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto output =
            executor_data->getPartitionTools()
                    .newPartitionGroup<std::pair<typename Tp::first_type, typename Function::_R_type::value_type>>(
                            input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: flatMapValues " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                for (size_t i = 0; i < sz; i++) {
                    auto result = function.call(men_part[i].second, context);
                    for (auto it = result.begin(); it != result.end(); it++) {
                        men_writer.write(std::pair<typename Tp::first_type, typename Function::_R_type::value_type>(
                                men_part[i].first, std::move(*it)));
                    }
                }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) {
                    auto &pair = reader->next();
                    auto result = function.call(pair.second, context);
                    for (auto it = result.begin(); it != result.end(); it++) {
                        writer->write(std::pair<typename Tp::first_type, typename Function::_R_type::value_type>(
                                pair.first, std::move(*it)));
                    }
                }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Tp, typename Function>
void IPipeImplCLass::mapValues() {
    IGNIS_TRY()
    auto input = executor_data->getAndDeletePartitions<Tp>();
    auto output = executor_data->getPartitionTools()
                          .newPartitionGroup<std::pair<typename Tp::first_type, typename Function::_R_type>>(
                                  input->partitions());
    bool isMemory =
            executor_data->getPartitionTools().isMemory(*input) && executor_data->getPartitionTools().isMemory(*output);
    bool cache = input->cache() && isMemory;
    auto &context = executor_data->getContext();
    Function function;

    function.before(context);
    IGNIS_LOG(info) << "General: mapValues " << input->partitions() << " partitions";
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < input->partitions(); p++) {
            auto writer = (*output)[p]->writeIterator();
            auto sz = (*input)[p]->size();
            if (isMemory) {
                auto &men_writer = executor_data->getPartitionTools().toMemory(*writer);
                auto &men_part = executor_data->getPartitionTools().toMemory(*(*input)[p]);
                if (cache) {
                    for (size_t i = 0; i < sz; i++) {
                        men_writer.write(std::pair<typename Tp::first_type, typename Function::_R_type>(
                                men_part[i].first, function.call(men_part[i].second, context)));
                    }
                } else {
                    for (size_t i = 0; i < sz; i++) {
                        men_writer.write(std::pair<typename Tp::first_type, typename Function::_R_type>(
                                std::move(men_part[i].first), function.call(men_part[i].second, context)));
                    }
                }
            } else {
                auto reader = (*input)[p]->readIterator();
                for (size_t i = 0; i < sz; i++) {
                    auto &pair = reader->next();
                    writer->write(std::pair<typename Tp::first_type, typename Function::_R_type>(
                            std::move(pair.first), function.call(pair.second, context)));
                }
            }
            (*input)[p].reset();
            (*output)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    function.after(context);
    executor_data->setPartitions(output);
    IGNIS_CATCH()
}

#undef IPipeImplCLass
