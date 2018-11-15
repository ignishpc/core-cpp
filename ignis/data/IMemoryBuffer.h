
#ifndef IGNIS_IMEMORYBUFFER_H
#define IGNIS_IMEMORYBUFFER_H

#include <thrift/transport/TBufferTransports.h>
#include "../IHeaders.h"

namespace ignis {
    namespace data {
        /*
         * 64 bits implementation of thrift::transport::TMemoryBuffer
         * A memory buffer is a transport that simply reads from and writes to an
         * in memory buffer. Anytime you call write on it, the data is simply placed
         * into a buffer, and anytime you call read, data is read from that buffer.
         *
         * The buffers are allocated using C constructs malloc, realloc, and the size
         * doubles as necessary.  We've considered using scoped
         *
         */
        class IMemoryBuffer : public transport::TVirtualTransport<IMemoryBuffer, transport::TBufferBase> {

        public:
            static const uint32_t defaultSize = 1024;

            /*
             * This enum specifies how a IMemoryBuffer should treat
             * memory passed to it via constructors or resetBuffer.
             *
             * OBSERVE:
             *   IMemoryBuffer will simply store a pointer to the memory.
             *   It is the callers responsibility to ensure that the pointer
             *   remains valid for the lifetime of the IMemoryBuffer,
             *   and that it is properly cleaned up.
             *   Note that no data can be written to observed buffers.
             *
             * COPY:
             *   IMemoryBuffer will make an internal copy of the buffer.
             *   The caller has no responsibilities.
             *
             * TAKE_OWNERSHIP:
             *   IMemoryBuffer will become the "owner" of the buffer,
             *   and will be responsible for freeing it.
             *   The memory must have been allocated with malloc.
             */
            enum MemoryPolicy {
                OBSERVE = 1, COPY = 2, TAKE_OWNERSHIP = 3
            };

            /*
             * Construct a IMemoryBuffer with a default-sized buffer,
             * owned by the IMemoryBuffer object.
             */
            IMemoryBuffer();

            /*
             * Construct a IMemoryBuffer with a buffer of a specified size,
             * owned by the IMemoryBuffer object.
             *
             * @param sz  The initial size of the buffer.
             */
            IMemoryBuffer(size_t sz);

            /*
             * Construct a IMemoryBuffer with buf as its initial contents.
             *
             * @param buf    The initial contents of the buffer.
             *               Note that, while buf is a non-const pointer,
             *               IMemoryBuffer will not write to it if policy == OBSERVE,
             *               so it is safe to const_cast<uint8_t*>(whatever).
             * @param sz     The size of @c buf.
             * @param policy See @link MemoryPolicy @endlink .
             */
            IMemoryBuffer(uint8_t *buf, size_t sz, MemoryPolicy policy = OBSERVE);

            virtual ~IMemoryBuffer();

            bool isOpen();

            bool peek();

            void open();

            void close();

            void getBuffer(uint8_t **bufPtr, size_t *sz);

            std::string getBufferAsString();

            void appendBufferToString(std::string &str);

            void resetBuffer();

            /// See constructor documentation.
            void resetBuffer(uint8_t *buf, size_t sz, MemoryPolicy policy);

            /// See constructor documentation.
            void resetBuffer(size_t sz);

            std::string readAsString(size_t len);

            uint32_t readAppendToString(std::string &str, size_t len);

            // return number of bytes read
            uint32_t readEnd();

            // Return number of bytes written
            uint32_t writeEnd();

            size_t available_read() const;

            size_t available_write() const;

            /*
             * Returns a pointer to where the client can write data to append to
             * the IMemoryBuffer, and ensures the buffer is big enough to accommodate a
             * write of the provided length.  The returned pointer is very convenient for
             * passing to read(), recv(), or similar. You must call wroteBytes() as soon
             * as data is written or the buffer will not be aware that data has changed.
             */
            uint8_t *getWritePtr(uint32_t len);

            /*
             * Informs the buffer that the client has written 'len' bytes into storage
             * that had been provided by getWritePtr().
             */
            void wroteBytes(uint32_t len);

            /*
             * TVirtualTransport provides a default implementation of readAll().
             * We want to use the TBufferBase version instead.
             */
            uint32_t readAll(uint8_t *buf, uint32_t len);

            /*
             * brief Get the current buffer size
             * returns the current buffer size
             */
            size_t getBufferSize() const;

            /* brief Get the current maximum buffer size
             * @return The current maximum buffer size
             */
            size_t getMaxBufferSize() const;

            /*
             * brief Change the maximum buffer size
             * @param[in]  maxSize  the new maximum buffer size allowed to grow to
             * @throws  TTransportException(BAD_ARGS) if maxSize is less than the current buffer size
             */
            void setMaxBufferSize(size_t maxSize);

            void setBufferSize(size_t new_size);

        protected:
            void swap(IMemoryBuffer &that);

            void ensureCanWrite(size_t len);

            void computeRead(size_t len, uint8_t **out_start, size_t *out_give);

            uint32_t readSlow(uint8_t *buf, uint32_t len);

            void writeSlow(const uint8_t *buf, uint32_t len);

            const uint8_t *borrowSlow(uint8_t *buf, uint32_t *len);

            // Data buffer
            uint8_t *buffer_;

            // Allocated buffer size
            size_t bufferSize_;

            // Maximum allowed size
            size_t maxBufferSize_;

            // Is this object the owner of the buffer?
            bool owner_;
        private:

            // Common initialization done by all constructors.
            void initCommon(uint8_t *buf, size_t size, bool owner, size_t wPos);
        };
    }
}

#endif
