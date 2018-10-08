
#include "IRawObject.h"

#include "../../../data/IZlibTransport.h"
#include "iterator/ITransportIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data;

IRawObject::IRawObject(const std::shared_ptr<transport::TTransport> &transport, int8_t compression) :
        transport(transport), elems(0), compression(compression) {}

IRawObject::~IRawObject() {}

void IRawObject::readHeader(std::shared_ptr<transport::TTransport> transport) {
    auto col_reader = manager->reader();
    auto elem_reader = manager->collectionManager()->reader();
    bool native;
    data::IObjectProtocol data_proto(transport);
    data_proto.readBool(native);
    col_reader->readType(data_proto);
    elems = handle::readSizeAux(data_proto);
    elem_reader->readType(data_proto);
}

void IRawObject::writeHeader(std::shared_ptr<transport::TTransport> transport) {
    auto col_writer = manager->writer();
    auto elem_writer = manager->collectionManager()->writer();
    bool native = false;
    data::IObjectProtocol data_proto(transport);
    data_proto.writeBool(native);
    col_writer->writeType(data_proto);
    handle::writeSizeAux(data_proto, elems);
    elem_writer->writeType(data_proto);
    transport->flush();
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IRawObject::readIterator() {
    this->transport->flush();
    return std::make_shared<iterator::IReadTransportIterator>(transport, manager, elems);

}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IRawObject::writeIterator() {
    return std::make_shared<iterator::IWriteTransportIterator>(transport, manager, elems);
}

void IRawObject::read(std::shared_ptr<transport::TTransport> trans) {
    this->transport->flush();
    auto data_transport = std::make_shared<data::IZlibTransport>(trans);
    readHeader(data_transport);
    uint8_t buffer[256];
    size_t bytes;
    while ((bytes = data_transport->read(buffer, 256)) > 0) {
        this->transport->write(buffer, bytes);
    }
    transport->flush();
}

void IRawObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<data::IZlibTransport>(trans, compression);
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

