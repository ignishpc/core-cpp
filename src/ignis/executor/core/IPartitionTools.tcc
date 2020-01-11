
#include "IPartitionTools.h"

#define IPartitionToolsClass ignis::executor::core::IPartitionTools

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>> IPartitionToolsClass::newPartition() {
    auto partitionType = properties.partitionType();
    if (partitionType == storage::IMemoryPartition<Tp>::TYPE) {
        return newMemoryPartition<Tp>();
    } else if (partitionType == storage::IRawMemoryPartition<Tp>::TYPE) {
        return newRawMemoryPartition<Tp>();
    } else if (partitionType == storage::IDiskPartition<Tp>::TYPE) {
        return newDiskPartition<Tp>();
    } else {
        throw exception::IInvalidArgument("unknown partition type: " + partitionType);
    }
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>>
IPartitionToolsClass::newPartition(storage::IPartition<Tp> &part) {
    auto partitionType = properties.partitionType();
    if (partitionType == storage::IMemoryPartition<Tp>::TYPE) {
        return newMemoryPartition<Tp>(part.size());
    } else if (partitionType == storage::IRawMemoryPartition<Tp>::TYPE) {
        return newRawMemoryPartition<Tp>(part.bytes());
    } else if (partitionType == storage::IDiskPartition<Tp>::TYPE) {
        return newDiskPartition<Tp>();
    } else {
        throw exception::IInvalidArgument("unknown partition type: " + partitionType);
    }
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>>
IPartitionToolsClass::newPartitionGroup(int partitions) {
    auto group = std::make_shared<storage::IPartitionGroup<Tp>>
            ();
    for (int i = 0; i < partitions; i++) {
        auto part = newPartition<Tp>();
        group->add(part);
    }
    return group;
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartitionGroup<Tp>>
IPartitionToolsClass::newPartitionGroup(storage::IPartitionGroup<Tp> &partitions) {
    auto group = std::make_shared<storage::IPartitionGroup<Tp>>
            ();
    for (int i = 0; i < partitions; i++) {
        auto part = newPartition<Tp>((*group)[i]);
        group->add(part);
    }
    return group;
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IMemoryPartition<Tp>>
IPartitionToolsClass::newMemoryPartition(int64_t elems) {
    return std::make_shared<storage::IMemoryPartition<Tp>>
            (elems);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IRawMemoryPartition<Tp>>
IPartitionToolsClass::newRawMemoryPartition(int64_t sz) {
    auto cmp = properties.partitionCompression();
    return std::make_shared<storage::IRawMemoryPartition<Tp>>
            (sz, cmp);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IDiskPartition<Tp>>
IPartitionToolsClass::newDiskPartition(const std::string &name, bool persist, bool read) {
    auto cmp = properties.partitionCompression();
    auto path = properties.partitionDirectory();
    if (name.empty()) {
        path.append("/partition");
        path.append(std::to_string(context.executorId()));
        path.append(".");
        #pragma omp critical
        {
            path.append(std::to_string(partition_id_gen++));
        }
    } else {
        path += "/" + name;
    }
    return std::make_shared<storage::IDiskPartition<Tp>>
            (path, cmp, persist, read);
}

template<typename Tp>
bool IPartitionToolsClass::isMemory(storage::IPartitionGroup<Tp> &group) {
    if (group.partitions() == 0) {
        return false;
    }
    return isMemory(*group[0]);
}

template<typename Tp>
bool IPartitionToolsClass::isMemory(storage::IPartition<Tp> &part) {
    return part.type() == storage::IMemoryPartition<Tp>::TYPE;
}

template<typename Tp>
bool IPartitionToolsClass::isRawMemory(storage::IPartitionGroup<Tp> &group) {
    if (group.partitions() == 0) {
        return false;
    }
    return isRawMemory(*group[0]);
}

template<typename Tp>
bool IPartitionToolsClass::isRawMemory(storage::IPartition<Tp> &part) {
    return part.type() == storage::IRawMemoryPartition<Tp>::TYPE;
}

template<typename Tp>
bool IPartitionToolsClass::isDisk(storage::IPartitionGroup<Tp> &group) {
    if (group.partitions() == 0) {
        return false;
    }
    return isDisk(*group[0]);
}

template<typename Tp>
bool IPartitionToolsClass::isDisk(storage::IPartition<Tp> &part) {
    return part.type() == storage::IDiskPartition<Tp>::TYPE;
}

template<typename Tp>
ignis::executor::core::storage::IMemoryPartition<Tp> &IPartitionToolsClass::toMemory(storage::IPartition<Tp> &st) {
    return reinterpret_cast<storage::IMemoryPartition<Tp> &>(st);
}

template<typename Tp>
ignis::executor::core::storage::IMemoryReadIterator<Tp> &IPartitionToolsClass::toMemory(api::IReadIterator<Tp> &it) {
    return reinterpret_cast<storage::IMemoryReadIterator<Tp> &>(it);
}

template<typename Tp>
ignis::executor::core::storage::IMemoryWriteIterator<Tp> &IPartitionToolsClass::toMemory(api::IWriteIterator<Tp> &it) {
    return reinterpret_cast<storage::IMemoryWriteIterator<Tp> &>(it);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IMemoryPartition<Tp>> &
IPartitionToolsClass::toMemory(std::shared_ptr<storage::IPartition<Tp>> &st) {
    return reinterpret_cast<std::shared_ptr<ignis::executor::core::storage::IMemoryPartition<Tp>> &>(st);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IMemoryReadIterator<Tp> > &
IPartitionToolsClass::toMemory(std::shared_ptr<api::IReadIterator<Tp>> &it) {
    return reinterpret_cast<std::shared_ptr<ignis::executor::core::storage::IMemoryReadIterator<Tp> > &>(it);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IMemoryWriteIterator<Tp>> &
IPartitionToolsClass::toMemory(std::shared_ptr<api::IWriteIterator<Tp>> &it) {
    return reinterpret_cast<std::shared_ptr<ignis::executor::core::storage::IMemoryWriteIterator<Tp>> &>(it);
}

#undef IPartitionToolsClass