
#include "IRawMemoryObject.h"

#include <thrift/transport/TZlibTransport.h>
#include "ITransportIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using namespace apache::thrift::transport;

IRawMemoryObject::IRawMemoryObject(int8_t compression, uint32_t sz)
        : raw_memory(make_shared<TMemoryBuffer>(sz)),
          IRawObject(make_shared<TZlibTransport>(raw_memory, 128, 1024, 128, 1024, compression),compression){
}

shared_ptr<ignis::executor::api::IReadIterator<IObject::Any>> IRawMemoryObject::readIterator() {
    raw_memory.reset();
    return IRawObject::readIterator();

}

shared_ptr<ignis::executor::api::IWriteIterator<IObject::Any>> IRawMemoryObject::writeIterator() {
    raw_memory.reset();
    return IRawObject::writeIterator();
}

void IRawMemoryObject::write(shared_ptr<TTransport> trans, int8_t compression) {
    raw_memory.reset();
    IRawObject::write(trans, compression);
}

void IRawMemoryObject::read(shared_ptr<TTransport> trans) {
    raw_memory.reset();
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






