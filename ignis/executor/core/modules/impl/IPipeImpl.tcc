
#include "IPipeImpl.h"


#define IPipeImplCLass ignis::executor::core::modules::impl::IPipeImpl

template<typename Function>
void IPipeImplCLass::map() {
    try {
        auto cores = executor_data->getContext().cores();
        auto input = executor_data->getPartitions<typename Function::_T_type>();
        auto output = newPartitionGroup<typename Function::_R_type>(input->partitions());
        auto& context = executor_data->getContext();
        Function function;

        function.before(context);
        IGNIS_LOG(info) << "General: using " << cores << " threads with " << input->partitions() << " partitions";
        #pragma omp parallel for num_threads(cores) default(shared) schedule(static)
        for (int i = 0; i < input->partitions(); i++) {
            auto reader = (*input)[i]->readIterator();
            auto writer = (*output)[i]->writeIterator();
            //TODO use base function
            /*Avoid polymorphism, the compiler can detect that the elements are sequentially in a memory array.*/
            if(isMemory(*(*input)[i]) && isMemory(*(*output)[i])){
                auto & men_reader = reinterpret_cast<storage::IMemoryReadIterator<typename Function::_T_type>&>(*reader);
                auto & men_writer = reinterpret_cast<storage::IMemoryWriteIterator<typename Function::_R_type>&>(*writer);
                for (size_t e = 0; e < (*input)[i]->size(); e++) {
                    men_writer.write(function.call(men_reader.next(), context));
                }
            }else {
                for (size_t e = 0; e < (*input)[i]->size(); e++) {
                    writer->write(function.call(reader->next(), context));
                }
            }
        }
        function.after(context);
        executor_data->setPartitions(output);
    } catch (std::exception &ex) {
        throw exception::IException(ex);
    }
}

template<typename Function>
void IPipeImplCLass::filter() {
    try {
        auto cores = executor_data->getContext().cores();
        auto input = executor_data->getPartitions<typename Function::_T_type>();
        auto output = newPartitionGroup<typename Function::_T_type>(*input);
        auto& context = executor_data->getContext();
        Function function;

        function.before(context);
        IGNIS_LOG(info) << "General: using " << cores << " threads with " << input->partitions() << " partitions";
        #pragma omp parallel for num_threads(cores) default(shared) schedule(static)
        for (int i = 0; i < input->partitions(); i++) {
            auto reader = (*input)[i]->readIterator();
            auto writer = (*output)[i]->writeIterator();
            //TODO use base function
            /*Avoid polymorphism, the compiler can detect that the elements are sequentially in a memory array.*/
            if(isMemoryStorage(*(*input)[i]) && isMemoryStorage(*(*output)[i])){
                auto & men_reader = reinterpret_cast<storage::IMemoryReadIterator<typename Function::_T_type>&>(*reader);
                auto & men_writer = reinterpret_cast<storage::IMemoryWriteIterator<typename Function::_R_type>&>(*writer);
                for (size_t e = 0; e < (*input)[i]->size(); e++) {
                    auto &value = men_reader.next();
                    if(function.call(value, context)){
                        men_writer.write(value);
                    }
                }
            }else {
                for (size_t e = 0; e < (*input)[i]->size(); e++) {
                    auto &value = reader->next();
                    if(function.call(value, context)){
                        writer->write(value);
                    }
                }
            }
        }
        function.after(context);
        executor_data->setPartitions(output);
    } catch (std::exception &ex) {
        throw exception::IException(ex);
    }
}

template<typename Function>
void IPipeImplCLass::flatmap() {

}

template<typename Function>
void IPipeImplCLass::mapPartitions(bool preservesPartitioning) {

}

template<typename Function>
void IPipeImplCLass::mapPartitionsWithIndex(bool preservesPartitioning) {

}

#undef IPipeImplCLass
