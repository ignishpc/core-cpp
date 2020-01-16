
#ifndef IGNIS_IPARTITIONTOOL_H
#define IGNIS_IPARTITIONTOOL_H

#include "IPropertyParser.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include "ignis/executor/core/storage/IMemoryPartition.h"
#include "ignis/executor/core/storage/IRawMemoryPartition.h"
#include "ignis/executor/core/storage/IDiskPartition.h"
#include "ignis/executor/core/storage/IVoidPartition.h"
#include "ignis/executor/api/IContext.h"

namespace ignis {
    namespace executor {
        namespace core {
            class IPartitionTools {
            public:
                IPartitionTools(IPropertyParser &properties, api::IContext &context);

                template<typename Tp>
                std::shared_ptr<storage::IPartition<Tp>> newPartition();

                template<typename Tp>
                std::shared_ptr<storage::IPartition<Tp>> newPartition(storage::IPartition<Tp> &part);

                template<typename Tp>
                std::shared_ptr<storage::IPartitionGroup<Tp>> newPartitionGroup(int partitions = 0);

                template<typename Tp>
                std::shared_ptr<storage::IPartitionGroup<Tp>>
                newPartitionGroup(storage::IPartitionGroup<Tp> &partitions);

                template<typename Tp>
                std::shared_ptr<storage::IMemoryPartition<Tp>> newMemoryPartition(int64_t elems = 1000);

                template<typename Tp>
                std::shared_ptr<storage::IRawMemoryPartition<Tp>> newRawMemoryPartition(int64_t sz = 120 * 1024 * 1024);

                template<typename Tp>
                std::shared_ptr<storage::IDiskPartition<Tp>>
                newDiskPartition(const std::string &name = "", bool persist = false, bool read = false);

                std::shared_ptr<storage::IVoidPartition> newVoidPartition(int64_t sz = 120 * 1024 * 1024);

                template<typename Tp>
                bool isMemory(storage::IPartitionGroup<Tp> &group);

                template<typename Tp>
                bool isMemory(storage::IPartition<Tp> &part);

                template<typename Tp>
                bool isRawMemory(storage::IPartitionGroup<Tp> &group);

                template<typename Tp>
                bool isRawMemory(storage::IPartition<Tp> &part);

                template<typename Tp>
                bool isDisk(storage::IPartitionGroup<Tp> &group);

                template<typename Tp>
                bool isDisk(storage::IPartition<Tp> &part);

                template<typename Tp>
                storage::IMemoryPartition<Tp> &toMemory(storage::IPartition<Tp> &st);

                template<typename Tp>
                storage::IMemoryReadIterator<Tp> &toMemory(api::IReadIterator<Tp> &it);

                template<typename Tp>
                storage::IMemoryWriteIterator<Tp> &toMemory(api::IWriteIterator<Tp> &it);

                template<typename Tp>
                std::shared_ptr<storage::IMemoryPartition<Tp>> &toMemory(std::shared_ptr<storage::IPartition<Tp>> &st);

                template<typename Tp>
                std::shared_ptr<storage::IMemoryReadIterator<Tp> > &
                toMemory(std::shared_ptr<api::IReadIterator<Tp>> &it);

                template<typename Tp>
                std::shared_ptr<storage::IMemoryWriteIterator<Tp>> &
                toMemory(std::shared_ptr<api::IWriteIterator<Tp>> &it);

                void createDirectoryIfNotExists(const std::string &path);

            private:
                IPropertyParser &properties;
                api::IContext &context;
                int partition_id_gen;
            };
        }
    }
}

#include "IPartitionTools.tcc"

#endif
