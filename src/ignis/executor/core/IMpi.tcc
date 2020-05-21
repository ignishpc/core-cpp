
#include "IMpi.h"
#include "ILog.h"
#include "storage/IMemoryPartition.h"
#include "storage/IRawMemoryPartition.h"
#include "storage/IDiskPartition.h"
#include "io/INativeWriter.h"

#include <iostream>

#define IMpiClass ignis::executor::core::IMpi


template<typename Tp>
void IMpiClass::gather(storage::IPartition <Tp> &part, int root) {
    if (executors() == 1) { return; }
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isContiguousType<Tp>()) {
            auto &men = partition_tools.toMemory(part);
            int sz = men.size() * sizeof(Tp);
            std::vector<int> szv;
            std::vector<int> displs;
            if (isRoot(root)) { szv.resize(executors(), sz); }
            comm.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, root);
            if (isRoot(root)) {
                displs = this->displs(szv);
                men.resize(displs.back() / sizeof(Tp));
                //Use same buffer to rcv elements
                move(&men[0], szv[rank()], displs[rank()]);
                comm.Gatherv(MPI::IN_PLACE, 0, MPI::BYTE, &men[0], &szv[0], &displs[0], MPI::BYTE, root);
            } else {
                comm.Gatherv(&men[0], sz, MPI::BYTE, nullptr, nullptr, nullptr, MPI::BYTE, root);
            }
        } else {
            auto &men = partition_tools.toMemory(part);
            auto buffer = std::make_shared<transport::IMemoryBuffer>();
            int sz = 0;
            std::vector<int> szv;
            std::vector<int> displs;
            if (!isRoot(root)) {
                part.write((std::shared_ptr<transport::ITransport> &) buffer, properties.msgCompression());
                sz = buffer->writeEnd();
                buffer->resetBuffer();
            }
            if (isRoot(root)) { szv.resize(executors(), sz); }
            comm.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, root);
            if (isRoot(root)) {
                displs = this->displs(szv);
                buffer->getWritePtr(displs.back());
            }
            comm.Gatherv(buffer->getWritePtr(sz), sz, MPI::BYTE,
                         buffer->getWritePtr(sz), &szv[0], &displs[0], MPI::BYTE, root);
            if (isRoot(root)) {
                auto ptr = buffer->getWritePtr(sz);
                storage::IMemoryPartition <Tp> rcv;
                for (int i = 0; i < executors(); i++) {
                    if (i != root) {
                        auto view = std::make_shared<transport::IMemoryBuffer>(ptr + displs[i], szv[i]);
                        rcv.read((std::shared_ptr<transport::ITransport> &) view);
                    } else {
                        //Avoid serialization own elements
                        part.moveTo(rcv);
                    }
                }
                std::swap(men, rcv);
            }
        }
    } else if (part.type() == storage::IRawMemoryPartition<Tp>::TYPE) {
        auto &raw = partition_tools.toRawMemory(part);
        raw.sync();
        std::pair<int, int> sz(raw.size(), raw.end() - raw.begin(false));
        std::vector<std::pair<int, int>> elems_szv;
        std::vector<int> szv;
        std::vector<int> displs;
        if (isRoot(root)) { elems_szv.resize(executors(), sz); }
        comm.Gather(&sz, 2, MPI::INT, &elems_szv[0], 2, MPI::INT, root);
        if (isRoot(root)) {
            szv = this->szVector(elems_szv);
            displs = this->displs(szv);
            auto n = sumElems(elems_szv);
            raw.resize(n, displs.back());
            auto ptr = raw.begin(false);
            move(ptr, szv[rank()], displs[rank()]);
            comm.Gatherv(MPI::IN_PLACE, 0, MPI::BYTE, ptr, &szv[0], &displs[0], MPI::BYTE, root);
        } else {
            comm.Gatherv(raw.begin(false), sz.second, MPI::BYTE, nullptr, nullptr, nullptr, MPI::BYTE, root);
        }
    } else {
        auto &disk = partition_tools.toDisk(part);
        disk.sync();
        std::string path = disk.getPath();
        std::string full_path;
        int sz = path.size();
        std::vector<int> szv;
        std::vector<int> displs;
        if (isRoot(root)) { szv.resize(executors(), sz); }
        comm.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, root);
        if (isRoot(root)) {
            displs = this->displs(szv);
            full_path.resize(displs.back());
        }
        comm.Gatherv(const_cast<char *>(path.c_str()), sz, MPI::BYTE, const_cast<char *>(full_path.c_str()), &szv[0],
                     &displs[0], MPI::BYTE, root);
        if (isRoot(root)) {
            auto root_path = disk.getPath();
            disk.rename(root_path + ".tmp");
            storage::IDiskPartition <Tp> rcv(root_path, properties.partitionCompression(), false, false);
            for (int i = 0; i < executors(); i++) {
                if (i != root) {
                    std::string src(full_path, displs[i], szv[i]);
                    storage::IDiskPartition <Tp> aux(src, properties.partitionCompression(), false, true);
                    aux.copyTo(rcv);
                } else {
                    part.moveTo(rcv);
                }
            }
            rcv.destroy = disk.destroy;
            disk.destroy = true;
            std::swap(disk, rcv);
        }
    }
}

