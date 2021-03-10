
#include "IMemoryBuffer.h"
#include <algorithm>

using namespace ignis::executor::core::transport;
using namespace apache::thrift::transport;

void IMemoryBuffer::initCommon(uint8_t *buf, size_t size, bool owner, size_t wPos) {

    maxBufferSize_ = std::numeric_limits<size_t>::max();

    if (buf == nullptr && size != 0) {
        assert(owner);
        buf = (uint8_t *) std::malloc(size);
        if (buf == nullptr) { throw std::bad_alloc(); }
    }

    buffer_ = buf;
    bufferSize_ = size;

    rBase_ = buffer_;
    rBound_ = buffer_ + wPos;
    wBase_ = buffer_ + wPos;
    wBound_ = buffer_ + bufferSize_;

    owner_ = owner;

    // rBound_ is really an artifact.  In principle, it should always be
    // equal to wBase_.  We update it in a few places (computeRead, etc.).
}

IMemoryBuffer::IMemoryBuffer() { initCommon(NULL, defaultSize, true, 0); }

IMemoryBuffer::IMemoryBuffer(size_t sz) { initCommon(NULL, sz, true, 0); }

IMemoryBuffer::IMemoryBuffer(uint8_t *buf, size_t sz, IMemoryBuffer::MemoryPolicy policy) {
    if (buf == NULL && sz != 0) {
        throw TTransportException(TTransportException::BAD_ARGS, "TMemoryBuffer given null buffer with non-zero size.");
    }

    switch (policy) {
        case OBSERVE:
        case TAKE_OWNERSHIP:
            initCommon(buf, sz, policy == TAKE_OWNERSHIP, sz);
            break;
        case COPY:
            initCommon(NULL, sz, true, 0);
            this->write(buf, sz);
            break;
        default:
            throw TTransportException(TTransportException::BAD_ARGS, "Invalid MemoryPolicy for TMemoryBuffer");
    }
}

IMemoryBuffer::~IMemoryBuffer() {
    if (owner_) { std::free(buffer_); }
}

bool IMemoryBuffer::isOpen() { return true; }

bool IMemoryBuffer::peek() { return (rBase_ < wBase_); }

void IMemoryBuffer::open() {}

void IMemoryBuffer::close() {}

void IMemoryBuffer::getBuffer(uint8_t **bufPtr, size_t *sz) {
    *bufPtr = rBase_;
    *sz = static_cast<size_t>(wBase_ - rBase_);
}

std::string IMemoryBuffer::getBufferAsString() {
    if (buffer_ == nullptr) { return ""; }
    uint8_t *buf;
    size_t sz;
    getBuffer(&buf, &sz);
    return std::string((char *) buf, (std::string::size_type) sz);
}

void IMemoryBuffer::appendBufferToString(std::string &str) {
    if (buffer_ == nullptr) { return; }
    uint8_t *buf;
    size_t sz;
    getBuffer(&buf, &sz);
    str.append((char *) buf, sz);
}

void IMemoryBuffer::resetBuffer() {
    rBase_ = buffer_;
    rBound_ = buffer_;
    wBase_ = buffer_;
    // It isn't safe to write into a buffer we don't own.
    if (!owner_) {
        wBound_ = wBase_;
        bufferSize_ = 0;
    }
}

void IMemoryBuffer::resetBuffer(uint8_t *buf, size_t sz, IMemoryBuffer::MemoryPolicy policy) {
    // Use a variant of the copy-and-swap trick for assignment operators.
    // This is sub-optimal in terms of performance for two reasons:
    //   1/ The constructing and swapping of the (small) values
    //      in the temporary object takes some time, and is not necessary.
    //   2/ If policy == COPY, we allocate the new buffer before
    //      freeing the old one, precluding the possibility of
    //      reusing that memory.
    // I doubt that either of these problems could be optimized away,
    // but the second is probably no a common case, and the first is minor.
    // I don't expect resetBuffer to be a common operation, so I'm willing to
    // bite the performance bullet to make the method this simple.

    // Construct the new buffer.
    IMemoryBuffer new_buffer(buf, sz, policy);
    // Move it into ourself.
    this->swap(new_buffer);
    // Our old self gets destroyed
}

void IMemoryBuffer::resetBuffer(size_t sz) {
    // Construct the new buffer.
    IMemoryBuffer new_buffer(sz);
    // Move it into ourself.
    this->swap(new_buffer);
    // Our old self gets destroyed.
}

std::string IMemoryBuffer::readAsString(size_t len) {
    std::string str;
    (void) readAppendToString(str, len);
    return str;
}

uint32_t IMemoryBuffer::readAppendToString(std::string &str, size_t len) {
    // Don't get some stupid assertion failure.
    if (buffer_ == nullptr) { return 0; }

    uint8_t *start;
    size_t give;
    computeRead(len, &start, &give);

    // Append to the provided string.
    str.append((char *) start, give);

    return give;
}

