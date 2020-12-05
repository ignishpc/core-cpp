
#include "IMpiTest.h"
#include <ghc/filesystem.hpp>

#define IMpiTestClass ignis::executor::core::IMpiTest

template<typename Ps>
std::shared_ptr<ignis::executor::core::storage::IPartition<typename Ps::value_type>> IMpiTestClass<Ps>::create() {
    return create(Ps::TYPE);
}
template<typename Ps>
std::shared_ptr<ignis::executor::core::storage::IPartition<typename Ps::value_type>>
IMpiTestClass<Ps>::create(const std::string &type) {
    if (storage::IDiskPartition<Tp>::TYPE == type) {
        std::string path =
                ghc::filesystem::current_path().generic_string() + "/diskpartitionTest" + std::to_string(disk_id++);
        return std::make_shared<storage::IDiskPartition<Tp>>(path);
    } else if (storage::IRawMemoryPartition<Tp>::TYPE == type) {
        return std::make_shared<storage::IRawMemoryPartition<Tp>>();
    }
    return std::make_shared<storage::IMemoryPartition<Tp>>();
}

template<typename Ps>
void IMpiTestClass<Ps>::insert(api::IVector<Tp> &v, storage::IPartition<Tp> &part) {
    auto it = part.writeIterator();
    for (auto &e : v) { it->write(e); }
}

template<typename Ps>
void IMpiTestClass<Ps>::get(api::IVector<Tp> &v, storage::IPartition<Tp> &part) {
    auto it = part.readIterator();
    while (it->hasNext()) { v.push_back(it->next()); }
}

template<typename Ps>
void IMpiTestClass<Ps>::setUp() {
    executor_data = std::make_shared<IExecutorData>();
    auto &props = executor_data->getContext().props();
    props["ignis.transport.compression"] = "6";
    props["ignis.partition.compression"] = "6";
    props["ignis.partition.serialization"] = "native";
    props["ignis.executor.directory"] = ghc::filesystem::current_path().string();
    props["ignis.partition.type"] = Ps::TYPE;
}

template<typename Ps>
void IMpiTestClass<Ps>::tearDown() {}

