
#include "IVoidPartition.h"
#include "ignis/executor/core/exception/ILogicError.h"
#include "ignis/executor/core/transport/IZlibTransport.h"

using namespace ignis::executor;
using namespace ignis::executor::core::storage;
using ignis::executor::core::exception::ILogicError;

const std::string IVoidPartition::TYPE = "void";

IVoidPartition::IVoidPartition(size_t size) : buffer(std::make_shared<core::transport::IMemoryBuffer>(size)) {}

IVoidPartition::IVoidPartition(const std::string &path)
    : path(path), file(std::make_shared<transport::IFileTransport>(path, false, true)) {}

std::shared_ptr<api::IReadIterator<IVoidPartition::VOID_TYPE>> IVoidPartition::readIterator() {
    throw ILogicError("void partition not support iteration");
}

std::shared_ptr<api::IWriteIterator<IVoidPartition::VOID_TYPE>> IVoidPartition::writeIterator() {
    throw ILogicError("void partition not support iteration");
}

void IVoidPartition::read(std::shared_ptr<transport::ITransport> &trans) {
    uint8_t bb[256];
    uint32_t read;
    if (path.empty()) {
        while ((read = trans->read(bb, 256)) > 0) { buffer->write(bb, read); }
    } else {
        while ((read = trans->read(bb, 256)) > 0) { file->write(bb, read); }
    }
}

void IVoidPartition::write(std::shared_ptr<transport::ITransport> &trans, int8_t compression) {
    auto read_trans = readTransport();
    auto zlib = std::make_shared<transport::IZlibTransport>(read_trans);
    auto zlib_out = std::make_shared<transport::IZlibTransport>(trans, compression);
    uint8_t bb[256];
    uint32_t read;
    while ((read = zlib->read(bb, 256)) > 0) { zlib_out->write(bb, read); }
    zlib_out->flush();
}

void IVoidPartition::copyFrom(IPartition<VOID_TYPE> &source) { throw ILogicError("void partition not support copy"); }

void IVoidPartition::moveFrom(IPartition<VOID_TYPE> &source) { throw ILogicError("void partition not support move"); }

std::shared_ptr<IPartition<IVoidPartition::VOID_TYPE>> IVoidPartition::clone() {
    if (path.empty()) {
        auto partition = std::make_shared<IVoidPartition>(buffer->getMaxBufferSize());
        uint8_t *bufPtr;
        size_t sz;
        buffer->getBuffer(&bufPtr, &sz);
        partition->buffer->write(bufPtr, sz);
        return partition;
    } else {
        auto newPartition = std::make_shared<IVoidPartition>(path + "_" + std::to_string(copies++));
        auto read_trans = readTransport();
        newPartition->read(read_trans);
        return newPartition;
    }
}

size_t IVoidPartition::size() { throw ILogicError("void partition not support size"); }

size_t IVoidPartition::bytes() {
    if (path.empty()) {
        return buffer->getBufferSize();
    } else {
        return ::lseek64(file->getFD(), 0, SEEK_CUR);
    }
}

void IVoidPartition::clear() {
    if (path.empty()) {
        buffer->resetBuffer();
    } else {
        file->flush();
        if (::ftruncate64(file->getFD(), 0) != 0) {
            throw exception::ILogicError("error: " + path + " truncate error");
        }
    }
}

void IVoidPartition::fit() {
    if (path.empty()) { buffer->setBufferSize(buffer->getBufferSize()); }
}

std::shared_ptr<core::transport::ITransport> IVoidPartition::readTransport() {
    if (path.empty()) {
        uint8_t *ptr;
        size_t sz;
        buffer->getBuffer(&ptr, &sz);
        return std::make_shared<transport::IMemoryBuffer>(ptr, sz, transport::IMemoryBuffer::OBSERVE);
    } else {
        return std::make_shared<transport::IFileTransport>(path, true, false);
    }
}

const std::string &IVoidPartition::type() { return TYPE; }

IVoidPartition::~IVoidPartition() {}