template<typename Tp>
void IMpiClass::bcast(storage::IPartition <Tp> &part, int root) {
    if (executors() == 1) { return; }
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isContiguousType<Tp>()) {
            auto &men = partition_tools.toMemory(part);
            int sz = men.size();
            comm.Bcast(&sz, 1, MPI::INT, 0);
            if (!isRoot(root)) {
                men.resize(sz);
            }
            comm.Bcast(&men[0], sz * sizeof(Tp), MPI::BYTE, root);
        } else {
            auto buffer = std::make_shared<transport::IMemoryBuffer>(part.bytes());
            int sz;
            if (isRoot(root)) {
                part.write((std::shared_ptr<transport::ITransport> &) buffer, properties.msgCompression());
                sz = buffer->writeEnd();
                buffer->resetBuffer();
            }
            comm.Bcast(&sz, 1, MPI::INT, root);
            comm.Bcast(buffer->getWritePtr(sz), sz, MPI::BYTE, root);
            if (!isRoot(root)) {
                buffer->wroteBytes(sz);
                part.clear();
                part.read((std::shared_ptr<transport::ITransport> &) buffer);
            }
        }
    } else if (part.type() == storage::IRawMemoryPartition<Tp>::TYPE) {
        auto &raw = partition_tools.toRawMemory(part);
        raw.sync();
        std::pair<int, int> sz(raw.size(), (int) (raw.end() - raw.begin(false)));
        comm.Bcast(&sz, 2, MPI::INT, root);
        if (!isRoot(root)) {
            raw.resize(sz.first, sz.second);
        }
        comm.Bcast(raw.begin(false), sz.second, MPI::BYTE, root);
    } else {
        auto &disk = partition_tools.toDisk(part);
        disk.sync();
        std::string path = disk.getPath();
        int sz = path.size();
        comm.Bcast(&sz, 1, MPI::INT, root);
        path.resize(sz);
        comm.Bcast(const_cast<char *>(path.c_str()), sz, MPI::BYTE, root);
        if (!isRoot(root)) {
            disk.destroy = true;
            auto rcv = partition_tools.copyDiskPartition<Tp>(path);
            std::swap(disk, *rcv);
        }
    }
}