template<typename Ps>
void IMpiTestClass<Ps>::gatherTest(int root) {
    int n = 100;
    int rank = executor_data->getContext().mpiGroup().Get_rank();
    int size = executor_data->getContext().mpiGroup().Get_size();
    api::IVector<Tp> elems = IElements<Tp>().create(n * size, 0);
    api::IVector<Tp> local_elems(elems.begin() + n * rank, elems.begin() + n * (rank + 1));
    auto part = create();
    insert(local_elems, *part);
    executor_data->mpi().gather(*part, root);
    if (rank == root) {
        api::IVector<Tp> result;
        get(result, *part);
        CPPUNIT_ASSERT(elems == result);
    }
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::bcastTest() {
    int n = 100;
    int rank = executor_data->getContext().mpiGroup().Get_rank();
    api::IVector<Tp> elems = IElements<Tp>().create(n, 0);
    auto part = create();
    if (rank == 0) {
        insert(elems, *part);
    } else {
        /*Ensures that the partition will be cleaned*/
        part->writeIterator()->write(elems.back());
    }
    executor_data->mpi().bcast(*part, 0);
    api::IVector<Tp> result;
    get(result, *part);
    CPPUNIT_ASSERT(elems == result);
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::sendRcvTest() {
    int n = 100;
    int rank = executor_data->mpi().rank();
    api::IVector<Tp> elems = IElements<Tp>().create(n, 0);
    auto part = create();
    if (rank % 2 == 0) {
        if (rank + 1 < executor_data->mpi().executors()) {
            executor_data->mpi().recv(*part, rank + 1, 0);
            api::IVector<Tp> result;
            get(result, *part);

            CPPUNIT_ASSERT(elems == result);
        }
    } else {
        insert(elems, *part);
        executor_data->mpi().send(*part, rank - 1, 0);
    }
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::sendRcvGroupTest(const std::string &partitionType) {
    int n = 100;
    int rank = executor_data->mpi().rank();
    api::IVector<Tp> elems = IElements<Tp>().create(n, 0);
    if (rank % 2 == 0) {
        if (rank + 1 < executor_data->mpi().executors()) {
            auto part = create(partitionType);
            executor_data->mpi().recv(executor_data->mpi().native(), *part, rank + 1, 0);
            api::IVector<Tp> result;
            get(result, *part);

            CPPUNIT_ASSERT(elems == result);
        }
    } else {
        auto part = create();
        insert(elems, *part);
        executor_data->mpi().send(executor_data->mpi().native(), *part, rank - 1, 0);
    }
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::sendRcvGroupToVoidTest() {
    int n = 100;
    int rank = executor_data->mpi().rank();
    api::IVector<Tp> elems = IElements<Tp>().create(n, 0);
    if (rank % 2 == 0) {
        if (rank + 1 < executor_data->mpi().executors()) {
            auto part = std::make_shared<storage::IVoidPartition>();
            executor_data->mpi().recvVoid(executor_data->mpi().native(), *part, rank + 1, 0);
            auto men_part = std::make_shared<storage::IMemoryPartition<Tp>>();
            auto trans = part->readTransport();
            men_part->read(trans);
            api::IVector<Tp> result;
            get(result, *men_part);

            CPPUNIT_ASSERT(elems == result);
        }
    } else {
        auto part = create();
        insert(elems, *part);
        executor_data->mpi().send(executor_data->mpi().native(), *part, rank - 1, 0);
    }
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::driverGatherTest() {
    int n = 100;
    int driver = 0;
    int rank = executor_data->getContext().mpiGroup().Get_rank();
    int size = executor_data->getContext().mpiGroup().Get_size();
    api::IVector<Tp> elems = IElements<Tp>().create(n * (size - 1), 0);
    storage::IPartitionGroup<Tp> part_group;
    if (rank != driver) {
        auto part = create();
        api::IVector<Tp> local_elems(elems.begin() + n * (rank - 1), elems.begin() + n * rank);
        insert(local_elems, *part);
        part_group.add(part);
    }

    executor_data->mpi().driverGather(executor_data->mpi().native(), part_group);

    if (rank == driver) {
        api::IVector<Tp> result;
        get(result, *part_group[0]);
        CPPUNIT_ASSERT(elems == result);
    }
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::driverScatterTest() {
    int n = 100;
    int driver = 0;
    int rank = executor_data->getContext().mpiGroup().Get_rank();
    int size = executor_data->getContext().mpiGroup().Get_size();
    api::IVector<Tp> elems = IElements<Tp>().create(n * (size - 1), 0);
    storage::IPartitionGroup<Tp> part_group;
    if (rank == driver) {
        auto part = std::make_shared<storage::IMemoryPartition<Tp>>();//Scatter is always from user memory array
        insert(elems, *part);
        part_group.add(part);
    }

    executor_data->mpi().driverScatter(executor_data->mpi().native(), part_group, (size - 1) * 2);

    if (rank != driver) {
        api::IVector<Tp> local_elems(elems.begin() + n * (rank - 1), elems.begin() + n * rank);
        api::IVector<Tp> result;
        for (auto &part : part_group) { get(result, *part); }
        CPPUNIT_ASSERT(local_elems == result);
    }
    executor_data->mpi().barrier();
}

template<typename Ps>
void IMpiTestClass<Ps>::driverScatterVoidTest() {
    int n = 100;
    int driver = 0;
    int rank = executor_data->getContext().mpiGroup().Get_rank();
    int size = executor_data->getContext().mpiGroup().Get_size();
    api::IVector<Tp> elems = IElements<Tp>().create(n * (size - 1), 0);

    if (rank == driver) {
        storage::IPartitionGroup<Tp> part_group;
        auto part = std::make_shared<storage::IMemoryPartition<Tp>>();//Scatter is always from user memory array
        insert(elems, *part);
        part_group.add(part);

        executor_data->mpi().driverScatter(executor_data->mpi().native(), part_group, (size - 1) * 2);
    } else {
        storage::IPartitionGroup<storage::IVoidPartition::VOID_TYPE> part_group;

        executor_data->mpi().driverScatterVoid(executor_data->mpi().native(), part_group, (size - 1) * 2);

        api::IVector<Tp> local_elems(elems.begin() + n * (rank - 1), elems.begin() + n * rank);
        api::IVector<Tp> result;
        for (auto &part : part_group) {
            auto &void_partition = reinterpret_cast<storage::IVoidPartition &>(*part);
            auto men_part = std::make_shared<storage::IMemoryPartition<Tp>>();
            auto trans = void_partition.readTransport();
            men_part->read(trans);
            get(result, *men_part);
        }

        CPPUNIT_ASSERT(local_elems == result);
    }

    executor_data->mpi().barrier();
}

#undef IMpiTestClass
