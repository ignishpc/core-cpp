
#include "IExecutorData.h"
#include "selector/ISelector.h"
#include <fstream>
#include <ghc/filesystem.hpp>
#include <omp.h>

using namespace ignis::executor::core;

IExecutorData::IExecutorData()
    : properties(context.props()), partition_tools(properties, context), _mpi(properties, partition_tools, context) {}

bool IExecutorData::hasPartitions() { return (bool) partitions; }

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

void IExecutorData::setCores(int cores) { omp_set_num_threads(this->cores = cores); }

int IExecutorData::getCores() { return this->cores; }

int IExecutorData::getMpiCores() { return context.mpi_thread_group.size(); }

void IExecutorData::enableMpiCores() {
    double ratio = properties.transportCores();
    int64_t mpiCores;
    if (ratio > 1) {
        mpiCores = std::min(cores, (int) std::ceil(ratio));
    } else {
        mpiCores = std::ceil(cores * ratio);
    }

    if (mpiCores > 1 && context.mpi_thread_group.size() == 1 && context.executors() > 1) {
        context.mpi_thread_group[0].Set_name("ignis_thread_0");
        IGNIS_LOG(info) << "Duplicating mpi group for " << mpiCores << " threads";
        for (int i = 1; i < mpiCores; i++) {
            context.mpi_thread_group.push_back(context.mpi_thread_group[i - 1].Dup());
            context.mpi_thread_group.back().Set_name(("ignis_thread_" + std::to_string(i)).c_str());
            IGNIS_LOG(info) << "mpi group " << context.mpi_thread_group.size() << " ready";
        }
    }
}

void IExecutorData::setMpiGroup(const MPI::Intracomm &group) {
    for (int i = 1; i < context.mpi_thread_group.size(); i++) { context.mpi_thread_group[i].Free(); }
    context.mpi_thread_group.clear();
    context.mpi_thread_group.push_back(group);
}

std::shared_ptr<selector::ISelectorGroup> IExecutorData::loadLibrary(const rpc::ISource &source, bool withBackup) {
    IGNIS_LOG(info) << "Loading function";
    if (source.obj.__isset.bytes) { throw exception::IInvalidArgument("C++ not support function serialization"); }
    auto lib = library_loader.load<selector::ISelectorGroup>(source.obj.name);
    for (auto &tp : lib->args) {
        if (types.find(tp.first) == types.end()) { types[tp.first] = std::make_pair(tp.second, lib); }
    }

    if (source.params.size() > 0) {
        IGNIS_LOG(info) << "Loading user variables";
        loadParameters(source);
    }
    IGNIS_LOG(info) << "Function loaded";
    if (withBackup) {
        std::ofstream backup(infoDirectory() + "/sources" + std::to_string(context.executorId()) + ".bak",
                             std::ios::app);
        backup << source.obj.name << "\n";
    }
    return lib;
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
                if (loaded.find(source.obj.name) == loaded.end()) {
                    loadLibrary(source, false);
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

bool IExecutorData::hasVariable(const std::string key) { return variables.find(key) != variables.end(); }

void IExecutorData::removeVariable(const std::string key) {
    auto entry = variables.find(key);
    if (entry != variables.end()) { variables.erase(entry); }
}

IExecutorData::~IExecutorData() {}
