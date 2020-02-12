
#include "IIOImpl.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <algorithm>
#include "ignis/executor/core/storage/IVoidPartition.h"
#include "ignis/executor/api/IJsonValue.h"
#include <rapidjson/error/en.h>
#include <rapidjson/reader.h>
#include <rapidjson/istreamwrapper.h>

using namespace ignis::executor::core::modules::impl;
using namespace ignis::executor::core::storage;
using ignis::executor::api::IJsonValue;

IIOImpl::IIOImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

IIOImpl::~IIOImpl() {}

std::string IIOImpl::partitionFileName(const std::string &path, int64_t index) {
    if (!boost::filesystem::is_directory(path)) {
        boost::system::error_code error;
        boost::filesystem::create_directories(path, error);
        if (!boost::filesystem::is_directory(path)) {
            throw exception::IInvalidArgument("Unable to create directory " + path + " " + error.message());
        }
    }

    auto str_index = std::to_string(index);
    auto zeros = std::max(6 - (int) str_index.size(), 0);
    auto name_path = path + "/part" + std::string(zeros, '0') + str_index;

}

std::ifstream IIOImpl::openFileRead(const std::string &path) {
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

std::ofstream IIOImpl::openFileWrite(const std::string &path) {
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
        IGNIS_LOG(info) << "IO: reading text file";
        auto file = openFileRead(path);
        auto size = boost::filesystem::file_size(path);
        auto executorId = executor_data->getContext().executorId();
        auto executors = executor_data->getContext().executors();
        size_t ex_chunk = size / executors;
        size_t ex_chunk_init = executorId * ex_chunk;
        size_t ex_chunk_end = ex_chunk_init + ex_chunk;
        size_t minPartitionSize = executor_data->getProperties().partitionMinimal();


        IGNIS_LOG(info) << "IO: file has " << size << " Bytes";

        if (executorId == executors - 1) {
            ex_chunk_end = size;
        } else {
            file.seekg(ex_chunk_init);
            int value;
            while ((value = file.get()) != '\n' && value != EOF);
            ex_chunk_init = file.tellg();
        }

        if (ex_chunk / minPartitionSize < minPartitions) {
            minPartitionSize = ex_chunk / minPartitions;
        }

        auto partitionGroup = executor_data->getPartitionTools().newPartitionGroup<std::string>();
        auto partition = executor_data->getPartitionTools().newPartition<std::string>();
        auto write_iterator = partition->writeIterator();
        partitionGroup->add(partition);
        size_t partitionInit = ex_chunk_init;
        size_t elements = 0;
        std::string buffer;

        while (file.tellg() < ex_chunk_end) {
            std::getline(file, buffer, '\n');
            if (((size_t) file.tellg() - partitionInit) > minPartitionSize) {
                partition = executor_data->getPartitionTools().newPartition<std::string>();
                write_iterator = partition->writeIterator();
                partitionGroup->add(partition);
                partitionInit = file.tellg();
            }
            elements++;
            write_iterator->write(buffer);
        }
        ex_chunk_end = file.tellg();
        IGNIS_LOG(info) << "IO: created  " << partitionGroup->partitions() << " partitions, " <<
                        elements << " lines and " << ex_chunk_end - ex_chunk_init << " Bytes read ";

        executor_data->setPartitions(partitionGroup);
    IGNIS_CATCH()
}

void IIOImpl::partitionTextFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: reading partitions text file";
        auto group = executor_data->getPartitionTools().newPartitionGroup<std::string>();

        for (int64_t p = 0; p < partitions; p++) {
            auto file_name = partitionFileName(path, first + p);
            auto file = openFileRead(file_name);

            auto partition = executor_data->getPartitionTools().newPartition<std::string>();
            auto write_iterator = partition->writeIterator();
            std::string buffer;
            while (!file.eof()) {
                std::getline(file, buffer, '\n');
                write_iterator->write(buffer);
            }
            group->add(partition);
        }
    IGNIS_CATCH()
}