template<typename Tp>
void IMpiClass::driverGather(const MPI::Intracomm &group, storage::IPartitionGroup <Tp> &part_group) {
    bool driver = group.Get_rank() == 0;
    bool exec0 = group.Get_rank() == 1;
    int max_partition;
    std::vector<int> partition_size;
    int8_t protocol;
    bool same_protocol;
    std::string storage;

    if (driver) {
        protocol = core::protocol::IObjectProtocol::CPP_PROTOCOL;
    } else {
        max_partition = (int) part_group.partitions();
        for (int64_t i = 0; i < part_group.partitions(); i++) {
            if (i == 0) {
                storage = part_group[i]->type();
            }
            partition_size[i] = (int) part_group[i]->size();
        }
    }

    group.Allreduce(MPI_IN_PLACE, &max_partition, 1, MPI::INT, MPI::MAX);
    if (max_partition == 0) {
        return;
    }
    group.Gather(MPI_IN_PLACE, max_partition, MPI::INT, &partition_size[0], max_partition, MPI::INT, 0);
    group.Bcast(&protocol, 1, MPI::BYTE, 0);

    if (driver) {
        int length;
        group.Recv(&same_protocol, 1, MPI::BOOL, 1, 0);
        group.Recv(&length, 1, MPI::INT, 1, 0);
        storage.resize(length);
        group.Recv(const_cast<char *>(storage.c_str()), length, MPI::BYTE, 1, 0);
    } else if (exec0) {
        same_protocol = protocol == core::protocol::IObjectProtocol::CPP_PROTOCOL;
        int length = storage.length();
        group.Send(&same_protocol, 1, MPI::BOOL, 0, 0);
        group.Send(&length, 1, MPI::INT, 0, 0);
        group.Send(storage.c_str(), length, MPI::BYTE, 0, 0);
    }

    IGNIS_LOG(info) << "Comm: driverGather storage: " << storage << ", operations: " << max_partition;

    std::shared_ptr<storage::IPartition < Tp>>
    part_sp;
    for (int64_t i = 0; i < max_partition; i++) {
        if (i < part_group.partitions()) {
            part_sp = part_group[i];
        }
        if (storage == storage::IMemoryPartition<Tp>::TYPE) {
            if (isContiguousType<Tp>() && same_protocol) {
                if (driver) {
                    auto part = std::make_shared<storage::IMemoryPartition < Tp>>
                    ();
                    auto &men = reinterpret_cast<storage::IMemoryPartition <Tp> &>(part);
                    int sz = 0;
                    std::vector<int> szv;
                    std::vector<int> displs;
                    szv.resize(group.Get_size(), sz);
                    group.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, 0);
                    displs = this->displs(szv);
                    men.resize(displs.back() / sizeof(Tp));
                    group.Gatherv(MPI::IN_PLACE, 0, MPI::BYTE, &men[0], &szv[0], &displs[0], MPI::BYTE, 0);
                    if (group.Get_size() == 2) {
                        part_group.add(part);
                        continue;
                    }
                    int64_t offset = 0;
                    for (int sz : szv) {
                        auto part = std::make_shared<storage::IMemoryPartition < Tp>>
                        ();
                        auto &rcv = reinterpret_cast<storage::IMemoryPartition <Tp> &>(*part);
                        men.resize(sz / sizeof(Tp));
                        std::memcpy(&rcv[0], &men[offset], men.size());
                        offset += men.size();
                        part_group.add(part);
                    }
                } else {
                    if (!part_sp) {
                        part_sp = std::make_shared<storage::IMemoryPartition < Tp>>
                        ();
                    }
                    auto &men = reinterpret_cast<storage::IMemoryPartition <Tp> &>(*part_sp);
                    int sz = men.size() * sizeof(Tp);
                    group.Gather(&sz, 1, MPI::INT, nullptr, 1, MPI::INT, 0);
                    comm.Gatherv(&men[0], sz, MPI::BYTE, nullptr, nullptr, nullptr, MPI::BYTE, 0);
                }
            } else {
                if (driver) {
                    auto buffer = std::make_shared<transport::IMemoryBuffer>();
                    int sz = 0;
                    std::vector<int> szv;
                    std::vector<int> displs;
                    szv.resize(group.Get_size(), sz);
                    comm.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, 0);
                    displs = this->displs(szv);
                    buffer->getWritePtr(displs.back());
                    comm.Gatherv(buffer->getWritePtr(sz), sz, MPI::BYTE, buffer->getWritePtr(sz), &szv[0], &displs[0],
                                 MPI::BYTE, 0);
                    auto ptr = buffer->getWritePtr(sz);
                    for (int j = 1; j < group.Get_size(); j++) {
                        auto view = std::make_shared<transport::IMemoryBuffer>(ptr + displs[j], szv[j]);
                        part_sp = std::make_shared<storage::IMemoryPartition < Tp>>
                        ();
                        part_sp->read((std::shared_ptr<transport::ITransport> &) view);
                        part_group.add(part_sp);
                    }
                } else {
                    if (!part_sp) {
                        part_sp = std::make_shared<storage::IMemoryPartition < Tp>>
                        ();
                    }
                    auto &men = reinterpret_cast<storage::IMemoryPartition <Tp> &>(*part_sp);
                    auto buffer = std::make_shared<transport::IMemoryBuffer>();
                    int sz = 0;
                    men.write((std::shared_ptr<transport::ITransport> &) buffer, properties.msgCompression());
                    sz = buffer->writeEnd();
                    buffer->resetBuffer();
                    comm.Gather(&sz, 1, MPI::INT, nullptr, 1, MPI::INT, 0);
                    comm.Gatherv(buffer->getWritePtr(sz), sz, MPI::BYTE, nullptr, nullptr, nullptr, MPI::BYTE, 0);
                }
            }
        } else if (storage == storage::IRawMemoryPartition<Tp>::TYPE) {
            if (driver) {
                part_sp = std::make_shared<storage::IRawMemoryPartition < Tp>>
                ();
                auto &raw = reinterpret_cast<storage::IRawMemoryPartition <Tp> &>(*part_sp);
                std::pair<int, int> sz(0, 0);
                std::vector<std::pair<int, int>> elems_szv;
                std::vector<int> szv;
                std::vector<int> displs;
                elems_szv.resize(group.Get_size(), sz);
                comm.Gather(&sz, 2, MPI::INT, &elems_szv[0], 2, MPI::INT, 0);
                szv = this->szVector(elems_szv);
                displs = this->displs(szv);
                raw.resize(elems_szv[0].first, displs.back());
                auto ptr = raw.begin(false) - storage::IRawMemoryPartition<Tp>::HEADER;
                comm.Gatherv(MPI::IN_PLACE, 0, MPI::BYTE, ptr, &szv[0], &displs[0], MPI::BYTE, 0);
                raw.buffer->wroteBytes(elems_szv[0].second);
                part_group.add(part_sp);

                for (int j = 1; j < group.Get_size(); j++) {
                    auto part = std::make_shared<storage::IRawMemoryPartition < Tp>>
                    ();
                    part->buffer = std::make_shared<transport::IMemoryBuffer>(ptr + displs[i], szv[i]);
                    part->elems = elems_szv[i].first;
                    part_group.add(part);
                }
            } else {
                if (!part_sp) {
                    part_sp = std::make_shared<storage::IRawMemoryPartition < Tp>>
                    ();
                }
                auto &raw = reinterpret_cast<storage::IRawMemoryPartition <Tp> &>(*part_sp);
                raw.sync();
                std::pair<int, int> sz(raw.size(), raw.buffer->writeEnd());
                comm.Gather(&sz, 2, MPI::INT, nullptr, 2, MPI::INT, 0);
                auto ptr = raw.begin(false) - storage::IRawMemoryPartition<Tp>::HEADER;
                comm.Gatherv(ptr, sz.second, MPI::BYTE, nullptr, nullptr, nullptr, MPI::BYTE, 0);
            }
        } else {
            if (driver) {
                std::vector<int> lengthv(max_partition * group.Get_size(), 0);
                int sz = 0;
                std::vector<int> szv(group.Get_size());
                group.Gather(MPI_IN_PLACE, lengthv.size(), MPI::INT, &lengthv[0], lengthv.size(), MPI::INT, 0);
                group.Gather(&sz, 1, MPI::INT, &szv[0], szv.size(), MPI::INT, 0);
                std::vector<int> displs = this->displs(szv);
                std::string full_path(displs.back(), '\0');
                group.Gatherv(MPI_IN_PLACE, 0, MPI::BYTE, const_cast<char *>(full_path.c_str()), &szv[0], &displs[0],
                              MPI::BYTE, 0);
                int64_t offset = 0;
                for (int length: lengthv) {
                    if (length == 0) {
                        continue;
                    }
                    std::string src(full_path, offset, length);
                    offset += length;
                    auto part = std::make_shared<storage::IDiskPartition < Tp>>
                    (src, properties.partitionCompression(),
                            false, true);
                    part_group.add(part);
                }
            } else {
                std::vector<int> lengthv;
                std::string full_path;
                for (auto part: part_group) {
                    auto &disk = reinterpret_cast<storage::IDiskPartition <Tp> &>(*part);
                    auto &path = disk.getPath();
                    lengthv.push_back(path.length());
                    full_path.append(path);
                }
                int sz = full_path.length();
                lengthv.resize(max_partition, 0);
                group.Gather(MPI_IN_PLACE, lengthv.size(), MPI::INT, nullptr, 0, MPI::INT, 0);
                group.Gather(&sz, 1, MPI::INT, nullptr, 0, MPI::INT, 0);
                group.Gatherv(const_cast<char *>(full_path.c_str()), lengthv.back(), MPI::BYTE, nullptr, nullptr,
                              nullptr, MPI::BYTE, 0);
            }
            break;
        }
    }

}

