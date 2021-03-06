
#include "ICommImpl.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"

using namespace ignis::executor::core::modules::impl;

ICommImpl::ICommImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {
    const_cast<MPI::Intracomm &>(executor_data->mpi().native()).Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
}

ICommImpl::~ICommImpl() {}

std::string ICommImpl::openGroup() {
    IGNIS_TRY()
    IGNIS_LOG(info) << "Comm: creating group server";
    char port_name[MPI_MAX_PORT_NAME];
    MPI::Open_port(MPI::INFO_NULL, port_name);
    auto handle = std::shared_ptr<std::string>(new std::string(port_name), [](const std::string *group_name) {
        MPI::Close_port(group_name->c_str());//Close port on context clear
        delete group_name;
    });
    IGNIS_LOG(info) << "Comm: group created on " << port_name;
    executor_data->setVariable<std::shared_ptr<std::string>>("server", handle);
    return port_name;
    IGNIS_CATCH()
}

void ICommImpl::closeGroup() {
    IGNIS_TRY()
    IGNIS_LOG(info) << "Comm: closing group server";
    executor_data->removeVariable("server");
    IGNIS_CATCH()
}

void ICommImpl::joinToGroup(const std::string &id, bool leader) {
    IGNIS_TRY()
    executor_data->setMpiGroup(joinToGroupImpl(id, leader));
    IGNIS_CATCH()
}

void ICommImpl::joinToGroupName(const std::string &id, bool leader, const std::string &name) {
    IGNIS_TRY()
    groups[name] = joinToGroupImpl(id, leader);
    IGNIS_CATCH()
}

bool ICommImpl::hasGroup(const std::string &name) {
    IGNIS_TRY()
    return groups.find(name) != groups.end();
    IGNIS_CATCH()
}

void ICommImpl::destroyGroup(const std::string &name) {
    IGNIS_TRY()
    if (name.empty()) {
        executor_data->destroyMpiGroup();
        return;
    }

    auto entry = groups.find(name);
    if (entry != groups.end()) {
        entry->second.Free();
        groups.erase(entry);
    }
    IGNIS_CATCH()
}

void ICommImpl::destroyGroups() {
    IGNIS_TRY()
    for (auto &elem : groups) { elem.second.Free(); }
    groups.clear();
    executor_data->destroyMpiGroup();
    IGNIS_CATCH()
}

int8_t ICommImpl::getProtocol() { return protocol::IObjectProtocol::CPP_PROTOCOL; }

void ICommImpl::setPartitionsVoid(const std::vector<std::string> &partitions) {
    IGNIS_TRY()
    auto group = executor_data->getPartitionTools().newPartitionGroup<storage::IVoidPartition::VOID_TYPE>();
    for (int64_t i = 0; i < partitions.size(); i++) {
        auto &bytes = partitions[i];
        auto buffer = std::make_shared<transport::IMemoryBuffer>((uint8_t *) bytes.c_str(), bytes.size());
        auto part = executor_data->getPartitionTools().newVoidPartition(bytes.size());
        part->read((std::shared_ptr<transport::ITransport> &) buffer);
        group->add(part);
    }
    IGNIS_CATCH()
}

void ICommImpl::driverScatterVoid(const std::string &group, int64_t partitions) {
    IGNIS_TRY()
    auto comm = getGroup(group);
    auto part_group = executor_data->getPartitionTools().newPartitionGroup<storage::IVoidPartition::VOID_TYPE>();
    executor_data->mpi().driverScatterVoid(comm, *part_group, partitions);
    executor_data->setPartitions<storage::IVoidPartition::VOID_TYPE>(part_group);
    IGNIS_CATCH()
}

MPI::Intracomm ICommImpl::joinToGroupImpl(const std::string &id, bool leader) {
    bool root = executor_data->hasVariable("server");
    MPI::Intracomm comm = executor_data->mpi().native();
    MPI::Intracomm comm1;
    MPI::Intercomm intercomm;
    if (leader) {
        const char *port = root ? id.c_str() : nullptr;
        intercomm = comm.Accept(port, MPI::INFO_NULL, 0);
    } else {
        const char *port = id.c_str();
        intercomm = comm.Connect(port, MPI::INFO_NULL, 0);
    }
    comm1 = intercomm.Merge(leader ? 0 : 1);
    intercomm.Free();
    comm1.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
    return comm1;
}

MPI::Intracomm &ICommImpl::getGroup(const std::string &id) {
    auto it = groups.find(id);
    if (it == groups.end()) { throw ignis::executor::core::exception::ILogicError("Group " + id + " not found"); }
    return it->second;
}

