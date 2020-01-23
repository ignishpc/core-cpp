
#include "IMpi.h"
#include <cstring>

using namespace ignis::executor::core;

IMpi::IMpi(IPropertyParser &properties, const MPI::Intracomm &comm) : properties(properties), comm(comm) {}

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

