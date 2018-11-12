#include "IZlibTransport.h"

using namespace ignis::data;

IZlibTransport::IZlibTransport(std::shared_ptr<TTransport> transport, int16_t comp_level, int urbuf_size,
                               int crbuf_size, int uwbuf_size, int cwbuf_size) :
        transport::TZlibTransport(transport, urbuf_size, crbuf_size, uwbuf_size, cwbuf_size, comp_level) {}


void IZlibTransport::IZlibTransport::flush() {
    try {
        this->TZlibTransport::flush();
    } catch (transport::TZlibTransportException &ex) {//Exception when there is no more data to flush
        if (ex.getZlibStatus() != Z_BUF_ERROR) {
            throw ex;
        } else {
            transport_->flush();
        }
    }

}

void IZlibTransport::restart(){
    inflateEnd(rstream_);
    deflateEnd(wstream_);
    delete rstream_;
    delete wstream_;
    initZlib();
}

