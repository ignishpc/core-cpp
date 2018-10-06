
#include "IRawObject.h"

#include <thrift/transport/TZlibTransport.h>
#include "iterator/ITransportIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data::serialization;

IRawObject::IRawObject(const std::shared_ptr<transport::TTransport> &transport, int8_t compression) : transport(transport),
                                                                                           elems(0),
                                                                                           compression(compression) {}
IRawObject::~IRawObject(){}

void IRawObject::readHeader(std::shared_ptr<transport::TTransport> transport) {
    auto col_reader = manager->getClassManagerType()->getTypeHandle()->reader();
    auto elem_reader = manager->getClassManagerType()->getElemClassManager()->getTypeHandle()->reader();
    bool native;
    data::IObjectProtocol data_proto(transport);
    data_proto.readBool(native);
    col_reader->readType(data_proto);
    elems = readSizeAux(data_proto);
    elem_reader->readType(data_proto);
}

void IRawObject::writeHeader(std::shared_ptr<transport::TTransport> transport) {
    auto col_writer = manager->getClassManagerType()->getTypeHandle()->writer();
    auto elem_writer = manager->getClassManagerType()->getElemClassManager()->getTypeHandle()->writer();
    bool native = false;
    data::IObjectProtocol data_proto(transport);
    data_proto.writeBool(native);
    col_writer->writeType(data_proto);
    writeSizeAux(data_proto, elems);
    elem_writer->writeType(data_proto);
    transport->flush();
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IRawObject::readIterator() {
    return std::make_shared<iterator::IReadTransportIterator>(transport, manager, elems);

}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IRawObject::writeIterator() {
    elems = 0;
    return std::make_shared<iterator::IWriteTransportIterator>(transport, manager, elems);
}

void IRawObject::read(std::shared_ptr<transport::TTransport> trans) {
    auto data_transport = std::make_shared<transport::TZlibTransport>(trans);
    readHeader(data_transport);
    uint8_t buffer[256];
    size_t bytes;
    while ((bytes = data_transport->read(buffer, 256)) > 0) {
        this->transport->write(buffer, bytes);
    }
    transport->flush();
}

void IRawObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<transport::TZlibTransport>(trans, 128, 1024, 128, 1024, compression);
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

bool IRawObject::fastWrite(std::shared_ptr<transport::TTransport> transport) {
    return false;
}

void IRawObject::copyFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()));
}

void IRawObject::moveFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()), true);
}

size_t IRawObject::getSize() {
    return elems;
}

std::string IRawObject::getType() {
    return "raw";
}

