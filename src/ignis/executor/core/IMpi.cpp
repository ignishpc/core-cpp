
#include "IMpi.h"
#include <cstring>

using namespace ignis::executor::core;

IMpi::IMpi(IPropertyParser &properties, IPartitionTools &partition_tools, api::IContext& context)
    : properties(properties), partition_tools(partition_tools), context(context) {}

bool IMpi::isRoot(int root) { return native().Get_rank() == root; }

int IMpi::rank() { return native().Get_rank(); }

int IMpi::executors() { return native().Get_size(); }

std::vector<int> IMpi::displs(const std::vector<int> &sz) {
    std::vector<int> d;//Last value is not used by mpi but is used as total size
    d.reserve(sz.size() + 1);
    d.push_back(0);
    for (int i = 0; i < sz.size(); i++) { d.push_back(sz[i] + d[i]); }
    return std::move(d);
}

const MPI::Intracomm &IMpi::native() { return context.mpiGroup(); }

std::vector<int> IMpi::szVector(const std::vector<std::pair<int, int>> &elems_szv) {
    std::vector<int> szv;
    szv.reserve(elems_szv.size());
    for (int i = 0; i < elems_szv.size(); i++) { szv.push_back(elems_szv[i].second); }
    return std::move(szv);
}

int IMpi::sumElems(const std::vector<std::pair<int, int>> &elems_szv) {
    int n = 0;
    for (int i = 0; i < elems_szv.size(); i++) { n += elems_szv[i].first; }
    return n;
}

void IMpi::move(void *begin, size_t n, size_t displ) {
    if (displ > 0) { std::memmove(((char *) begin) + displ, begin, n); }
}

void IMpi::barrier() { native().Barrier(); }

void IMpi::driverScatterVoid(const MPI::Intracomm &group,
                             storage::IPartitionGroup<storage::IVoidPartition::VOID_TYPE> &part_group,
                             int64_t partitions) {
    auto id = group.Get_rank();
    bool exec0 = id == 1;
    auto execs = group.Get_size() - 1;
    bool same_protocol;
    int64_t sz = 0;
    uint8_t *src;
    std::vector<int> partsv;
    std::vector<int> szv;
    std::vector<int> displs;
    auto buffer = std::make_shared<transport::IMemoryBuffer>();

    int8_t protocol;
    group.Bcast(&protocol, 1, MPI::BYTE, 0);

    same_protocol = protocol == core::protocol::IObjectProtocol::IGNIS_PROTOCOL;
    if (exec0) { group.Send(&same_protocol, 1, MPI::BOOL, 0, 0); }

    IGNIS_LOG(info) << "Comm: driverScatter(Void) partitions: " << partitions;

    auto execs_parts = partitions / execs;
    if (partitions % execs > 0) { execs_parts++; }

    partsv.resize(execs_parts, 0);

    group.Scatter(&partsv[0], execs_parts, MPI::INT, &partsv[0], execs_parts, MPI::INT, 0);

    for (auto &len : partsv) { sz += len; }
    src = buffer->getWritePtr(sz);

    group.Scatterv(src, &szv[0], &displs[0], MPI::BYTE, src, sz, MPI::BYTE, 0);

    int64_t offset = 0;
    for (auto &len : partsv) {
        if (len == 0) { break; }
        auto view = std::make_shared<transport::IMemoryBuffer>(src + offset, len);
        auto part = std::make_shared<storage::IVoidPartition>(len);
        part->read((std::shared_ptr<transport::ITransport> &) view);
        offset += len;
        part_group.add(part);
    }
}

IMpi::MsgOpt IMpi::getMsgOpt(const MPI::Intracomm &group, const std::string &ptype, bool send, int other, int tag){
    auto id = group.Get_rank();
    int source = send ? id : other;
    int dest = send ? other : id;
    MsgOpt opt;

    if (id == source) {
        int8_t protocol = core::protocol::IObjectProtocol::CPP_PROTOCOL;
        std::string storage = ptype;
        int length = storage.length();
        group.Send(&protocol, 1, MPI::BYTE, dest, tag);
        group.Recv(&opt.same_protocol, 1, MPI::BOOL, dest, tag);
        group.Send(&length, 1, MPI::INT, dest, tag);
        group.Send(const_cast<char *>(storage.c_str()), length, MPI::BYTE, dest, tag);
        group.Recv(&opt.same_storage, 1, MPI::BOOL, dest, tag);
    } else {
        int8_t protocol;
        std::string storage;
        int length;
        group.Recv(&protocol, 1, MPI::BYTE, source, tag);
        opt.same_protocol = protocol == core::protocol::IObjectProtocol::CPP_PROTOCOL;
        group.Send(&opt.same_protocol, 1, MPI::BOOL, source, tag);
        group.Recv(&length, 1, MPI::INT, source, tag);
        storage.resize(length);
        group.Recv(const_cast<char *>(storage.c_str()), length, MPI::BYTE, source, tag);
        opt.same_storage = ptype == storage;
        group.Send(&opt.same_storage, 1, MPI::BOOL, source, tag);
    }
    return opt;
}

void IMpi::recvVoid(const MPI::Intracomm &group, storage::IVoidPartition &part, int source, int tag) {
    MsgOpt opt = getMsgOpt(group, part.type(), false, source, tag);
    recvVoid(group, part, source,tag, opt);
}

void IMpi::recvVoid(const MPI::Intracomm &group, storage::IVoidPartition &part, int source, int tag, const MsgOpt &o){
    auto buffer = std::make_shared<transport::IMemoryBuffer>(part.bytes());
    int sz;

    group.Recv(&sz, 1, MPI::INT, source, tag);
    group.Recv(buffer->getWritePtr(sz), sz, MPI::BYTE, source, tag);
    buffer->wroteBytes(sz);
    part.read((std::shared_ptr<transport::ITransport> &) buffer);
}