uint32_t IMemoryBuffer::readEnd() {
    // This cast should be safe, because buffer_'s size is a uint32_t
    uint32_t bytes = static_cast<uint32_t>(rBase_ - buffer_);
    if (rBase_ == wBase_) { resetBuffer(); }
    return bytes;
}

uint32_t IMemoryBuffer::writeEnd() {
    // This cast should be safe, because buffer_'s size is a uint32_t
    return static_cast<uint32_t>(wBase_ - buffer_);
}

size_t IMemoryBuffer::available_read() const {
    // Remember, wBase_ is the real rBound_.
    return static_cast<size_t>(wBase_ - rBase_);
}

size_t IMemoryBuffer::available_write() const { return static_cast<size_t>(wBound_ - wBase_); }

uint8_t *IMemoryBuffer::getWritePtr(uint32_t len) {
    ensureCanWrite(len);
    return wBase_;
}

void IMemoryBuffer::wroteBytes(uint32_t len) {
    size_t avail = available_write();
    if (len > avail) { throw TTransportException("Client wrote more bytes than size of buffer."); }
    wBase_ += len;
}

uint32_t IMemoryBuffer::readAll(uint8_t *buf, uint32_t len) { return TBufferBase::readAll(buf, len); }

size_t IMemoryBuffer::getBufferSize() const { return bufferSize_; }

size_t IMemoryBuffer::getMaxBufferSize() const { return maxBufferSize_; }

void IMemoryBuffer::setMaxBufferSize(size_t maxSize) {
    if (maxSize < bufferSize_) {
        throw TTransportException(TTransportException::BAD_ARGS,
                                  "Maximum buffer size would be less than current buffer size");
    }
    maxBufferSize_ = maxSize;
}

void IMemoryBuffer::swap(IMemoryBuffer &that) {
    using std::swap;
    swap(buffer_, that.buffer_);
    swap(bufferSize_, that.bufferSize_);

    swap(rBase_, that.rBase_);
    swap(rBound_, that.rBound_);
    swap(wBase_, that.wBase_);
    swap(wBound_, that.wBound_);

    swap(owner_, that.owner_);
}

void IMemoryBuffer::ensureCanWrite(size_t len) {
    // Check available space
    size_t avail = available_write();
    if (len <= avail) { return; }

    if (!owner_) { throw TTransportException("Insufficient space in external MemoryBuffer"); }

    // Grow the buffer as necessary.
    size_t new_size = bufferSize_;
    while (len > avail) {
        if (new_size > maxBufferSize_ / 2) {
            if (available_write() + maxBufferSize_ - bufferSize_ < len) {
                throw TTransportException(TTransportException::BAD_ARGS, "Internal buffer size overflow");
            }
            new_size = maxBufferSize_ - 1;
        }
        new_size = new_size > 0 ? new_size * 2 : 1;
        avail = available_write() + (new_size - bufferSize_);
    }
    setBufferSize(new_size);
}

void IMemoryBuffer::computeRead(size_t len, uint8_t **out_start, size_t *out_give) {
    // Correct rBound_ so we can use the fast path in the future.
    rBound_ = wBase_;

    // Decide how much to give.
    size_t give = (std::min)(len, available_read());

    *out_start = rBase_;
    *out_give = give;

    // Preincrement rBase_ so the caller doesn't have to.
    rBase_ += give;
}

uint32_t IMemoryBuffer::readSlow(uint8_t *buf, uint32_t len) {
    uint8_t *start;
    size_t give;
    computeRead(len, &start, &give);

    // Copy into the provided buffer.
    memcpy(buf, start, give);

    return give;
}

void IMemoryBuffer::writeSlow(const uint8_t *buf, uint32_t len) {
    ensureCanWrite(len);

    // Copy into the buffer and increment wBase_.
    memcpy(wBase_, buf, len);
    wBase_ += len;
}

const uint8_t *IMemoryBuffer::borrowSlow(uint8_t *buf, uint32_t *len) {
    (void) buf;
    rBound_ = wBase_;
    if (available_read() >= *len) {
        *len = available_read();
        return rBase_;
    }
    return nullptr;
}

void IMemoryBuffer::setBufferSize(size_t new_size) {
    if (!owner_) { throw TTransportException("resize in buffer we don't own"); }
    if (bufferSize_ == new_size) { return; }
    // Allocate into a new pointer so we don't bork ours if it fails.
    uint8_t *new_buffer = static_cast<uint8_t *>(std::realloc(buffer_, new_size));
    if (new_buffer == nullptr) { throw std::bad_alloc(); }

    rBase_ = new_buffer + std::min(static_cast<size_t>(rBase_ - buffer_), new_size);
    rBound_ = new_buffer + std::min(static_cast<size_t>(rBound_ - buffer_), new_size);
    wBase_ = new_buffer + std::min(static_cast<size_t>(wBase_ - buffer_), new_size);
    wBound_ = new_buffer + new_size;
    buffer_ = new_buffer;
    bufferSize_ = new_size;
}
