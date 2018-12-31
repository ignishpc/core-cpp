
#include "IRawMemoryObject.h"

#include "iterator/ITransportIterator.h"
#include "../../../data/IZlibTransport.h"

using namespace ignis::executor::core::storage;
using namespace ignis::data;

const std::string IRawMemoryObject::TYPE = "rawMemory";

IRawMemoryObject::IRawMemoryObject(const std::shared_ptr<data::IMemoryBuffer> &buffer,
                                   int8_t compression, size_t elems, int8_t type, bool read_only)
        : IRawObject(std::make_shared<data::IZlibTransport>(buffer, compression), compression, elems, type),
          raw_memory(buffer), read_only(read_only) {}

IRawMemoryObject::IRawMemoryObject(const std::shared_ptr<data::IMemoryBuffer> &buffer, int8_t compression)
        : IRawObject(std::make_shared<data::IZlibTransport>(buffer, compression),
                     compression), raw_memory(buffer), read_only(false) {}

IRawMemoryObject::IRawMemoryObject(int8_t compression, uint32_t sz)
        : IRawMemoryObject(std::make_shared<data::IMemoryBuffer>(sz), compression) {
}

IRawMemoryObject::~IRawMemoryObject() {

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

void IRawMemoryObject::copyTo(IObject &target){
    if (!read_only) {
        this->transport->flush();
        return IRawMemoryObject(readObservation(), this->compression, elems, type, true).
                setManager(manager).copyTo(target);
    }
    IRawObject::copyTo(target);
}

std::shared_ptr<ignis::data::IMemoryBuffer> IRawMemoryObject::readObservation() {
    flush();
    uint8_t *ptr;
    size_t size;
    raw_memory->getBuffer(&ptr, &size);
    return std::make_shared<data::IMemoryBuffer>(ptr, size);
}

void IRawMemoryObject::fit() {
    this->transport->flush();
    raw_memory->setBufferSize(raw_memory->getBufferSize());
}

void IRawMemoryObject::clear() {
    IRawObject::clear();
    std::dynamic_pointer_cast<data::IZlibTransport> (transport)->restart();
    raw_memory->resetBuffer();
}

std::string IRawMemoryObject::getType() {
    return TYPE;
}

void IRawMemoryObject::flush() {
    if(!read_only){
        IRawObject::flush();
    }
}