template<typename Tp>
void IMpiClass::driverGather0(const MPI::Intracomm &group, storage::IPartitionGroup <Tp> &part_group) {
    int rank = group.Get_rank();
    auto sub_group = group.Split(rank < 2, rank);
    if (rank < 2) {
        driverGather(sub_group, part_group);
    }
    sub_group.Free();
}

template<typename Tp>
void
IMpiClass::driverScatter(const MPI::Intracomm &group, storage::IPartitionGroup <Tp> &part_group, int64_t partitions) {
    auto id = rank();
    bool driver = group.Get_rank() == 0;
    bool exec0 = group.Get_rank() == 1;
    auto execs = executors();
    bool same_protocol;
    uint8_t *ptr = nullptr;
    std::vector<int> szv;
    std::vector<int> displs;
    auto buffer = std::make_shared<transport::IMemoryBuffer>();

    if (driver) {
        int8_t protocol = core::protocol::IObjectProtocol::CPP_PROTOCOL;
        group.Send(&protocol, 1, MPI::BYTE, 0, 0);
        group.Recv(&same_protocol, 1, MPI::BOOL, 0, 0);
    } else if (exec0) {
        int8_t protocol;
        group.Recv(&protocol, 1, MPI::BYTE, 1, 0);
        same_protocol = protocol == core::protocol::IObjectProtocol::CPP_PROTOCOL;
        group.Send(&same_protocol, 1, MPI::BOOL, 1, 0);
    }

    IGNIS_LOG(info) << "Comm: driverScatter partitions: " << part_group.partitions();

    if (driver) {
        auto &men = reinterpret_cast<storage::IMemoryPartition <Tp> &>(*part_group[0]);
        ptr = (uint8_t *) &men[0];
        auto elems = men.size();
        auto executor_elems = elems / execs;
        auto remainder = elems % execs;
        szv.reserve(execs + 1);
        szv.push_back(0);
        szv.insert(szv.end(), remainder, executor_elems + 1);
        szv.insert(szv.end(), execs - remainder, executor_elems);
    }

    if (isContiguousType<Tp>() && same_protocol) {
        int sz;
        displs = this->displs(szv);
        group.Scatter(&szv[0], szv.size(), MPI::INT, &sz, 1, MPI::INT, 0);
        group.Scatterv(ptr, &szv[0], &displs[0], MPI::BYTE, buffer->getWritePtr(sz), sz, MPI::BYTE, 0);
        auto local_partitions = partitions / execs + ((partitions % execs < id) ? 1 : 0);

        if (!driver) {
            auto executor_elems = sz / local_partitions;
            auto remainder = sz % local_partitions;
            for (int i = 0; i < local_partitions; i++) {
                auto part = std::make_shared<storage::IMemoryPartition < Tp>>
                (sz);
                part->resize(sz);
                if (i < remainder) {
                    std::memcpy(&(*part)[0], buffer->getWritePtr(0), (executor_elems + 1) * sizeof(Tp));
                } else {
                    std::memcpy(&(*part)[0], buffer->getWritePtr(0), executor_elems * sizeof(Tp));
                }
                part_group.add(part);
            }
        }

    } else {
        std::vector<int> szv_part;
        auto max_exec_partitions = (int) std::ceil(partitions / (float) execs);
        if (driver) {
            auto cmp = properties.msgCompression();
            auto zlib = std::make_shared<transport::IZlibTransport>(buffer, cmp);
            protocol::IObjectProtocol proto(zlib);
            szv_part.resize(max_exec_partitions * (execs + 1), 0);
            auto writeBase = buffer->writeEnd();
            int64_t offset = 0;
            for (int64_t i = 0; i < execs + 1; i++) {
                auto &szi = szv[i + i];
                auto exec_partitions = partitions / execs + ((partitions % execs < i) ? 1 : 0);
                auto executor_elems = szi / exec_partitions;
                auto remainder = szi % exec_partitions;
                for (int64_t j = 0; j < exec_partitions; j++) {
                    if (j < remainder) {
                        proto.writeObject(api::IVector<Tp>::view((Tp *) &ptr[offset], executor_elems + 1), false);
                        offset += executor_elems + 1;
                    } else {
                        proto.writeObject(api::IVector<Tp>::view((Tp *) &ptr[offset], executor_elems), false);
                        offset += executor_elems;
                    }
                    zlib->flush();
                    zlib->reset();
                    szv_part[i * max_exec_partitions + j];
                }
                szi = (int) (buffer->writeEnd() - writeBase);
                writeBase = buffer->writeEnd();
            }
            buffer->resetBuffer();
            part_group.clear();
            displs = this->displs(szv);
        } else {
            szv_part.resize(max_exec_partitions);
        }
        int sz;
        group.Scatter(&szv[0], szv.size(), MPI::INT, &sz, 1, MPI::INT, 0);
        group.Scatter(MPI_IN_PLACE, szv_part.size(), MPI::INT, &szv_part[0], szv_part.size(), MPI::INT, 0);
        group.Scatterv(ptr, &szv[0], &displs[0], MPI::BYTE, ptr = buffer->getWritePtr(sz), sz, MPI::BYTE, 0);

        if (!driver) {
            int64_t offset = 0;
            for (int i = 0; i < szv_part.size(); i++) {
                auto view = std::make_shared<transport::IMemoryBuffer>(ptr + offset, szv_part[i]);
                offset += szv_part[i];
                auto part = std::make_shared<storage::IMemoryPartition < Tp>>
                (sz);
                part->read((std::shared_ptr<transport::ITransport> &) view);
                part_group.add(part);
            }
        }
    }
}

