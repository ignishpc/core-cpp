
#include "ILog.h"
#include "IMpi.h"
#include "io/INativeWriter.h"
#include "storage/IDiskPartition.h"
#include "storage/IMemoryPartition.h"
#include "storage/IRawMemoryPartition.h"

#include <iostream>

#define IMpiClass ignis::executor::core::IMpi


template<typename Tp>
void IMpiClass::gather(storage::IPartition<Tp> &part, int root) {
    if (executors() == 1) { return; }
    gatherImpl(comm, part, root, true);
}

template<typename Tp>
void IMpiClass::bcast(storage::IPartition<Tp> &part, int root) {
    if (executors() == 1) { return; }
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isContiguousType<Tp>()) {
            auto &men = partition_tools.toMemory(part);
            int sz = men.size();
            comm.Bcast(&sz, 1, MPI::INT, 0);
            if (!isRoot(root)) { men.resize(sz); }
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
        if (!isRoot(root)) { raw.resize(sz.first, sz.second); }
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
        comm.Barrier();
    }
}

template<typename Tp>
void IMpiClass::driverGather(const MPI::Intracomm &group, storage::IPartitionGroup<Tp> &part_group) {
    bool driver = group.Get_rank() == 0;
    bool exec0 = group.Get_rank() == 1;
    int max_partition = 0;
    int8_t protocol;
    bool same_protocol;
    std::string storage;
    std::string storage_v;
    int storage_length = 0;

    if (driver) {
        protocol = core::protocol::IObjectProtocol::CPP_PROTOCOL;
    } else {
        max_partition = (int) part_group.partitions();
        if (!part_group.empty()) { storage = part_group[0]->type() + ";"; }
        storage_length = storage.length();
    }

    group.Allreduce(MPI_IN_PLACE, &max_partition, 1, MPI::INT, MPI::MAX);
    if (max_partition == 0) { return; }
    group.Allreduce(MPI_IN_PLACE, &storage_length, 1, MPI::INT, MPI::MAX);
    group.Bcast(&protocol, 1, MPI::BYTE, 0);

    storage.resize(storage_length);
    storage_v.resize(storage_length * group.Get_size());
    group.Allgather(const_cast<char *>(storage.c_str()), storage_length, MPI::BYTE,
                    const_cast<char *>(storage_v.c_str()), storage_length, MPI::BYTE);

    for (int i = 0; i < storage_v.size();) {
        if (storage_v[i] != '\0') { storage = storage_v.substr(i, storage_length - 1); }
        i += storage_length;
    }

    if (driver) {
        group.Recv(&same_protocol, 1, MPI::BOOL, 1, 0);
    } else {
        same_protocol = protocol == core::protocol::IObjectProtocol::CPP_PROTOCOL;
        if (exec0) { group.Send(&same_protocol, 1, MPI::BOOL, 0, 0); }
    }

    IGNIS_LOG(info) << "Comm: driverGather storage: " << storage << ", operations: " << max_partition;
    if (storage == storage::IDiskPartition<Tp>::TYPE && !same_protocol) {
        //Other languages can use their own serialization, so driver will use RawMemory
        storage = storage::IRawMemoryPartition<Tp>::TYPE;
    }

    std::shared_ptr<storage::IPartition<Tp>> part_sp;
    for (int64_t i = 0; i < max_partition; i++) {
        if (i < part_group.partitions()) {
            part_sp = part_group[i];
        } else if (!part_sp || !part_sp->empty()) {
            part_sp = partition_tools.newPartition<Tp>(storage);
            if (driver) { part_group.add(part_sp); }
        }
        gatherImpl(group, *part_sp, 0, same_protocol);
    }
}

template<typename Tp>
void IMpiClass::driverGather0(const MPI::Intracomm &group, storage::IPartitionGroup<Tp> &part_group) {
    int rank = group.Get_rank();
    auto sub_group = group.Split(rank < 2, rank);
    if (rank < 2) { driverGather(sub_group, part_group); }
    sub_group.Free();
}

