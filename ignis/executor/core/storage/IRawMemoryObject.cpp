
#include "IRawMemoryObject.h"

#include "iterator/ITransportIterator.h"
#include "../../../data/IZlibTransport.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data;

const std::string IRawMemoryObject::TYPE = "raw memory";

IRawMemoryObject::IRawMemoryObject(const std::shared_ptr<transport::TMemoryBuffer> &buffer,
                                   int8_t compression, size_t elems, int8_t type, bool read_only)
        : IRawObject(std::make_shared<data::IZlibTransport>(buffer, compression), compression, elems, type),
          raw_memory(buffer), read_only(read_only) {}

IRawMemoryObject::IRawMemoryObject(const std::shared_ptr<transport::TMemoryBuffer> &buffer, int8_t compression)
        : IRawObject(std::make_shared<data::IZlibTransport>(buffer, compression),
                     compression), raw_memory(buffer), read_only(false) {}

IRawMemoryObject::IRawMemoryObject(int8_t compression, uint32_t sz)
        : IRawMemoryObject(std::make_shared<transport::TMemoryBuffer>(sz), compression) {
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IRawMemoryObject::readIterator() {
    if (!read_only) {
        this->transport->flush();
        return IRawMemoryObject(readObservation(), this->compression, elems, type, true).
                setManager(manager).readIterator();
    }
    return this->IRawObject::readIterator();
}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IRawMemoryObject::writeIterator() {
    return this->IRawObject::writeIterator();
}

void IRawMemoryObject::read(std::shared_ptr<transport::TTransport> trans) {
    this->IRawObject::read(trans);
}

void IRawMemoryObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    if (!read_only) {
        this->transport->flush();
        IRawMemoryObject(readObservation(), this->compression, elems, type, true).setManager(manager).
                write(trans, compression);
        return;
    }
    this->IRawObject::write(trans, compression);
}

std::shared_ptr<ignis::transport::TMemoryBuffer> IRawMemoryObject::readObservation() {
    this->transport->flush();
    uint8_t *ptr;
    uint32_t size;
    raw_memory->getBuffer(&ptr, &size);
    return std::make_shared<transport::TMemoryBuffer>(ptr, size);
}

void IRawMemoryObject::fit() {
    raw_memory->setMaxBufferSize(raw_memory->getBufferSize());
}

void IRawMemoryObject::clear() {
    elems = 0;
    std::dynamic_pointer_cast<data::IZlibTransport> (transport)->restart();
    raw_memory->resetBuffer();
}

std::string IRawMemoryObject::getType() {
    return TYPE;
}

IRawMemoryObject::~IRawMemoryObject() {

}






