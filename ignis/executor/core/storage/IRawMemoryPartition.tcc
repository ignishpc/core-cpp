
#include "IRawMemoryPartition.h"

#define IRawMemoryPartitionClass ignis::executor::core::storage::IRawMemoryPartition

template<typename Tp>
const std::string IRawMemoryPartitionClass<Tp>::TYPE = "RawMemory";

template<typename Tp>
IRawMemoryPartitionClass<Tp>::IRawMemoryPartition(size_t bytes, int8_t compression) :
        IRawMemoryPartition(std::make_shared<transport::IMemoryBuffer>(bytes + IRawMemoryPartitionClass<Tp>::HEADER),
                            compression) {}

template<typename Tp>
IRawMemoryPartitionClass<Tp>::IRawMemoryPartition(std::shared_ptr<transport::IMemoryBuffer> &&bb, int8_t compression)
        : IRawPartition<Tp>((std::shared_ptr<transport::ITransport> &) bb, compression), buffer(bb) {}

template<typename Tp>
IRawMemoryPartitionClass<Tp>::~IRawMemoryPartition() {}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>> IRawMemoryPartitionClass<Tp>::clone() {
    auto newPartition = std::make_shared<IRawMemoryPartition < Tp>>
    (this->bytes());
    this->copyTo(*newPartition);
    return newPartition;
}

template<typename Tp>
void IRawMemoryPartitionClass<Tp>::clear() {
    buffer->resetBuffer();
    this->elems = 0;
}

template<typename Tp>
void IRawMemoryPartitionClass<Tp>::fit() {
    buffer->setBufferSize(buffer->getBufferSize());
}

template<typename Tp>
const std::string &IRawMemoryPartitionClass<Tp>::type() {
    return TYPE;
}

template<typename Tp>
size_t IRawMemoryPartitionClass<Tp>::bytes() {
    uint8_t *ptr;
    size_t sz;
    buffer->getBuffer(&ptr, &sz);
    size_t n = sz - IRawMemoryPartitionClass<Tp>::HEADER + this->header_size;
    return n > 0 ? n : 0;
}

template<typename Tp>
uint8_t *IRawMemoryPartitionClass<Tp>::begin(bool header) {
    uint8_t *ptr;
    size_t sz;
    buffer->getBuffer(&ptr, &sz);
    if (sz == 0) {
        return ptr;
    }
    if (header) {
        return ptr + IRawMemoryPartitionClass<Tp>::HEADER - this->header_size;
    }
    return ptr + IRawMemoryPartitionClass<Tp>::HEADER;
}

template<typename Tp>
uint8_t *IRawMemoryPartitionClass<Tp>::end() {
    uint8_t *ptr;
    size_t sz;
    buffer->getBuffer(&ptr, &sz);
    return ptr + sz;
}

template<typename Tp>
void IRawMemoryPartitionClass<Tp>::reserve(size_t bytes) {
    if (bytes < buffer->getBufferSize() - IRawMemoryPartitionClass<Tp>::HEADER) {
        buffer->setBufferSize(bytes + IRawMemoryPartitionClass<Tp>::HEADER);
    }
}

template<typename Tp>
void IRawMemoryPartitionClass<Tp>::resize(size_t elems, size_t bytes) {
    buffer->resetBuffer();
    this->elems = elems;
    reserve(bytes);
    writeHeader();
    buffer->wroteBytes(bytes);
}

template<typename Tp>
void IRawMemoryPartitionClass<Tp>::writeHeader() {
    protocol::IObjectProtocol proto(this->zlib);
    this->zlib->reset();
    auto write_pos = buffer->getBufferSize() - buffer->available_write();
    buffer->resetBuffer();//Reset write
    auto ptr = buffer->getWritePtr(0);
    bool native;
    proto.writeBool(native);
    IHeader<Tp>().write(proto, this->elems);
    this->zlib->flush();
    /*Align header to the left*/
    this->header_size = (int) buffer->writeEnd();
    for (size_t i = 1; i < this->header_size + 1; i++) {
        ptr[IRawMemoryPartitionClass<Tp>::HEADER - i] = ptr[this->header_size - i];
    }
    buffer->wroteBytes(std::max(write_pos, (size_t) IRawMemoryPartitionClass<Tp>::HEADER) - this->header_size);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::transport::ITransport> IRawMemoryPartitionClass<Tp>::readTransport() {
    return observer(IRawMemoryPartitionClass<Tp>::HEADER - this->header_size);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::transport::ITransport> &IRawMemoryPartitionClass<Tp>::transport() {
    return (std::shared_ptr<transport::ITransport> &) buffer;
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::transport::IMemoryBuffer> IRawMemoryPartitionClass<Tp>::observer(int init) {
    uint8_t *ptr;
    size_t sz;
    buffer->getBuffer(&ptr, &sz);
    return std::make_shared<transport::IMemoryBuffer>(ptr + init, sz - init, transport::IMemoryBuffer::OBSERVE);
}


#undef IRawMemoryPartitionClass
