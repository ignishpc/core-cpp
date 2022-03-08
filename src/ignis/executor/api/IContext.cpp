
#include "IContext.h"
#include <omp.h>
#include <ignis/executor/core/exception/IInvalidArgument.h>

using namespace ignis::executor::api;

IContext::IContext() : mpi_thread_group(1, MPI::COMM_WORLD) {}

IContext::~IContext() {}

int IContext::threads() { return omp_get_max_threads(); }

int IContext::executors() { return mpiGroup().Get_size(); }

int IContext::executorId() { return mpiGroup().Get_rank(); }

int IContext::threadId() { return omp_get_thread_num(); }

void IContext::clearVars(){
    variables.clear();
}

const MPI::Intracomm &IContext::mpiGroup() {
    if(mpi_thread_group.size() == 1){
        return mpi_thread_group[0];
    }
    auto id = threadId();

    if(id < mpi_thread_group.size()){
        return mpi_thread_group[id];
    }

    throw core::exception::IInvalidArgument("Requested more transport cores than available");
}

std::unordered_map<std::string, std::string> &IContext::props() { return properties; }

bool IContext::isVar(const std::string &name) { return variables.find(name) != variables.end(); }

bool IContext::rmVar(const std::string &name) {
    auto it = variables.find(name);
    if (it == variables.end()) { return false; }
    variables.erase(it);
    return true;
}