void IIOImpl::partitionObjectFileVoid(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: reading partitions object file";
        auto group = executor_data->getPartitionTools().newPartitionGroup<IVoidPartition::VOID_TYPE>();

        for (int64_t p = 0; p < partitions; p++) {
            auto file_name = partitionFileName(path, first + p);
            openFileRead(file_name);//Only to check
            auto transport = std::make_shared<transport::IFileTransport>(file_name);

            auto partition = executor_data->getPartitionTools().newVoidPartition(
                    boost::filesystem::file_size(file_name));
            partition->read(reinterpret_cast<std::shared_ptr<transport::ITransport> &>(transport));

            group->add(partition);
        }
    IGNIS_CATCH()
}

class PartitionJsonHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, PartitionJsonHandler> {
public:

    bool Null() { return assign(IJsonValue()); }

    bool Bool(bool value) { return assign(IJsonValue(value)); }

    bool Int(int value) { return assign(IJsonValue((int64_t) value)); }

    bool Uint(unsigned value) { return assign(IJsonValue((int64_t) value)); }

    bool Int64(int64_t value) { return assign(IJsonValue(value)); }

    bool Uint64(uint64_t value) { return assign(IJsonValue((int64_t) value)); }

    bool Double(double value) { return assign(IJsonValue(value)); }

    bool RawNumber(const Ch *str, rapidjson::SizeType len, bool copy) { return assign(IJsonValue(std::string(str))); }

    bool String(const Ch *str, rapidjson::SizeType len, bool copy) { return assign(IJsonValue(std::string(str))); }

    bool Key(const Ch *str, rapidjson::SizeType len, bool copy) { return assign(IJsonValue(std::string(str))); }

    bool StartObject() {
        bool check = assign(IJsonValue(std::unordered_map<std::string, ignis::executor::api::IJsonValue>()));
        stack.push_back(top);
        return check;
    }

    bool StartArray() {
        bool check = assign(IJsonValue(std::vector<ignis::executor::api::IJsonValue>()));
        stack.push_back(top);
        return check;
    }

    bool EndObject(rapidjson::SizeType memberCount) {
        stack.resize(stack.size() - 1);
        top = stack.back();
    }

    bool EndArray(rapidjson::SizeType memberCount) {
        stack.resize(stack.size() - 1);
        top = stack.back();
    }

    IJsonValue root() { return rootArray.getArray()[0]; }

    PartitionJsonHandler() {
        rootArray = IJsonValue(std::vector<ignis::executor::api::IJsonValue>());
        stack.push_back(&rootArray);
    }

private:

    bool assign(IJsonValue &&value) {
        if (key.empty()) {
            const_cast<std::vector<IJsonValue> &>(stack[stack.size() - 1]->getArray()).push_back(std::move(value));
            top = &const_cast<std::vector<IJsonValue> &>(stack[stack.size() - 1]->getArray()).back();
        } else {
            top = &(const_cast<std::unordered_map<std::string, IJsonValue> &>(stack[stack.size() - 1]->getMap())
                    [std::move(key)] = std::move(value));
        }
    }

    std::string key;
    IJsonValue rootArray;
    IJsonValue *top;
    std::vector<IJsonValue *> stack;
};

void IIOImpl::partitionJsonFileVoid(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: reading partitions json file";
        auto group = executor_data->getPartitionTools().newPartitionGroup<api::IJsonValue>(partitions);

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            auto file = openFileRead(file_name);
            rapidjson::IStreamWrapper wrapper(file);
            rapidjson::Reader reader;
            PartitionJsonHandler handler;

            if (!reader.Parse<rapidjson::kParseIterativeFlag>(wrapper, handler)) {
                std::string error = rapidjson::GetParseError_En(reader.GetParseErrorCode());

                throw exception::IInvalidArgument(path + " is not valid. " + error
                                                  + " at offset " + std::to_string(reader.GetErrorOffset()));
            }
            if (!handler.root().isArray()) {
                throw exception::IInvalidArgument(path + " is not a json array");
            }

            auto write_iterator = (*group)[p]->writeIterator();
            for (const api::IJsonValue &value: handler.root().getArray()) {
                api::IJsonValue &noconst = const_cast<api::IJsonValue &>(value);
                write_iterator->write(std::move(noconst));
            }
        }

        executor_data->setPartitions(group);
    IGNIS_CATCH()
}

