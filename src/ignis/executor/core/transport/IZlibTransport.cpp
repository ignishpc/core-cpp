#include "IZlibTransport.h"

using namespace ignis::executor::core::transport;
using namespace apache::thrift::transport;

IZlibTransport::IZlibTransport(const std::shared_ptr<TTransport> &transport, int16_t comp_level, int urbuf_size,
                               int crbuf_size, int uwbuf_size, int cwbuf_size)
    : IVirtualTransport(transport, urbuf_size, crbuf_size, uwbuf_size, cwbuf_size, comp_level),
      in_compression(comp_level), winit(false), rinit(false) {}


void IZlibTransport::reset() {
    urpos_ = 0;
    uwpos_ = 0;
    rinit = false;
    winit = false;
    in_compression = comp_level_;
    input_ended_ = false;
    output_finished_ = false;
    inflateEnd(rstream_);
    deflateEnd(wstream_);
    delete rstream_;
    delete wstream_;
    initZlib();
}

std::shared_ptr<TTransport> IZlibTransport::getTransport() { return transport_; }

int16_t IZlibTransport::getInCompression() {
    if (!rinit) {
        uint8_t v;
        read(&v, 0);
    }
    return in_compression;
}

bool IZlibTransport::isOpen() const {
    if (in_compression > 0) {
        return TZlibTransport::isOpen();
    } else {
        return transport_->isOpen();
    }
}

bool IZlibTransport::peek() {
    if (in_compression > 0) {
        return TZlibTransport::peek();
    } else {
        return transport_->peek();
    }
}

uint32_t IZlibTransport::read(uint8_t *buf, uint32_t len) {
    if (!rinit) {
        uint8_t v = comp_level_;
        rinit = transport_->read(&v, 1) > 0;
        in_compression = v;
    }
    if (in_compression > 0) {
        return TZlibTransport::read(buf, len);
    } else {
        return transport_->read(buf, len);
    }
}

void IZlibTransport::write(const uint8_t *buf, uint32_t len) {
    if (!winit) {
        winit = true;
        uint8_t v = (uint8_t) comp_level_;
        transport_->write(&v, 1);
    }
    if (in_compression > 0) {
        TZlibTransport::write(buf, len);
    } else {
        transport_->write(buf, len);
    }
}

void IZlibTransport::finish() {
    if (in_compression > 0) { TZlibTransport::finish(); }
}

const uint8_t *IZlibTransport::borrow(uint8_t *buf, uint32_t *len) {
    if (!rinit && *len > 0) {
        uint32_t clen = 1;
        const uint8_t *cbuf = transport_->borrow(buf, &clen);
        if (clen >= 1 && cbuf != nullptr) {
            rinit = true;
            in_compression = cbuf[0];
            transport_->consume(1);
            return transport_->borrow(buf, len);
        }
        return cbuf;
    }
    if (in_compression > 0) {
        return TZlibTransport::borrow(buf, len);
    } else {
        return transport_->borrow(buf, len);
    }
}

void IZlibTransport::consume(uint32_t len) {
    if (in_compression > 0) {
        TZlibTransport::consume(len);
    } else {
        transport_->consume(len);
    }
}

void IZlibTransport::verifyChecksum() {
    if (in_compression > 0) { TZlibTransport::verifyChecksum(); }
}


/*flush, flushToZlib, flushToTransport, checkZlibRv copied from the parent class.
 * checkZlibRv throws an exception when there is no more data to write and
 * Storages can flush twice to ensure that flush has been called before reading. */
void IZlibTransport::flush() {
    if (!winit) {
        uint8_t tmp;
        write(&tmp, 0);
    } else if (in_compression == 0) {
        transport_->flush();
        return;
    }
    ///////////

    if (output_finished_) { throw TTransportException(TTransportException::BAD_ARGS, "flush() called after finish()"); }

    flushToZlib(uwbuf_, uwpos_, Z_BLOCK);
    uwpos_ = 0;

    if (wstream_->avail_out < 6) {
        transport_->write(cwbuf_, cwbuf_size_ - wstream_->avail_out);
        wstream_->next_out = cwbuf_;
        wstream_->avail_out = cwbuf_size_;
    }

    flushToTransport(Z_FULL_FLUSH);
}

void IZlibTransport::flushToZlib(const uint8_t *buf, int len, int flush) {
    wstream_->next_in = const_cast<uint8_t *>(buf);
    wstream_->avail_in = len;

    while (true) {
        if ((flush == Z_NO_FLUSH || flush == Z_BLOCK) && wstream_->avail_in == 0) { break; }

        // If our ouput buffer is full, flush to the underlying transport.
        if (wstream_->avail_out == 0) {
            transport_->write(cwbuf_, cwbuf_size_);
            wstream_->next_out = cwbuf_;
            wstream_->avail_out = cwbuf_size_;
        }

        int zlib_rv = deflate(wstream_, flush);

        if (flush == Z_FINISH && zlib_rv == Z_STREAM_END) {
            assert(wstream_->avail_in == 0);
            output_finished_ = true;
            break;
        }

        checkZlibRv(zlib_rv, wstream_->msg);

        if ((flush == Z_SYNC_FLUSH || flush == Z_FULL_FLUSH) && wstream_->avail_in == 0 && wstream_->avail_out != 0) {
            break;
        }
    }
}

void IZlibTransport::flushToTransport(int flush) {
    // write pending data in uwbuf_ to zlib
    flushToZlib(uwbuf_, uwpos_, flush);
    uwpos_ = 0;

    // write all available data from zlib to the transport
    transport_->write(cwbuf_, cwbuf_size_ - wstream_->avail_out);
    wstream_->next_out = cwbuf_;
    wstream_->avail_out = cwbuf_size_;

    // flush the transport
    transport_->flush();
}

inline void IZlibTransport::checkZlibRv(int status, const char *message) {
    if (status != Z_OK && status != Z_BUF_ERROR) { throw TZlibTransportException(status, message); }
}

IZlibTransportFactory::IZlibTransportFactory(int compression) : compression(compression) {}

std::shared_ptr<ITransport> IZlibTransportFactory::getTransport(std::shared_ptr<ITransport> trans) {
    return std::make_shared<IZlibTransport>(trans, compression, 128, 1024, 128, 1024);
}

IZlibTransportFactory::~IZlibTransportFactory() {}
