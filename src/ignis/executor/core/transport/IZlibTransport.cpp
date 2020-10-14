#include "IZlibTransport.h"

using namespace ignis::executor::core::transport;
using namespace apache::thrift::transport;

IZlibTransport::IZlibTransport(std::shared_ptr<TTransport> transport, int16_t comp_level, int urbuf_size,
                               int crbuf_size, int uwbuf_size, int cwbuf_size)
    : TZlibTransport(transport, urbuf_size, crbuf_size, uwbuf_size, cwbuf_size, comp_level) {}


void IZlibTransport::reset() {
    urpos_ = 0;
    uwpos_ = 0;
    input_ended_ = false;
    output_finished_ = false;
    inflateEnd(rstream_);
    deflateEnd(wstream_);
    delete rstream_;
    delete wstream_;
    initZlib();
}

/*flush, flushToZlib, flushToTransport, checkZlibRv copied from the parent class.
 * checkZlibRv throws an exception when there is no more data to write and
 * Storages can flush twice to ensure that flush has been called before reading. */
void IZlibTransport::flush() {
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
