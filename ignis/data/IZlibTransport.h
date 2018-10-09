
#ifndef IGNIS_IZLIBTRANSPORT_H
#define IGNIS_IZLIBTRANSPORT_H

#include "../IHeaders.h"
#include <thrift/transport/TZlibTransport.h>

namespace ignis {
    namespace data {
        class IZlibTransport : public transport::TZlibTransport {
        public:
            IZlibTransport(
                    std::shared_ptr<TTransport> transport,
                    int16_t comp_level = Z_DEFAULT_COMPRESSION,
                    int urbuf_size = 128,
                    int crbuf_size = 1024,
                    int uwbuf_size = 128,
                    int cwbuf_size = 1024);

            virtual void flush() override;

            virtual void restart();

        };
    }
}

#endif
