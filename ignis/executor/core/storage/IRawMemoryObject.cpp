
#include "IRawMemoryObject.h"

#include <thrift/transport/TZlibTransport.h>
#include "iterator/ITransportIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;

IRawMemoryObject::IRawMemoryObject(int8_t compression, uint32_t sz)
        : raw_memory(std::make_shared<transport::TMemoryBuffer>(sz)),
          IRawObject(std::make_shared<transport::TZlibTransport>(raw_memory, 128, 1024, 128, 1024, compression), compression) {
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IRawMemoryObject::readIterator() {
    auto read_transport = std::make_shared<transport::TZlibTransport>(readObservation());

}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IRawMemoryObject::writeIterator() {
    raw_memory->resetBuffer();
    return IRawObject::writeIterator();
}

void IRawMemoryObject::read(std::shared_ptr<transport::TTransport> trans) {
    clear();
    IRawObject::read(trans);
}

void IRawMemoryObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    IRawObject(std::make_shared<transport::TZlibTransport>(readObservation()), this->compression).write(trans, compression);
}

bool IRawMemoryObject::fastWrite(std::shared_ptr<transport::TTransport> transport) {
    uint8_t *buffer;
    uint32_t size;
    raw_memory->getBuffer(&buffer, &size);
    transport->write(buffer, raw_memory->available_read());
    transport->flush();
    return true;
}

std::shared_ptr<ignis::transport::TMemoryBuffer> IRawMemoryObject::readObservation() {
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
    raw_memory->resetBuffer();
}

std::string IRawMemoryObject::getType() {
    return "raw memory";
}

IRawMemoryObject::~IRawMemoryObject() {

}






