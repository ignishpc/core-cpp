
#include "IRawPartition.h"

#define IRawPartitionClass ignis::executor::core::storage::IRawPartition
#define IHeaderClass ignis::executor::core::storage::IHeader
#define IRawReadIteratorClass ignis::executor::core::storage::IRawReadIterator
#define IRawWriteIteratorClass ignis::executor::core::storage::IRawWriteIterator

template<typename Tp>
const int IRawPartitionClass<Tp>::HEADER = 30;

template<typename Tp>
IRawPartitionClass<Tp>::IRawPartition(std::shared_ptr<transport::ITransport> &trans, int8_t compression) :
        elems(0), compression(compression), header_size(0),
        zlib(std::make_shared<transport::IZlibTransport>(trans, compression)) {}

template<typename Tp>
void IRawPartitionClass<Tp>::read(std::shared_ptr<transport::ITransport> &trans) {
    auto zlib_in = std::make_shared<transport::IZlibTransport>(trans);
    this->readHeader(reinterpret_cast<std::shared_ptr<transport::ITransport> &>(zlib_in));
    this->sync();
    writeHeader();

    uint8_t bb[256];
    uint32_t read;
    while ((read = zlib_in->read(bb, 256)) > 0) {
        this->zlib->write(bb, read);
    }
}

template<typename Tp>
void IRawPartitionClass<Tp>::write(std::shared_ptr<transport::ITransport> &trans, int8_t compression) {
    this->sync();
    writeHeader();
    auto source = readTransport();

    uint8_t bb[256];
    uint32_t read;
    if (compression == this->compression) {
        while ((read = source->read(bb, 256)) > 0) {
            trans->write(bb, read);
        }
        trans->flush();
    } else {
        auto zlib = std::make_shared<transport::IZlibTransport>(source);
        auto zlib_out = std::make_shared<transport::IZlibTransport>(trans, compression);
        while ((read = zlib->read(bb, 256)) > 0) {
            zlib_out->write(bb, read);
        }
        zlib_out->flush();
    }
}

template<typename Tp>
void IRawPartitionClass<Tp>::write(std::shared_ptr<transport::ITransport> &trans){
    write(trans, compression);
}

template<typename Tp>
std::shared_ptr<ignis::executor::api::IReadIterator<Tp>> IRawPartitionClass<Tp>::readIterator() {
    sync();
    auto zlib_it = std::make_shared<transport::IZlibTransport>(readTransport());
    auto proto = std::make_shared<protocol::IObjectProtocol>(zlib_it);
    proto->readSerialization();
    IHeader<Tp>().read(*proto);
    return std::make_shared<storage::IRawReadIterator<Tp>>(proto, elems);
}

template<typename Tp>
std::shared_ptr<ignis::executor::api::IWriteIterator<Tp>> IRawPartitionClass<Tp>::writeIterator() {
    if (header_size == 0) {
        writeHeader();
    }
    return std::make_shared<storage::IRawWriteIterator<Tp>>(
            std::make_shared<protocol::IObjectProtocol>(zlib), elems);
}

template<typename Tp>
void IRawPartitionClass<Tp>::sync() {
    if (elems > 0) {
        zlib->flush();
    }
}

template<typename Tp>
size_t IRawPartitionClass<Tp>::size() {
    return this->elems;
}

template<typename Tp>
void IRawPartitionClass<Tp>::readHeader(std::shared_ptr<transport::ITransport> &trans) {
    protocol::IObjectProtocol proto(trans);
    proto.readSerialization();
    this->elems += IHeader<Tp>().read(proto);
}

template<typename Tp>
IRawPartitionClass<Tp>::~IRawPartition() {}

