
#include "ICommModuleTest.h"

#define ICommModuleTestClass ignis::executor::core::modules::ICommModuleTest

ICommModuleTestClass::ICommModuleTest() : comm(std::make_shared<ICommModule>(executor_data)) {}

void ICommModuleTestClass::setUp() {}

void ICommModuleTestClass::tearDown() {}


template<typename Tp>
void ICommModuleTestClass::getSetPartitionsImpl(const std::string &partitionType, int64_t parts, int64_t getParts, bool setInt) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    executor_data->setCores(1);
    auto elems = IElements<Tp>().create(100 * std::max(parts, (int64_t) 1), 0);
    registerType<Tp>();
    loadToPartitions(elems, std::max(parts, (int64_t) 1));
    if (parts == 0) {
        elems.clear();
        executor_data->getPartitions<Tp>()->clear();
    }

    std::vector<std::string> binary;
    comm->getPartitions2(binary, comm->getProtocol(), getParts);
    if(setInt){
        comm->setPartitions2(binary, newSource("TypeInt"));
    }else{
        comm->setPartitions(binary);
    }

    auto result = getFromPartitions<Tp>();

    CPPUNIT_ASSERT(elems == result);
}

template<typename Tp>
void ICommModuleTestClass::importDataTestImpl(int cores, const std::string &partitionType, int64_t parts, bool withVoid) {
    executor_data->getContext().props()["ignis.partition.type"] = partitionType;
    auto bak = executor_data->mpi().native();
    auto executors = executor_data->mpi().executors();
    auto rank = executor_data->mpi().rank();
    int sources = std::ceil(0.25 * executors);
    int color = rank < sources ? 1 : 0;
    auto group = bak.Split(color, rank);
    executor_data->setMpiGroup(group);
    auto elems = IElements<Tp>().create(200 * parts  * (executors - sources), 0);
    registerType<Tp>();

    std::string address;
    if (color == 0){
        auto local_elems = rankVector(elems);
        loadToPartitions<Tp>(local_elems, parts);
        std::cerr << "##" << executor_data->mpi().executors() << std::endl;
        if(executor_data->mpi().isRoot(0)) {
            comm->openGroup(address);
            std::ofstream("../address.txt") << address;
        }
        bak.Barrier();

    }else{
        bak.Barrier();
        std::ifstream("../address.txt") >> address;
    }

    comm->joinToGroupName(address, color == 0, "group");
    comm->closeGroup();

    if(withVoid){
        comm->importData("group", color == 0, cores);
    }else{
        comm->importData4("group", color == 0, cores, newSource("TypeInt"));
    }

    if (color == 1){
        auto result = getFromPartitions<Tp>();
        loadToPartitions(result, 1);
        executor_data->mpi().gather(*((*executor_data->getPartitions<Tp>())[0]), 0);
        result = getFromPartitions<Tp>();
        if(executor_data->mpi().isRoot(0)){
            CPPUNIT_ASSERT(elems == result);
        }
    }

    comm->destroyGroup("group");
    group.Free();
    executor_data->setMpiGroup(bak);
    executor_data->mpi().barrier();
}

#undef ICommModuleTest
