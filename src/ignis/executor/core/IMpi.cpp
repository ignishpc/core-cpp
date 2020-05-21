
#include "IMpi.h"
#include <cstring>

using namespace ignis::executor::core;

IMpi::IMpi(IPropertyParser &properties, IPartitionTools &partition_tools, const MPI::Intracomm &comm) :
        properties(properties), partition_tools(partition_tools), comm(comm) {}

bool IMpi::isRoot(int root) { return comm.Get_rank() == root; }

int IMpi::rank() { return comm.Get_rank(); }

int IMpi::executors() { return comm.Get_size(); }

std::vector<int> IMpi::displs(const std::vector<int> &sz) {
    std::vector<int> d;//Last value is not used by mpi but is used as total size
    d.reserve(sz.size() + 1);
    d.push_back(0);
    for (int i = 0; i < sz.size(); i++) {
        d.push_back(sz[i] + d[i]);
    }
    return std::move(d);
}

const MPI::Intracomm &IMpi::native() {
    return comm;
}

std::vector<int> IMpi::szVector(const std::vector<std::pair<int, int>> &elems_szv) {
    std::vector<int> szv;
    szv.reserve(elems_szv.size());
    for (int i = 0; i < elems_szv.size(); i++) {
        szv.push_back(elems_szv[i].second);
    }
    return std::move(szv);
}

int IMpi::sumElems(const std::vector<std::pair<int, int>> &elems_szv) {
    int n = 0;
    for (int i = 0; i < elems_szv.size(); i++) {
        n += elems_szv[i].first;
    }
    return n;
}

void IMpi::move(void *begin, size_t n, size_t displ) {
    if (displ > 0) {
        std::memcpy(((char *) begin) + displ, begin, n);
    }
}

void IMpi::barrier() { comm.Barrier(); }

void IMpi::driverScatterVoid(const MPI::Intracomm &group,
                             storage::IPartitionGroup<storage::IVoidPartition::VOID_TYPE> &part_group,
                             int64_t partitions) {
    auto id = rank();
    bool exec0 = group.Get_rank() == 1;
    auto execs = executors();
    auto max_exec_partitions = (int) std::ceil(partitions / (float) execs);
    bool same_protocol;
    uint8_t *ptr = nullptr;
    auto buffer = std::make_shared<transport::IMemoryBuffer>();

    if (exec0) {
        int8_t protocol;
        group.Recv(&protocol, 1, MPI::BYTE, 1, 0);
        same_protocol = protocol == core::protocol::IObjectProtocol::IGNIS_PROTOCOL;//force data serializtion
        group.Send(&same_protocol, 1, MPI::BOOL, 1, 0);
    }

    IGNIS_LOG(info) << "Comm: driverScatterVoid partitions: " << part_group.partitions();

    auto cmp = properties.msgCompression();
    auto zlib = std::make_shared<transport::IZlibTransport>(buffer, cmp);
    protocol::IObjectProtocol proto(zlib);
    std::vector<int> szv_part;
    szv_part.resize(max_exec_partitions);

    int sz;
    group.Scatter(nullptr, 0, MPI::INT, &sz, 1, MPI::INT, 0);
    group.Scatter(nullptr, 0, MPI::INT, &szv_part[0], szv_part.size(), MPI::INT, 0);
    group.Scatterv(nullptr, nullptr, nullptr, MPI::BYTE, ptr = buffer->getWritePtr(sz), sz, MPI::BYTE, 0);

    int64_t offset = 0;
    for (int i = 0; i < szv_part.size(); i++) {
        auto view = std::make_shared<transport::IMemoryBuffer>(ptr + offset, szv_part[i]);
        offset += szv_part[i];
        auto part = std::make_shared<storage::IVoidPartition>(szv_part[i]);
        part->read((std::shared_ptr<transport::ITransport> &) view);
        part_group.add(part);
    }


}