template<typename Tp>
void IRawPartitionClass<Tp>::copyFrom(IPartition <Tp> &source) {
    if (source.type() == "Disk" || source.type() == "RawMemory") {
        auto &raw_source = reinterpret_cast<IRawPartition <Tp> &>(source);
        raw_source.sync();
        sync();
        writeHeader();
        this->elems += raw_source.elems;
        uint8_t bb[256];
        uint32_t read;
        /*Direct copy*/
        if (raw_source.compression == this->compression) {
            auto source_buffer = raw_source.readTransport();
            source_buffer->read(bb, raw_source.header_size);//skip header
            auto &target = transport();
            while ((read = source_buffer->read(bb, 256)) > 0) {
                target->write(bb, read);
            }
        } else {
            /*Read header to initialize zlib*/
            auto source_buffer = raw_source.readTransport();
            auto source_zlib = std::make_shared<transport::IZlibTransport>(source_buffer);
            auto source_proto = std::make_shared<protocol::IObjectProtocol>(source_zlib);
            source_proto->readSerialization();
            IHeader<Tp>().read(*source_proto);
            while ((read = source_zlib->read(bb, 256)) > 0) {
                this->zlib->write(bb, read);
            }
        }
    } else {
        auto reader = source.readIterator();
        auto writer = this->writeIterator();
        for (int64_t i = 0; i < source.size(); i++) {
            writer->write(reader->next());
        }
    }
}

template<typename Tp>
void IRawPartitionClass<Tp>::moveFrom(IPartition <Tp> &source) {
    copyFrom(source);
    source.clear();
}

template<typename H>
size_t IHeaderClass<H>::read(protocol::IProtocol &proto) {
    io::IReader<std::vector<H>>().readType(proto);
    auto sz = io::readSizeAux(proto);
    io::IReader<H>().readType(proto);
    return sz;
}

template<typename H>
void IHeaderClass<H>::write(protocol::IProtocol &proto, size_t elems) {
    io::IWriter<std::vector<H>>().writeType(proto);
    io::writeSizeAux(proto, elems);
    io::IWriter<H>().writeType(proto);
}

template<>
struct IHeaderClass<uint8_t> {
    size_t read(protocol::IProtocol &proto) {
        io::IReader<std::vector<uint8_t>>().readType(proto);
        return io::readSizeAux(proto);
    }

    void write(protocol::IProtocol &proto, size_t elems) {
        io::IWriter<std::vector<uint8_t>>().writeType(proto);
        io::writeSizeAux(proto, elems);
    }
};

template<typename H1, typename H2>
struct IHeaderClass<std::pair<H1, H2>> {
    size_t read(protocol::IProtocol &proto) {
        io::IReader<std::vector<std::pair<H1, H2>>>().readType(proto);
        auto sz = io::readSizeAux(proto);
        io::IReader<H1>().readType(proto);
        io::IReader<H2>().readType(proto);
        return sz;
    }

    void write(protocol::IProtocol &proto, size_t elems) {
        io::IWriter<std::vector<std::pair<H1, H2>>>().writeType(proto);
        io::writeSizeAux(proto, elems);
        io::IWriter<H1>().writeType(proto);
        io::IWriter<H2>().writeType(proto);
    }
};

template<typename Tp>
IRawReadIteratorClass<Tp>::IRawReadIterator(std::shared_ptr<protocol::IProtocol> proto, size_t &elems) :
        proto(proto), elems(elems), pos(0) {}

template<typename Tp>
Tp &IRawReadIteratorClass<Tp>::next() {
    pos++;
    shared = reader(*proto);
    return *shared;
}

template<typename Tp>
std::shared_ptr<Tp> IRawReadIteratorClass<Tp>::nextShared() {
    pos++;
    return shared = reader(*proto);
}

template<typename Tp>
bool IRawReadIteratorClass<Tp>::hasNext() {
    return pos < elems;
}

template<typename Tp>
IRawReadIteratorClass<Tp>::~IRawReadIterator() {}

template<typename Tp>
IRawWriteIteratorClass<Tp>::IRawWriteIterator(std::shared_ptr<protocol::IProtocol> proto, size_t &elems):
        proto(proto), elems(elems) {}

template<typename Tp>
void IRawWriteIteratorClass<Tp>::write(Tp &obj) {
    writer(*proto, obj);
    elems++;
}

template<typename Tp>
void IRawWriteIteratorClass<Tp>::write(Tp &&obj) {
    writer(*proto, std::forward<Tp>(obj));
    elems++;
}

template<typename Tp>
IRawWriteIteratorClass<Tp>::~IRawWriteIterator() {}

#undef IRawPartitionClass
#undef IRawReadIteratorClass
#undef IRawWriteIteratorClass