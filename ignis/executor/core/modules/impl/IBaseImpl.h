
#ifndef IGNIS_IBASEIMPL_H
#define IGNIS_IBASEIMPL_H

#include <memory>
#include "executor/core/IMpi.h"
#include "executor/core/storage/IMemoryPartition.h"
#include "executor/core/storage/IRawMemoryPartition.h"
#include "executor/core/storage/IDiskPartition.h"
#include "executor/core/IExecutorData.h"
#include "executor/core/ILog.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                namespace impl {
                    class IBaseImpl {
                    public:
                        IBaseImpl(std::shared_ptr<IExecutorData> &executor_data);

                        template<typename Tp>
                        std::shared_ptr<storage::IPartition < Tp>> newPartition();

                        template<typename Tp>
                        std::shared_ptr<storage::IPartition < Tp>> newPartition(
                        storage::IPartition <Tp> &part
                        );

                        template<typename Tp>
                        std::shared_ptr<storage::IPartitionGroup < Tp>> newPartitionGroup(
                        int partitions = 0
                        );

                        template<typename Tp>
                        std::shared_ptr<storage::IPartitionGroup < Tp>>
                        newPartitionGroup(
                        storage::IPartitionGroup <Tp> &partitions
                        );

                        template<typename Tp>
                        std::shared_ptr<storage::IMemoryPartition < Tp>> newMemoryPartition(
                        int64_t elems = 1000
                        );

                        template<typename Tp>
                        std::shared_ptr<storage::IRawMemoryPartition < Tp>>
                        newRawMemoryPartition(
                        int64_t sz = 120 * 1024 * 1024
                        );

                        template<typename Tp>
                        std::shared_ptr<storage::IDiskPartition < Tp>>
                        newDiskPartition(
                        const std::string &name = "",
                        bool persist = false,
                        bool read = false
                        );

                        template<typename Tp>
                        bool isMemory(storage::IPartitionGroup <Tp> &group);

                        template<typename Tp>
                        bool isMemory(storage::IPartition <Tp> &part);

                        template<typename Tp>
                        bool isRawMemory(storage::IPartitionGroup <Tp> &group);

                        template<typename Tp>
                        bool isRawMemory(storage::IPartition <Tp> &part);

                        template<typename Tp>
                        bool isDisk(storage::IPartitionGroup <Tp> &group);

                        template<typename Tp>
                        bool isDisk(storage::IPartition <Tp> &part);

                        template<typename Tp>
                        storage::IMemoryPartition <Tp> &toMemory(storage::IPartition <Tp> &st);

                        template<typename Tp>
                        storage::IMemoryReadIterator <Tp> &toMemory(api::IReadIterator <Tp> &it);

                        template<typename Tp>
                        storage::IMemoryWriteIterator <Tp> &toMemory(api::IWriteIterator <Tp> &it);

                        template<typename Sp>
                        std::shared_ptr<Sp> &toMemory(std::shared_ptr<Sp> &it);

                        virtual ~IBaseImpl();

                    protected:
                        std::shared_ptr<IExecutorData> executor_data;
                    };
                }
            }
        }
    }
}

#include "IBaseImpl.tcc"

#endif

