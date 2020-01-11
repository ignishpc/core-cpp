
#include "IModuleTest.h"

#define IModuleTestClass ignis::executor::core::modules::IModuleTest

template<typename Tp>
ignis::executor::api::IVector<Tp> IModuleTestClass::rankVector(const api::IVector <Tp> &elems) {
    auto n = elems.size() / executor_data->getContext().executors();
    auto rank = executor_data->getContext().executorId();
    return api::IVector<Tp>(elems.begin() + n * rank, elems.begin() + n * (rank + 1));
}

template<typename Tp>
void IModuleTestClass::loadToPartitions(const api::IVector <Tp> &elems, int partitions) {
    auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(partitions);
    executor_data->setPartitions(group);
    int part_size = elems.size() / partitions;
    for (int p = 0; p < partitions; p++) {
        auto writer = (*group)[p]->writeIterator();
        for (int i = part_size * p; i < part_size * (p + 1) && i < elems.size(); i++) {
            writer->write(const_cast<Tp&>(elems[i]));
        }
    }
}

template<typename Tp>
ignis::executor::api::IVector<Tp> IModuleTestClass::getFromPartitions() {
    api::IVector <Tp> elems;
    auto group = executor_data->getPartitions<Tp>();
    for (int p = 0; p < group->partitions(); p++) {
        auto reader = (*group)[p]->readIterator();
        while (reader->hasNext()) {
            elems.push_back(reader->next());
        }
    }
    return std::move(elems);
}

#undef IModuleTestClass