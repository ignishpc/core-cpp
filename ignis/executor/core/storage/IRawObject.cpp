
#include "IRawObject.h"

#include <thrift/transport/TZlibTransport.h>
#include "ITransportIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;
using namespace apache::thrift::transport;

IRawObject::IRawObject(const std::shared_ptr<TTransport> &transport, int8_t compression) : transport(transport),
                                                                                           elems(0),
                                                                                           compression(compression) {}
IRawObject::~IRawObject(){}

void IRawObject::readHeader(shared_ptr<TTransport> transport) {
    auto data_type = ((ITypeHandle<Any> &) type_handle).collectionHandle();
    auto data_reader = data_type->reader();
    bool native;
    data::IObjectProtocol data_proto(transport);
    data_proto.readBool(native);
    data_reader->readType(data_proto);
    elems = readSizeAux(data_proto);
    type_handle->reader()->readType(data_proto);
}

void IRawObject::writeHeader(shared_ptr<TTransport> transport) {
    auto data_type = ((ITypeHandle<Any> &) type_handle).collectionHandle();
    auto data_writer = data_type->writer();
    bool native = false;
    data::IObjectProtocol data_proto(transport);
    data_proto.writeBool(native);
    data_writer->writeType(data_proto);
    writeSizeAux(data_proto, elems);
    type_handle->writer()->writeType(data_proto);
    transport->flush();
}

shared_ptr<ignis::executor::api::IReadIterator<IObject::Any>> IRawObject::readIterator() {
    return make_shared<IReadTransportIterator>(transport, type_handle, elems);

}

shared_ptr<ignis::executor::api::IWriteIterator<IObject::Any>> IRawObject::writeIterator() {
    elems = 0;
    return make_shared<IWriteTransportIterator>(transport, type_handle, elems);
}

void IRawObject::write(shared_ptr<TTransport> trans, int8_t compression) {
    auto data_transport = make_shared<TZlibTransport>(trans, 128, 1024, 128, 1024, compression);
    writeHeader(data_transport);
    if (!(this->compression == compression && fastWrite(trans))) {
        uint8_t buffer[256];
        size_t bytes;
        while ((bytes = this->transport->read(buffer, 256)) > 0) {
            data_transport->write(buffer, bytes);
        }
        data_transport->flush();
    }
}

bool IRawObject::fastWrite(std::shared_ptr<apache::thrift::transport::TTransport> transport) {
    return false;
}

void IRawObject::read(shared_ptr<TTransport> trans) {
    auto data_transport = make_shared<TZlibTransport>(trans);
    readHeader(data_transport);
    uint8_t buffer[256];
    size_t bytes;
    while ((bytes = data_transport->read(buffer, 256)) > 0) {
        this->transport->write(buffer, bytes);
    }
    transport->flush();
}

size_t IRawObject::getSize() {
    return elems;
}