void ICommImpl::importDataAux(const MPI::Intracomm &group, bool source,
                              std::vector<std::pair<int64_t, int64_t>> &ranges, std::vector<int64_t> &queue) {
    int64_t rank = group.Get_rank();
    int64_t local_rank = executor_data->mpi().rank();
    int64_t executors = group.Get_size();
    int64_t local_executors = executor_data->getContext().executors();
    int64_t remote_executors = executors - local_executors;
    int64_t local_root = rank - local_rank;
    int64_t remote_root = local_root == 0 ? local_executors : 0;
    int64_t source_root = source ? local_root : remote_root;
    int64_t target_executors = source ? remote_executors : local_executors;

    int64_t numPartitions = 0;
    if (source) {
        auto input = executor_data->getPartitions<char>(true);
        numPartitions = input->partitions();
    }
    std::vector<int64_t> executors_count(executors);
    group.Allgather(&numPartitions, 1, MPI::LONG_LONG, &executors_count[0], 1, MPI::LONG_LONG);
    numPartitions = 0;
    for (int64_t i = 0; i < executors; i++) { numPartitions += executors_count[i]; }
    IGNIS_LOG(info) << "General: importData " << numPartitions << "partitions";
    std::vector<std::pair<int64_t, int64_t>> source_ranges;
    int64_t offset = 0;
    int64_t init,end = source_root + executors - target_executors;

    for(int64_t i = source_root; i < end ;i++){
        source_ranges.emplace_back(offset, offset + executors_count[i]);
        offset += executors_count[i];
    }

    int64_t block = numPartitions / target_executors;
    int64_t remainder = numPartitions % target_executors;
    std::vector<std::pair<int64_t, int64_t>> target_ranges;
    for(int64_t i = 0; i < target_executors ;i++){
        if( i< remainder){
            init = (block + 1) * i;
            end = init + block + 1;
        }else{
            init = (block + 1) * remainder + block * (i-remainder);
            end = init + block;
        }
        target_ranges.emplace_back(init, end);
    }

    if(source_root == 0){
        ranges.insert(ranges.end(), source_ranges.begin(), source_ranges.end());
        ranges.insert(ranges.end(), target_ranges.begin(), target_ranges.end());
    }else{
        ranges.insert(ranges.end(), target_ranges.begin(), target_ranges.end());
        ranges.insert(ranges.end(), source_ranges.begin(), source_ranges.end());
    }

    std::vector<int64_t> global_queue;
    int64_t m = executors % 2 == 0 ? executors : executors + 1;
    int64_t id = 0;
    int64_t id2 = m * m - 2;
    for (int64_t i = 0; i < m - 1; i++) {
        if (rank == id % (m - 1)) { global_queue.push_back(m - 1); }
        if (rank == m - 1) { global_queue.push_back(id % (m - 1)); }
        id += 1;
        for (int64_t j = 1; j < m / 2; j++) {
            if (rank == id % (m - 1)) { global_queue.push_back(id2 % (m - 1)); }
            if (rank == id2 % (m - 1)) { global_queue.push_back(id % (m - 1)); }
            id += 1;
            id2 -= 1;
        }
    }

    for (auto other : global_queue) {
        if (local_root > 0) {
            if (other < local_root) { queue.push_back(other); }
        } else {
            if (other >= remote_root) { queue.push_back(other); }
        }
    }
}

void ICommImpl::importDataVoid(const std::string &group, bool source, int64_t threads) {
    auto import_comm = getGroup(group);
    auto executors = import_comm.Get_size();
    int64_t me = import_comm.Get_rank();
    std::vector<std::pair<int64_t, int64_t>> ranges;
    std::vector<int64_t> queue;
    importDataAux(import_comm, source, ranges, queue);
    int64_t offset = ranges[me].first;
    IGNIS_LOG(info) << "General: importData receiving partitions";

    auto parts = std::make_shared<storage::IPartitionGroup<storage::IVoidPartition::VOID_TYPE>>();
    for (int64_t i = 0; i < ranges[me].second - ranges[me].first; i++) {
        parts->add(executor_data->getPartitionTools().newVoidPartition());
    }

    auto shared = *parts;
    auto threads_comm = executor_data->duplicate(import_comm, threads);
    auto type = shared[0]->type();
    IMpi::MsgOpt opt;
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(threads)
    {
        IGNIS_OMP_TRY()
        auto comm = threads_comm[executor_data->getContext().threadId()];
        for (int64_t i = 0; i < queue.size(); i++) {
            int64_t other = queue[i];
            if (other == executors) { continue; }
            int64_t init = std::max(ranges[me].first, ranges[other].first);
            int64_t end = std::min(ranges[me].second, ranges[other].second);
            if (end - init < 1) { continue; }
#pragma omp master
            { opt = executor_data->mpi().getMsgOpt(comm, type, source, other, 0); };
#pragma omp barrier
#pragma omp for schedule(static, 1)
            for (int64_t j = 0; j < end - init; j++) {
                executor_data->mpi().recvVoid(comm, (storage::IVoidPartition &) *shared[init - offset + j], other, 0,
                                              opt);
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    for (int64_t i = 1; i < threads_comm.size(); i++) { threads_comm[i].Free(); }

    executor_data->setPartitions(parts);
}