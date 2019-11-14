
#include "IIOImpl.h"
#include <boost/filesystem.hpp>
#include <fstream>

using namespace ignis::executor::core::modules::impl;

IIOImpl::IIOImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

IIOImpl::~IIOImpl() {}

void IIOImpl::textFile(const std::string &path, int64_t minPartitions) {
    try {
        IGNIS_LOG(info) << "IO: opening file";
        if (!boost::filesystem::exists(path)) {
            throw exception::IInvalidArgument(path + " was not found");
        }

        std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
        if (!file.good()) {
            throw exception::IInvalidArgument(path + " cannot be opened");
        }
        IGNIS_LOG(info) << "IO: file opening successful";

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
    } catch (std::exception &ex) {
        throw exception::IException(ex);
    }
}
