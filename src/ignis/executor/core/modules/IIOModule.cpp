
#include "IIOModule.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/executor/core/selector/ISelector.h"
#include <fstream>

using namespace ignis::executor::core::modules;

IIOModule::IIOModule(std::shared_ptr<IExecutorData> &executor_data) : IModule(executor_data), impl(executor_data) {}

IIOModule::~IIOModule() {}

void IIOModule::loadClass(const ::ignis::rpc::ISource &src) {
    IGNIS_RPC_TRY()
    executor_data->loadLibrary(src)->loadClass(executor_data->getContext());
    IGNIS_RPC_CATCH()
}

void IIOModule::loadLibrary(const std::string &path) {
    IGNIS_RPC_TRY()
    rpc::ISource src;
    for (auto &f : executor_data->loadLibraryFunctions(path)) {
        src.obj.name = f;
        executor_data->loadLibrary(src, true, true);
    }
    IGNIS_RPC_CATCH()
}

int64_t IIOModule::partitionCount() {
    IGNIS_RPC_TRY()
    return executor_data->getPartitions<char>(true)->partitions();
    IGNIS_RPC_CATCH()
}

void IIOModule::countByPartition(std::vector<int64_t> &_return) {
    IGNIS_RPC_TRY()
    for (auto &part : *(executor_data->getPartitions<char>(true))) { _return.push_back(part->size()); }
    IGNIS_RPC_CATCH()
}

int64_t IIOModule::partitionApproxSize() {
    IGNIS_RPC_TRY()
    return typeFromPartition()->partitionApproxSize(impl);
    IGNIS_RPC_CATCH()
}

void IIOModule::plainFile(const std::string& path, const std::string& delim) {
    IGNIS_RPC_TRY()
    impl.plainFile(path, 1, delim);
    IGNIS_RPC_CATCH()
}

void IIOModule::plainFile3(const std::string& path, const int64_t minPartitions, const std::string& delim) {
    IGNIS_RPC_TRY()
    impl.plainFile(path, minPartitions, delim);
    IGNIS_RPC_CATCH()
}

void IIOModule::textFile(const std::string &path) {
    IGNIS_RPC_TRY()
    impl.textFile(path, 1);
    IGNIS_RPC_CATCH()
}

void IIOModule::textFile2(const std::string &path, const int64_t minPartitions) {
    IGNIS_RPC_TRY()
    impl.textFile(path, minPartitions);
    IGNIS_RPC_CATCH()
}

void IIOModule::partitionObjectFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_RPC_TRY()
    try {
        IGNIS_LOG(info) << "IO: reading header";
        auto file_name = impl.partitionFileName(path, first);
        impl.openFileRead(file_name);

        auto header_name = file_name + ".header";
        auto header_file = impl.openFileRead(header_name);

        std::string header(100, 0);
        header_file.read(const_cast<char *>(header.c_str()), 100);
        header.resize(header_file.gcount());

        auto file = std::make_shared<transport::IFileTransport>(file_name);
        auto merged = std::make_shared<transport::IHeaderTransport>(
                (std::shared_ptr<core::transport::ITransport> &) file, header);

        std::string type = dynamic_types.typeFromBytes((std::shared_ptr<core::transport::ITransport> &) merged);

        typeFromName(type)->partitionObjectFile(impl, path, first, partitions);
        return;
    } catch (exception::IException &ex) {
        IGNIS_LOG(warning) << "IO: Deserialization failed, avoiding for now: " << ex.what();
    }
    impl.partitionObjectFileVoid(path, first, partitions);

    IGNIS_RPC_CATCH()
}

void IIOModule::partitionObjectFile4(const std::string &path, const int64_t first, const int64_t partitions,
                                     const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->partitionObjectFile(impl, path, first, partitions);
    IGNIS_RPC_CATCH()
}

void IIOModule::partitionTextFile(const std::string &path, const int64_t first, const int64_t partitions) {
    IGNIS_RPC_TRY()
    impl.partitionTextFile(path, first, partitions);
    IGNIS_RPC_CATCH()
}

void IIOModule::partitionJsonFile4a(const std::string &path, const int64_t first, const int64_t partitions,
                                    const bool objectMapping) {
    IGNIS_RPC_TRY()
    IGNIS_LOG(info) << "IO: reading header";
    if (objectMapping) { IGNIS_LOG(warning) << "IO: ObjectMapping=true ignored without src param"; }
    impl.partitionJsonFileVoid(path, first, partitions);
    IGNIS_RPC_CATCH()
}

void IIOModule::partitionJsonFile4b(const std::string &path, const int64_t first, const int64_t partitions,
                                    const rpc::ISource &src) {
    IGNIS_RPC_TRY()
    typeFromSource(src)->partitionJsonFile(impl, path, first, partitions);
    IGNIS_RPC_CATCH()
}

void IIOModule::saveAsObjectFile(const std::string &path, const int8_t compression, const int64_t first) {
    IGNIS_RPC_TRY()
    typeFromPartition()->saveAsObjectFile(impl, path, compression, first);
    IGNIS_RPC_CATCH()
}

void IIOModule::saveAsTextFile(const std::string &path, const int64_t first) {
    IGNIS_RPC_TRY()
    typeFromPartition()->saveAsTextFile(impl, path, first);
    IGNIS_RPC_CATCH()
}

void IIOModule::saveAsJsonFile(const std::string &path, const int64_t first, const bool pretty) {
    IGNIS_RPC_TRY()
    typeFromPartition()->saveAsJsonFile(impl, path, first, pretty);
    IGNIS_RPC_CATCH()
}
