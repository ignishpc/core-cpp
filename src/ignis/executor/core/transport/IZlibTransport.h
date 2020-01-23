
#ifndef IGNIS_IZLIBTRANSPORT_H
#define IGNIS_IZLIBTRANSPORT_H

#include "ITransport.h"
#include <thrift/transport/TZlibTransport.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace transport {
                class IZlibTransport : public apache::thrift::transport::TZlibTransport {
                public:
                    IZlibTransport(
                            std::shared_ptr<TTransport> transport,
                            int16_t comp_level = Z_DEFAULT_COMPRESSION,
                            int urbuf_size = 256 * 1024,
                            int crbuf_size = 256 * 1024,
                            int uwbuf_size = 256 * 1024,
                            int cwbuf_size = 256 * 1024);

                    virtual void flush() override;

                    virtual void reset();

                    using apache::thrift::transport::TZlibTransport::readAvail;

                private:
                    void flushToZlib(const uint8_t *buf, int len, int flush);

                    void checkZlibRv(int status, const char *message);

                    void flushToTransport(int flush);

                };

                class IZlibTransportFactory : public apache::thrift::transport::TTransportFactory {
                public:
                    IZlibTransportFactory(int compression);

                    std::shared_ptr<ITransport> getTransport(std::shared_ptr<ITransport> trans) override;

                    virtual ~IZlibTransportFactory();

                private:
                    int compression;
                };
            }
        }
    }
}

#endif