template<typename Tp>
void IMpiClass::send(const MPI::Intracomm &group, storage::IPartition <Tp> &part, int dest, int tag) {
    sendRecv(group, part, rank(), dest, tag);
}

template<typename Tp>
void IMpiClass::send(storage::IPartition <Tp> &part, int dest, int tag) {
    sendRecv(part, rank(), dest, tag);
}

template<typename Tp>
void IMpiClass::recv(const MPI::Intracomm &group, storage::IPartition <Tp> &part, int source, int tag) {
    sendRecv(group, part, source, rank(), tag);
}

template<typename Tp>
void IMpiClass::recv(storage::IPartition <Tp> &part, int source, int tag) {
    sendRecv(part, source, rank(), tag);
}

template<typename Tp>
void IMpiClass::sendRecv(storage::IPartition <Tp> &part, int source, int dest, int tag) {
    auto id = rank();
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isContiguousType<Tp>()) {
            auto &men = reinterpret_cast<storage::IMemoryPartition <Tp> &>(part);
            int sz = men.size();
            if (id == source) {
                comm.Send(&sz, 1, MPI::INT, dest, tag);
                comm.Send(&men[0], sz * sizeof(Tp), MPI::BYTE, dest, tag);
            } else {
                int init = sz;
                comm.Recv(&sz, 1, MPI::INT, source, tag);
                men.resize(init + sz);
                comm.Recv(&men[init], sz * sizeof(Tp), MPI::BYTE, source, tag);
            }
        } else {
            auto buffer = std::make_shared<transport::IMemoryBuffer>(part.bytes());
            int sz;
            if (id == source) {
                part.write((std::shared_ptr<transport::ITransport> &) buffer, properties.msgCompression());
                sz = buffer->writeEnd();
                buffer->resetBuffer();
                comm.Send(&sz, 1, MPI::INT, dest, tag);
                comm.Send(buffer->getWritePtr(sz), sz, MPI::BYTE, dest, tag);
            } else {
                comm.Recv(&sz, 1, MPI::INT, source, tag);
                comm.Recv(buffer->getWritePtr(sz), sz, MPI::BYTE, source, tag);
                buffer->wroteBytes(sz);
                part.read((std::shared_ptr<transport::ITransport> &) buffer);
            }
        }
    } else if (part.type() == storage::IRawMemoryPartition<Tp>::TYPE) {
        auto &raw = reinterpret_cast<storage::IRawMemoryPartition <Tp> &>(part);
        raw.sync();
        std::pair<int, int> sz(raw.size(), (int) (raw.end() - raw.begin(false)));
        if (id == source) {
            comm.Send(&sz, 2, MPI::INT, dest, tag);
            comm.Send(raw.begin(false), sz.second, MPI::BYTE, dest, tag);
        } else {
            auto init = sz;
            comm.Recv(&sz, 2, MPI::INT, source, tag);
            raw.resize(init.first + sz.first, init.second + sz.second);
            comm.Recv(raw.begin(false) + init.first, sz.second, MPI::BYTE, source, tag);
        }
    } else {
        auto &disk = reinterpret_cast<storage::IDiskPartition <Tp> &>(part);
        disk.sync();
        std::string path = disk.getPath();
        int sz = path.size();
        if (id == source) {
            comm.Send(&sz, 1, MPI::INT, dest, tag);
            comm.Send(const_cast<char *>(path.c_str()), sz, MPI::BYTE, dest, tag);
        } else {
            comm.Recv(&sz, 1, MPI::INT, source, tag);
            path.resize(sz);
            comm.Recv(const_cast<char *>(path.c_str()), sz, MPI::BYTE, source, tag);
            storage::IDiskPartition <Tp> rcv(path, properties.partitionCompression(), true, true);
            if (disk.size() == 0) {
                disk.destroy = true;
                std::swap(disk, rcv);
            } else {
                rcv.copyTo(disk);
            }
        }
    }
}

