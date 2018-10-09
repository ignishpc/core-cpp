
#include "IRawObject.h"

#include "../../../data/IZlibTransport.h"
#include "iterator/ITransportIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data;

IRawObject::IRawObject(const std::shared_ptr<transport::TTransport> &transport, int8_t compression) :
        IRawObject(transport, compression, 0, -1) {}

IRawObject::IRawObject(const std::shared_ptr<transport::TTransport> &transport, int8_t compression, size_t elems,
                       int8_t type) :
        transport(transport), elems(elems), compression(compression), type(type) {}

IRawObject::~IRawObject() {}

void IRawObject::readHeader(std::shared_ptr<transport::TTransport> transport) {
    data::handle::IReader<std::vector<IObject::Any>> col_reader;
    bool native;
    data::IObjectProtocol data_proto(transport);
    data_proto.readBool(native);
    col_reader.readType(data_proto);
    elems = handle::readSizeAux(data_proto);
    type = handle::readTypeAux(data_proto);
}

void IRawObject::writeHeader(std::shared_ptr<transport::TTransport> transport) {
    data::handle::IWriter<std::vector<IObject::Any>> col_writer;
    data::IObjectProtocol data_proto(transport);
    bool native = false;
    data_proto.writeBool(native);
    col_writer.writeType(data_proto);
    handle::writeSizeAux(data_proto, elems);
    if (manager) {
        manager->writer()->writeType(data_proto);
    } else {
        data::handle::writeTypeAux(data_proto, type);
    }
    transport->flush();
}

std::shared_ptr<iterator::ICoreReadIterator<IObject::Any>> IRawObject::readIterator() {
    return std::make_shared<iterator::IReadTransportIterator>(transport, manager, elems);

}

std::shared_ptr<iterator::ICoreWriteIterator<IObject::Any>> IRawObject::writeIterator() {
    return std::make_shared<iterator::IWriteTransportIterator>(transport, manager, elems);
}

void IRawObject::read(std::shared_ptr<transport::TTransport> trans) {
    clear();
    auto data_transport = std::make_shared<data::IZlibTransport>(trans);
    readHeader(data_transport);
    uint8_t buffer[256];
    size_t bytes;
    while ((bytes = data_transport->read(buffer, 256)) > 0) {
        this->transport->write(buffer, bytes);
    }
    this->transport->flush();
}

void IRawObject::write(std::shared_ptr<transport::TTransport> trans, int8_t compression) {
    auto data_transport = std::make_shared<data::IZlibTransport>(trans, compression);
    writeHeader(data_transport);
    uint8_t buffer[256];
    size_t bytes;
    while ((bytes = this->transport->read(buffer, 256)) > 0) {
        data_transport->write(buffer, bytes);
    }
    data_transport->flush();
}

void IRawObject::copyFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()));
}

void IRawObject::moveFrom(IObject &source) {
    iterator::readToWrite(*(source.readIterator()), *(this->writeIterator()), true);
    source.clear();
}

size_t IRawObject::getSize() {
    return elems;
}

std::string IRawObject::getType() {
    return "raw";
}

