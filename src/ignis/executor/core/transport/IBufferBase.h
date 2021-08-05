
#ifndef IGNIS_IBUFFERBASE_H
#define IGNIS_IBUFFERBASE_H

#include <cstring>
#include "ignis/executor/core/transport/ITransportException.h"
#include <thrift/transport/TVirtualTransport.h>
//Copy from thrift/transport/TBufferTransports.h

#ifdef __GNUC__
    #define TDB_LIKELY(val) (__builtin_expect((val), 1))
    #define TDB_UNLIKELY(val) (__builtin_expect((val), 0))
#else
    #define TDB_LIKELY(val) (val)
    #define TDB_UNLIKELY(val) (val)
#endif

namespace ignis {
    namespace executor {
        namespace core {
            namespace transport {
                /**
                 * Base class for all transports that use read/write buffers for performance.
                 *
                 * TBufferBase is designed to implement the fast-path "memcpy" style
                 * operations that work in the common case.  It does so with small and
                 * (eventually) nonvirtual, inlinable methods.  TBufferBase is an abstract
                 * class.  Subclasses are expected to define the "slow path" operations
                 * that have to be done when the buffers are full or empty.
                 *
                 */
                class IBufferBase : public apache::thrift::transport::TVirtualTransport<IBufferBase> {

                public:
                    /**
                   * Fast-path read.
                   *
                   * When we have enough data buffered to fulfill the read, we can satisfy it
                   * with a single memcpy, then adjust our internal pointers.  If the buffer
                   * is empty, we call out to our slow path, implemented by a subclass.
                   * This method is meant to eventually be nonvirtual and inlinable.
                   */
                    uint32_t read(uint8_t *buf, uint32_t len) {
                        uint8_t *new_rBase = rBase_ + len;
                        if (TDB_LIKELY(new_rBase <= rBound_)) {
                            std::memcpy(buf, rBase_, len);
                            rBase_ = new_rBase;
                            return len;
                        }
                        return readSlow(buf, len);
                    }

                    /**
                   * Shortcutted version of readAll.
                   */
                    uint32_t readAll(uint8_t *buf, uint32_t len) {
                        uint8_t *new_rBase = rBase_ + len;
                        if (TDB_LIKELY(new_rBase <= rBound_)) {
                            std::memcpy(buf, rBase_, len);
                            rBase_ = new_rBase;
                            return len;
                        }
                        return apache::thrift::transport::readAll(*this, buf, len);
                    }

                    /**
                   * Fast-path write.
                   *
                   * When we have enough empty space in our buffer to accommodate the write, we
                   * can satisfy it with a single memcpy, then adjust our internal pointers.
                   * If the buffer is full, we call out to our slow path, implemented by a
                   * subclass.  This method is meant to eventually be nonvirtual and
                   * inlinable.
                   */
                    void write(const uint8_t *buf, uint32_t len) {
                        uint8_t *new_wBase = wBase_ + len;
                        if (TDB_LIKELY(new_wBase <= wBound_)) {
                            std::memcpy(wBase_, buf, len);
                            wBase_ = new_wBase;
                            return;
                        }
                        writeSlow(buf, len);
                    }

                    /**
                   * Fast-path borrow.  A lot like the fast-path read.
                   */
                    const uint8_t *borrow(uint8_t *buf, uint32_t *len) {
                        if (TDB_LIKELY(static_cast<ptrdiff_t>(*len) <= rBound_ - rBase_)) {
                            // With strict aliasing, writing to len shouldn't force us to
                            // refetch rBase_ from memory.
                            *len = static_cast<uint32_t>(rBound_ - rBase_);
                            return rBase_;
                        }
                        return borrowSlow(buf, len);
                    }

                    /**
                   * Consume doesn't require a slow path.
                   */
                    void consume(uint32_t len) {
                        if (TDB_LIKELY(static_cast<ptrdiff_t>(len) <= rBound_ - rBase_)) {
                            rBase_ += len;
                        } else {
                            throw apache::thrift::transport::TTransportException(
                                    apache::thrift::transport::TTransportException::BAD_ARGS,
                                    "consume did not follow a borrow.");
                        }
                    }

                protected:
                    /// Slow path read.
                    virtual uint32_t readSlow(uint8_t *buf, uint32_t len) = 0;

                    /// Slow path write.
                    virtual void writeSlow(const uint8_t *buf, uint32_t len) = 0;

                    /**
                   * Slow path borrow.
                   *
                   * POSTCONDITION: return == NULL || rBound_ - rBase_ >= *len
                   */
                    virtual const uint8_t *borrowSlow(uint8_t *buf, uint32_t *len) = 0;

                    /**
                   * Trivial constructor.
                   *
                   * Initialize pointers safely.  Constructing is not a very
                   * performance-sensitive operation, so it is okay to just leave it to
                   * the concrete class to set up pointers correctly.
                   */
                    IBufferBase() : rBase_(nullptr), rBound_(nullptr), wBase_(nullptr), wBound_(nullptr) {}

                    /// Convenience mutator for setting the read buffer.
                    void setReadBuffer(uint8_t *buf, uint32_t len) {
                        rBase_ = buf;
                        rBound_ = buf + len;
                    }

                    /// Convenience mutator for setting the write buffer.
                    void setWriteBuffer(uint8_t *buf, uint32_t len) {
                        wBase_ = buf;
                        wBound_ = buf + len;
                    }

                    ~IBufferBase() override = default;

                    /// Reads begin here.
                    uint8_t *rBase_;
                    /// Reads may extend to just before here.
                    uint8_t *rBound_;

                    /// Writes begin here.
                    uint8_t *wBase_;
                    /// Writes may extend to just before here.
                    uint8_t *wBound_;
                };
            }// namespace transport
        }    // namespace core
    }        // namespace executor
}// namespace ignis
#undef TDB_LIKELY
#undef TDB_UNLIKELY
#endif
