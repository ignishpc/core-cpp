
#ifndef IGNIS_ITRANSPORT_H
#define IGNIS_ITRANSPORT_H

#include "ignis/executor/core/transport/ITransportException.h"
#include <thrift/transport/TSimpleFileTransport.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TVirtualTransport.h>

namespace ignis {
    namespace executor {
        namespace core {
            namespace transport {
                using ITransport = apache::thrift::transport::TTransport;
                using IFileTransport = apache::thrift::transport::TSimpleFileTransport;

                template<class Transport_, class Super_ = apache::thrift::transport::TTransportDefaults>
                class IVirtualTransport : public Super_ {
                public:
                    /*
                     * Implementations of the *_virt() functions, to call the subclass's
                     * non-virtual implementation function.
                     */
                    uint32_t read_virt(uint8_t *buf, uint32_t len) override {
                        return static_cast<Transport_ *>(this)->read(buf, len);
                    }

                    uint32_t readAll_virt(uint8_t *buf, uint32_t len) override {
                        return static_cast<Transport_ *>(this)->readAll(buf, len);
                    }

                    void write_virt(const uint8_t *buf, uint32_t len) override {
                        static_cast<Transport_ *>(this)->write(buf, len);
                    }

                    const uint8_t *borrow_virt(uint8_t *buf, uint32_t *len) override {
                        return static_cast<Transport_ *>(this)->borrow(buf, len);
                    }

                    void consume_virt(uint32_t len) override { static_cast<Transport_ *>(this)->consume(len); }

                    /*
                     * Provide a default readAll() implementation that invokes
                     * read() non-virtually.
                     *
                     * Note: subclasses that use IVirtualTransport to derive from another
                     * transport implementation (i.e., not TTransportDefaults) should beware that
                     * this may override any non-default readAll() implementation provided by
                     * the parent transport class.  They may need to redefine readAll() to call
                     * the correct parent implementation, if desired.
                     */
                    uint32_t readAll(uint8_t *buf, uint32_t len) {
                        auto *trans = static_cast<Transport_ *>(this);
                        return ::apache::thrift::transport::readAll(*trans, buf, len);
                    }

                protected:
                    template<typename... Args>
                    IVirtualTransport(Args &&...args) : Super_(args...) {}
                };

            }// namespace transport
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#endif
