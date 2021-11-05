
#ifndef IGNIS_IRAWMEMORYPARTITION_H
#define IGNIS_IRAWMEMORYPARTITION_H

#include "IRawPartition.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                template<typename Tp>
                class IRawMemoryPartition : public IRawPartition<Tp> {
                public:
                    typedef Tp value_type;
                    static const std::string TYPE;

                    IRawMemoryPartition(size_t bytes = 1024 * 1024, int8_t compression = 6);

                    virtual std::shared_ptr<IPartition<Tp>> clone();

                    virtual void clear();

                    virtual void fit();

                    virtual const std::string &type();

                    virtual size_t bytes();

                    virtual void sync();

                    virtual void reserve(size_t bytes);

                    virtual ~IRawMemoryPartition();

                private:
                    friend class core::IMpi;

                    uint8_t *begin(bool header);

                    uint8_t *end();

                    void resize(size_t elems, size_t bytes);

                    IRawMemoryPartition(std::shared_ptr<transport::IMemoryBuffer> &&bb, int8_t compression);

                    virtual void writeHeader();

                    virtual std::shared_ptr<transport::ITransport> readTransport();

                    virtual std::shared_ptr<transport::ITransport> &transport();

                    std::shared_ptr<transport::IMemoryBuffer> observer(int init = 0);

                    std::shared_ptr<transport::IMemoryBuffer> buffer;
                };
            }// namespace storage
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IRawMemoryPartition.tcc"

#endif
