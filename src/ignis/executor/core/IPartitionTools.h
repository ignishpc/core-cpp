
#ifndef IGNIS_IPARTITIONTOOL_H
#define IGNIS_IPARTITIONTOOL_H

#include "IPropertyParser.h"
#include "ignis/executor/api/IContext.h"
#include "ignis/executor/core/ILog.h"
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include "ignis/executor/core/storage/IDiskPartition.h"
#include "ignis/executor/core/storage/IMemoryPartition.h"
#include "ignis/executor/core/storage/IRawMemoryPartition.h"
#include "ignis/executor/core/storage/IVoidPartition.h"

namespace ignis {
    namespace executor {
        namespace core {
            class IPartitionTools {
            public:
                IPartitionTools(IPropertyParser &properties, api::IContext &context);

                template<typename Tp>
                inline std::shared_ptr<storage::IPartition<Tp>> newPartition();

                template<typename Tp>
                inline std::shared_ptr<storage::IPartition<Tp>> newPartition(const std::string &type);

                template<typename Tp>
                inline std::shared_ptr<storage::IPartition<Tp>> newPartition(storage::IPartition<Tp> &part);

                template<typename Tp>
                inline std::shared_ptr<storage::IPartitionGroup<Tp>> newPartitionGroup(int64_t partitions = 0);

                template<typename Tp>
                inline std::shared_ptr<storage::IPartitionGroup<Tp>>
                newPartitionGroup(storage::IPartitionGroup<Tp> &partitions);

                template<typename Tp>
                inline std::shared_ptr<storage::IMemoryPartition<Tp>> newMemoryPartition(int64_t elems = 1000);

                template<typename Tp>
                inline std::shared_ptr<storage::IRawMemoryPartition<Tp>> newRawMemoryPartition(int64_t sz = 120 * 1024 *
                                                                                                            1024);

                template<typename Tp>
                inline std::shared_ptr<storage::IDiskPartition<Tp>>
                newDiskPartition(const std::string &name = "", bool persist = false, bool read = false);

                template<typename Tp>
                inline std::shared_ptr<storage::IDiskPartition<Tp>>
                copyDiskPartition(const std::string &path, const std::string &name = "", bool persist = false);

                std::shared_ptr<storage::IVoidPartition> newVoidPartition(int64_t sz = 120 * 1024 * 1024);

                template<typename Tp>
                inline bool isMemory(storage::IPartitionGroup<Tp> &group);

                template<typename Tp>
                inline bool isMemory(storage::IPartition<Tp> &part);

                template<typename Tp>
                inline bool isRawMemory(storage::IPartitionGroup<Tp> &group);

                template<typename Tp>
                inline bool isRawMemory(storage::IPartition<Tp> &part);

                template<typename Tp>
                inline bool isDisk(storage::IPartitionGroup<Tp> &group);

                template<typename Tp>
                inline bool isDisk(storage::IPartition<Tp> &part);

                template<typename Tp>
                inline storage::IMemoryPartition<Tp> &toMemory(storage::IPartition<Tp> &st);

                template<typename Tp>
                inline storage::IMemoryReadIterator<Tp> &toMemory(api::IReadIterator<Tp> &it);

                template<typename Tp>
                inline storage::IMemoryWriteIterator<Tp> &toMemory(api::IWriteIterator<Tp> &it);

                template<typename Tp>
                inline std::shared_ptr<storage::IMemoryPartition<Tp>> &
                toMemory(std::shared_ptr<storage::IPartition<Tp>> &st);

                template<typename Tp>
                inline std::shared_ptr<storage::IMemoryReadIterator<Tp>> &
                toMemory(std::shared_ptr<api::IReadIterator<Tp>> &it);

                template<typename Tp>
                inline std::shared_ptr<storage::IMemoryWriteIterator<Tp>> &
                toMemory(std::shared_ptr<api::IWriteIterator<Tp>> &it);

                template<typename Tp>
                inline storage::IRawMemoryPartition<Tp> &toRawMemory(storage::IPartition<Tp> &st);

                template<typename Tp>
                inline std::shared_ptr<storage::IRawMemoryPartition<Tp>> &
                toRawMemory(std::shared_ptr<storage::IPartition<Tp>> &st);

                template<typename Tp>
                inline storage::IDiskPartition<Tp> &toDisk(storage::IPartition<Tp> &st);

                template<typename Tp>
                inline std::shared_ptr<storage::IDiskPartition<Tp>> &
                toDisk(std::shared_ptr<storage::IPartition<Tp>> &st);

                void createDirectoryIfNotExists(const std::string &path);

            private:
                inline std::string diskPath(const std::string &name = "");

                bool createHardLink(const std::string &target, const std::string &link);

                bool copyFile(const std::string &from, const std::string &to);

                IPropertyParser &properties;
                api::IContext &context;
                int64_t partition_id_gen;
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis

#include "IPartitionTools.tcc"

#endif
