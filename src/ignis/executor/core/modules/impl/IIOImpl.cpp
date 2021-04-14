
#include "IIOImpl.h"
#include "ignis/executor/api/IJsonValue.h"
#include "ignis/executor/core/storage/IVoidPartition.h"
#include <algorithm>
#include <fstream>
#include <ghc/filesystem.hpp>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>

using namespace ignis::executor::core::modules::impl;
using namespace ignis::executor::core::storage;
using ignis::executor::api::IJsonValue;

IIOImpl::IIOImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

IIOImpl::~IIOImpl() {}

std::string IIOImpl::partitionFileName(const std::string &path, int64_t index) {
    if (!ghc::filesystem::is_directory(path)) {
        std::error_code error;
        ghc::filesystem::create_directories(path, error);
        if (!ghc::filesystem::is_directory(path)) {
            throw exception::IInvalidArgument("Unable to create directory " + path + " " + error.message());
        }
    }

    auto str_index = std::to_string(index);
    auto zeros = std::max(6 - (int) str_index.size(), 0);
    return path + "/part" + std::string(zeros, '0') + str_index;
}

std::ifstream IIOImpl::openFileRead(const std::string &path) {
    IGNIS_LOG(info) << "IO: opening file " << path;
    if (!ghc::filesystem::exists(path)) { throw exception::IInvalidArgument(path + " was not found"); }

    std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
    if (!file.good()) { throw exception::IInvalidArgument(path + " cannot be opened"); }
    IGNIS_LOG(info) << "IO: file opening successful";
    return file;
}

std::ofstream IIOImpl::openFileWrite(const std::string &path) {
    IGNIS_LOG(info) << "IO: creating file " << path;
    if (ghc::filesystem::exists(path)) {
        if (executor_data->getProperties().ioOverwrite()) {
            IGNIS_LOG(warning) << "IO: " << path << " already exists";
            if (!ghc::filesystem::remove(path)) { throw exception::ILogicError(path + " can not be removed"); }
        } else {
            throw exception::IInvalidArgument(path + " already exists");
        }
    }
    std::ofstream file(path, std::ifstream::out | std::ifstream::binary | std::fstream::trunc);
    if (!file.good()) { throw exception::IInvalidArgument(path + " cannot be opened"); }
    IGNIS_LOG(info) << "IO: file created successful";
    return file;
}

void IIOImpl::textFile(const std::string &path, int64_t minPartitions) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "IO: reading text file";
    auto size = ghc::filesystem::file_size(path);
    IGNIS_LOG(info) << "IO: file has " << size << " Bytes";
    auto result = executor_data->getPartitionTools().newPartitionGroup<std::string>();
    decltype(result) thread_groups[ioCores()];
    size_t total_bytes = 0;
    size_t elements = 0;

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel reduction(+ : total_bytes, elements) firstprivate(minPartitions) num_threads(ioCores())
    {
        IGNIS_OMP_TRY()
        std::ifstream file;
#pragma omp critical
        { file = openFileRead(path); };
        auto id = executor_data->getContext().threadId();
        auto globalThreadId = executor_data->getContext().executorId() * executor_data->getCores() + id;
        auto threads = executor_data->getContext().executors() * executor_data->getCores();
        size_t ex_chunk = size / threads;
        size_t ex_chunk_init = globalThreadId * ex_chunk;
        size_t ex_chunk_end = ex_chunk_init + ex_chunk;
        size_t minPartitionSize = executor_data->getProperties().partitionMinimal();
        minPartitions = (int64_t) std::ceil(minPartitions / (float) threads);

        if (globalThreadId > 0) {
            file.seekg(ex_chunk_init > 0 ? ex_chunk_init - 1 : ex_chunk_init);
            int value;
            while ((value = file.get()) != '\n' && value != EOF) {}
            ex_chunk_init = file.tellg();
            if (globalThreadId == threads - 1) { ex_chunk_end = size; }
        }

        if (ex_chunk / minPartitionSize < minPartitions) { minPartitionSize = ex_chunk / minPartitions; }

        thread_groups[id] = executor_data->getPartitionTools().newPartitionGroup<std::string>();
        auto partition = executor_data->getPartitionTools().newPartition<std::string>();
        auto write_iterator = partition->writeIterator();
        thread_groups[id]->add(partition);
        size_t partitionInit = ex_chunk_init;
        size_t filepos = ex_chunk_init;
        std::string buffer;

        if (executor_data->getPartitionTools().isMemory(*partition)) {
            auto part_men = executor_data->getPartitionTools().toMemory(partition);
            while (filepos < ex_chunk_end) {
                if ((filepos - partitionInit) > minPartitionSize) {
                    part_men->fit();
                    part_men = executor_data->getPartitionTools().newMemoryPartition<std::string>();
                    thread_groups[id]->add(part_men);
                    partitionInit = filepos;
                }
                std::getline(file, buffer, '\n');
                filepos += buffer.size() + 1;
                elements++;
                part_men->inner().push_back(buffer);
            }
        } else {
            while (filepos < ex_chunk_end) {
                if ((filepos - partitionInit) > minPartitionSize) {
                    partition->fit();
                    partition = executor_data->getPartitionTools().newPartition<std::string>();
                    write_iterator = partition->writeIterator();
                    thread_groups[id]->add(partition);
                    partitionInit = filepos;
                }
                std::getline(file, buffer, '\n');
                filepos += buffer.size() + 1;
                elements++;
                write_iterator->write(buffer);
            }
        }

        total_bytes += (size_t) file.tellg() - ex_chunk_init;

        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    for (auto group : thread_groups) {
        for (auto part : *group) { result->add(part); }
    }

    IGNIS_LOG(info) << "IO: created  " << result->partitions() << " partitions, " << elements << " lines and "
                    << total_bytes << " Bytes read ";

    executor_data->setPartitions(result);
    IGNIS_CATCH()
}

