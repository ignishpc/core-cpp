
#include "IRawMemoryObject.h"

#include "iterator/ITransportIterator.h"
#include "../../../data/IZlibTransport.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data;

IRawMemoryObject::IRawMemoryObject(std::shared_ptr<transport::TMemoryBuffer> buffer, int8_t compression)
        : IRawObject(std::make_shared<data::IZlibTransport>(buffer, compression),
                     compression), raw_memory(buffer) {}

IRawMemoryObject::IRawMemoryObject(int8_t compression, uint32_t sz)
        : IRawMemoryObject(std::make_shared<transport::TMemoryBuffer>(sz), compression) {
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IRawMemoryObject::readIterator() {
    this->transport->flush();
    auto read_transport = std::make_shared<data::IZlibTransport>(readObservation());
    return std::make_shared<iterator::IReadTransportIterator>(read_transport, manager, elems);
}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IRawMemoryObject::writeIterator() {
    return IRawObject::writeIterator();
}

void IRawMemoryObject::read(std::shared_ptr<transport::TTransport> trans) {
    clear();
    IRawObject::read(trans);
}

void IRawMemoryObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    IRawObject(std::make_shared<data::IZlibTransport>(readObservation()), this->compression).write(trans,
                                                                                                        compression);
}

bool IRawMemoryObject::fastWrite(std::shared_ptr<transport::TTransport> transport) {
    uint8_t *buffer;
    uint32_t size;
    raw_memory->getBuffer(&buffer, &size);
    transport->write(buffer, raw_memory->available_read());
    transport->flush();
    return true;
}

std::shared_ptr<ignis::transport::TTransport> IRawMemoryObject::readObservation() {
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
    this->transport->flush();
    raw_memory->resetBuffer();
}

std::string IRawMemoryObject::getType() {
    return "raw memory";
}

IRawMemoryObject::~IRawMemoryObject() {

}






