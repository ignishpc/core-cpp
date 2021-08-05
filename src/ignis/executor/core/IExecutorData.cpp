#include "IExecutorData.h"
#include <fstream>
#include <ghc/filesystem.hpp>
#include <ignis/executor/core/selector/ISelector.h>
#include <omp.h>

using namespace ignis::executor::core;

IExecutorData::IExecutorData()
    : properties(context.props()), partition_tools(properties, context), _mpi(properties, partition_tools, context),
      library_loader(properties) {}

bool IExecutorData::hasPartitions() { return (bool) partitions; }

bool IExecutorData::hasVoidPartitions() {
    if (!hasPartitions()) { return true; }
    auto parts = getPartitions<storage::IVoidPartition::VOID_TYPE>(true);
    return parts->empty() || (*parts)[0]->type() == storage::IVoidPartition::TYPE;
}

void IExecutorData::deletePartitions() { partitions.reset(); }

int64_t IExecutorData::clearVariables() {
    context.variables.clear();
    int64_t n = variables.size();
    variables.clear();
    return n;
}

std::string IExecutorData::infoDirectory() {
    std::string info = properties.executorDirectory() + "/info";
    partition_tools.createDirectoryIfNotExists(info);
    return info;
}

IPropertyParser &IExecutorData::getProperties() { return properties; }

IPartitionTools &IExecutorData::getPartitionTools() { return partition_tools; }

IMpi &IExecutorData::mpi() { return _mpi; }

void IExecutorData::setCores(int cores) { omp_set_num_threads(cores); }

int IExecutorData::getCores() { return omp_get_max_threads(); }

int IExecutorData::getMpiCores() { return context.mpi_thread_group.size(); }

void IExecutorData::enableMpiCores() {
    double ratio = properties.transportCores();
    int64_t mpiCores;
    if (ratio > 1) {
        mpiCores = std::min(getCores(), (int) std::ceil(ratio));
    } else {
        mpiCores = std::ceil(getCores() * ratio);
    }

    if (mpiCores > 1 && context.mpi_thread_group.size() == 1 && context.executors() > 1) {
        context.mpi_thread_group = duplicate(context.mpi_thread_group[0], mpiCores);
    }
}

std::vector<MPI::Intracomm> IExecutorData::duplicate(const MPI::Intracomm &comm, int64_t threads) {
    std::vector<MPI::Intracomm> result;
    result.push_back(comm);
    if (threads == 1) { return result; };
    result.back().Set_name("ignis_thread_0");
    IGNIS_LOG(info) << "Duplicating mpi group for " << threads << " threads";
    for (int64_t i = 1; i < threads; i++) {
        result.push_back(result.back().Dup());
        result.back().Set_name(("ignis_thread_" + std::to_string(i)).c_str());
        IGNIS_LOG(info) << "mpi group " << i << " ready";
    }
    return result;
}

void IExecutorData::setMpiGroup(const MPI::Intracomm &group) {
    context.mpi_thread_group.clear();
    context.mpi_thread_group.push_back(group);
}

void IExecutorData::destroyMpiGroup() {
    for (auto comm : context.mpi_thread_group) {
        if (comm != MPI::COMM_WORLD) { comm.Free(); }
    }
    context.mpi_thread_group.clear();
    setMpiGroup(MPI::COMM_WORLD);
}

std::vector<std::string> IExecutorData::loadLibraryFunctions(const std::string &path) {
    return library_loader.loadLibrary(path);
}

std::shared_ptr<selector::ISelectorGroup> IExecutorData::loadLibrary(const rpc::ISource &source, bool withBackup,
                                                                     bool fast) {
    if (source.obj.__isset.bytes) { throw exception::IInvalidArgument("C++ not support function serialization"); }
    std::shared_ptr<selector::ISelectorGroup> group;
    int sep = source.obj.name.find(':');
    if (sep == std::string::npos) {
        if (functions.find(source.obj.name) == functions.end()) {
            throw exception::IInvalidArgument("Function " + source.obj.name + " not found");
        }
        group = functions[source.obj.name];
    } else {
        group = library_loader.loadFunction(source.obj.name);
        registerFunction(group);
        for (auto &tp : group->args) { registerType(tp); }

        if (withBackup) {
            std::ofstream backup(infoDirectory() + "/sources" + std::to_string(context.executorId()) + ".bak",
                                 std::ios::app);
            backup << source.obj.name << "\n";
        }
    }

    if (fast) { return group; }

    if (source.params.size() > 0) {
        IGNIS_LOG(info) << "Loading user variables";
        loadParameters(source);
    }
    IGNIS_LOG(info) << "Function loaded";
    return group;
}

void IExecutorData::loadParameters(const rpc::ISource &source) {
    for (auto &entry : source.params) {
        context.variables[entry.first] = std::make_shared<IBytesVariable>(std::move(entry.second));
    }
}

void IExecutorData::reloadLibraries() {
    auto backup_path = infoDirectory() + "/sources" + std::to_string(context.executorId()) + ".bak";
    if (ghc::filesystem::exists(backup_path)) {
        IGNIS_LOG(info) << "Function backup found, loading";
        std::ifstream backup(infoDirectory() + "/sources" + std::to_string(context.executorId()) + ".bak");
        rpc::ISource source;
        std::set<std::string> loaded;
        while (!backup.eof()) {
            std::getline(backup, source.obj.name, '\n');
            try {
                if (!source.obj.name.empty() && loaded.find(source.obj.name) == loaded.end()) {
                    loadLibrary(source, false, true);
                    loaded.insert(source.obj.name);
                }
            } catch (exception::IException &ex) { IGNIS_LOG(error) << ex.toString(); } catch (std::exception &ex) {
                IGNIS_LOG(error) << ex.what();
            }
        }
    }
}

std::shared_ptr<selector::ITypeSelector> IExecutorData::getType(const std::string &id) {
    auto result = types.find(id);
    if (result != types.end()) { return result->second.first; }
    return std::shared_ptr<selector::ITypeSelector>();
}

ignis::executor::api::IContext &IExecutorData::getContext() { return context; }

void IExecutorData::registerType(const std::shared_ptr<selector::ITypeSelector> &type) {
    types[type->info().getStandardName()] = std::make_pair(type, std::shared_ptr<selector::ISelectorGroup>());
}

void IExecutorData::registerFunction(const std::shared_ptr<selector::ISelectorGroup> &f) { functions[f->name] = f; }

bool IExecutorData::hasVariable(const std::string key) { return variables.find(key) != variables.end(); }

void IExecutorData::removeVariable(const std::string key) {
    auto entry = variables.find(key);
    if (entry != variables.end()) { variables.erase(entry); }
}

IExecutorData::~IExecutorData() {}
