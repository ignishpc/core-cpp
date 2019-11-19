
#include "IIOImpl.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <algorithm>

using namespace ignis::executor::core::modules::impl;

IIOImpl::IIOImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

IIOImpl::~IIOImpl() {}

std::string IIOImpl::partitionFileName(const std::string &path, int64_t index) {
    auto str_index = std::to_string(index);
    auto zeros = std::max(5 - (int) str_index.size(), 0);
    auto name_path = path + ".part" + std::string(zeros, '0') + str_index;

}

std::ifstream IIOImpl::openFileRead(const std::string &path){
    IGNIS_LOG(info) << "IO: opening file " << path;
    if (!boost::filesystem::exists(path)) {
        throw exception::IInvalidArgument(path + " was not found");
    }

    std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
    if (!file.good()) {
        throw exception::IInvalidArgument(path + " cannot be opened");
    }
    IGNIS_LOG(info) << "IO: file opening successful";
    return std::move(file);
}

std::ofstream IIOImpl::openFileWrite(const std::string &path){
    IGNIS_LOG(info) << "IO: opening file " << path;
    if (boost::filesystem::exists(path)) {
        if (executor_data->getProperties().ioOverwrite()) {
            IGNIS_LOG(warning) << "IO: " << path << " already exists";
            if (!boost::filesystem::remove(path)) {
                throw exception::ILogicError(path + " can not be removed");
            }
        } else {
            throw exception::IInvalidArgument(path + " already exists");
        }
    }
    std::ofstream file(path, std::ifstream::out | std::ifstream::binary | std::fstream::trunc);
    if (!file.good()) {
        throw exception::IInvalidArgument(path + " cannot be opened");
    }
    IGNIS_LOG(info) << "IO: file opening successful";
    return std::move(file);
}

void IIOImpl::textFile(const std::string &path, int64_t minPartitions) {
    IGNIS_TRY()
        auto file = openFileRead(path);
        auto size = boost::filesystem::file_size(path);
        auto executorId = executor_data->getContext().executorId();
        auto executors = executor_data->getContext().executors();;
        size_t file_chunk = size / executors;
        size_t ex_chunk_init = executorId * file_chunk;
        size_t ex_chunk_end = ex_chunk_init + file_chunk;
        size_t minPartitionSize = executor_data->getProperties().minPartitionSize();


        IGNIS_LOG(info) << "IO: file has " << size << " Bytes";

        if (executorId != 0) {
            file.seekg(ex_chunk_init);
            while (file.get() != '\n');
            ex_chunk_init = file.tellg();
        }

        if (file_chunk / minPartitionSize < minPartitions) {
            minPartitionSize = file_chunk / minPartitions;
        }

        auto partitionGroup = newPartitionGroup<std::string>();
        auto partition = newPartition<std::string>();
        auto write_iterator = partition->writeIterator();
        partitionGroup->add(partition);
        size_t partitionInit = ex_chunk_init;
        size_t elements = 0;
        std::string buffer;

        while (file.tellg() < ex_chunk_end || (executorId == executors - 1)) {
            if (!std::getline(file, buffer, '\n')) {
                break;
            }
            elements++;
            write_iterator->write(buffer);
            if (((size_t) file.tellg() - partitionInit) > minPartitionSize) {
                partition = newPartition<std::string>();
                write_iterator = partition->writeIterator();
                partitionGroup->add(partition);
                partitionInit = file.tellg();
            }
        }
        ex_chunk_end = file.tellg();
        IGNIS_LOG(info) << "IO: created  " << partitionGroup->partitions() << " partitions, " <<
                        elements << " lines and " << ex_chunk_end - ex_chunk_init << " Bytes read ";

        executor_data->setPartitions(partitionGroup);
    IGNIS_CATCH()
}

void IIOImpl::openPartitionObjectFileUnknown(const std::string &path, int64_t first, int64_t partitions) {
    //TODO
}
