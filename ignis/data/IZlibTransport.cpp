#include "IZlibTransport.h"

using namespace ignis::data;

IZlibTransport::IZlibTransport(std::shared_ptr<TTransport> transport, int16_t comp_level, int urbuf_size,
                               int crbuf_size, int uwbuf_size, int cwbuf_size) :
        transport::TZlibTransport(transport, urbuf_size, crbuf_size, uwbuf_size, cwbuf_size, comp_level) {}

void IZlibTransport::IZlibTransport::flush() {
    //Bug exception when there is no more data
    if (uwpos_ > 0 || wstream_->avail_out < cwbuf_size_) {
        //Uncompress data > 0 or buffer free space compress smaller than initial
        TZlibTransport::flush();
    } else {
        //Restart zlib
        deflateEnd(wstream_);
        wstream_->total_in = 0;
        wstream_->total_out = 0;
        deflateInit(wstream_, comp_level_);
    }
    transport_->flush();
}