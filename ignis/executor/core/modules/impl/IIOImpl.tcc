
#include "IIOImpl.h"
#include <fstream>
#include "executor/core/io/IPrinter.h"
#include "executor/core/io/IJsonPrinter.h"
#include "executor/core/transport/IZlibTransport.h"
#include "executor/core/protocol/IObjectProtocol.h"

#define IIOImplClass ignis::executor::core::modules::impl::IIOImpl

template<typename Tp>
void IIOImplClass::openPartitionObjectFile(const std::string &path, int64_t first, int64_t partitions) {
    IGNIS_TRY()
        auto group = newPartitionGroup<Tp>(partitions);

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
void IIOImplClass::saveAsPartitionObjectFile(const std::string &path, int8_t compression, int64_t first) {
    IGNIS_TRY()
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
        auto group = executor_data->getPartitions<Tp>();

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            auto file = openFileWrite(file_name);
            IGNIS_LOG(info) << "IO: saving text file " << file_name;
            auto &part = *(*group)[p];
            auto reader = part.readIterator();
            io::IPrinter<Tp> printer;
            for (int64_t i = 0; i < part.size(); i++) {
                printer(reader->next(), file);
            }
        }
    IGNIS_CATCH()
}

template<typename Tp>
void IIOImplClass::saveAsJsonFile(const std::string &path, int64_t first) {
    IGNIS_TRY()
        auto group = executor_data->getPartitions<Tp>();

        for (int64_t p = 0; p < group->partitions(); p++) {
            auto file_name = partitionFileName(path, first + p);
            auto file = openFileWrite(file_name);
            IGNIS_LOG(info) << "IO: saving json file " << file_name;
            auto &part = *(*group)[p];
            auto reader = part.readIterator();
            io::IJsonPrinter<Tp> printer;
            for (int64_t i = 0; i < part.size(); i++) {
                printer(reader->next(), file);
            }
        }
    IGNIS_CATCH()
}

#undef IIOImplClass
