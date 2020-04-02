
#include "IMpi.h"
#include "storage/IMemoryPartition.h"
#include "storage/IRawMemoryPartition.h"
#include "storage/IDiskPartition.h"

#include <iostream>

#define IMpiClass ignis::executor::core::IMpi


template<typename Tp>
void IMpiClass::gather(storage::IPartition<Tp> &part, int root) {
    if (executors() == 1) { return; }
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isPrimitive<Tp>()()) {
            auto &men = reinterpret_cast<storage::IMemoryPartition<Tp> &>(part);
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
            auto &men = reinterpret_cast<storage::IMemoryPartition<Tp> &>(part);
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
                storage::IMemoryPartition<Tp> rcv;
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
        auto &raw = reinterpret_cast<storage::IRawMemoryPartition<Tp> &>(part);
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
        auto &disk = reinterpret_cast<storage::IDiskPartition<Tp> &>(part);
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
            disk.rename(path + ".tmp");
            storage::IDiskPartition<Tp> rcv(root_path, properties.partitionCompression(), false, false);
            for (int i = 0; i < executors(); i++) {
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
    }
}

template<typename Tp>
void IMpiClass::bcast(storage::IPartition<Tp> &part, int root) {
    if (executors() == 1) { return; }
    if (part.type() == storage::IMemoryPartition<Tp>::TYPE) {
        if (isPrimitive<Tp>()()) {
            auto &men = reinterpret_cast<storage::IMemoryPartition<Tp> &>(part);
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
            comm.Bcast(&sz, 1, MPI::LONG_INT, root);
            comm.Bcast(buffer->getWritePtr(sz), sz, MPI::BYTE, root);
            if (!isRoot(root)) {
                buffer->wroteBytes(sz);
                part.clear();
                part.read((std::shared_ptr<transport::ITransport> &) buffer);
            }
        }
    } else if (part.type() == storage::IRawMemoryPartition<Tp>::TYPE) {
        auto &raw = reinterpret_cast<storage::IRawMemoryPartition<Tp> &>(part);
        raw.sync();
        std::pair<int, int> sz(raw.size(), (int) (raw.end() - raw.begin(false)));
        comm.Bcast(&sz, 2, MPI::INT, root);
        if (!isRoot(root)) {
            raw.resize(sz.first, sz.second);
        }
        comm.Bcast(raw.begin(false), sz.second, MPI::BYTE, root);
    } else {
        auto &disk = reinterpret_cast<storage::IDiskPartition<Tp> &>(part);
        disk.sync();
        std::string path = disk.getPath();
        int sz = path.size();
        comm.Bcast(&sz, 1, MPI::INT, root);
        path.resize(sz);
        comm.Bcast(const_cast<char *>(path.c_str()), sz, MPI::BYTE, root);
        if (!isRoot(root)) {
            storage::IDiskPartition<Tp> rcv(path, properties.partitionCompression(), false, true);
            disk.destroy = true;
            std::swap(disk, rcv);
        }
    }
}

template<typename Tp>
bool IMpiClass::isPrimitiveType(){
    return isPrimitive<Tp>()();
}

template<typename Tp>
struct IMpiClass::isPrimitive {
    inline bool operator()() {
        return std::is_integral<Tp>::value || std::is_floating_point<Tp>::value;
    }
};

template<typename Tp1, typename Tp2>
struct IMpiClass::isPrimitive<std::pair<Tp1, Tp2>> {
    inline bool operator()() {
        return isPrimitive<Tp1>()() && isPrimitive<Tp2>()();
    }
};

#undef IMpiClass
