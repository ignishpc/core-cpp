
#include "IVoidPartition.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include "ignis/executor/core/transport/IZlibTransport.h"

using namespace ignis::executor;
using namespace ignis::executor::core::storage;
using ignis::executor::core::exception::ILogicError;

const std::string IVoidPartition::TYPE = "void";

IVoidPartition::IVoidPartition(size_t size) : buffer(std::make_shared<core::transport::IMemoryBuffer>(size)) {
}

std::shared_ptr<api::IReadIterator<IVoidPartition::VOID_TYPE>> IVoidPartition::readIterator() {
    throw ILogicError("void partition not support iteration");
}

std::shared_ptr<api::IWriteIterator<IVoidPartition::VOID_TYPE>> IVoidPartition::writeIterator() {
    throw ILogicError("void partition not support iteration");
}

void IVoidPartition::read(std::shared_ptr<transport::ITransport> &trans) {
    uint8_t bb[256];
    uint32_t read;
    while ((read = trans->read(bb, 256)) > 0) {
        buffer->write(bb, read);
    }
}

void IVoidPartition::write(std::shared_ptr<transport::ITransport> &trans, int8_t compression) {
    auto zlib = std::make_shared<transport::IZlibTransport>(buffer);
    auto zlib_out = std::make_shared<transport::IZlibTransport>(trans, compression);
    uint8_t bb[256];
    uint32_t read;
    while ((read = zlib->read(bb, 256)) > 0) {
        zlib_out->write(bb, read);
    }
    zlib_out->flush();
}

void IVoidPartition::copyFrom(IPartition<VOID_TYPE> &source) {
    throw ILogicError("void partition not support copy");
}

void IVoidPartition::moveFrom(IPartition<VOID_TYPE> &source) {
    throw ILogicError("void partition not support move");
}

std::shared_ptr<IPartition<IVoidPartition::VOID_TYPE>> IVoidPartition::clone() {
    auto partition = std::make_shared<IVoidPartition>(buffer->getMaxBufferSize());
    uint8_t *bufPtr;
    size_t sz;
    buffer->getBuffer(&bufPtr,&sz);
    partition->buffer->write(bufPtr, sz);
}

size_t IVoidPartition::size() {
    throw ILogicError("void partition not support size");
}

size_t IVoidPartition::bytes() {
    return buffer->getBufferSize();
}

void IVoidPartition::clear() {
    buffer->resetBuffer();
}

void IVoidPartition::fit() {
}

const std::string &IVoidPartition::type() {
    return TYPE;
}

IVoidPartition::~IVoidPartition() {}