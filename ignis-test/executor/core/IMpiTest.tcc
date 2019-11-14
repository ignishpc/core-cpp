
#include "IMpiTest.h"
#include <boost/filesystem.hpp>

#define IMpiTestClass ignis::executor::core::IMpiTest

template<typename Ps>
std::shared_ptr<ignis::executor::core::storage::IPartition<typename Ps::value_type>> IMpiTestClass<Ps>::create() {
    if (storage::IDiskPartition<Tp>::TYPE == Ps::TYPE) {
        std::string path = boost::filesystem::current_path().generic_string() + "/diskpartitionTest" +
                           std::to_string(disk_id++);
        return std::make_shared<storage::IDiskPartition < Tp>>
        (path);
    } else if (storage::IRawMemoryPartition<Tp>::TYPE == Ps::TYPE) {
        return std::make_shared<storage::IRawMemoryPartition < Tp>>
        ();
    } else if (storage::IMemoryPartition<Tp>::TYPE == Ps::TYPE) {
        return std::make_shared<storage::IMemoryPartition < Tp>>
        ();
    }
}

template<typename Ps>
void IMpiTestClass<Ps>::insert(api::IVector <Tp> &v, storage::IPartition <Tp> &part) {
    auto it = part.writeIterator();
    for (auto &e:v) {
        it->write(e);
    }
}

template<typename Ps>
void IMpiTestClass<Ps>::get(api::IVector <Tp> &v, storage::IPartition <Tp> &part) {
    auto it = part.readIterator();
    while (it->hasNext()) {
        v.push_back(it->next());
    }
}

template<typename Ps>
void IMpiTestClass<Ps>::setUp() {
    executor_data = std::make_shared<IExecutorData>();
    auto &props = executor_data->getContext().props();
    props["ignis.transport.compression"] = "6";
    props["ignis.partition.compression"] = "6";
    props["ignis.partition.serialization"] = "native";
}

template<typename Ps>
void IMpiTestClass<Ps>::tearDown() {}

template<typename Ps>
void IMpiTestClass<Ps>::gatherTest() {
    int n = 100;
    int rank = executor_data->getContext().mpiGroup().Get_rank();
    int size = executor_data->getContext().mpiGroup().Get_size();
    api::IVector <Tp> elems = IElements<Tp>().create(n * size, 0);
    api::IVector <Tp> local_elems(elems.begin() + n * rank, elems.begin() + n * (rank + 1));
    auto part = create();
    insert(local_elems, *part);
    executor_data->mpi().gather(*part, 0);
    if(rank == 0){
        api::IVector <Tp> result;
        get(result, *part);
        CPPUNIT_ASSERT(elems == result);
    }
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::bcastTest() {
    int n = 100;
    int rank = executor_data->getContext().mpiGroup().Get_rank();
    api::IVector <Tp> elems = IElements<Tp>().create(n, 0);
    auto part = create();
    if (rank == 0) {
        insert(elems, *part);
    }
    executor_data->mpi().bcast(*part, 0);
    api::IVector <Tp> result;
    get(result, *part);
    CPPUNIT_ASSERT(elems == result);
    executor_data->mpi().barrier();
}

#undef IMpiTestClass
