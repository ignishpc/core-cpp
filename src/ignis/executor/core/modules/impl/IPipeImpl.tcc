
#include "IPipeImpl.h"

#define IPipeImplCLass ignis::executor::core::modules::impl::IPipeImpl

template<typename Function>
void IPipeImplCLass::map() {
    IGNIS_TRY()
        auto input = executor_data->getPartitions<typename Function::_T_type>();
        auto output = newPartitionGroup<typename Function::_R_type>(input->partitions());
        auto &context = executor_data->getContext();
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
                    if (isMemory(*(*input)[p]) && isMemory(*(*output)[p])) {
                        auto &men_writer = toMemory(*writer);
                        auto &men_part = toMemory(*(*input)[p]);
                        for (size_t i = 0; i < sz; i++) {
                            men_writer.write(function.call(men_part[i], context));
                        }
                    } else {
                        auto reader = (*input)[p]->readIterator();
                        for (size_t i = 0; i < sz; i++) {
                            writer->write(function.call(reader->next(), context));
                        }
                    }
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
        auto input = executor_data->getPartitions<typename Function::_T_type>();
        auto output = newPartitionGroup<typename Function::_T_type>(input->partitions());
        bool cache = input->cache() && isMemory(*input);
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
                    if (isMemory(*input) && isMemory(*output)) {
                        auto &men_writer = toMemory(*writer);
                        auto &men_part = toMemory(*(*input)[p]);
                        if (cache) {
                            for (size_t i = 0; i < sz; i++) {
                                if (function.call(men_part[i], context)) {
                                    if (cache) {
                                        men_writer.write(men_part[i]);
                                    } else {
                                        men_writer.write(std::move(men_part[i]));
                                    }
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
        auto input = executor_data->getPartitions<typename Function::_T_type>();
        auto output = newPartitionGroup<typename Function::_R_type::value_type>(input->partitions());
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
                    if (isMemory(*(*input)[p]) && isMemory(*(*output)[p])) {
                        auto &men_writer = toMemory(*writer);
                        auto &men_part = toMemory(*(*input)[p]);
                        for (size_t i = 0; i < sz; i++) {
                            auto result = function.call(men_part[i], context);
                            for (auto it = result.begin(); it != result.end(); it++) {
                                men_writer.write(std::move(*it));
                            }
                        }
                    } else {
                        auto reader = (*input)[p]->readIterator();
                        for (size_t i = 0; i < sz; i++) {
                            auto result = function.call(reader->next(), context);
                            for (auto it = result.begin(); it != result.end(); it++) {
                                writer->write(std::move(*it));
                            }
                        }
                    }
                }
            IGNIS_OMP_CATCH()
        }
        IGNIS_OMP_EXCEPTION_END()
        function.after(context);
        executor_data->setPartitions(output);
    IGNIS_CATCH()
}

template<typename Function>
void IPipeImplCLass::mapPartitions(bool preservesPartitioning) {
    IGNIS_TRY()
        auto input = executor_data->getPartitions<typename Function::_T_type::value_type>();
        auto output = newPartitionGroup<typename Function::_R_type::value_type>(input->partitions());
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
                    auto reader = (*input)[p]->readIterator();
                    auto result = function.call(*reader, context);
                    for (auto it = result.begin(); it != result.end(); it++) {
                        writer->write(std::move(*it));
                    }
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
        auto input = executor_data->getPartitions<typename Function::_T2_type::value_type>();
        auto output = newPartitionGroup<typename Function::_R_type::value_type>(input->partitions());
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
                    auto reader = (*input)[p]->readIterator();
                    auto result = function.call(p, *reader, context);
                    for (auto it = result.begin(); it != result.end(); it++) {
                        writer->write(std::move(*it));
                    }
                }
            IGNIS_OMP_CATCH()
        }
        IGNIS_OMP_EXCEPTION_END()
        function.after(context);
        executor_data->setPartitions(output);
    IGNIS_CATCH()
}

#undef IPipeImplCLass
