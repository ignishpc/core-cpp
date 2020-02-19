
#include "IIOImpl.h"
#include <fstream>
#include <climits>
#include "ignis/executor/core/io/IPrinter.h"
#include "ignis/executor/core/io/IJsonWriter.h"
#include "ignis/executor/core/io/IJsonReader.h"
#include "ignis/executor/core/transport/IZlibTransport.h"
#include "ignis/executor/core/protocol/IObjectProtocol.h"

#define IIOImplClass ignis::executor::core::modules::impl::IIOImpl

template<typename Tp>
int64_t IIOImplClass::partitionApproxSize() {
    IGNIS_TRY()
        auto group = executor_data->getPartitions<Tp>();
        if (group->partitions() == 0) {
            return 0;
        }

        auto partitionType = (*group)[0]->type();
        int64_t size = 0;

        if (partitionType == storage::IMemoryPartition<Tp>::TYPE) {
            if (executor_data->mpi().isPrimitiveType<Tp>()) {
                for (auto &partition: *group) {
                    size += partition->size();
                }
                size *= sizeof(Tp);
            } else {
                return LONG_MAX; //UNKNOWN
            }
        } else {
            for (auto &partition: *group) {
                size += partition->bytes();
            }
        }

        return size;
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::partitionObjectFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: reading partitions object file";
        auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(partitions);

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            openFileRead(file_name);//Only to check
            storage::IDiskPartition<Tp> open(file_name, 0, true, true);
            (*group)[p]->copyFrom(open);
        }

        executor_data->setPartitions(group);
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::partitionJsonFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: reading partitions json file";
        auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(partitions);

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            auto file = openFileRead(file_name);
            io::IJsonReader<api::IWriteIterator<Tp>> reader;

            auto partition = executor_data->getPartitionTools().newPartition<Tp>();
            auto write_iterator = partition->writeIterator();

            reader(file, *write_iterator);
            group->add(partition);
        }

        executor_data->setPartitions(group);
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::saveAsObjectFile(const std::string &path, int8_t compression, int64_t first) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: saving as object file";
        auto group = executor_data->getPartitions<Tp>();

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            openFileWrite(file_name);//Only to check
            auto compression = executor_data->getProperties().ioCompression();

            IGNIS_LOG(info) << "IO: saving partition object file " << file_name;
            storage::IDiskPartition<Tp> save(file_name, compression, true);
            (*group)[p]->copyTo(save);
            save.sync();
        }
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::saveAsTextFile(const std::string &path, int64_t first) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: saving as text file";
        auto group = executor_data->getPartitions<Tp>();

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            auto file = openFileWrite(file_name);
            IGNIS_LOG(info) << "IO: saving text file " << file_name;
            auto &part = *(*group)[p];
            io::IPrinter<api::IReadIterator<Tp>> printer;
            printer(*part.readIterator(), file);
        }
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::saveAsJsonFile(const std::string &path, int64_t first, bool pretty) {
    IGNIS_TRY()
        IGNIS_LOG(info) << "IO: saving as json file";
        auto group = executor_data->getPartitions<Tp>();

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            auto file = openFileWrite(file_name + ".json");
            IGNIS_LOG(info) << "IO: saving json file " << file_name;
            auto &part = *(*group)[p];
            io::IJsonWriter<api::IReadIterator<Tp>> writer;
            writer(file, *part.readIterator(), pretty);
        }
        auto header = path + "/json";
        auto file = openFileWrite(header);
        storage::IDiskPartition<Tp> st(header, 0, true);
        st.sync();

    IGNIS_CATCH()
}

#undef IIOImplClass
