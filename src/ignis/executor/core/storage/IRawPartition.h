
#ifndef IGNIS_IRAWPARTITION_H
#define IGNIS_IRAWPARTITION_H

#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/transport/IZlibTransport.h"
#include "IPartition.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                template<typename Tp>
                class IRawPartition : public IPartition<Tp> {
                public:

                    IRawPartition(std::shared_ptr<transport::ITransport> &trans, int8_t compression = 6);

                    virtual std::shared_ptr<api::IReadIterator<Tp>> readIterator();

                    virtual std::shared_ptr<api::IWriteIterator<Tp>> writeIterator();

                    virtual void read(std::shared_ptr<transport::ITransport> &trans);

                    virtual void write(std::shared_ptr<transport::ITransport> &trans, int8_t compression);

                    virtual void copyFrom(IPartition<Tp> &source);

                    virtual void moveFrom(IPartition<Tp> &source);

                    virtual std::shared_ptr<IPartition<Tp>> clone() = 0;

                    virtual size_t size();

                    virtual size_t bytes() = 0;

                    virtual void clear() = 0;

                    virtual void fit() = 0;

                    virtual const std::string &type() = 0;

                    virtual void sync();

                    virtual ~IRawPartition();

                protected:
                    static const int HEADER;
                    int header_size;
                    std::shared_ptr<transport::IZlibTransport> zlib;
                    size_t elems;
                    int8_t compression;

                    virtual std::shared_ptr<transport::ITransport> readTransport() = 0;

                    virtual std::shared_ptr<transport::ITransport> &transport() = 0;

                    void readHeader(std::shared_ptr<transport::ITransport> &trans);

                    virtual void writeHeader() = 0;
                };

                template<typename H>
                struct IHeader {
                    size_t read(protocol::IProtocol &proto);

                    void write(protocol::IProtocol &proto, size_t elems);
                };

                template<typename Tp>
                class IRawReadIterator : public api::IReadIterator<Tp> {
                public:

                    IRawReadIterator(std::shared_ptr<protocol::IProtocol> proto, size_t &elems);

                    Tp &next();

                    std::shared_ptr<Tp> nextShared();

                    bool hasNext();

                    virtual ~IRawReadIterator();

                private:
                    std::shared_ptr<protocol::IProtocol> proto;
                    std::shared_ptr<Tp> shared;
                    size_t pos;
                    size_t &elems;
                    io::IReader<std::shared_ptr<Tp>> reader;
                };

                template<typename Tp>
                class IRawWriteIterator : public api::IWriteIterator<Tp> {
                public:

                    IRawWriteIterator(std::shared_ptr<protocol::IProtocol> proto, size_t &elems);

                    void write(Tp &obj);

                    void write(Tp &&obj);

                    virtual ~IRawWriteIterator();

                private:
                    std::shared_ptr<protocol::IProtocol> proto;
                    size_t &elems;
                    io::IWriter<Tp> writer;
                };
            }
        }
    }
}

#include "IRawPartition.tcc"

#endif

