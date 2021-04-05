
#ifndef IGNIS_IZLIBTRANSPORT_H
#define IGNIS_IZLIBTRANSPORT_H

#include "ITransport.h"
#include <thrift/transport/TZlibTransport.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace transport {
                class IZlibTransport
                    : public IVirtualTransport<IZlibTransport, apache::thrift::transport::TZlibTransport> {
                public:
                    IZlibTransport(const std::shared_ptr<TTransport> &transport,
                                   int16_t comp_level = Z_DEFAULT_COMPRESSION, int urbuf_size = 256 * 1024,
                                   int crbuf_size = 256 * 1024, int uwbuf_size = 256 * 1024,
                                   int cwbuf_size = 256 * 1024);

                    void flush() override;

                    void reset();

                    std::shared_ptr<TTransport> getTransport();

                    int16_t getInCompression();

                    bool isOpen() const override;

                    bool peek() override;

                    uint32_t read(uint8_t *buf, uint32_t len);

                    void write(const uint8_t *buf, uint32_t len);

                    void finish();

                    const uint8_t *borrow(uint8_t *buf, uint32_t *len);

                    void consume(uint32_t len);

                    void verifyChecksum();

                private:
                    int16_t in_compression;

                    bool winit, rinit;

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

                class TZlibTransportFactoryExt : public apache::thrift::transport::TTransportFactory {
                public:
                    TZlibTransportFactoryExt(int compression);

                    std::shared_ptr<ITransport> getTransport(std::shared_ptr<ITransport> trans) override;

                    virtual ~TZlibTransportFactoryExt();

                private:
                    int compression;
                };

            }// namespace transport
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