template<typename Tp>
void IMpiClass::driverScatter(const MPI::Intracomm &group, storage::IPartitionGroup<Tp> &part_group,
                              int64_t partitions) {
    auto id = group.Get_rank();
    bool driver = id == 0;
    bool exec0 = id == 1;
    auto execs = group.Get_size() - 1;
    bool same_protocol;
    int64_t sz = 0;
    uint8_t *src = nullptr;
    std::vector<int> partsv;
    std::vector<int> szv;
    std::vector<int> displs;
    auto buffer = std::make_shared<transport::IMemoryBuffer>();

    int8_t protocol = core::protocol::IObjectProtocol::CPP_PROTOCOL;
    group.Bcast(&protocol, 1, MPI::BYTE, 0);

    if (driver) {
        auto &men = reinterpret_cast<storage::IMemoryPartition<Tp> &>(*part_group[0]);
        src = (uint8_t *) &men[0];
        sz = men.size();
        group.Recv(&same_protocol, 1, MPI::BOOL, 1, 0);
    } else {
        same_protocol = protocol == core::protocol::IObjectProtocol::CPP_PROTOCOL;
        if (exec0) { group.Send(&same_protocol, 1, MPI::BOOL, 0, 0); }
    }

    IGNIS_LOG(info) << "Comm: driverScatter partitions: " << partitions;

    auto execs_parts = partitions / execs;
    if (partitions % execs > 0) { execs_parts++; }

    if (driver) {
        auto execs_elems = sz / partitions;
        auto remainder = sz % partitions;
        partsv.reserve(execs_parts * (execs + 1));
        partsv.insert(partsv.end(), execs_parts, 0);
        partsv.insert(partsv.end(), remainder, (execs_elems + 1) * sizeof(Tp));
        partsv.insert(partsv.end(), partitions - remainder, execs_elems * sizeof(Tp));
        partsv.resize(execs_parts * (execs + 1), 0);

        szv.push_back(0);
        displs.push_back(0);
        int64_t offset = 0;
        if (isContiguousType<Tp>() && same_protocol) {
            for (int64_t i = execs_parts; i < partsv.size(); i++) {
                offset += partsv[i];
                if ((i + 1) % execs_parts == 0) {
                    displs.push_back(szv.back() + displs.back());
                    szv.push_back(offset - displs.back());
                }
            }
        } else {
            auto cmp = properties.msgCompression();
            auto zlib = std::make_shared<transport::IZlibTransport>(buffer, cmp);
            protocol::IObjectProtocol proto(zlib);
            int64_t wrote = 0;
            for (int64_t i = execs_parts; i < partsv.size(); i++) {
                proto.writeObject(api::IVector<Tp>::view((Tp *) &src[offset], partsv[i] / sizeof(Tp)));
                offset += partsv[i];
                zlib->flush();
                zlib->reset();
                partsv[i] = buffer->writeEnd() - wrote;
                wrote += partsv[i];
                if ((i + 1) % execs_parts == 0) {
                    displs.push_back(szv.back() + displs.back());
                    szv.push_back(buffer->writeEnd() - displs.back());
                }
            }
            src = buffer->getWritePtr(0) - buffer->writeEnd();
        }
    } else {
        partsv.resize(execs_parts, 0);
    }

    group.Scatter(&partsv[0], execs_parts, MPI::INT, driver ? MPI_IN_PLACE : &partsv[0], execs_parts, MPI::INT, 0);

    if (!driver) {
        for (auto &len : partsv) { sz += len; }
        src = buffer->getWritePtr(sz);
    }

    group.Scatterv(src, &szv[0], &displs[0], MPI::BYTE, driver ? MPI_IN_PLACE : src, sz, MPI::BYTE, 0);

    if (!driver) {
        if (isContiguousType<Tp>() && same_protocol) {
            int64_t offset = 0;
            for (auto &len : partsv) {
                if (len == 0) { break; }
                auto part = std::make_shared<storage::IMemoryPartition<Tp>>(len / sizeof(Tp));
                part->resize(len / sizeof(Tp));
                std::memcpy(&(*part)[0], src + offset, len);
                offset += len;
                part_group.add(part);
            }
        } else {
            int64_t offset = 0;
            for (auto &len : partsv) {
                if (len == 0) { break; }
                auto view = std::make_shared<transport::IMemoryBuffer>(src + offset, len);
                auto part = partition_tools.newPartition<Tp>();
                part->read((std::shared_ptr<transport::ITransport> &) view);
                offset += len;
                part_group.add(part);
            }
        }
    }
}