template<typename Tp>
void IMpiClass::sendRecv(const MPI::Intracomm &group, storage::IPartition <Tp> &part, int source, int dest, int tag) {
    auto id = group.Get_rank();
    int8_t protocol;
    bool same_protocol;
    std::string storage;
    bool same_storage;

    if (id == source) {
        int length = storage.length();
        protocol = core::protocol::IObjectProtocol::CPP_PROTOCOL;
        storage = part.type();
        group.Send(&protocol, 1, MPI::BYTE, dest, tag);
        group.Send(&length, 1, MPI::INT, dest, tag);
        group.Send(const_cast<char *>(storage.c_str()), length, MPI::BYTE, 1, 0);
        group.Recv(&same_protocol, 1, MPI::BOOL, source, tag);
    } else {
        int length;
        group.Recv(&protocol, 1, MPI::BYTE, source, tag);
        group.Recv(&length, 1, MPI::INT, dest, tag);
        group.Recv(const_cast<char *>(storage.c_str()), length, MPI::BYTE, 1, 0);
        same_protocol = protocol == core::protocol::IObjectProtocol::CPP_PROTOCOL;
        same_storage = part.type() == storage;
        group.Send(&same_protocol, 1, MPI::BOOL, dest, tag);
    }


}

template<typename Tp>
bool IMpiClass::isContiguousType() {
    return io::isContiguous<Tp>()();
}


#undef IMpiClass
