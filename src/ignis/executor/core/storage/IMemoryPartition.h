
#ifndef IGNIS_IMEMORYPARTITION_H
#define IGNIS_IMEMORYPARTITION_H

#include "IPartition.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {

                template<typename Tp>
                class IMemoryPartition : public IPartition<Tp> {
                public:
                    static const std::string TYPE;

                    IMemoryPartition(size_t size = 1024);

                    IMemoryPartition(IVector<Tp> &&elements);

                    virtual std::shared_ptr<api::IReadIterator<Tp>> readIterator();

                    virtual std::shared_ptr<api::IWriteIterator<Tp>> writeIterator();

                    virtual void read(std::shared_ptr<transport::ITransport> &trans);

                    virtual void write(std::shared_ptr<transport::ITransport> &trans, int8_t compression);

                    virtual void write(std::shared_ptr<transport::ITransport> &trans, int8_t compression, bool native);

                    virtual void copyFrom(IPartition<Tp> &source);

                    virtual void moveFrom(IPartition<Tp> &source);

                    virtual std::shared_ptr<IPartition<Tp>> clone();

                    virtual size_t size();

                    virtual size_t bytes();

                    virtual void clear();

                    virtual void fit();

                    virtual const std::string &type();

                    inline Tp &operator[](int64_t index);

                    Tp *begin();

                    Tp *end();

                    void resize(int64_t size);

                    IVector<Tp> &inner();

                    virtual ~IMemoryPartition();

                private:
                    IVector<Tp> elements;
                };

                template<typename Tp>
                class IMemoryReadIterator : public api::IReadIterator<Tp> {
                public:
                    IMemoryReadIterator(IVector<Tp> &elements);

                    Tp &next();

                    std::shared_ptr<Tp> nextShared();

                    bool hasNext();

                    virtual ~IMemoryReadIterator();

                private:
                    IVector<Tp> &elements;
                    int64_t index;
                };

                template<typename Tp>
                class IMemoryWriteIterator : public api::IWriteIterator<Tp> {
                public:
                    IMemoryWriteIterator(IVector<Tp> &elements);

                    void write(Tp &obj);

                    void write(Tp &&obj);

                    virtual ~IMemoryWriteIterator();

                private:
                    IVector<Tp> &elements;
                };
            }// namespace storage
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IMemoryPartition.tcc"

#endif
