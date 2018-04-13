
#include "IRawMemoryObject.h"
#include "../../../data/IObjectProtocol.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using apache::thrift::transport::TMemoryBuffer;

IRawMemoryObject::IRawMemoryObject() : raw_memory(make_shared<TMemoryBuffer>()), elems(0) {}

void IRawMemoryObject::writeHeader() {
    uint8_t *buffer;
    uint32_t size;
    raw_memory->getBuffer(&buffer, &size);
    auto tmp = make_shared<TMemoryBuffer>(buffer, size, TMemoryBuffer::OBSERVE);
    data::IObjectProtocol proto(tmp);
    IWriterType<vector<Any>>().writeType(proto);
    writeSizeAux(proto, elems);
    type_handle->writer()->writeType(proto);
    bytes = raw_memory->writeEnd();
}

IRawMemoryObject::IRawMemoryObject(const shared_ptr<TMemoryBuffer> &raw_memory) : raw_memory(raw_memory) {}


shared_ptr<ignis::executor::api::IReadIterator<IObject::Any>> IRawMemoryObject::readIterator() {
    if (raw_memory->writeEnd() > bytes) {
        writeHeader();
    }
    uint8_t *buffer;
    uint32_t size;
    raw_memory->getBuffer(&buffer, &size);
    auto obs = make_shared<TMemoryBuffer>(buffer, raw_memory->available_read(), TMemoryBuffer::OBSERVE);

    return make_shared<IRawReadIterator>(obs, type_handle);
}

shared_ptr<ignis::executor::api::IWriteIterator<IObject::Any>> IRawMemoryObject::writeIterator() {
    return make_shared<IRawWriteIterator>(raw_memory, type_handle, elems);
}

void IRawMemoryObject::write(std::shared_ptr<apache::thrift::transport::TTransport> &trans) {
    if (raw_memory->writeEnd() > bytes) {
        writeHeader();
    }
    uint8_t *buffer;
    uint32_t size;
    raw_memory->getBuffer(&buffer, &size);
    trans->write(buffer, raw_memory->available_read());
}

shared_ptr<IObject::Handle> IRawMemoryObject::getObject() {
    if (raw_memory->writeEnd() > bytes) {
        writeHeader();
    }
    data::IObjectProtocol proto(raw_memory);
    auto c_handle = ((ITypeHandle<Any>&)*type_handle).collectionHandle();
    auto reader = c_handle->reader();
    bool native;
    proto.readBool(native);
    reader->readType(proto);
    return make_shared<Handle>((Any*)reader->readPtr(proto), (shared_ptr<ITypeHandleBase<Any>>&)c_handle);
}

IRawReadIterator::IRawReadIterator(const shared_ptr<TMemoryBuffer> &raw_memory,
                                   const shared_ptr<ITypeHandleBase<IObject::Any>> &type_handle)
        : raw_memory(raw_memory),
          type_handle(type_handle),
          protocol(make_shared<data::IObjectProtocol>(raw_memory)),
          reader(type_handle->reader()) {
    auto c_handle = ((ITypeHandle<IObject::Any>&)*type_handle).collectionHandle();
    auto reader = c_handle->reader();
    bool native;
    protocol->readBool(native);
    reader->readType(*protocol);
    size = readSizeAux(*protocol);
}

IRawWriteIterator::IRawWriteIterator(const shared_ptr<TMemoryBuffer> &raw_memory,
                                     const shared_ptr<ITypeHandleBase<IObject::Any>> &type_handle,
                                     size_t &elems)
        : raw_memory(raw_memory),
          type_handle(type_handle),
          protocol(make_shared<data::IObjectProtocol>(raw_memory)),
          writer(type_handle->writer()),
          elems(elems) {}

bool IRawReadIterator::hashNext() {
    return size < read;
}

IObject::Any *IRawReadIterator::next() {
    read++;
    actual = make_shared<IObject::Handle>(reader->readPtr(*protocol), type_handle);
    return actual->data();
}

void IRawWriteIterator::write(IObject::Any &obj) {
    elems++;
    (*writer)(obj, *protocol);
}

void IRawWriteIterator::write(IObject::Any &&obj) {
    elems++;
    (*writer)(obj, *protocol);
}





