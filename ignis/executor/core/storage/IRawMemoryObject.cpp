
#include "IRawMemoryObject.h"

#include <thrift/transport/TZlibTransport.h>
#include "ITransportIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using namespace apache::thrift::transport;

IRawMemoryObject::IRawMemoryObject(int8_t compression, uint32_t sz)
        : raw_memory(make_shared<TMemoryBuffer>(sz)),
          IRawObject(make_shared<TZlibTransport>(raw_memory, 128, 1024, 128, 1024, compression), compression) {
}

std::shared_ptr<TMemoryBuffer> IRawMemoryObject::readObservation() {
    uint8_t *ptr;
    uint32_t size;
    raw_memory->getBuffer(&ptr, &size);
    return make_shared<TMemoryBuffer>(ptr, size);
}

shared_ptr<ICoreReadIterator<IObject::Any>> IRawMemoryObject::readIterator() {
    auto read_transport = make_shared<TZlibTransport>(readObservation());

}

shared_ptr<ICoreWriteIterator<IObject::Any>> IRawMemoryObject::writeIterator() {
    raw_memory->resetBuffer();
    return IRawObject::writeIterator();
}

void IRawMemoryObject::write(shared_ptr<TTransport> trans, int8_t compression) {
    IRawObject(make_shared<TZlibTransport>(readObservation()), this->compression).write(trans, compression);
}

void IRawMemoryObject::read(shared_ptr<TTransport> trans) {
    clear();
    IRawObject::read(trans);
}

bool IRawMemoryObject::fastWrite(std::shared_ptr<apache::thrift::transport::TTransport> transport) {
    uint8_t *buffer;
    uint32_t size;
    raw_memory->getBuffer(&buffer, &size);
    transport->write(buffer, raw_memory->available_read());
    transport->flush();
    return true;
}

IRawMemoryObject::~IRawMemoryObject() {

}

void IRawMemoryObject::fit() {
    raw_memory->setMaxBufferSize(raw_memory->getBufferSize());
}

void IRawMemoryObject::clear() {
    elems = 0;
    raw_memory->resetBuffer();
}

string IRawMemoryObject::getType() {
    return "raw memory";
}






