
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
                for (auto &part: (*group)) {
                    auto &men = executor_data->getPartitionTools().toMemory(*part);
                    men.write((std::shared_ptr<transport::ITransport> &) buffer, cmp, contiguous);
                    partitions.push_back(buffer->getBufferAsString());
                    buffer->resetBuffer();
                }
            } else {
                for (auto &part: (*group)) {
                    part->write((std::shared_ptr<transport::ITransport> &) buffer, cmp);
                    partitions.push_back(buffer->getBufferAsString());
                    buffer->resetBuffer();
                }
            }
        } else if (executor_data->getPartitionTools().isMemory(*group) &&
                   protocol == getProtocol() &&
                   group->partitions() == 1) {
            auto &men = reinterpret_cast<storage::IMemoryPartition <Tp> &>(*(*group)[0]);
            auto zlib = std::make_shared<transport::IZlibTransport>(buffer, cmp);
            protocol::IObjectProtocol proto(zlib);
            int64_t partition_elems = men.size() / minPartitions;
            int64_t remainder = men.size() % minPartitions;
            int64_t offset = 0;
            for (int64_t p = 0; p < minPartitions; p++) {
                int64_t sz = partition_elems + (p < remainder ? 1 : 0);
                proto.writeObject(api::IVector<Tp>::view(&men[offset], sz), contiguous);
                offset += sz;
                zlib->flush();
                zlib->reset();
                buffer->resetBuffer();
            }

        } else if (group->partitions() > 0) {
            int64_t elements = 0;
            for (auto &part: (*group)) {
                elements += part->size();
            }
            storage::IMemoryPartition <Tp> part;
            (1024 * 1024, cmp);
            auto writer = part.writeIterator();
            int64_t partition_elems = elements / minPartitions;
            int64_t remainder = elements % minPartitions;
            int64_t i = 0;
            int64_t ew = 0, er = 0;
            auto it = (*group)[0]->readIterator();
            for (int64_t p = 0; p < minPartitions; p++) {
                part.clear();
                ew = partition_elems;
                if (p < remainder) {
                    ew++;
                }

                for (; ew > 0 && i < group->partitions();) {
                    if (er == 0) {
                        er = (*group)[i]->size();
                        it = (*group)[i++]->readIterator();
                    }
                    for (; ew > 0 && er > 0; ew--, er--) {
                        writer->write(std::move(it->next()));
                    }
                }
                part.write((std::shared_ptr<transport::ITransport> &) buffer, cmp);
                partitions.push_back(buffer->getBufferAsString());
                buffer->resetBuffer();
            }
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
void ICommImplClass::driverGather(const std::string &id) {
    IGNIS_TRY()
        auto group = getGroup(id);
        if (group.Get_rank() == 0) {
            executor_data->setPartitions<Tp>(executor_data->getPartitionTools().newPartitionGroup<Tp>());
        }
        executor_data->mpi().driverGather(group, *executor_data->getPartitions<Tp>());
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverGather0(const std::string &id) {
    IGNIS_TRY()
        auto group = getGroup(id);
        if (group.Get_rank() == 0) {
            executor_data->setPartitions<Tp>(executor_data->getPartitionTools().newPartitionGroup<Tp>());
        }
        executor_data->mpi().driverGather0(group, *executor_data->getPartitions<Tp>());
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverScatter(const std::string &id, int64_t partitions) {
    IGNIS_TRY()
        auto group = getGroup(id);
        if (group.Get_rank() != 0) {
            executor_data->setPartitions<Tp>(executor_data->getPartitionTools().newPartitionGroup<Tp>());
        }
        executor_data->mpi().driverScatter(group, *executor_data->getPartitions<Tp>(), partitions);
    IGNIS_CATCH()
}