template<typename Tp>
void IMpiClass::send(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int dest, int tag) {
    sendRecv(group, part, group.Get_rank(), dest, tag);
}

template<typename Tp>
void IMpiClass::send(storage::IPartition<Tp> &part, int dest, int tag) {
    sendRecv(part, rank(), dest, tag);
}

template<typename Tp>
void IMpiClass::recv(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int source, int tag) {
    sendRecv(group, part, source, group.Get_rank(), tag);
}

template<typename Tp>
void IMpiClass::recv(storage::IPartition<Tp> &part, int source, int tag) {
    sendRecv(part, source, rank(), tag);
}

template<typename Tp>
void IMpiClass::gatherImpl(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int root, bool same_protocol) {
    int rank = group.Get_rank();
    int executors = group.Get_size();
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isContiguousType<Tp>() && same_protocol) {
            auto &men = partition_tools.toMemory(part);
            int sz = men.size() * sizeof(Tp);
            std::vector<int> szv;
            std::vector<int> displs;
            if (rank == root) { szv.resize(executors, sz); }
            group.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, root);
            if (rank == root) {
                displs = this->displs(szv);
                men.resize(displs.back() / sizeof(Tp));
                //Use same buffer to rcv elements
                move(&men[0], szv[rank], displs[rank]);
                group.Gatherv(MPI::IN_PLACE, 0, MPI::BYTE, &men[0], &szv[0], &displs[0], MPI::BYTE, root);
            } else {
                group.Gatherv(&men[0], sz, MPI::BYTE, nullptr, nullptr, nullptr, MPI::BYTE, root);
            }
        } else {
            auto &men = partition_tools.toMemory(part);
            auto buffer = std::make_shared<transport::IMemoryBuffer>();
            int sz = 0;
            std::vector<int> szv;
            std::vector<int> displs;
            if (rank != root) {
                part.write((std::shared_ptr<transport::ITransport> &) buffer, properties.msgCompression());
                sz = buffer->writeEnd();
                buffer->resetBuffer();
            }
            if (rank == root) { szv.resize(executors, sz); }
            group.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, root);
            if (rank == root) {
                displs = this->displs(szv);
                buffer->getWritePtr(displs.back());
            }
            group.Gatherv(buffer->getWritePtr(sz), sz, MPI::BYTE, buffer->getWritePtr(sz), &szv[0], &displs[0],
                          MPI::BYTE, root);
            if (rank == root) {
                auto ptr = buffer->getWritePtr(sz);
                storage::IMemoryPartition<Tp> rcv;
                for (int i = 0; i < executors; i++) {
                    if (i != rank) {
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
        raw.writeHeader();
        if (!same_protocol) {//Headers for dynamic languages
            auto HEADER = storage::IRawMemoryPartition<Tp>::HEADER;
            std::vector<int> hsz;
            if (rank == root) { hsz.resize(executors, raw.header_size); }
            group.Gather(&raw.header_size, 1, MPI::INT, &hsz[0], 1, MPI::INT, root);
            group.Gather(rank == root ? MPI::IN_PLACE : (raw.begin(false) - HEADER), HEADER, MPI::BYTE,
                         (raw.begin(false) - HEADER), HEADER, MPI::BYTE, root);
            raw.writeHeader();//C++ ignore and write his header
        }
        std::pair<int, int> sz(raw.size(), raw.end() - raw.begin(false));
        std::vector<std::pair<int, int>> elems_szv;
        std::vector<int> szv;
        std::vector<int> displs;
        if (rank == root) { elems_szv.resize(executors, sz); }
        group.Gather(&sz, 2, MPI::INT, &elems_szv[0], 2, MPI::INT, root);
        if (rank == root) {
            szv = this->szVector(elems_szv);
            displs = this->displs(szv);
            auto n = sumElems(elems_szv);
            raw.resize(n, displs.back());
            auto ptr = raw.begin(false);
            move(ptr, szv[rank], displs[rank]);
            group.Gatherv(MPI::IN_PLACE, 0, MPI::BYTE, ptr, &szv[0], &displs[0], MPI::BYTE, root);
        } else {
            group.Gatherv(raw.begin(false), sz.second, MPI::BYTE, nullptr, nullptr, nullptr, MPI::BYTE, root);
        }
    } else {
        auto &disk = partition_tools.toDisk(part);
        disk.sync();
        std::string path = disk.getPath();
        std::string full_path;
        int sz = path.size();
        std::vector<int> szv;
        std::vector<int> displs;
        if (rank == root) { szv.resize(executors, sz); }
        group.Gather(&sz, 1, MPI::INT, &szv[0], 1, MPI::INT, root);
        if (rank == root) {
            displs = this->displs(szv);
            full_path.resize(displs.back());
        }
        group.Gatherv(const_cast<char *>(path.c_str()), sz, MPI::BYTE, const_cast<char *>(full_path.c_str()), &szv[0],
                      &displs[0], MPI::BYTE, root);
        if (rank == root) {
            auto root_path = disk.getPath();
            disk.rename(root_path + ".tmp");
            storage::IDiskPartition<Tp> rcv(root_path, properties.partitionCompression(), false, false);
            for (int i = 0; i < executors; i++) {
                if (i != root) {
                    std::string src(full_path, displs[i], szv[i]);
                    storage::IDiskPartition<Tp> aux(src, properties.partitionCompression(), false, true);
                    aux.copyTo(rcv);
                } else {
                    part.moveTo(rcv);
                }
            }
            rcv.destroy = disk.destroy;
            disk.destroy = true;
            std::swap(disk, rcv);
        }
        group.Barrier();
    }
}

template<typename Tp>
void IMpiClass::sendRecv(storage::IPartition<Tp> &part, int source, int dest, int tag) {
    sendRecvImpl(comm, part, source, dest, tag, true);
}

template<typename Tp>
void IMpiClass::sendRecv(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int source, int dest, int tag) {
    auto id = group.Get_rank();
    bool same_protocol;
    bool same_storage;

    if (id == source) {
        int8_t protocol = core::protocol::IObjectProtocol::CPP_PROTOCOL;
        std::string storage = part.type();
        int length = storage.length();
        group.Send(&protocol, 1, MPI::BYTE, dest, tag);
        group.Recv(&same_protocol, 1, MPI::BOOL, dest, tag);
        group.Send(&length, 1, MPI::INT, dest, tag);
        group.Send(const_cast<char *>(storage.c_str()), length, MPI::BYTE, dest, tag);
        group.Recv(&same_storage, 1, MPI::BOOL, dest, tag);
    } else {
        int8_t protocol;
        std::string storage;
        int length;
        group.Recv(&protocol, 1, MPI::BYTE, source, tag);
        same_protocol = protocol == core::protocol::IObjectProtocol::CPP_PROTOCOL;
        group.Send(&same_protocol, 1, MPI::BOOL, source, tag);
        group.Recv(&length, 1, MPI::INT, source, tag);
        storage.resize(length);
        group.Recv(const_cast<char *>(storage.c_str()), length, MPI::BYTE, source, tag);
        same_storage = part.type() == storage;
        group.Send(&same_storage, 1, MPI::BOOL, source, tag);
    }

    if (same_storage) {
        sendRecvImpl(group, part, source, dest, tag, same_protocol);
    } else {
        auto buffer = std::make_shared<transport::IMemoryBuffer>(part.bytes());
        int sz;
        if (id == source) {
            part.write((std::shared_ptr<transport::ITransport> &) buffer, properties.msgCompression());
            sz = buffer->writeEnd();
            buffer->resetBuffer();
            group.Send(&sz, 1, MPI::INT, dest, tag);
            group.Send(buffer->getWritePtr(sz), sz, MPI::BYTE, dest, tag);
        } else {
            group.Recv(&sz, 1, MPI::INT, source, tag);
            group.Recv(buffer->getWritePtr(sz), sz, MPI::BYTE, source, tag);
            buffer->wroteBytes(sz);
            part.read((std::shared_ptr<transport::ITransport> &) buffer);
        }
    }
}

template<typename Tp>
void IMpiClass::sendRecvImpl(const MPI::Intracomm &group, storage::IPartition<Tp> &part, int source, int dest, int tag,
                             bool same_protocol) {
    auto id = group.Get_rank();
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isContiguousType<Tp>() && same_protocol) {
            auto &men = reinterpret_cast<storage::IMemoryPartition<Tp> &>(part);
            int sz = men.size();
            if (id == source) {
                group.Send(&sz, 1, MPI::INT, dest, tag);
                group.Send(&men[0], sz * sizeof(Tp), MPI::BYTE, dest, tag);
            } else {
                int init = sz;
                group.Recv(&sz, 1, MPI::INT, source, tag);
                men.resize(init + sz);
                group.Recv(&men[init], sz * sizeof(Tp), MPI::BYTE, source, tag);
            }
        } else {
            auto buffer = std::make_shared<transport::IMemoryBuffer>(part.bytes());
            int sz;
            if (id == source) {
                part.write((std::shared_ptr<transport::ITransport> &) buffer, properties.msgCompression());
                sz = buffer->writeEnd();
                buffer->resetBuffer();
                group.Send(&sz, 1, MPI::INT, dest, tag);
                group.Send(buffer->getWritePtr(sz), sz, MPI::BYTE, dest, tag);
            } else {
                group.Recv(&sz, 1, MPI::INT, source, tag);
                group.Recv(buffer->getWritePtr(sz), sz, MPI::BYTE, source, tag);
                buffer->wroteBytes(sz);
                part.read((std::shared_ptr<transport::ITransport> &) buffer);
            }
        }
    } else if (part.type() == storage::IRawMemoryPartition<Tp>::TYPE) {
        auto &raw = reinterpret_cast<storage::IRawMemoryPartition<Tp> &>(part);
        auto HEADER = storage::IRawMemoryPartition<Tp>::HEADER;
        raw.sync();
        raw.writeHeader();
        if (!same_protocol) {//Headers for dynamic languages
            if (id == source) {
                group.Send(&raw.header_size, 1, MPI::INT, dest, tag);
            } else {
                group.Recv(&raw.header_size, 1, MPI::INT, source, tag);
            }
        }
        std::pair<int, int> sz(raw.size(), (int) (raw.end() - raw.begin(false) + HEADER));
        if (id == source) {
            group.Send(&sz, 2, MPI::INT, dest, tag);
            group.Send(raw.begin(false) - HEADER, sz.second, MPI::BYTE, dest, tag);
        } else {
            group.Recv(&sz, 2, MPI::INT, source, tag);
            if (part.empty()) {
                raw.resize(sz.first, sz.second - HEADER);
                group.Recv(raw.begin(false) - HEADER, sz.second, MPI::BYTE, source, tag);
                raw.writeHeader();
            } else {
                storage::IRawMemoryPartition<Tp> tmp;
                tmp.resize(sz.first, sz.second - HEADER);
                group.Recv(raw.begin(false) - HEADER, sz.second, MPI::BYTE, source, tag);
                tmp.writeHeader();
                tmp.moveTo(raw);
            }
        }
    } else {
        auto &disk = reinterpret_cast<storage::IDiskPartition<Tp> &>(part);
        disk.sync();
        std::string path = disk.getPath();
        int sz = path.size();
        if (id == source) {
            group.Send(&sz, 1, MPI::INT, dest, tag);
            group.Send(const_cast<char *>(path.c_str()), sz, MPI::BYTE, dest, tag);
            group.Recv(&sz, 1, MPI::INT, dest, tag);//wait copyDiskPartition
        } else {
            group.Recv(&sz, 1, MPI::INT, source, tag);
            path.resize(sz);
            group.Recv(const_cast<char *>(path.c_str()), sz, MPI::BYTE, source, tag);
            auto rcv = partition_tools.copyDiskPartition<Tp>(path);
            group.Send(&sz, 1, MPI::INT, source, tag);
            if (disk.size() == 0) {
                disk.destroy = true;
                std::swap(disk, *rcv);
            } else {
                rcv->copyTo(disk);
            }
        }
    }
}

template<typename Tp>
bool IMpiClass::isContiguousType() {
    return io::isContiguous<Tp>()();
}


#undef IMpiClass
