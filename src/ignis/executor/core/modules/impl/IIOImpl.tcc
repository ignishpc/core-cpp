
#include "IIOImpl.h"
#include "ignis/executor/core/io/IJsonReader.h"
#include "ignis/executor/core/io/IJsonWriter.h"
#include "ignis/executor/core/io/IPrinter.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/transport/IZlibTransport.h"
#include <climits>
#include <fstream>

#define IIOImplClass ignis::executor::core::modules::impl::IIOImpl

template<typename Tp>
int64_t IIOImplClass::partitionApproxSize() {
    IGNIS_TRY()
    auto group = executor_data->getPartitions<Tp>();
    if (group->partitions() == 0) { return 0; }

    int64_t size = 0;
    if (executor_data->getPartitionTools().isMemory(*group)) {
        for (auto &partition : *group) { size += partition->size(); }

        if (executor_data->mpi().isContiguousType<Tp>()) {
            size *= sizeof(Tp);
        } else {
            size *= sizeof(Tp) + executor_data->getProperties().transportElemSize();
        }
    } else {
        for (auto &partition : *group) { size += partition->bytes(); }
    }

    return size;
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::partitionObjectFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "IO: reading partitions object file";
    auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(partitions);

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(ioCores())
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < partitions; p++) {
            std::string file_name;
#pragma omp critical
            {
                auto file_name = partitionFileName(path, first + p);
                openFileRead(path);//Only to check
            };
            storage::IDiskPartition<Tp> open(file_name, 0, true, true);
            (*group)[p]->copyFrom(open);
            (*group)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(group);
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::partitionJsonFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "IO: reading partitions json file";
    auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(partitions);

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
            io::IJsonReader<api::IWriteIterator<Tp>> reader;
            auto write_iterator = (*group)[p]->writeIterator();
            reader(file, *write_iterator);
            (*group)[p]->fit();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    executor_data->setPartitions(group);
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::saveAsObjectFile(const std::string &path, int8_t compression, int64_t first) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "IO: saving as object file";
    auto group = executor_data->getAndDeletePartitions<Tp>();
    auto compression = executor_data->getProperties().ioCompression();
    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(ioCores())
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < group->partitions(); p++) {
            std::string file_name;
#pragma omp critical
            {
                file_name = partitionFileName(path, first + p);
                openFileWrite(path);//Only to check
            };

            storage::IDiskPartition<Tp> save(file_name, compression, true);
            (*group)[p]->copyTo(save);
            save.sync();
            (*group)[p].reset();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::saveAsTextFile(const std::string &path, int64_t first) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "IO: saving as text file";
    auto group = executor_data->getAndDeletePartitions<Tp>();
    bool isMemory = executor_data->getPartitionTools().isMemory(*group);

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(ioCores())
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < group->partitions(); p++) {
            std::ofstream file;
#pragma omp critical
            {
                auto file_name = partitionFileName(path, first + p);
                file = openFileWrite(file_name);
            };

            auto &part = *(*group)[p];
            if (isMemory) {
                auto &men_part = executor_data->getPartitionTools().toMemory(part);
                io::IPrinter<typename std::remove_reference<decltype(men_part.inner())>::type> printer;
                printer(file, men_part.inner());
            } else {
                io::IPrinter<api::IReadIterator<Tp>> printer;
                printer(file, *part.readIterator());
            }
            (*group)[p].reset();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::saveAsJsonFile(const std::string &path, int64_t first, bool pretty) {
    IGNIS_TRY()
    IGNIS_LOG(info) << "IO: saving as json file";
    auto group = executor_data->getAndDeletePartitions<Tp>();

    IGNIS_OMP_EXCEPTION_INIT()
#pragma omp parallel num_threads(ioCores())
    {
        IGNIS_OMP_TRY()
#pragma omp for schedule(dynamic)
        for (int64_t p = 0; p < group->partitions(); p++) {
            std::ofstream file;
#pragma omp critical
            {
                auto file_name = partitionFileName(path, first + p);
                file = openFileWrite(file_name);
            };
            auto &part = *(*group)[p];
            io::IJsonWriter<api::IReadIterator<Tp>> writer;
            writer(file, *part.readIterator(), pretty);
            (*group)[p].reset();
        }
        IGNIS_OMP_CATCH()
    }
    IGNIS_OMP_EXCEPTION_END()

    auto header = path + "/json";
    auto file = openFileWrite(header);
    storage::IDiskPartition<Tp> st(header, 0, true);
    st.sync();

    IGNIS_CATCH()
}

#undef IIOImplClass
