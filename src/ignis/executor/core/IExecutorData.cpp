
#include "IExecutorData.h"
#include "selector/ISelector.h"
#include <omp.h>
#include <fstream>
#include <boost/filesystem.hpp>

using namespace ignis::executor::core;

IExecutorData::IExecutorData() : properties(context.props()),
                                 _mpi(properties, context.mpiGroup()),
                                 partition_tools(properties, context) {}

bool IExecutorData::hasPartitions() {
    return (bool) partitions;
}

void IExecutorData::deletePartitions() { partitions.reset(); }

int64_t IExecutorData::clearVariables() {
    context.variables.clear();
    int64_t n = variables.size();
    variables.clear();
    return n;
}

std::string IExecutorData::infoDirectory() {
    std::string info = properties.JobDirectory() + "/info";
    partition_tools.createDirectoryIfNotExists(info);
    return info;
}

IPropertyParser &IExecutorData::getProperties() {
    return properties;
}

IPartitionTools &IExecutorData::getPartitionTools() {
    return partition_tools;
}

IMpi IExecutorData::mpi() {
    return _mpi;
}

void IExecutorData::setCores(int cores) {
    omp_set_num_threads(cores);
}

std::shared_ptr<selector::ISelectorGroup> IExecutorData::loadLibrary(const rpc::ISource &source) {
    IGNIS_LOG(info) << "Loading function";
    if (source.obj.__isset.bytes) {
        throw exception::IInvalidArgument("C++ not support function serialization");
    }
    auto lib = library_loader.load<selector::ISelectorGroup>(source.obj.name);
    for (auto &tp: lib->args) {
        if (types.find(tp.first) == types.end()) {
            types[tp.first] = std::make_pair(tp.second, lib);
        }
    }

    if (source.params.size() > 0) {
        IGNIS_LOG(info) << "Loading user variables";
        loadParameters(source);
    }
    IGNIS_LOG(info) << "Function loaded";
    std::ofstream backup(infoDirectory() + "/sources" + std::to_string(context.executorId()) + ".bak", std::ios::app);
    backup << source.obj.name << "\n";
    return lib;
}

void IExecutorData::loadParameters(const rpc::ISource &source) {
    for (auto &entry: source.params) {
        context.variables[entry.first] = std::make_pair(false, std::make_shared<std::string>(std::move(entry.second)));
    }
}

void IExecutorData::reloadLibraries() {
    auto backup_path = infoDirectory() + "/sources" + std::to_string(context.executorId()) + ".bak";
    if (boost::filesystem::exists(backup_path)) {
        IGNIS_LOG(info) << "Function backup found, loading";
        std::ifstream backup(infoDirectory() + "/sources" + std::to_string(context.executorId()) + ".bak");
        rpc::ISource source;
        std::set<std::string> loaded;
        while (!backup.eof()) {
            std::getline(backup, source.obj.name, '\n');
            try {
                if (loaded.find(source.obj.name) == loaded.end()) {
                    loadLibrary(source);
                    loaded.insert(source.obj.name);
                }
            } catch (exception::IException &ex) {
                IGNIS_LOG(error) << ex.toString();
            } catch (std::exception &ex) {
                IGNIS_LOG(error) << ex.what();
            }
        }
    }
}

std::shared_ptr<selector::ITypeSelector> IExecutorData::getType(const std::string &id) {
    auto result = types.find(id);
    if (result != types.end()) {
        return result->second.first;
    }
    return std::shared_ptr<selector::ITypeSelector>();
}

ignis::executor::api::IContext &IExecutorData::getContext() {
    return context;
}

void IExecutorData::registerType(const std::shared_ptr<selector::ITypeSelector> &type) {
    types[type->info().getStandardName()] = std::make_pair(type, std::shared_ptr<selector::ISelectorGroup>());
}

bool IExecutorData::hasVariable(const std::string key) {
    return variables.find(key) != variables.end();
}

void IExecutorData::removeVariable(const std::string key) {
    auto entry = variables.find(key);
    if (entry != variables.end()) {
        variables.erase(entry);
    }
}

IExecutorData::~IExecutorData() {

}

