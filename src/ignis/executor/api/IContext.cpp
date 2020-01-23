
#include "IContext.h"
#include <omp.h>
#include "ignis/executor/core/ILibraryLoader.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"
#include "ignis/executor/core/transport/IZlibTransport.h"

using namespace ignis::executor::api;

IContext::IContext() : mpi_group(MPI::COMM_WORLD) {}

IContext::~IContext() {

}

int IContext::cores() { return omp_get_num_threads(); }

int IContext::executors() { return mpi_group.Get_size(); }

int IContext::executorId() { mpi_group.Get_rank(); }

int IContext::threadId(){ omp_get_thread_num();}

const MPI::Intracomm &IContext::mpiGroup() { return mpi_group; }

std::unordered_map<std::string, std::string> &IContext::props() {
    return properties;
}

std::shared_ptr<ignis::executor::core::protocol::IObjectProtocol> IContext::varProtocol(std::shared_ptr<void> &var) {
    std::string &bytes = *static_cast<std::string *>(var.get());
    char *cbytes = const_cast<char *>(bytes.c_str());
    auto buffer = std::make_shared<core::transport::IMemoryBuffer>((uint8_t *) cbytes, bytes.size());
    auto zlib = std::make_shared<core::transport::IZlibTransport>(buffer);
    return std::make_shared<core::protocol::IObjectProtocol>(zlib);
}

bool IContext::isVar(const std::string &name){
    return variables.find(name) != variables.end();
}

bool IContext::rmVar(const std::string &name) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        return false;
    }
    variables.erase(it);
    return true;
}



