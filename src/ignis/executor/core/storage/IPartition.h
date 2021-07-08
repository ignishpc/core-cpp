#ifndef IGNIS_IPARTITION_H
#define IGNIS_IPARTITION_H

#include "ignis/executor/api/IReadIterator.h"
#include "ignis/executor/api/IVector.h"
#include "ignis/executor/api/IWriteIterator.h"
#include "ignis/executor/core/RTTInfo.h"
#include "ignis/executor/core/transport/ITransport.h"
#include <memory>

namespace ignis {
    namespace executor {
        namespace core {
            class IMpi;
            namespace storage {

                using api::IVector;

                class IPartitionBase {
                public:
                    virtual void read(std::shared_ptr<transport::ITransport> &trans) = 0;

                    virtual void write(std::shared_ptr<transport::ITransport> &trans, int8_t compression) = 0;

                    virtual void write(std::shared_ptr<transport::ITransport> &trans) { write(trans, 0); }

                    virtual size_t size() = 0;

                    bool empty() { return size() == 0; }

                    virtual size_t bytes() = 0;

                    virtual void clear() = 0;

                    virtual void fit() = 0;

                    virtual const std::string &type() = 0;
                };

                template<typename Tp>
                class IPartition : public IPartitionBase {
                public:
                    typedef Tp value_type;

                    virtual std::shared_ptr<api::IReadIterator<Tp>> readIterator() = 0;

                    virtual std::shared_ptr<api::IWriteIterator<Tp>> writeIterator() = 0;

                    virtual std::shared_ptr<IPartition<Tp>> clone() = 0;

                    virtual void copyFrom(IPartition<Tp> &source) = 0;

                    virtual void copyTo(IPartition<Tp> &target) { target.copyFrom(*this); }

                    virtual void moveFrom(IPartition<Tp> &source) = 0;

                    virtual void moveTo(IPartition<Tp> &target) { target.moveFrom(*this); }
                };

                template<typename Tp>
                class IPartitionGroup {
                public:
                    typedef Tp value_type;

                    std::shared_ptr<IPartition<Tp>> &operator[](int64_t index) { return _partitions[index]; }

                    typename std::vector<std::shared_ptr<IPartition<Tp>>>::iterator begin() {
                        return _partitions.begin();
                    }

                    typename std::vector<std::shared_ptr<IPartition<Tp>>>::iterator end() { return _partitions.end(); }

                    int64_t partitions() { return _partitions.size(); }

                    bool empty() { return partitions() == 0; }

                    void add(const std::shared_ptr<IPartition<Tp>> &partition) { _partitions.push_back(partition); }

                    void remove(int64_t index) { _partitions.erase(_partitions.begin() + index); }

                    void clear() { _partitions.clear(); }

                    virtual RTTInfo elemType() { return RTTInfo::from<Tp>(); }

                    std::shared_ptr<IPartitionGroup<Tp>> clone() {
                        auto newGroup = std::make_shared<IPartitionGroup<Tp>>();
                        for (auto &p : *this) {
                            auto copy = p->clone();
                            newGroup->add(copy);
                        }
                        return newGroup;
                    }

                    std::shared_ptr<IPartitionGroup<Tp>> shadowCopy() {
                        auto newGroup = std::make_shared<IPartitionGroup<Tp>>();
                        newGroup->cache() = cache();
                        for (auto &p : *this) { newGroup->add(p); }
                        return newGroup;
                    }

                    void fit() {
                        //protect fit from casting
                        if(RTTInfo::from<Tp>() == elemType()){
                            for (auto &p : *this) { p->fit(); }
                        }
                    }

                    bool &cache() { return _cache; }

                private:
                    bool _cache;
                    std::vector<std::shared_ptr<IPartition<Tp>>> _partitions;
                };

                template<typename Tp>
                inline int64_t copy(api::IReadIterator<Tp> &reader, api::IWriteIterator<Tp> &writer) {
                    int64_t n = 0;
                    while (reader.hasNext()) {
                        writer.write(reader.next());
                        n++;
                    }
                    return n;
                }

            }// namespace storage
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
