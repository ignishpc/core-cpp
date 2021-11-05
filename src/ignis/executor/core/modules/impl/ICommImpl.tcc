
#include "ICommImpl.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"

#define ICommImplClass ignis::executor::core::modules::impl::ICommImpl


template<typename Tp>
std::vector<std::string> ICommImplClass::getPartitions(const int8_t protocol, int64_t minPartitions) {
    IGNIS_TRY()
    auto group = executor_data->getPartitions<Tp>();
    auto cmp = executor_data->getProperties().msgCompression();
    std::vector<std::string> partitions;
    bool contiguous = executor_data->mpi().isContiguousType<Tp>();
    auto buffer = std::make_shared<transport::IMemoryBuffer>();
    if (group->partitions() > minPartitions) {
        if (executor_data->getPartitionTools().isMemory(*group) && protocol == getProtocol()) {
            for (auto &part : (*group)) {
                auto &men = executor_data->getPartitionTools().toMemory(*part);
                men.write((std::shared_ptr<transport::ITransport> &) buffer, cmp, contiguous);
                partitions.push_back(buffer->getBufferAsString());
                buffer->resetBuffer();
            }
        } else {
            for (auto &part : (*group)) {
                part->write((std::shared_ptr<transport::ITransport> &) buffer, cmp);
                partitions.push_back(buffer->getBufferAsString());
                buffer->resetBuffer();
            }
        }
    } else if (executor_data->getPartitionTools().isMemory(*group) && group->partitions() == 1) {
        auto &men = reinterpret_cast<storage::IMemoryPartition<Tp> &>(*(*group)[0]);
        auto zlib = std::make_shared<transport::IZlibTransport>(buffer, cmp);
        protocol::IObjectProtocol proto(zlib);
        int64_t partition_elems = men.size() / minPartitions;
        int64_t remainder = men.size() % minPartitions;
        int64_t offset = 0;
        bool native = contiguous && protocol == getProtocol();
        for (int64_t p = 0; p < minPartitions; p++) {
            int64_t sz = partition_elems + (p < remainder ? 1 : 0);
            proto.writeObject(api::IVector<Tp>::view(&men[offset], sz), native);
            offset += sz;
            zlib->flush();
            zlib->reset();
            partitions.push_back(buffer->getBufferAsString());
            buffer->resetBuffer();
        }

    } else if (group->partitions() > 0) {
        int64_t elements = 0;
        for (auto &part : (*group)) { elements += part->size(); }
        storage::IMemoryPartition<Tp> part(1024 * 1024);
        auto writer = part.writeIterator();
        int64_t partition_elems = elements / minPartitions;
        int64_t remainder = elements % minPartitions;
        int64_t i = 0;
        int64_t ew = 0, er = 0;
        auto it = (*group)[0]->readIterator();
        for (int64_t p = 0; p < minPartitions; p++) {
            part.clear();
            writer = part.writeIterator();
            ew = partition_elems;
            if (p < remainder) { ew++; }
            while (ew > 0) {
                if (er == 0) {
                    er = (*group)[i]->size();
                    it = (*group)[i++]->readIterator();
                }
                int64_t n = std::min(ew, er);
                for (int64_t j = 0; j < n; j++) { writer->write(std::move(it->next())); }
                ew -= n;
                er -= n;
            }
            part.write((std::shared_ptr<transport::ITransport> &) buffer, cmp);
            partitions.push_back(buffer->getBufferAsString());
            buffer->resetBuffer();
        }
    } else {
        storage::IMemoryPartition<Tp> part;
        part.write((std::shared_ptr<transport::ITransport> &) buffer, cmp, false);
        for (int64_t i = 0; i < minPartitions; i++) { partitions.push_back(buffer->getBufferAsString()); }
    }
    return partitions;
    IGNIS_CATCH()
}


template<typename Tp>
void ICommImplClass::setPartitions(const std::vector<std::string> &partitions) {
    IGNIS_TRY()
    auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(partitions.size());
    for (int64_t i = 0; i < partitions.size(); i++) {
        auto &bytes = partitions[i];
        auto buffer = std::make_shared<transport::IMemoryBuffer>((uint8_t *) bytes.c_str(), bytes.size());
        (*group)[i]->read((std::shared_ptr<transport::ITransport> &) buffer);
    }
    executor_data->setPartitions<Tp>(group);
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverGather(const std::string &group) {
    IGNIS_TRY()
    auto comm = getGroup(group);
    if (comm.Get_rank() == 0) {
        executor_data->setPartitions<Tp>(executor_data->getPartitionTools().newPartitionGroup<Tp>());
    }
    executor_data->mpi().driverGather(comm, *executor_data->getPartitions<Tp>());
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverGather0(const std::string &group) {
    IGNIS_TRY()
    auto comm = getGroup(group);
    if (comm.Get_rank() == 0) {
        executor_data->setPartitions<Tp>(executor_data->getPartitionTools().newPartitionGroup<Tp>());
    }
    executor_data->mpi().driverGather0(comm, *executor_data->getPartitions<Tp>());
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverScatter(const std::string &group, int64_t partitions) {
    IGNIS_TRY()
    auto comm = getGroup(group);
    if (comm.Get_rank() != 0) {
        executor_data->setPartitions<Tp>(executor_data->getPartitionTools().newPartitionGroup<Tp>());
    }
    executor_data->mpi().driverScatter(comm, *executor_data->getPartitions<Tp>(), partitions);
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::importData(const std::string &group, bool source, int64_t threads) {
    auto import_comm = getGroup(group);
    auto executors = import_comm.Get_size();
    int64_t me = import_comm.Get_rank();
    std::vector<std::pair<int64_t, int64_t>> ranges;
    std::vector<int64_t> queue;
    importDataAux(import_comm, source, ranges, queue);
    int64_t offset = ranges[me].first;
    if (source) {
        IGNIS_LOG(info) << "General: importData sending partitions";
    } else {
        IGNIS_LOG(info) << "General: importData receiving partitions";
    }
    auto parts =
            source ? executor_data->getAndDeletePartitions<Tp>()
                   : executor_data->getPartitionTools().newPartitionGroup<Tp>(ranges[me].second - ranges[me].first);

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
                if (source) {
                    executor_data->mpi().send(comm, *shared[init - offset + j], other, 0, opt);
                    shared[init - offset + j].reset();
                } else {
                    executor_data->mpi().recv(comm, *shared[init - offset + j], other, 0, opt);
                }
            }
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    for (int64_t i = 1; i < threads_comm.size(); i++) { threads_comm[i].Free(); }

    if(source){
        executor_data->deletePartitions();
    }else{
        executor_data->setPartitions(parts);
    }
}

#undef ICommImplClass