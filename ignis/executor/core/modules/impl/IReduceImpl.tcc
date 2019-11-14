
#include "IReduceImpl.h"

#define IReduceImplClass ignis::executor::core::modules::impl::IReduceImpl

template<typename Function>
void IReduceImplClass::reduce() {
    try {
        auto input = executor_data->getPartitions<typename Function::_R_type>();
        auto partial_reduce = newPartition<typename Function::_R_type>();
        auto output = newPartitionGroup<typename Function::_R_type>();
        auto &context = executor_data->getContext();
        Function function;

        function.before(context);
        IGNIS_OMP_EXCEPTION_INIT()
        #pragma omp parallel
        {
            IGNIS_OMP_TRY()
                #pragma omp for schedule(dynamic)
                for (int64_t p = 0; p < input->partitions(); p++) {
                    if ((*input)[p]->size() == 0) {
                        continue;
                    }
                    auto acum = reducePartition(function, *(*input)[p]);
                    #pragma omp critical
                    {
                        partial_reduce->writeIterator()->write(std::move(acum));
                    }
                }
            IGNIS_OMP_CATCH()
        }
        IGNIS_OMP_EXCEPTION_END()

        auto elem_part = newMemoryPartition<typename Function::_R_type>(1);
        if (partial_reduce->size() > 0) {
            elem_part->writeIterator()->write(reducePartition(function, *partial_reduce));
        }
        partial_reduce.reset();
        executor_data->mpi().gather(*elem_part, 0);
        if (executor_data->mpi().isRoot(0)) {
            auto result = newMemoryPartition<typename Function::_R_type>(1);
            result->writeIterator()->write(reducePartition(function, *elem_part));
            output->add(result);
        }
        executor_data->setPartitions(output);
        function.after(context);
    } catch (std::exception &ex) {
        throw exception::IException(ex);
    }
}

template<typename Function>
void IReduceImplClass::treeReduce(int64_t depth) {

}

template<typename Function, typename Tp>
inline Tp IReduceImplClass::reducePartition(Function &f, storage::IPartition <Tp> &part) {
    auto &context = executor_data->getContext();
    if (isMemory(part)) {
        auto men_part = toMemory(part);
        auto acum = men_part[0];
        for (int64_t i = 1; i < men_part.size(); i++) {
            acum = f.call(acum, men_part[i], context);
        }
        return std::move(acum);
    } else {
        auto reader = part.readIterator();
        auto acum = reader->next();
        for (int64_t i = 1; i < part.size(); i++) {
            acum = f.call(acum, reader->next(), context);
        }
        return std::move(acum);
    }
}

#undef IReduceImplClass