void IIOImpl::partitionTextFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "IO: reading partitions text file";
    auto group = executor_data->getPartitionTools().newPartitionGroup<std::string>(partitions);

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(ioCores())
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < partitions; p++) {
            std::ifstream file;
#pragma omp critical
            {
                auto file_name = partitionFileName(path, first + p);
                file = openFileRead(path);
            };

            auto partition = (*group)[p];
            auto write_iterator = partition->writeIterator();
            std::string buffer;
            while (!file.eof()) {
                std::getline(file, buffer, '\n');
                write_iterator->write(buffer);
            }
            partition->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(group);
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

        auto partition = executor_data->getPartitionTools().newVoidPartition(ghc::filesystem::file_size(file_name));
        partition->read(reinterpret_cast<std::shared_ptr<transport::ITransport> &>(transport));

        partition->fit();
        group->add(partition);
    }

    executor_data->setPartitions(group);
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
        return true;
    }

    bool EndArray(rapidjson::SizeType memberCount) {
        stack.resize(stack.size() - 1);
        top = stack.back();
        return true;
    }

    IJsonValue root() { return rootArray.getArray()[0]; }

    PartitionJsonHandler() {
        rootArray = IJsonValue(std::vector<ignis::executor::api::IJsonValue>());
        stack.push_back(&rootArray);
    }

private:
    bool assign(IJsonValue &&value) {
        if (key.empty()) {
            const_cast<std::vector<IJsonValue> &>(stack[stack.size() - 1]->getArray()).push_back(value);
            top = &const_cast<std::vector<IJsonValue> &>(stack[stack.size() - 1]->getArray()).back();
        } else {
            top = &(const_cast<std::unordered_map<std::string, IJsonValue> &>(
                            stack[stack.size() - 1]->getMap())[std::move(key)] = value);
        }
        return true;
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

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(ioCores())
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < partitions; p++) {
            std::ifstream file;
#pragma omp critical
            {
                auto file_name = partitionFileName(path, first + p);
                file = openFileRead(path);
            };
            rapidjson::IStreamWrapper wrapper(file);
            rapidjson::Reader reader;
            PartitionJsonHandler handler;

            if (!reader.Parse<rapidjson::kParseIterativeFlag>(wrapper, handler)) {
                std::string error = rapidjson::GetParseError_En(reader.GetParseErrorCode());

                throw exception::IInvalidArgument(path + " is not valid. " + error + " at offset " +
                                                  std::to_string(reader.GetErrorOffset()));
            }
            if (!handler.root().isArray()) { throw exception::IInvalidArgument(path + " is not a json array"); }

            auto write_iterator = (*group)[p]->writeIterator();
            for (const api::IJsonValue &value : handler.root().getArray()) {
                api::IJsonValue &noconst = const_cast<api::IJsonValue &>(value);
                write_iterator->write(std::move(noconst));
            }
            (*group)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(group);
    IGNIS_CATCH()
}

int IIOImpl::ioCores() {
    double cores = executor_data->getProperties().ioCores();
    if (cores > 1) {
        return std::min(executor_data->getCores(), (int) std::ceil(executor_data->getProperties().ioCores()));
    }
    return std::max(1, (int) std::ceil(executor_data->getProperties().ioCores() * executor_data->getCores()));
}