
#include "IIOModule.h"
#include <fstream>
#include "ignis/executor/core/selector/ISelector.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"

using namespace ignis::executor::core::modules;

IIOModule::IIOModule(std::shared_ptr<IExecutorData> &executor_data) : IModule(executor_data), impl(executor_data) {}

IIOModule::~IIOModule() {}

int64_t IIOModule::partitionCount() {
    IGNIS_RPC_TRY()
        return executor_data->getPartitions<char>(true)->partitions();
    IGNIS_RPC_CATCH()
}

int64_t IIOModule::partitionApproxSize() {
    IGNIS_RPC_TRY()
        return typeFromPartition()->partitionApproxSize(impl);
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
            auto header_name = impl.partitionFileName(path, first) + ".header";
            auto header_file = impl.openFileRead(header_name);

            std::string header(100, 0);
            header_file.read(const_cast<char *>(header.c_str()), 100);
            header.resize(header_file.gcount());

            auto type = typeFromHeader(header);
            typeFromHeader(header)->partitionObjectFile(impl, path, first, partitions);
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
        try {
            if (objectMapping) {
                IGNIS_LOG(info) << "IO: reading header";
                auto header_name = path + "/json.header";
                auto header_file = impl.openFileRead(header_name);

                std::string header(100, 0);
                header_file.read(const_cast<char *>(header.c_str()), 100);
                header.resize(header_file.gcount());

                auto type = typeFromHeader(header);
                typeFromHeader(header)->partitionJsonFile(impl, path, first, partitions);
                return;
            }
        } catch (exception::IException &ex) {
            IGNIS_LOG(warning) << "IO: ObjectMapping failed, avoiding: " << ex.what();
        }
        impl.partitionObjectFileVoid(path, first, partitions);
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
